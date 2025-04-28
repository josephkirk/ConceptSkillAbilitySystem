// Copyright Koorogi Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "GameplayTagContainer.h"
#include "Concept.h"
#include "ConceptSkill.generated.h"

UENUM(BlueprintType)
enum class ESkillManifestationType : uint8
{
	Active UMETA(DisplayName = "Active Skill"),
	Passive UMETA(DisplayName = "Passive Ability"),
	Crafting UMETA(DisplayName = "Crafting Knowledge"),
	Proficiency UMETA(DisplayName = "Character Proficiency")
};

/**
 * UConceptSkill - Represents a skill formed from concepts
 * Implements the "Diverse Manifestation of Ability" design pillar
 */
UCLASS(BlueprintType)
class CONCEPTSKILLSYSTEM_API UConceptSkill : public UDataAsset
{
	GENERATED_BODY()

public:
	UConceptSkill();

	// The display name of the skill
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept Skill")
	FText DisplayName;

	// Description of the skill
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept Skill", meta = (MultiLine = true))
	FText Description;

	// Icon representing the skill
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept Skill")
	UTexture2D* Icon;

	// The type of manifestation this skill represents
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept Skill")
	ESkillManifestationType ManifestationType;

	// The concepts required to form this skill
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept Skill")
	TArray<TSoftObjectPtr<UConcept>> RequiredConcepts;

	// The minimum mastery level required for each concept (0-100)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept Skill", meta = (ClampMin = "0", ClampMax = "100"))
	int32 RequiredMasteryLevel;

	// The gameplay ability class this skill grants (if it's an active skill)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept Skill")
	TSubclassOf<class UGameplayAbility> GrantedAbility;

	// Gameplay tags that describe this skill
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept Skill")
	FGameplayTagContainer SkillTags;

	// Gameplay tags required to use this skill
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept Skill")
	FGameplayTagContainer RequiredTags;

	// Gameplay tags that block this skill
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept Skill")
	FGameplayTagContainer BlockedTags;

	// Gameplay effects granted by this skill (for passive abilities)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept Skill")
	TArray<TSubclassOf<class UGameplayEffect>> GrantedEffects;

	// The power level of this skill (calculated from component concepts)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept Skill", meta = (ClampMin = "1"))
	int32 Power;

	// Cooldown time for the skill (in seconds)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concept Skill", meta = (ClampMin = "0.0"))
	float Cooldown;

	// Get the unique identifier for this skill
	FPRIMARY_ASSET_ID GetPrimaryAssetId() const override;

	// Calculate the effective power based on mastery levels
	UFUNCTION(BlueprintCallable, Category = "Concept Skill")
	int32 CalculateEffectivePower(const TMap<TSoftObjectPtr<UConcept>, int32>& ConceptMasteryLevels) const;

	// Get the gameplay tag for this skill's manifestation type
	UFUNCTION(BlueprintCallable, Category = "Concept Skill")
	FGameplayTag GetManifestationTag() const;

	// Check if this skill has a specific tag
	UFUNCTION(BlueprintCallable, Category = "Concept Skill")
	bool HasSkillTag(const FGameplayTag& Tag) const;

	// Get the gameplay ability specification for this skill
	UFUNCTION(BlueprintCallable, Category = "Concept Skill")
	FGameplayAbilitySpec GetAbilitySpec(int32 Level = 1) const;
};
