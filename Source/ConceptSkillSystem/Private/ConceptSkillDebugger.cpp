// Copyright Epic Games, Inc. All Rights Reserved.

#include "ConceptSkillDebugger.h"
#include "ConceptSkillFunctionLibrary.h"
#include "Engine/Engine.h"

AConceptSkillDebugger::AConceptSkillDebugger()
{
	PrimaryActorTick.bCanEverTick = true;
	bDebugOnBeginPlay = true;
}

void AConceptSkillDebugger::BeginPlay()
{
	Super::BeginPlay();

	if (bDebugOnBeginPlay)
	{
		DebugConceptComponent();
		DebugConceptSkillManager();
	}
}

void AConceptSkillDebugger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AConceptSkillDebugger::DebugConceptComponent()
{
	UConceptComponent* ConceptComp = GetConceptComponent();
	if (!ConceptComp)
	{
		PrintDebug(TEXT("No ConceptComponent found on target actor"), FColor::Red);
		return;
	}

	PrintDebug(TEXT("Concept Component Debug Information:"), FColor::Green);

	// Print information about body part slots
	for (const auto& Pair : ConceptComp->BodyPartSlots)
	{
		EBodyPartType BodyPart = Pair.Key;
		const TArray<FConceptSlot>& Slots = Pair.Value;

		FString BodyPartName;
		switch (BodyPart)
		{
		case EBodyPartType::Head:
			BodyPartName = TEXT("Head");
			break;
		case EBodyPartType::Body:
			BodyPartName = TEXT("Body");
			break;
		case EBodyPartType::Arms:
			BodyPartName = TEXT("Arms");
			break;
		case EBodyPartType::Feet:
			BodyPartName = TEXT("Feet");
			break;
		default:
			BodyPartName = TEXT("Unknown");
			break;
		}

		PrintDebug(FString::Printf(TEXT("Body Part: %s, Slots: %d"), *BodyPartName, Slots.Num()), FColor::Yellow);

		// Print information about each slot
		for (int32 i = 0; i < Slots.Num(); ++i)
		{
			const FConceptSlot& Slot = Slots[i];
			FString ConceptName = Slot.IsEmpty() ? TEXT("Empty") : Slot.HeldConcept.Get()->GetName();
			FString UnlockedStatus = Slot.bIsUnlocked ? TEXT("Unlocked") : TEXT("Locked");
			FString MasteryInfo = Slot.IsEmpty() ? TEXT("") : FString::Printf(TEXT(", Mastery: %d"), Slot.MasteryLevel);

			PrintDebug(FString::Printf(TEXT("  Slot %d: %s, %s%s"), i, *ConceptName, *UnlockedStatus, *MasteryInfo), FColor::Cyan);
		}
	}

	// Print information about observed and acquired concepts
	PrintDebug(FString::Printf(TEXT("Observed Concepts: %d"), ConceptComp->ObservedConcepts.Num()), FColor::Yellow);
	PrintDebug(FString::Printf(TEXT("Acquired Concepts: %d"), ConceptComp->AcquiredConcepts.Num()), FColor::Yellow);
}

void AConceptSkillDebugger::DebugConceptSkillManager()
{
	UConceptSkillManager* SkillManager = GetConceptSkillManager();
	if (!SkillManager)
	{
		PrintDebug(TEXT("No ConceptSkillManager found on target actor"), FColor::Red);
		return;
	}

	PrintDebug(TEXT("Concept Skill Manager Debug Information:"), FColor::Green);

	// Print information about unlocked skills
	PrintDebug(FString::Printf(TEXT("Unlocked Skills: %d"), SkillManager->UnlockedSkills.Num()), FColor::Yellow);

	// Print information about each skill category
	PrintDebug(FString::Printf(TEXT("Active Skills: %d"), SkillManager->ActiveSkills.Num()), FColor::Yellow);
	PrintDebug(FString::Printf(TEXT("Passive Abilities: %d"), SkillManager->PassiveAbilities.Num()), FColor::Yellow);
	PrintDebug(FString::Printf(TEXT("Crafting Knowledge: %d"), SkillManager->CraftingKnowledge.Num()), FColor::Yellow);
	PrintDebug(FString::Printf(TEXT("Character Proficiencies: %d"), SkillManager->CharacterProficiencies.Num()), FColor::Yellow);

	// Print information about available skills
	PrintDebug(FString::Printf(TEXT("Available Skills: %d"), SkillManager->AvailableSkills.Num()), FColor::Yellow);
}

