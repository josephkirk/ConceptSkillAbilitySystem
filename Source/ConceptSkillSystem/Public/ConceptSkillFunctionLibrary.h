// Copyright Koorogi Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ConceptComponent.h"
#include "ConceptSkillManager.h"
#include "ConceptRegistry.h"
#include "ConceptSkillFunctionLibrary.generated.h"

/**
 * UConceptSkillFunctionLibrary - Blueprint Function Library for the Concept Skill System
 */
UCLASS()
class CONCEPTSKILLSYSTEM_API UConceptSkillFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Get the concept component from an actor
	UFUNCTION(BlueprintPure, Category = "Concept Skill System", meta = (DefaultToSelf = "Actor"))
	static UConceptComponent* GetConceptComponent(AActor* Actor);

	// Get the concept skill manager from an actor
	UFUNCTION(BlueprintPure, Category = "Concept Skill System", meta = (DefaultToSelf = "Actor"))
	static UConceptSkillManager* GetConceptSkillManager(AActor* Actor);

	// Check if an actor can observe a concept observable
	UFUNCTION(BlueprintCallable, Category = "Concept Skill System")
	static bool CanObserveConcept(AActor* Observer, AActor* Observable);

	// Observe a concept observable
	UFUNCTION(BlueprintCallable, Category = "Concept Skill System")
	static bool ObserveConcept(AActor* Observer, AActor* Observable);

	// Get all concepts of a specific tier from the registry
	UFUNCTION(BlueprintCallable, Category = "Concept Skill System", meta = (WorldContext = "WorldContextObject"))
	static TArray<UConcept*> GetConceptsByTier(const UObject* WorldContextObject, EConceptTier Tier);

	// Get all skills of a specific type from the registry
	UFUNCTION(BlueprintCallable, Category = "Concept Skill System", meta = (WorldContext = "WorldContextObject"))
	static TArray<UConceptSkill*> GetSkillsByType(const UObject* WorldContextObject, ESkillManifestationType Type);

	// Calculate the chance of acquiring a concept based on observation quality and difficulty
	UFUNCTION(BlueprintPure, Category = "Concept Skill System")
	static float CalculateConceptAcquisitionChance(UConcept* Concept, float ObservationQuality);

	// Check if an object has sufficient slots for a concept
	UFUNCTION(BlueprintPure, Category = "Concept Skill System")
	static bool HasAvailableSlotForConcept(AActor* Actor, UConcept* Concept);

	// Get the mastery level of a concept for an actor
	UFUNCTION(BlueprintPure, Category = "Concept Skill System")
	static int32 GetConceptMasteryLevel(AActor* Actor, UConcept* Concept);

	// Check if two concepts can be combined to form a skill
	UFUNCTION(BlueprintCallable, Category = "Concept Skill System", meta = (WorldContext = "WorldContextObject"))
	static TArray<UConceptSkill*> GetPossibleSkillsFromConcepts(const UObject* WorldContextObject, const TArray<UConcept*>& Concepts);
};
