// Copyright Koorogi Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Concept.generated.h"

UENUM(BlueprintType)
enum class EConceptTier : uint8
{
	Physical UMETA(DisplayName = "Physical"),
	Intermediate UMETA(DisplayName = "Intermediate"),
	Advanced UMETA(DisplayName = "Advanced"),
	Abstract UMETA(DisplayName = "Abstract")
};

/**
 * UConcept - The fundamental unit of understanding and power in the world
 * Represents the inherent nature of objects, events, and abstract forces
 */
UCLASS(BlueprintType)
class CONCEPTSKILLSYSTEM_API UConcept : public UDataAsset
{
	GENERATED_BODY()

public:
	UConcept();

	// The display name of the concept
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept")
	FText DisplayName;

	// Description of the concept
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept", meta = (MultiLine = true))
	FText Description;

	// The tier of reality this concept belongs to
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept")
	EConceptTier Tier;

	// Icon representing the concept
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept")
	UTexture2D* Icon;

	// Gameplay tags associated with this concept
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept")
	FGameplayTagContainer ConceptTags;

	// The power level of this concept (affects its effectiveness)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept", meta = (ClampMin = "1", ClampMax = "100"))
	int32 Power;

	// The difficulty to acquire this concept through observation
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept", meta = (ClampMin = "1", ClampMax = "100"))
	int32 AcquisitionDifficulty;

	// Related concepts that might be unlocked or discovered when this concept is acquired
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept")
	TArray<TSoftObjectPtr<UConcept>> RelatedConcepts;

	// The chance (0-1) that observing this concept will lead to acquisition
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float BaseAcquisitionChance;

	// Get the unique identifier for this concept
	FPRIMARY_ASSET_ID GetPrimaryAssetId() const override;
};
