// Copyright Koorogi Games, Inc. All Rights Reserved.

#include "ConceptSkillFunctionLibrary.h"
#include "ConceptObservable.h"

UConceptComponent* UConceptSkillFunctionLibrary::GetConceptComponent(AActor* Actor)
{
	if (!Actor)
	{
		return nullptr;
	}

	return Actor->FindComponentByClass<UConceptComponent>();
}

UConceptSkillManager* UConceptSkillFunctionLibrary::GetConceptSkillManager(AActor* Actor)
{
	if (!Actor)
	{
		return nullptr;
	}

	return Actor->FindComponentByClass<UConceptSkillManager>();
}

bool UConceptSkillFunctionLibrary::CanObserveConcept(AActor* Observer, AActor* Observable)
{
	if (!Observer || !Observable)
	{
		return false;
	}

	// Check if the observable implements the IConceptObservable interface
	if (!Observable->GetClass()->ImplementsInterface(UConceptObservable::StaticClass()))
	{
		return false;
	}

	// Check if the observable can be observed
	return IConceptObservable::Execute_CanBeObserved(Observable);
}

bool UConceptSkillFunctionLibrary::ObserveConcept(AActor* Observer, AActor* Observable)
{
	if (!CanObserveConcept(Observer, Observable))
	{
		return false;
	}

	// Trigger the observation
	IConceptObservable::Execute_OnObserved(Observable, Observer);

	return true;
}

TArray<UConcept*> UConceptSkillFunctionLibrary::GetConceptsByTier(const UObject* WorldContextObject, EConceptTier Tier)
{
	TArray<UConcept*> Concepts;

	UConceptRegistry* Registry = UConceptRegistry::GetConceptRegistry(WorldContextObject);
	if (!Registry)
	{
		return Concepts;
	}

	TArray<TSoftObjectPtr<UConcept>> ConceptPtrs = Registry->GetConceptsByTier(Tier);
	for (const auto& ConceptPtr : ConceptPtrs)
	{
		if (UConcept* Concept = ConceptPtr.Get())
		{
			Concepts.Add(Concept);
		}
	}

	return Concepts;
}

TArray<UConceptSkill*> UConceptSkillFunctionLibrary::GetSkillsByType(const UObject* WorldContextObject, ESkillManifestationType Type)
{
	TArray<UConceptSkill*> Skills;

	UConceptRegistry* Registry = UConceptRegistry::GetConceptRegistry(WorldContextObject);
	if (!Registry)
	{
		return Skills;
	}

	TArray<TSoftObjectPtr<UConceptSkill>> SkillPtrs = Registry->GetSkillsByType(Type);
	for (const auto& SkillPtr : SkillPtrs)
	{
		if (UConceptSkill* Skill = SkillPtr.Get())
		{
			Skills.Add(Skill);
		}
	}

	return Skills;
}

float UConceptSkillFunctionLibrary::CalculateConceptAcquisitionChance(UConcept* Concept, float ObservationQuality)
{
	if (!Concept)
	{
		return 0.0f;
	}

	// Base chance from the concept
	float BaseChance = Concept->BaseAcquisitionChance;

	// Apply observation quality multiplier
	float AcquisitionChance = BaseChance * ObservationQuality;

	// Apply difficulty modifier (higher difficulty = lower chance)
	float DifficultyModifier = 1.0f - (Concept->AcquisitionDifficulty / 100.0f);
	AcquisitionChance *= DifficultyModifier;

	// Ensure the chance is within valid range
	return FMath::Clamp(AcquisitionChance, 0.0f, 1.0f);
}

bool UConceptSkillFunctionLibrary::HasAvailableSlotForConcept(AActor* Actor, UConcept* Concept)
{
	if (!Actor || !Concept)
	{
		return false;
	}

	UConceptComponent* ConceptComp = GetConceptComponent(Actor);
	if (!ConceptComp)
	{
		return false;
	}

	// Try to find a suitable body part based on the concept's tier
	EBodyPartType TargetBodyPart;

	switch (Concept->Tier)
	{
	case EConceptTier::Abstract:
		TargetBodyPart = EBodyPartType::Head;
		break;
	case EConceptTier::Advanced:
		TargetBodyPart = EBodyPartType::Body;
		break;
	case EConceptTier::Intermediate:
		TargetBodyPart = EBodyPartType::Arms;
		break;
	case EConceptTier::Physical:
		TargetBodyPart = EBodyPartType::Feet;
		break;
	default:
		TargetBodyPart = EBodyPartType::Body;
		break;
	}

	// Check if there's an empty slot in the target body part
	FConceptSlot EmptySlot;
	if (ConceptComp->FindEmptySlotForConcept(Concept, TargetBodyPart, EmptySlot))
	{
		return true;
	}

	// If no suitable slot found in the target body part, try other body parts
	for (const auto& Pair : ConceptComp->BodyPartSlots)
	{
		if (Pair.Key != TargetBodyPart)
		{
			if (ConceptComp->FindEmptySlotForConcept(Concept, Pair.Key, EmptySlot))
			{
				return true;
			}
		}
	}

	return false;
}

int32 UConceptSkillFunctionLibrary::GetConceptMasteryLevel(AActor* Actor, UConcept* Concept)
{
	if (!Actor || !Concept)
	{
		return 0;
	}

	UConceptComponent* ConceptComp = GetConceptComponent(Actor);
	if (!ConceptComp)
	{
		return 0;
	}

	// Check each body part for the concept and find the highest mastery level
	int32 HighestMastery = 0;

	for (const auto& Pair : ConceptComp->BodyPartSlots)
	{
		const TArray<FConceptSlot>& Slots = Pair.Value;

		for (const FConceptSlot& Slot : Slots)
		{
			if (!Slot.IsEmpty() && Slot.HeldConcept.Get() == Concept)
			{
				HighestMastery = FMath::Max(HighestMastery, Slot.MasteryLevel);
			}
		}
	}

	return HighestMastery;
}

TArray<UConceptSkill*> UConceptSkillFunctionLibrary::GetPossibleSkillsFromConcepts(const UObject* WorldContextObject, const TArray<UConcept*>& Concepts)
{
	TArray<UConceptSkill*> PossibleSkills;

	if (Concepts.Num() == 0)
	{
		return PossibleSkills;
	}

	UConceptRegistry* Registry = UConceptRegistry::GetConceptRegistry(WorldContextObject);
	if (!Registry)
	{
		return PossibleSkills;
	}

	// Get all skills from the registry
	for (const auto& SkillPtr : Registry->AllSkills)
	{
		UConceptSkill* Skill = SkillPtr.Get();
		if (!Skill)
		{
			continue;
		}

		// Check if all the required concepts for this skill are in the provided concepts array
		bool HasAllRequiredConcepts = true;
		for (const auto& RequiredConcept : Skill->RequiredConcepts)
		{
			UConcept* Concept = RequiredConcept.Get();
			if (!Concept || !Concepts.Contains(Concept))
			{
				HasAllRequiredConcepts = false;
				break;
			}
		}

		if (HasAllRequiredConcepts)
		{
			PossibleSkills.Add(Skill);
		}
	}

	return PossibleSkills;
}