bool AConceptSkillDebugger::AddTestConcept(UConcept* Concept, EBodyPartType BodyPart)
{
	UConceptComponent* ConceptComp = GetConceptComponent();
	if (!ConceptComp || !Concept)
	{
		PrintDebug(TEXT("Cannot add test concept: Invalid component or concept"), FColor::Red);
		return false;
	}

	bool Success = ConceptComp->AcquireConcept(Concept, BodyPart);
	if (Success)
	{
		PrintDebug(FString::Printf(TEXT("Successfully added concept '%s' to %s"), *Concept->GetName(), *GetEnumValueAsString(TEXT("EBodyPartType"), BodyPart)), FColor::Green);
	}
	else
	{
		PrintDebug(FString::Printf(TEXT("Failed to add concept '%s' to %s"), *Concept->GetName(), *GetEnumValueAsString(TEXT("EBodyPartType"), BodyPart)), FColor::Red);
	}

	return Success;
}

bool AConceptSkillDebugger::IncreaseMastery(UConcept* Concept, int32 Amount)
{
	UConceptComponent* ConceptComp = GetConceptComponent();
	if (!ConceptComp || !Concept)
	{
		PrintDebug(TEXT("Cannot increase mastery: Invalid component or concept"), FColor::Red);
		return false;
	}

	// Find the slot containing the concept
	for (const auto& Pair : ConceptComp->BodyPartSlots)
	{
		EBodyPartType BodyPart = Pair.Key;
		const TArray<FConceptSlot>& Slots = Pair.Value;

		for (const FConceptSlot& Slot : Slots)
		{
			if (!Slot.IsEmpty() && Slot.HeldConcept.Get() == Concept)
			{
				bool Success = ConceptComp->IncreaseMastery(Slot.SlotId, Amount);
				if (Success)
				{
					PrintDebug(FString::Printf(TEXT("Successfully increased mastery of concept '%s' by %d"), *Concept->GetName(), Amount), FColor::Green);
				}
				else
				{
					PrintDebug(FString::Printf(TEXT("Failed to increase mastery of concept '%s'"), *Concept->GetName()), FColor::Red);
				}
				return Success;
			}
		}
	}

	PrintDebug(FString::Printf(TEXT("Concept '%s' not found in any slot"), *Concept->GetName()), FColor::Red);
	return false;
}

void AConceptSkillDebugger::CheckForNewSkills()
{
	UConceptSkillManager* SkillManager = GetConceptSkillManager();
	if (!SkillManager)
	{
		PrintDebug(TEXT("Cannot check for new skills: No ConceptSkillManager found"), FColor::Red);
		return;
	}

	// Store the number of unlocked skills before checking
	int32 NumSkillsBefore = SkillManager->UnlockedSkills.Num();

	// Check for new skills
	SkillManager->CheckForNewSkills();

	// Calculate how many new skills were unlocked
	int32 NumNewSkills = SkillManager->UnlockedSkills.Num() - NumSkillsBefore;

	if (NumNewSkills > 0)
	{
		PrintDebug(FString::Printf(TEXT("Unlocked %d new skills!"), NumNewSkills), FColor::Green);
	}
	else
	{
		PrintDebug(TEXT("No new skills unlocked"), FColor::Yellow);
	}
}

