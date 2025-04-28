// Copyright Koorogi Games, Inc. All Rights Reserved.

#include "ConceptObservableActor.h"
#include "ConceptComponent.h"

AConceptObservableActor::AConceptObservableActor()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseObservationQuality = 0.5f;
	bCanBeObserved = true;
	ObservationCooldown = 60.0f; // 1 minute cooldown by default
	LastObservedTime = -ObservationCooldown; // Allow immediate observation when game starts
}

void AConceptObservableActor::BeginPlay()
{
	Super::BeginPlay();
}

void AConceptObservableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TArray<UConcept*> AConceptObservableActor::GetObservableConcepts_Implementation() const
{
	TArray<UConcept*> Concepts;

	// Convert soft pointers to regular pointers
	for (const auto& ConceptPtr : ObservableConcepts)
	{
		if (UConcept* Concept = ConceptPtr.Get())
		{
			Concepts.Add(Concept);
		}
	}

	return Concepts;
}

float AConceptObservableActor::GetObservationQuality_Implementation() const
{
	return BaseObservationQuality;
}

void AConceptObservableActor::OnObserved_Implementation(AActor* Observer)
{
	// Update last observed time
	LastObservedTime = GetWorld()->GetTimeSeconds();

	// Try to find a concept component on the observer
	if (Observer)
	{
		UConceptComponent* ConceptComp = Observer->FindComponentByClass<UConceptComponent>();
		if (ConceptComp)
		{
			// Let the observer's concept component observe each concept
			for (UConcept* Concept : GetObservableConcepts_Implementation())
			{
				ConceptComp->ObserveConcept(Concept, GetObservationQuality_Implementation());
			}
		}
	}

	// Call blueprint event
	BP_OnObserved(Observer);
}

bool AConceptObservableActor::CanBeObserved_Implementation() const
{
	return bCanBeObserved && HasObservationCooldownElapsed();
}

bool AConceptObservableActor::HasObservationCooldownElapsed() const
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	return (CurrentTime - LastObservedTime) >= ObservationCooldown;
}
