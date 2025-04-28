// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Concept.h"
#include "ConceptSkill.h"
#include "ExampleConcepts.generated.h"

/**
 * UExampleConcepts - Data asset containing example concepts and skills
 * This is used to provide sample content for the Concept Skill System
 */
UCLASS(BlueprintType)
class CONCEPTSKILLSYSTEM_API UExampleConcepts : public UDataAsset
{
	GENERATED_BODY()

public:
	UExampleConcepts();

	// Physical tier concepts
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Example Concepts|Physical")
	TArray<UConcept*> PhysicalConcepts;

	// Intermediate tier concepts
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Example Concepts|Intermediate")
	TArray<UConcept*> IntermediateConcepts;

	// Advanced tier concepts
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Example Concepts|Advanced")
	TArray<UConcept*> AdvancedConcepts;

	// Abstract tier concepts
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Example Concepts|Abstract")
	TArray<UConcept*> AbstractConcepts;

	// Example skills
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Example Concepts|Skills")
	TArray<UConceptSkill*> ExampleSkills;

	// Create a set of example concepts
	UFUNCTION(BlueprintCallable, Category = "Example Concepts")
	void CreateExampleConcepts();

	// Create a set of example skills
	UFUNCTION(BlueprintCallable, Category = "Example Concepts")
	void CreateExampleSkills();

	// Get all example concepts
	UFUNCTION(BlueprintCallable, Category = "Example Concepts")
	TArray<UConcept*> GetAllExampleConcepts() const;

	// Get example concepts by tier
	UFUNCTION(BlueprintCallable, Category = "Example Concepts")
	TArray<UConcept*> GetExampleConceptsByTier(EConceptTier Tier) const;
};
