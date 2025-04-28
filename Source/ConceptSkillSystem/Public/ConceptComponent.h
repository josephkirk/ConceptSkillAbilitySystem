// Copyright Koorogi Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "ConceptSlot.h"
#include "Concept.h"
#include "ConceptComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnConceptAcquired, UConcept*, Concept, FConceptSlot, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnConceptMasteryChanged, UConcept*, Concept, int32, NewMasteryLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotUnlocked, FConceptSlot, UnlockedSlot);

/**
 * UConceptComponent - Component that manages a character's concept slots and abilities
 * Implements the "Embodied Knowledge" design pillar
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CONCEPTSKILLSYSTEM_API UConceptComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UConceptComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// The slots for each body part
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept System")
	TMap<EBodyPartType, TArray<FConceptSlot>> BodyPartSlots;

	// The maximum number of slots per body part (can be increased through progression)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept System")
	TMap<EBodyPartType, int32> MaxSlotsPerBodyPart;

	// The concepts that this character has observed but not yet acquired
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept System")
	TSet<TSoftObjectPtr<UConcept>> ObservedConcepts;

	// The concepts that this character has fully acquired
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept System")
	TSet<TSoftObjectPtr<UConcept>> AcquiredConcepts;

	// Delegates
	UPROPERTY(BlueprintAssignable, Category = "Concept System")
	FOnConceptAcquired OnConceptAcquired;

	UPROPERTY(BlueprintAssignable, Category = "Concept System")
	FOnConceptMasteryChanged OnConceptMasteryChanged;

	UPROPERTY(BlueprintAssignable, Category = "Concept System")
	FOnSlotUnlocked OnSlotUnlocked;

	// Observe a concept in the world, potentially leading to acquisition
	UFUNCTION(BlueprintCallable, Category = "Concept System")
	bool ObserveConcept(UConcept* Concept, float ObservationQuality = 1.0f);

	// Try to acquire a concept that has been observed
	UFUNCTION(BlueprintCallable, Category = "Concept System")
	bool TryAcquireConcept(UConcept* Concept);

	// Directly acquire a concept (for tutorial/quest rewards)
	UFUNCTION(BlueprintCallable, Category = "Concept System")
	bool AcquireConcept(UConcept* Concept, EBodyPartType TargetBodyPart);

	// Unlock a new concept slot for a specific body part
	UFUNCTION(BlueprintCallable, Category = "Concept System")
	bool UnlockConceptSlot(EBodyPartType BodyPart, EConceptTier MaxTier = EConceptTier::Physical);

	// Get all slots for a specific body part
	UFUNCTION(BlueprintCallable, Category = "Concept System")
	TArray<FConceptSlot> GetSlotsForBodyPart(EBodyPartType BodyPart) const;

	// Find the first empty slot in a body part that can hold the given concept
	UFUNCTION(BlueprintCallable, Category = "Concept System")
	bool FindEmptySlotForConcept(UConcept* Concept, EBodyPartType BodyPart, FConceptSlot& OutSlot);

	// Increase mastery of a concept in a specific slot
	UFUNCTION(BlueprintCallable, Category = "Concept System")
	bool IncreaseMastery(const FGuid& SlotId, int32 Amount);

	// Check if character has acquired a specific concept
	UFUNCTION(BlueprintCallable, Category = "Concept System")
	bool HasAcquiredConcept(UConcept* Concept) const;

	// Check if character has observed a specific concept
	UFUNCTION(BlueprintCallable, Category = "Concept System")
	bool HasObservedConcept(UConcept* Concept) const;

private:
	// Initialize slots for all body parts
	void InitializeSlots();

	// Find a slot by its unique ID
	bool FindSlotById(const FGuid& SlotId, FConceptSlot& OutSlot, EBodyPartType& OutBodyPart);

	// Update a slot in the body part slots map
	void UpdateSlot(const FConceptSlot& Slot, EBodyPartType BodyPart);
};
