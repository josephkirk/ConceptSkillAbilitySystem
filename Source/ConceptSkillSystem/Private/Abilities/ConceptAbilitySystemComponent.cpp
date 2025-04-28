// Copyright Epic Games, Inc. All Rights Reserved.

#include "Abilities/ConceptAbilitySystemComponent.h"
#include "Abilities/ConceptAbility.h"
#include "ConceptComponent.h"
#include "ConceptSkillManager.h"
#include "ConceptSkillFunctionLibrary.h"

UConceptAbilitySystemComponent::UConceptAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UConceptAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

FGameplayAbilitySpecHandle UConceptAbilitySystemComponent::GrantAbilityFromConceptSkill(UConceptSkill* Skill, int32 Level)
{
	if (!Skill || !Skill->GrantedAbility)
	{
		return FGameplayAbilitySpecHandle();
	}

	// Check if we already have this ability granted
	if (SkillToAbilityHandleMap.Contains(Skill))
	{
		// Update the level if needed
		FGameplayAbilitySpecHandle Handle = SkillToAbilityHandleMap[Skill];
		FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(Handle);
		if (AbilitySpec && AbilitySpec->Level != Level)
		{
			AbilitySpec->Level = Level;
			MarkAbilitySpecDirty(*AbilitySpec);
		}
		return Handle;
	}

	// Create the ability spec
	FGameplayAbilitySpec AbilitySpec(Skill->GrantedAbility, Level, INDEX_NONE);

	// Set the source object to the skill
	AbilitySpec.SourceObject = Skill;

	// Grant the ability
	FGameplayAbilitySpecHandle Handle = GiveAbility(AbilitySpec);

	// Store the mapping
	SkillToAbilityHandleMap.Add(Skill, Handle);

	// Initialize the ability
	UConceptAbility* ConceptAbility = Cast<UConceptAbility>(AbilitySpec.Ability);
	if (ConceptAbility)
	{
		ConceptAbility->SourceSkill = Skill;
		ConceptAbility->RequiredConcepts = Skill->RequiredConcepts;
		ConceptAbility->RequiredMasteryLevel = Skill->RequiredMasteryLevel;
	}

	return Handle;
}

bool UConceptAbilitySystemComponent::RemoveAbilityFromConceptSkill(UConceptSkill* Skill)
{
	if (!Skill || !SkillToAbilityHandleMap.Contains(Skill))
	{
		return false;
	}

	// Get the ability handle
	FGameplayAbilitySpecHandle Handle = SkillToAbilityHandleMap[Skill];

	// Remove the ability
	bool bRemoved = ClearAbility(Handle);

	// Remove from the map if successful
	if (bRemoved)
	{
		SkillToAbilityHandleMap.Remove(Skill);
	}

	return bRemoved;
}

void UConceptAbilitySystemComponent::UpdateAbilityLevelsFromConceptMastery()
{
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		return;
	}

	UConceptComponent* ConceptComp = UConceptSkillFunctionLibrary::GetConceptComponent(OwnerActor);
	UConceptSkillManager* SkillManager = UConceptSkillFunctionLibrary::GetConceptSkillManager(OwnerActor);

	if (!ConceptComp || !SkillManager)
	{
		return;
	}

	// Get all unlocked skills
	for (const auto& SkillPtr : SkillManager->UnlockedSkills)
	{
		UConceptSkill* Skill = SkillPtr.Get();
		if (Skill && Skill->GrantedAbility)
		{
			// Calculate the effective power based on concept mastery
			int32 EffectivePower = SkillManager->CalculateSkillEffectivePower(Skill);

			// Update the ability level
			FGameplayAbilitySpecHandle Handle = GetAbilityHandleFromConceptSkill(Skill);
			if (Handle.IsValid())
			{
				FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(Handle);
				if (AbilitySpec)
				{
					// Set the level based on effective power
					AbilitySpec->Level = FMath::Max(1, EffectivePower / 10); // Scale down the power to a reasonable level
					MarkAbilitySpecDirty(*AbilitySpec);
				}
			}
			else
			{
				// Grant the ability if it hasn't been granted yet
				int32 Level = FMath::Max(1, EffectivePower / 10);
				GrantAbilityFromConceptSkill(Skill, Level);
			}
		}
	}
}

FGameplayAbilitySpecHandle UConceptAbilitySystemComponent::GetAbilityHandleFromConceptSkill(UConceptSkill* Skill) const
{
	if (!Skill || !SkillToAbilityHandleMap.Contains(Skill))
	{
		return FGameplayAbilitySpecHandle();
	}

	return SkillToAbilityHandleMap[Skill];
}

FActiveGameplayEffectHandle UConceptAbilitySystemComponent::ApplyEffectFromConcept(UConcept* Concept, TSubclassOf<UGameplayEffect> EffectClass, float Level)
{
	if (!Concept || !EffectClass || !GetOwner())
	{
		return FActiveGameplayEffectHandle();
	}

	// Create effect context
	FGameplayEffectContextHandle EffectContext = MakeEffectContext();
	EffectContext.AddSourceObject(Concept);

	// Create effect spec
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(EffectClass, Level, EffectContext);
	if (!SpecHandle.IsValid())
	{
		return FActiveGameplayEffectHandle();
	}

	// Get the concept's power and apply it as a multiplier
	float PowerMultiplier = Concept->Power / 50.0f; // Normalize to a reasonable range
	SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Data.Scaling")), PowerMultiplier);

	// Apply the effect
	return ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}