void AConceptSkillDebugger::PrintAllConcepts()
{
	UConceptComponent* ConceptComp = GetConceptComponent();
	if (!ConceptComp)
	{
		PrintDebug(TEXT("Cannot print concepts: No ConceptComponent found"), FColor::Red);
		return;
	}

	PrintDebug(TEXT("All Acquired Concepts:"), FColor::Green);

	// Print information about acquired concepts
	int32 ConceptCount = 0;
	for (const auto& ConceptPtr : ConceptComp->AcquiredConcepts)
	{
		UConcept* Concept = ConceptPtr.Get();
		if (Concept)
		{
			FString TierName;
			switch (Concept->Tier)
			{
			case EConceptTier::Physical:
				TierName = TEXT("Physical");
				break;
			case EConceptTier::Intermediate:
				TierName = TEXT("Intermediate");
				break;
			case EConceptTier::Advanced:
				TierName = TEXT("Advanced");
				break;
			case EConceptTier::Abstract:
				TierName = TEXT("Abstract");
				break;
			default:
				TierName = TEXT("Unknown");
				break;
			}

			// Get the mastery level of the concept
			int32 MasteryLevel = UConceptSkillFunctionLibrary::GetConceptMasteryLevel(TargetActor, Concept);

			PrintDebug(FString::Printf(TEXT("%d. %s (Tier: %s, Power: %d, Mastery: %d)"), ++ConceptCount, *Concept->GetName(), *TierName, Concept->Power, MasteryLevel), FColor::Cyan);
		}
	}

	if (ConceptCount == 0)
	{
		PrintDebug(TEXT("No concepts acquired yet"), FColor::Yellow);
	}
}

void AConceptSkillDebugger::PrintAllSkills()
{
	UConceptSkillManager* SkillManager = GetConceptSkillManager();
	if (!SkillManager)
	{
		PrintDebug(TEXT("Cannot print skills: No ConceptSkillManager found"), FColor::Red);
		return;
	}

	PrintDebug(TEXT("All Unlocked Skills:"), FColor::Green);

	// Print information about unlocked skills
	int32 SkillCount = 0;
	for (const auto& SkillPtr : SkillManager->UnlockedSkills)
	{
		UConceptSkill* Skill = SkillPtr.Get();
		if (Skill)
		{
			FString TypeName;
			switch (Skill->ManifestationType)
			{
			case ESkillManifestationType::Active:
				TypeName = TEXT("Active");
				break;
			case ESkillManifestationType::Passive:
				TypeName = TEXT("Passive");
				break;
			case ESkillManifestationType::Crafting:
				TypeName = TEXT("Crafting");
				break;
			case ESkillManifestationType::Proficiency:
				TypeName = TEXT("Proficiency");
				break;
			default:
				TypeName = TEXT("Unknown");
				break;
			}

			// Calculate the effective power of the skill
			int32 EffectivePower = SkillManager->CalculateSkillEffectivePower(Skill);

			PrintDebug(FString::Printf(TEXT("%d. %s (Type: %s, Base Power: %d, Effective Power: %d)"), ++SkillCount, *Skill->GetName(), *TypeName, Skill->Power, EffectivePower), FColor::Cyan);
		}
	}

	if (SkillCount == 0)
	{
		PrintDebug(TEXT("No skills unlocked yet"), FColor::Yellow);
	}
}

UConceptComponent* AConceptSkillDebugger::GetConceptComponent() const
{
	if (!TargetActor)
	{
		return nullptr;
	}

	return TargetActor->FindComponentByClass<UConceptComponent>();
}

UConceptSkillManager* AConceptSkillDebugger::GetConceptSkillManager() const
{
	if (!TargetActor)
	{
		return nullptr;
	}

	return TargetActor->FindComponentByClass<UConceptSkillManager>();
}

void AConceptSkillDebugger::PrintDebug(const FString& Message, const FColor& Color, float Duration)
{
	// Print to screen
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, Color, Message);
	}

	// Print to log
	UE_LOG(LogTemp, Log, TEXT("[ConceptSkillDebugger] %s"), *Message);
}
