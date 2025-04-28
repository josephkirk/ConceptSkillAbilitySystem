// Copyright Koorogi Games, Inc. All Rights Reserved.

#include "ConceptSkillManager.h"

UConceptSkillManager::UConceptSkillManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UConceptSkillManager::BeginPlay()
{
	Super::BeginPlay();

	// Get the concept component from the same actor
	ConceptComponent = GetOwner()->FindComponentByClass<UConceptComponent>();

	// Check for skills that can be unlocked with starting concepts
	CheckForNewSkills();
}

void UConceptSkillManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UConceptSkillManager::CheckForNewSkills()
{
	if (!ConceptComponent)
	{
		return;
	}

	// Check each available skill to see if it can be unlocked
	for (const auto& SkillPtr : AvailableSkills)
	{
		UConceptSkill* Skill = SkillPtr.Get();
		if (Skill && !UnlockedSkills.Contains(Skill))
		{
			if (CanUnlockSkill(Skill))
			{
				UnlockSkill(Skill);
			}
		}
	}
}

bool UConceptSkillManager::UnlockSkill(UConceptSkill* Skill)
{
	if (!Skill || UnlockedSkills.Contains(Skill))
	{
		return false;
	}

	// Add to unlocked skills
	UnlockedSkills.Add(Skill);

	// Categorize the skill based on its manifestation type
	CategorizeSkill(Skill);

	// Broadcast delegate
	OnSkillUnlocked.Broadcast(Skill);

	return true;
}

bool UConceptSkillManager::RemoveSkill(UConceptSkill* Skill)
{
	if (!Skill || !UnlockedSkills.Contains(Skill))
	{
		return false;
	}

	// Remove from unlocked skills
	UnlockedSkills.Remove(Skill);

	// Remove from category
	RemoveSkillFromCategory(Skill);

	// Broadcast delegate
	OnSkillRemoved.Broadcast(Skill);

	return true;
}

bool UConceptSkillManager::CanUnlockSkill(UConceptSkill* Skill) const
{
	if (!Skill || !ConceptComponent)
	{
		return false;
	}

	// Check if all required concepts are acquired with sufficient mastery
	for (const auto& RequiredConcept : Skill->RequiredConcepts)
	{
		UConcept* Concept = RequiredConcept.Get();
		if (!Concept || !ConceptComponent->HasAcquiredConcept(Concept))
		{
			return false; // Missing a required concept
		}

		// Check mastery level
		bool HasSufficientMastery = false;

		// Check each body part for the concept with sufficient mastery
		for (const auto& Pair : ConceptComponent->BodyPartSlots)
		{
			const TArray<FConceptSlot>& Slots = Pair.Value;

			for (const FConceptSlot& Slot : Slots)
			{
				if (!Slot.IsEmpty() && Slot.HeldConcept.Get() == Concept && Slot.MasteryLevel >= Skill->RequiredMasteryLevel)
				{
					HasSufficientMastery = true;
					break;
				}
			}

			if (HasSufficientMastery)
			{
				break;
			}
		}

		if (!HasSufficientMastery)
		{
			return false; // Insufficient mastery for a required concept
		}
	}

	return true; // All requirements met
}

TArray<TSoftObjectPtr<UConceptSkill>> UConceptSkillManager::GetSkillsByType(ESkillManifestationType Type) const
{
	switch (Type)
	{
	case ESkillManifestationType::Active:
		return ActiveSkills;
	case ESkillManifestationType::Passive:
		return PassiveAbilities;
	case ESkillManifestationType::Crafting:
		return CraftingKnowledge;
	case ESkillManifestationType::Proficiency:
		return CharacterProficiencies;
	default:
		return TArray<TSoftObjectPtr<UConceptSkill>>();
	}
}

TMap<TSoftObjectPtr<UConcept>, int32> UConceptSkillManager::GetConceptMasteryLevels() const
{
	TMap<TSoftObjectPtr<UConcept>, int32> MasteryLevels;

	if (!ConceptComponent)
	{
		return MasteryLevels;
	}

	// Gather mastery levels from all body part slots
	for (const auto& Pair : ConceptComponent->BodyPartSlots)
	{
		const TArray<FConceptSlot>& Slots = Pair.Value;

		for (const FConceptSlot& Slot : Slots)
		{
			if (!Slot.IsEmpty())
			{
				// If the concept already exists in the map, keep the highest mastery level
				if (!MasteryLevels.Contains(Slot.HeldConcept) || MasteryLevels[Slot.HeldConcept] < Slot.MasteryLevel)
				{
					MasteryLevels.Add(Slot.HeldConcept, Slot.MasteryLevel);
				}
			}
		}
	}

	return MasteryLevels;
}

int32 UConceptSkillManager::CalculateSkillEffectivePower(UConceptSkill* Skill) const
{
	if (!Skill)
	{
		return 0;
	}

	// Get current mastery levels
	TMap<TSoftObjectPtr<UConcept>, int32> MasteryLevels = GetConceptMasteryLevels();

	// Calculate effective power based on mastery levels
	return Skill->CalculateEffectivePower(MasteryLevels);
}

void UConceptSkillManager::CategorizeSkill(UConceptSkill* Skill)
{
	if (!Skill)
	{
		return;
	}

	// Add to the appropriate category based on manifestation type
	switch (Skill->ManifestationType)
	{
	case ESkillManifestationType::Active:
		ActiveSkills.AddUnique(Skill);
		break;
	case ESkillManifestationType::Passive:
		PassiveAbilities.AddUnique(Skill);
		break;
	case ESkillManifestationType::Crafting:
		CraftingKnowledge.AddUnique(Skill);
		break;
	case ESkillManifestationType::Proficiency:
		CharacterProficiencies.AddUnique(Skill);
		break;
	}
}

void UConceptSkillManager::RemoveSkillFromCategory(UConceptSkill* Skill)
{
	if (!Skill)
	{
		return;
	}

	// Remove from the appropriate category based on manifestation type
	switch (Skill->ManifestationType)
	{
	case ESkillManifestationType::Active:
		ActiveSkills.Remove(Skill);
		break;
	case ESkillManifestationType::Passive:
		PassiveAbilities.Remove(Skill);
		break;
	case ESkillManifestationType::Crafting:
		CraftingKnowledge.Remove(Skill);
		break;
	case ESkillManifestationType::Proficiency:
		CharacterProficiencies.Remove(Skill);
		break;
	}
}
