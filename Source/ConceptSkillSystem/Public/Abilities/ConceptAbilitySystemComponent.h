// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ConceptAbilitySystemComponent.generated.h"

/**
 * UConceptAbilitySystemComponent - Extends UAbilitySystemComponent for the Concept Skill System
 * Provides integration between Concept Skills and Gameplay Abilities
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CONCEPTSKILLSYSTEM_API UConceptAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UConceptAbilitySystemComponent();

	virtual void BeginPlay() override;

	// Grant an ability based on a concept skill
	UFUNCTION(BlueprintCallable, Category = "Concept Ability System")
	FGameplayAbilitySpecHandle GrantAbilityFromConceptSkill(UConceptSkill* Skill, int32 Level = 1);

	// Remove an ability granted from a concept skill
	UFUNCTION(BlueprintCallable, Category = "Concept Ability System")
	bool RemoveAbilityFromConceptSkill(UConceptSkill* Skill);

	// Update ability levels based on concept mastery
	UFUNCTION(BlueprintCallable, Category = "Concept Ability System")
	void UpdateAbilityLevelsFromConceptMastery();

	// Get the ability spec handle for a concept skill
	UFUNCTION(BlueprintCallable, Category = "Concept Ability System")
	FGameplayAbilitySpecHandle GetAbilityHandleFromConceptSkill(UConceptSkill* Skill) const;

	// Apply a gameplay effect based on a concept
	UFUNCTION(BlueprintCallable, Category = "Concept Ability System")
	FActiveGameplayEffectHandle ApplyEffectFromConcept(UConcept* Concept, TSubclassOf<UGameplayEffect> EffectClass, float Level = 1.0f);

private:
	// Map of concept skills to their granted ability spec handles
	TMap<TSoftObjectPtr<UConceptSkill>, FGameplayAbilitySpecHandle> SkillToAbilityHandleMap;
};
