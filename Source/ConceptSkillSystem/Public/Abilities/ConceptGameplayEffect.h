// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Concept.h"
#include "ConceptGameplayEffect.generated.h"

/**
 * UConceptGameplayEffect - Base class for gameplay effects in the Concept Skill System
 * Extends UGameplayEffect to integrate with the Gameplay Ability System
 */
UCLASS()
class CONCEPTSKILLSYSTEM_API UConceptGameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UConceptGameplayEffect();

	// The concept that this effect is associated with
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept Effect")
	TSoftObjectPtr<UConcept> SourceConcept;

	// The tier of the effect, which affects its potency
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept Effect")
	EConceptTier EffectTier;

	// The power scaling of the effect based on concept tier
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept Effect")
	float TierScaling;

	// Tags that describe this effect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept Effect")
	FGameplayTagContainer EffectTags;

	// Calculate the effective magnitude of this effect based on the source concept
	UFUNCTION(BlueprintCallable, Category = "Concept Effect")
	float CalculateEffectMagnitude(const UConcept* Concept, int32 MasteryLevel) const;
};
