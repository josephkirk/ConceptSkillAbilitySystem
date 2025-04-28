// Copyright Koorogi Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConceptSlot.h"
#include "Concept.h"
#include "ConceptualObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnObjectConceptAdded, UConcept*, Concept, FConceptSlot, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObjectSlotUnlocked, FConceptSlot, UnlockedSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObjectQualityChanged, int32, NewQuality);

UENUM(BlueprintType)
enum class EObjectQuality : uint8
{
	Poor UMETA(DisplayName = "Poor"),
	Common UMETA(DisplayName = "Common"),
	Uncommon UMETA(DisplayName = "Uncommon"),
	Rare UMETA(DisplayName = "Rare"),
	Epic UMETA(DisplayName = "Epic"),
	Legendary UMETA(DisplayName = "Legendary")
};

/**
 * AConceptualObject - Base class for objects that can hold concepts
 * Implements the "Concepts in the World" design pillar
 */
UCLASS()
class CONCEPTSKILLSYSTEM_API AConceptualObject : public AActor
{
	GENERATED_BODY()

public:
	AConceptualObject();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// The intrinsic concepts that are inherent to this object type
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept System")
	TArray<TSoftObjectPtr<UConcept>> IntrinsicConcepts;

	// The slots that can hold additional concepts
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept System")
	TArray<FConceptSlot> ConceptSlots;

	// The quality of the object, which affects how many slots it has and what tier of concepts it can hold
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept System")
	EObjectQuality Quality;

	// The durability of the object (0-100)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept System", meta = (ClampMin = "0", ClampMax = "100"))
	int32 Durability;

	// The chance of object degradation when adding a concept (0-1)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept System", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float DegradationChance;

	// Delegates
	UPROPERTY(BlueprintAssignable, Category = "Concept System")
	FOnObjectConceptAdded OnObjectConceptAdded;

	UPROPERTY(BlueprintAssignable, Category = "Concept System")
	FOnObjectSlotUnlocked OnObjectSlotUnlocked;

	UPROPERTY(BlueprintAssignable, Category = "Concept System")
	FOnObjectQualityChanged OnObjectQualityChanged;

	// Add a concept to an empty slot
	UFUNCTION(BlueprintCallable, Category = "Concept System")
	bool AddConcept(UConcept* Concept, bool bRiskDegradation = true);

	// Remove a concept from a slot
	UFUNCTION(BlueprintCallable, Category = "Concept System")
	bool RemoveConcept(const FGuid& SlotId);

	// Unlock a new concept slot
	UFUNCTION(BlueprintCallable, Category = "Concept System")
	bool UnlockConceptSlot(EConceptTier MaxTier = EConceptTier::Physical);

	// Upgrade the quality of the object
	UFUNCTION(BlueprintCallable, Category = "Concept System")
	bool UpgradeQuality();

	// Check if the object has a specific concept (either intrinsic or in slots)
	UFUNCTION(BlueprintCallable, Category = "Concept System")
	bool HasConcept(UConcept* Concept) const;

	// Get all concepts in this object (both intrinsic and in slots)
	UFUNCTION(BlueprintCallable, Category = "Concept System")
	TArray<TSoftObjectPtr<UConcept>> GetAllConcepts() const;

	// Get the maximum number of slots based on quality
	UFUNCTION(BlueprintCallable, Category = "Concept System")
	int32 GetMaxSlots() const;

	// Get the maximum tier of concepts this object can hold based on quality
	UFUNCTION(BlueprintCallable, Category = "Concept System")
	EConceptTier GetMaxConceptTier() const;

	// Apply degradation to the object
	UFUNCTION(BlueprintCallable, Category = "Concept System")
	bool ApplyDegradation(int32 Amount);

private:
	// Initialize slots based on quality
	void InitializeSlots();

	// Find the first empty slot that can hold the given concept
	bool FindEmptySlotForConcept(UConcept* Concept, FConceptSlot& OutSlot);

	// Find a slot by its unique ID
	bool FindSlotById(const FGuid& SlotId, FConceptSlot& OutSlot, int32& OutIndex);
};
