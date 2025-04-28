// Copyright Epic Games, Inc. All Rights Reserved.

#include "Abilities/ConceptAbility.h"
#include "ConceptComponent.h"
#include "ConceptSkillFunctionLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/ConceptAttributeSet.h"
#include "ConceptSkillManager.h"

UConceptAbility::UConceptAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("State.Debuff.Stun")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("State.Dead")));
	
	RequiredMasteryLevel = 50;
	MasteryScaling = 1.0f;
}

float UConceptAbility::CalculateAbilityPower(const AActor* SourceActor) const
{
	if (!SourceActor)
	{
		return 0.0f;
	}

	// Get the concept component
	UConceptComponent* ConceptComp = UConceptSkillFunctionLibrary::GetConceptComponent(const_cast<AActor*>(SourceActor));
	if (!ConceptComp)
	{
		return 0.0f;
	}

	// Calculate the average mastery level of all required concepts
	float TotalMastery = 0.0f;
	int32 ConceptsFound = 0;

	for (const auto& RequiredConcept : RequiredConcepts)
	{
		UConcept* Concept = RequiredConcept.Get();
		if (Concept && ConceptComp->HasAcquiredConcept(Concept))
		{
			// Find the highest mastery level for this concept across all body parts
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

			TotalMastery += HighestMastery;
			ConceptsFound++;
		}
	}

	if (ConceptsFound == 0)
	{
		return 0.0f; // Missing required concepts
	}

	// Calculate average mastery
	float AverageMastery = TotalMastery / ConceptsFound;

	// Calculate power scaling based on mastery level
	// At minimum required mastery, power is 1.0
	// At maximum mastery (100), power is 1.0 + MasteryScaling
	float MasteryRatio = FMath::Max(0.0f, (AverageMastery - RequiredMasteryLevel) / (100.0f - RequiredMasteryLevel));
	float PowerMultiplier = 1.0f + (MasteryRatio * MasteryScaling);

	return PowerMultiplier;
}

TArray<TSubclassOf<UGameplayEffect>> UConceptAbility::GetAbilityEffects() const
{
	return AbilityEffects;
}

void UConceptAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// Call the parent implementation
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Get the owning actor
	AActor* OwningActor = ActorInfo->AvatarActor.Get();
	if (!OwningActor)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Apply gameplay effects with attribute scaling
	if (ActorInfo->AbilitySystemComponent.IsValid())
	{
		for (TSubclassOf<UGameplayEffect> EffectClass : EffectsToApply)
		{
			if (EffectClass)
			{
				FGameplayEffectContextHandle EffectContext = ActorInfo->AbilitySystemComponent->MakeEffectContext();
				EffectContext.AddSourceObject(this);

				// Create effect spec with the ability level
				FGameplayEffectSpecHandle EffectSpec = ActorInfo->AbilitySystemComponent->MakeOutgoingSpec(EffectClass, GetAbilityLevel(), EffectContext);
				
				// Apply attribute scaling to the effect magnitude
				if (EffectSpec.IsValid())
				{
					// Get the base magnitude and apply scaling
					for (FGameplayEffectModifiedAttribute& ModifiedAttribute : EffectSpec.Data->ModifiedAttributes)
					{
						for (FGameplayEffectAttributeModifier& AttributeMod : EffectSpec.Data->Spec.Modifiers)
						{
							if (AttributeMod.Attribute == ModifiedAttribute.Attribute)
							{
								// Apply our attribute scaling to the magnitude
								float ScaledMagnitude = ApplyAttributeScaling(AttributeMod.ModifierMagnitude.GetValueChecked<float>());
								AttributeMod.ModifierMagnitude.SetValue(ScaledMagnitude);
							}
						}
					}
				
					// Apply the effect
					ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
				}
			}
		}
	}

	// Execute gameplay cue for visual/audio feedback
	AActor* TargetActor = ActorInfo->AvatarActor.Get();
	if (TargetActor)
	{
		ExecuteGameplayCue(TargetActor);
	}
}

void UConceptAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// Call the parent implementation
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UConceptAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	// Call parent implementation first
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	// Check if the actor has the required concepts with sufficient mastery
	if (ActorInfo && ActorInfo->AvatarActor.IsValid())
	{
		UConceptComponent* ConceptComp = UConceptSkillFunctionLibrary::GetConceptComponent(ActorInfo->AvatarActor.Get());
		if (!ConceptComp)
		{
			return false;
		}

		// Check each required concept
		for (const auto& RequiredConcept : RequiredConcepts)
		{
			UConcept* Concept = RequiredConcept.Get();
			if (!Concept || !ConceptComp->HasAcquiredConcept(Concept))
			{
				return false; // Missing a required concept
			}

			// Check mastery level
			bool HasSufficientMastery = false;

			// Check each body part for the concept with sufficient mastery
			for (const auto& Pair : ConceptComp->BodyPartSlots)
			{
				const TArray<FConceptSlot>& Slots = Pair.Value;

				for (const FConceptSlot& Slot : Slots)
				{
					if (!Slot.IsEmpty() && Slot.HeldConcept.Get() == Concept && Slot.MasteryLevel >= RequiredMasteryLevel)
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
	}

	return true;
}

void UConceptAbility::ExecuteGameplayCue(AActor* Target)
{
	if (!Target)
	{
		return;
	}

	// Get the owning actor
	AActor* OwningActor = GetOwningActorFromActorInfo();
	if (!OwningActor)
	{
		return;
	}

	// Try to get the concept skill manager from the owning actor
	UConceptSkillManager* ConceptSkillManager = OwningActor->FindComponentByClass<UConceptSkillManager>();
	if (!ConceptSkillManager)
	{
		return;
	}

	// Get the source skill
	UConceptSkill* Skill = SourceSkill.Get();
	if (!Skill)
	{
		return;
	}

	// Execute the gameplay cue for this skill
	ConceptSkillManager->ExecuteGameplayCueForSkill(Skill, Target);
}

UConceptAttributeSet* UConceptAbility::GetConceptAttributeSet() const
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC)
	{
		return nullptr;
	}

	// Try to get the concept attribute set from the ability system component
	return Cast<UConceptAttributeSet>(ASC->GetAttributeSet(UConceptAttributeSet::StaticClass()));
}

float UConceptAbility::GetSkillPotency() const
{
	UConceptAttributeSet* AttributeSet = GetConceptAttributeSet();
	if (!AttributeSet)
	{
		return 1.0f; // Default value if attribute set is not available
	}

	return AttributeSet->GetSkillPotency();
}

float UConceptAbility::GetConceptPower() const
{
	UConceptAttributeSet* AttributeSet = GetConceptAttributeSet();
	if (!AttributeSet)
	{
		return 0.0f; // Default value if attribute set is not available
	}

	return AttributeSet->GetConceptPower();
}

float UConceptAbility::ApplyAttributeScaling(float BaseValue) const
{
	// Apply skill potency and concept power scaling
	float SkillPotencyMultiplier = GetSkillPotency();
	float ConceptPowerBonus = GetConceptPower() * 0.01f; // 1% bonus per point of concept power

	// Apply mastery scaling from the ability
	float MasteryMultiplier = MasteryScaling;

	// Calculate final value with all scaling factors
	return BaseValue * SkillPotencyMultiplier * (1.0f + ConceptPowerBonus) * MasteryMultiplier;
}
