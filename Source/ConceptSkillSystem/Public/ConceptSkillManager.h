// Copyright Koorogi Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayAbilitySpec.h"
#include "ConceptComponent.h"
#include "ConceptSkill.h"
#include "ConceptSkillManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillUnlocked, UConceptSkill*, Skill);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillRemoved, UConceptSkill*, Skill);

/**
 * UConceptSkillManager - Component that manages skill creation and usage
 * Implements the "Synergistic and Emergent Capabilities" design pillar
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CONCEPTSKILLSYSTEM_API UConceptSkillManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UConceptSkillManager();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Reference to the concept component on the same actor
	UPROPERTY(BlueprintReadOnly, Category = "Concept Skill System")
	UConceptComponent* ConceptComponent;

	// The skills this character has unlocked
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept Skill System")
	TArray<TSoftObjectPtr<UConceptSkill>> UnlockedSkills;

	// The active skills that can be used
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept Skill System")
	TArray<TSoftObjectPtr<UConceptSkill>> ActiveSkills;

	// The passive abilities that are always active
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept Skill System")
	TArray<TSoftObjectPtr<UConceptSkill>> PassiveAbilities;

	// The crafting knowledge skills
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept Skill System")
	TArray<TSoftObjectPtr<UConceptSkill>> CraftingKnowledge;

	// The character proficiencies
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept Skill System")
	TArray<TSoftObjectPtr<UConceptSkill>> CharacterProficiencies;

	// All available skills in the game (loaded from data assets)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept Skill System")
	TArray<TSoftObjectPtr<UConceptSkill>> AvailableSkills;

	// Delegates
	UPROPERTY(BlueprintAssignable, Category = "Concept Skill System")
	FOnSkillUnlocked OnSkillUnlocked;

	UPROPERTY(BlueprintAssignable, Category = "Concept Skill System")
	FOnSkillRemoved OnSkillRemoved;

	// Check for new skills that can be unlocked based on acquired concepts
	UFUNCTION(BlueprintCallable, Category = "Concept Skill System")
	void CheckForNewSkills();

	// Unlock a specific skill if requirements are met
	UFUNCTION(BlueprintCallable, Category = "Concept Skill System")
	bool UnlockSkill(UConceptSkill* Skill);

	// Remove a skill
	UFUNCTION(BlueprintCallable, Category = "Concept Skill System")
	bool RemoveSkill(UConceptSkill* Skill);

	// Check if a skill can be unlocked (has all required concepts with sufficient mastery)
	UFUNCTION(BlueprintCallable, Category = "Concept Skill System")
	bool CanUnlockSkill(UConceptSkill* Skill) const;

	// Get all skills of a specific manifestation type
	UFUNCTION(BlueprintCallable, Category = "Concept Skill System")
	TArray<TSoftObjectPtr<UConceptSkill>> GetSkillsByType(ESkillManifestationType Type) const;

	// Get the mastery levels of all concepts the character has
	UFUNCTION(BlueprintCallable, Category = "Concept Skill System")
	TMap<TSoftObjectPtr<UConcept>, int32> GetConceptMasteryLevels() const;

	// Calculate the effective power of a skill based on current concept mastery
	UFUNCTION(BlueprintCallable, Category = "Concept Skill System")
	int32 CalculateSkillEffectivePower(UConceptSkill* Skill) const;

private:
	// Categorize skills by their manifestation type
	void CategorizeSkill(UConceptSkill* Skill);

	// Remove a skill from its category
	void RemoveSkillFromCategory(UConceptSkill* Skill);
};
