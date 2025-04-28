// Copyright Epic Games, Inc. All Rights Reserved.

#include "Abilities/ConceptGameplayEffect.h"
#include "ConceptSkillTags.h"

UConceptGameplayEffect::UConceptGameplayEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
	EffectTier = EConceptTier::Physical;
	TierScaling = 0.25f; // 25% increase per tier
}

float UConceptGameplayEffect::CalculateEffectMagnitude(const UConcept* Concept, int32 MasteryLevel) const
{
	if (!Concept)
	{
		return 1.0f;
	}

	// Base magnitude is 1.0
	float Magnitude = 1.0f;

	// Scale based on concept power
	Magnitude *= (Concept->Power / 50.0f); // Normalize to a reasonable range

	// Scale based on mastery level (0-100%)
	float MasteryMultiplier = 1.0f + (MasteryLevel / 100.0f);
	Magnitude *= MasteryMultiplier;

	// Scale based on concept tier
	int32 ConceptTierValue = static_cast<int32>(Concept->Tier);
	int32 EffectTierValue = static_cast<int32>(EffectTier);
	int32 TierDifference = ConceptTierValue - EffectTierValue;

	// If concept tier is higher than effect tier, increase magnitude
	if (TierDifference > 0)
	{
		Magnitude *= (1.0f + (TierDifference * TierScaling));
	}
	// If concept tier is lower than effect tier, decrease magnitude
	else if (TierDifference < 0)
	{
		Magnitude *= (1.0f / (1.0f + (FMath::Abs(TierDifference) * TierScaling)));
	}

	return Magnitude;
}
