// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Abilities/ConceptAttributeSet.h"
#include "ConceptCharacter.generated.h"

/**
 * AConceptCharacter - Base character class for the Concept Skill System
 * Implements the AbilitySystemInterface for integration with the Gameplay Ability System
 */
UCLASS()
class CONCEPTSKILLSYSTEM_API AConceptCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AConceptCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	// IAbilitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Get the concept component
	UFUNCTION(BlueprintCallable, Category = "Concept Character")
	class UConceptComponent* GetConceptComponent() const;

	// Get the concept skill manager
	UFUNCTION(BlueprintCallable, Category = "Concept Character")
	class UConceptSkillManager* GetConceptSkillManager() const;

	// Get the attribute set
	UFUNCTION(BlueprintCallable, Category = "Concept Character")
	class UConceptAttributeSet* GetAttributeSet() const;

	// Observe a concept from an object
	UFUNCTION(BlueprintCallable, Category = "Concept Character")
	bool ObserveConcept(AActor* ObservableObject);

	// Acquire a concept directly
	UFUNCTION(BlueprintCallable, Category = "Concept Character")
	bool AcquireConcept(class UConcept* Concept, enum class EBodyPartType BodyPart);

	// Unlock a skill if requirements are met
	UFUNCTION(BlueprintCallable, Category = "Concept Character")
	bool UnlockSkill(class UConceptSkill* Skill);

	// Check for new skills that can be unlocked
	UFUNCTION(BlueprintCallable, Category = "Concept Character")
	void CheckForNewSkills();

protected:
	// The concept component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Concept Character")
	class UConceptComponent* ConceptComponent;

	// The concept skill manager
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Concept Character")
	class UConceptSkillManager* ConceptSkillManager;

	// The ability system component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Concept Character")
	class UAbilitySystemComponent* AbilitySystemComponent;

	// The attribute set
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Concept Character")
	class UConceptAttributeSet* AttributeSet;

private:
	// Initialize ability system
	void InitializeAbilitySystem();
};
