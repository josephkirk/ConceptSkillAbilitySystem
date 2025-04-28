// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * FConceptSkillTags - Struct containing all gameplay tags used by the Concept Skill System
 */
struct FConceptSkillTags
{
public:
	static void InitializeTags();

	// Concept Tier Tags
	static FGameplayTag Concept_Tier_Physical;
	static FGameplayTag Concept_Tier_Intermediate;
	static FGameplayTag Concept_Tier_Advanced;
	static FGameplayTag Concept_Tier_Abstract;

	// Concept Element Tags
	static FGameplayTag Concept_Element_Fire;
	static FGameplayTag Concept_Element_Water;
	static FGameplayTag Concept_Element_Earth;
	static FGameplayTag Concept_Element_Air;

	// Concept Property Tags
	static FGameplayTag Concept_Property_Heat;
	static FGameplayTag Concept_Property_Cold;
	static FGameplayTag Concept_Property_Flow;
	static FGameplayTag Concept_Property_Stability;

	// Concept Abstract Tags
	static FGameplayTag Concept_Abstract_Energy;
	static FGameplayTag Concept_Abstract_Balance;
	static FGameplayTag Concept_Abstract_Harmony;
	static FGameplayTag Concept_Abstract_Creation;

	// Body Part Tags
	static FGameplayTag BodyPart_Head;
	static FGameplayTag BodyPart_Body;
	static FGameplayTag BodyPart_Arms;
	static FGameplayTag BodyPart_Feet;

	// Skill Manifestation Tags
	static FGameplayTag Skill_Manifestation_Active;
	static FGameplayTag Skill_Manifestation_Passive;
	static FGameplayTag Skill_Manifestation_Crafting;
	static FGameplayTag Skill_Manifestation_Proficiency;

	// Skill Effect Tags
	static FGameplayTag Skill_Effect_Damage;
	static FGameplayTag Skill_Effect_Healing;
	static FGameplayTag Skill_Effect_Buff;
	static FGameplayTag Skill_Effect_Debuff;
	static FGameplayTag Skill_Effect_Utility;

	// Object Quality Tags
	static FGameplayTag Object_Quality_Poor;
	static FGameplayTag Object_Quality_Common;
	static FGameplayTag Object_Quality_Uncommon;
	static FGameplayTag Object_Quality_Rare;
	static FGameplayTag Object_Quality_Epic;
	static FGameplayTag Object_Quality_Legendary;

	// Helper functions
	static FGameplayTag GetConceptTierTag(EConceptTier Tier);
	static FGameplayTag GetBodyPartTag(EBodyPartType BodyPart);
	static FGameplayTag GetSkillManifestationTag(ESkillManifestationType Type);
	static FGameplayTag GetObjectQualityTag(EObjectQuality Quality);
};
