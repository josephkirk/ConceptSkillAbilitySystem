// Copyright Koorogi Games, Inc. All Rights Reserved.

#include "ConceptSkill.h"

UConceptSkill::UConceptSkill()
{
	ManifestationType = ESkillManifestationType::Active;
	RequiredMasteryLevel = 50;
	Power = 10;
	Cooldown = 5.0f;
}

FPrimaryAssetId UConceptSkill::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(TEXT("ConceptSkill"), GetFName());
}

int32 UConceptSkill::CalculateEffectivePower(const TMap<TSoftObjectPtr<UConcept>, int32>& ConceptMasteryLevels) const
{
	if (RequiredConcepts.Num() == 0)
	{
		return Power;
	}
	
	// Base power from the skill itself
	int32 EffectivePower = Power;
	
	// Calculate average mastery level of all required concepts
	int32 TotalMastery = 0;
	int32 ConceptsFound = 0;
	
	for (const auto& RequiredConcept : RequiredConcepts)
	{
		if (ConceptMasteryLevels.Contains(RequiredConcept))
		{
			TotalMastery += ConceptMasteryLevels[RequiredConcept];
			ConceptsFound++;
		}
	}
	
	if (ConceptsFound == 0)
	{
		return 0; // Missing required concepts
	}
	
	// Calculate average mastery
	float AverageMastery = static_cast<float>(TotalMastery) / ConceptsFound;
	
	// Scale power based on mastery level (50% at minimum required mastery, 150% at max mastery)
	float MasteryMultiplier = 0.5f + (AverageMastery / 100.0f);
	
	// Apply multiplier to base power
	EffectivePower = FMath::RoundToInt(EffectivePower * MasteryMultiplier);
	
	return FMath::Max(1, EffectivePower);
}
