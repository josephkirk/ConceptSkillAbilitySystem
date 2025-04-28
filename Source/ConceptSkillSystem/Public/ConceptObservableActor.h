// Copyright Koorogi Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConceptObservable.h"
#include "Concept.h"
#include "ConceptObservableActor.generated.h"

/**
 * AConceptObservableActor - Base class for actors that can be observed to learn concepts
 * Implements the "Understanding Through Observation and Interaction" design pillar
 */
UCLASS()
class CONCEPTSKILLSYSTEM_API AConceptObservableActor : public AActor, public IConceptObservable
{
	GENERATED_BODY()

public:
	AConceptObservableActor();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// The concepts that can be observed from this actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept System")
	TArray<TSoftObjectPtr<UConcept>> ObservableConcepts;

	// The base observation quality for this actor (0.0-1.0)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept System", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float BaseObservationQuality;

	// Whether this actor can currently be observed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept System")
	bool bCanBeObserved;

	// The cooldown time between observations (in seconds)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept System", meta = (ClampMin = "0.0"))
	float ObservationCooldown;

	// The time when this actor was last observed
	UPROPERTY(BlueprintReadOnly, Category = "Concept System")
	float LastObservedTime;

	// IConceptObservable interface
	virtual TArray<UConcept*> GetObservableConcepts_Implementation() const override;
	virtual float GetObservationQuality_Implementation() const override;
	virtual void OnObserved_Implementation(AActor* Observer) override;
	virtual bool CanBeObserved_Implementation() const override;

	// Called when the actor is observed
	UFUNCTION(BlueprintImplementableEvent, Category = "Concept System")
	void BP_OnObserved(AActor* Observer);

	// Check if the observation cooldown has elapsed
	UFUNCTION(BlueprintCallable, Category = "Concept System")
	bool HasObservationCooldownElapsed() const;
};
