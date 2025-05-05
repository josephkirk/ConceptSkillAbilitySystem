// Copyright Koorogi Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Concept.h"
#include "ConceptSlot.generated.h"

UENUM(BlueprintType)
enum class EBodyPartType : uint8
{
	Head UMETA(DisplayName = "Head"),
	Body UMETA(DisplayName = "Body"),
	Arms UMETA(DisplayName = "Arms"),
	Feet UMETA(DisplayName = "Feet"),
	None UMETA(DisplayName = "None") // For object slots that aren't tied to body parts
};

/**
 * FConceptSlot - Represents a slot that can hold a concept
 * Used by both characters (body parts) and objects
 */
USTRUCT(BlueprintType)
struct CONCEPTSKILLSYSTEM_API FConceptSlot
{
	GENERATED_BODY()

public:
	FConceptSlot();

	// The concept held in this slot (if any)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept Slot")
	TSoftObjectPtr<UConcept> HeldConcept;

	// The body part this slot is associated with (for character slots)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept Slot")
	EBodyPartType BodyPart;

	// Whether this slot is unlocked and available for use
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept Slot")
	bool bIsUnlocked;

	// The mastery level of the concept in this slot (0-100)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept Slot", meta = (ClampMin = "0", ClampMax = "100"))
	int32 MasteryLevel;

	// The maximum tier of concept this slot can hold
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept Slot")
	EConceptTier MaxTier;

	// Unique identifier for this slot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept Slot")
	FGuid SlotId;

	// Grid coordinates for inventory system based on user suggestion
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept Slot")
	int32 XCoordinate;  // X-coordinate in the grid

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept Slot")
	int32 YCoordinate;  // Y-coordinate in the grid

	// Check if the slot is empty
	bool IsEmpty() const;

	// Check if the slot can hold the given concept
	bool CanHoldConcept(const UConcept* Concept) const;

	// Set the concept in this slot
	bool SetConcept(TSoftObjectPtr<UConcept> NewConcept);

	// Clear the concept from this slot
	void ClearConcept();

	// Increase the mastery level of the concept in this slot
	void IncreaseMastery(int32 Amount);
};
