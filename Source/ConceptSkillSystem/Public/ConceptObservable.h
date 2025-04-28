// Copyright Koorogi Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Concept.h"
#include "ConceptObservable.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UConceptObservable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for objects that can be observed to learn concepts
 * Implements the "Understanding Through Observation and Interaction" design pillar
 */
class CONCEPTSKILLSYSTEM_API IConceptObservable
{
	GENERATED_BODY()

public:
	// Get the observable concepts from this object
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Concept System")
	TArray<UConcept*> GetObservableConcepts() const;

	// Get the observation quality for this object (0.0-1.0)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Concept System")
	float GetObservationQuality() const;

	// Called when this object is observed by a character
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Concept System")
	void OnObserved(AActor* Observer);

	// Check if this object can be observed
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Concept System")
	bool CanBeObserved() const;
};
