// Copyright Epic Games, Inc. All Rights Reserved.

#include "Abilities/ConceptAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"

UConceptAttributeSet::UConceptAttributeSet()
{
	// Initialize attributes with default values
	ConceptPower.SetBaseValue(0.0f);
	ConceptPower.SetCurrentValue(0.0f);

	ConceptCapacity.SetBaseValue(5.0f);
	ConceptCapacity.SetCurrentValue(5.0f);

	ConceptMastery.SetBaseValue(1.0f);
	ConceptMastery.SetCurrentValue(1.0f);

	ObservationRange.SetBaseValue(500.0f);
	ObservationRange.SetCurrentValue(500.0f);

	ObservationSpeed.SetBaseValue(1.0f);
	ObservationSpeed.SetCurrentValue(1.0f);

	SkillPotency.SetBaseValue(1.0f);
	SkillPotency.SetCurrentValue(1.0f);

	CraftingProficiency.SetBaseValue(1.0f);
	CraftingProficiency.SetCurrentValue(1.0f);
}

void UConceptAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Register attributes for replication
	DOREPLIFETIME_CONDITION_NOTIFY(UConceptAttributeSet, ConceptPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UConceptAttributeSet, ConceptCapacity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UConceptAttributeSet, ConceptMastery, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UConceptAttributeSet, ObservationRange, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UConceptAttributeSet, ObservationSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UConceptAttributeSet, SkillPotency, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UConceptAttributeSet, CraftingProficiency, COND_None, REPNOTIFY_Always);
}

void UConceptAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// Clamp attributes to ensure they stay within valid ranges
	if (Attribute == GetConceptPowerAttribute())
	{
		NewValue = FMath::Max(0.0f, NewValue);
	}
	else if (Attribute == GetConceptCapacityAttribute())
	{
		NewValue = FMath::Max(1.0f, NewValue); // Minimum of 1 slot
	}
	else if (Attribute == GetConceptMasteryAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.1f, 10.0f); // Reasonable range for mastery
	}
	else if (Attribute == GetObservationRangeAttribute())
	{
		NewValue = FMath::Max(100.0f, NewValue); // Minimum observation range
	}
	else if (Attribute == GetObservationSpeedAttribute())
	{
		NewValue = FMath::Max(0.1f, NewValue); // Minimum observation speed
	}
	else if (Attribute == GetSkillPotencyAttribute())
	{
		NewValue = FMath::Max(0.1f, NewValue); // Minimum skill potency
	}
	else if (Attribute == GetCraftingProficiencyAttribute())
	{
		NewValue = FMath::Max(0.1f, NewValue); // Minimum crafting proficiency
	}
}

void UConceptAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// Get the Target actor
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	UAbilitySystemComponent* TargetAbilitySystemComponent = Data.Target.AbilityActorInfo.Get()->AbilitySystemComponent.Get();
	if (TargetAbilitySystemComponent)
	{
		TargetActor = TargetAbilitySystemComponent->GetAvatarActor();
		TargetController = TargetAbilitySystemComponent->GetAvatarActor()->GetInstigatorController();
	}

	// Get the Source actor
	AActor* SourceActor = nullptr;
	AController* SourceController = nullptr;
	UAbilitySystemComponent* SourceAbilitySystemComponent = Data.EffectSpec.GetContext().GetOriginalInstigatorAbilitySystemComponent();
	if (SourceAbilitySystemComponent)
	{
		SourceActor = SourceAbilitySystemComponent->GetAvatarActor();
		SourceController = SourceAbilitySystemComponent->GetAvatarActor()->GetInstigatorController();
	}

	// Get the DamageType
	FGameplayTagContainer EffectTags;
	Data.EffectSpec.GetAllAssetTags(EffectTags);

	// Handle specific attribute changes
	if (Data.EvaluatedData.Attribute == GetConceptPowerAttribute())
	{
		// Handle concept power changes
		// For example, notify UI or trigger special effects
	}
	else if (Data.EvaluatedData.Attribute == GetConceptCapacityAttribute())
	{
		// Handle concept capacity changes
		// This might trigger slot unlocks in the ConceptComponent
	}
}

// Replication notify callbacks
void UConceptAttributeSet::OnRep_ConceptPower(const FGameplayAttributeData& OldConceptPower)
{
	REPORTING_PROPERTY_NOTIFY(UConceptAttributeSet, ConceptPower, OldConceptPower);
}

void UConceptAttributeSet::OnRep_ConceptCapacity(const FGameplayAttributeData& OldConceptCapacity)
{
	REPORTING_PROPERTY_NOTIFY(UConceptAttributeSet, ConceptCapacity, OldConceptCapacity);
}

void UConceptAttributeSet::OnRep_ConceptMastery(const FGameplayAttributeData& OldConceptMastery)
{
	REPORTING_PROPERTY_NOTIFY(UConceptAttributeSet, ConceptMastery, OldConceptMastery);
}

void UConceptAttributeSet::OnRep_ObservationRange(const FGameplayAttributeData& OldObservationRange)
{
	REPORTING_PROPERTY_NOTIFY(UConceptAttributeSet, ObservationRange, OldObservationRange);
}

void UConceptAttributeSet::OnRep_ObservationSpeed(const FGameplayAttributeData& OldObservationSpeed)
{
	REPORTING_PROPERTY_NOTIFY(UConceptAttributeSet, ObservationSpeed, OldObservationSpeed);
}

void UConceptAttributeSet::OnRep_SkillPotency(const FGameplayAttributeData& OldSkillPotency)
{
	REPORTING_PROPERTY_NOTIFY(UConceptAttributeSet, SkillPotency, OldSkillPotency);
}

void UConceptAttributeSet::OnRep_CraftingProficiency(const FGameplayAttributeData& OldCraftingProficiency)
{
	REPORTING_PROPERTY_NOTIFY(UConceptAttributeSet, CraftingProficiency, OldCraftingProficiency);
}
