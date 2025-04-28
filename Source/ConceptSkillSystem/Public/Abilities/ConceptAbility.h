// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ConceptSkill.h"
#include "Abilities/ConceptAttributeSet.h"
#include "ConceptAbility.generated.h"

/**
 * UConceptAbility - Base class for abilities in the Concept Skill System
 * Extends UGameplayAbility to integrate with the Gameplay Ability System
 */
UCLASS()
class CONCEPTSKILLSYSTEM_API UConceptAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UConceptAbility();

	// The concept skill that this ability is associated with
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept Ability")
	TSoftObjectPtr<UConceptSkill> SourceSkill;

	// The concepts required for this ability
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept Ability")
	TArray<TSoftObjectPtr<UConcept>> RequiredConcepts;

	// The minimum mastery level required for each concept
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept Ability", meta = (ClampMin = "0", ClampMax = "100"))
	int32 RequiredMasteryLevel;

	// The power scaling of the ability based on concept mastery
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept Ability", meta = (ClampMin = "0.0", ClampMax = "2.0"))
	float MasteryScaling;

	// Tags that describe this ability
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept Ability")
	FGameplayTagContainer AbilityTags;

	// Tags that are applied to the target of this ability
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept Ability")
	FGameplayTagContainer TargetTags;

	// Calculate the effective power of this ability based on the activator's concept mastery levels
	UFUNCTION(BlueprintCallable, Category = "Concept Ability")
	virtual float CalculateAbilityPower(const AActor* SourceActor) const;

	// Get the gameplay effects to apply with this ability
	UFUNCTION(BlueprintCallable, Category = "Concept Ability")
	virtual TArray<TSubclassOf<UGameplayEffect>> GetAbilityEffects() const;

	// Override to add concept-specific activation logic
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// Override to add concept-specific end ability logic
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	// Override to check concept requirements
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const override;

	// Execute a gameplay cue for this ability
	UFUNCTION(BlueprintCallable, Category = "Concept Ability")
	virtual void ExecuteGameplayCue(AActor* Target);

	// Get the attribute set from the ability system component
	UFUNCTION(BlueprintCallable, Category = "Concept Ability")
	virtual UConceptAttributeSet* GetConceptAttributeSet() const;

	// Get the skill potency from the attribute set
	UFUNCTION(BlueprintCallable, Category = "Concept Ability")
	virtual float GetSkillPotency() const;

	// Get the concept power from the attribute set
	UFUNCTION(BlueprintCallable, Category = "Concept Ability")
	virtual float GetConceptPower() const;

	// Apply ability scaling based on attributes
	UFUNCTION(BlueprintCallable, Category = "Concept Ability")
	virtual float ApplyAttributeScaling(float BaseValue) const;

protected:
	// Gameplay effects to apply when the ability is activated
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Concept Ability")
	TArray<TSubclassOf<UGameplayEffect>> AbilityEffects;
};
