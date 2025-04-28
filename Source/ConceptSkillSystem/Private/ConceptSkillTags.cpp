// Copyright Epic Games, Inc. All Rights Reserved.

#include "ConceptSkillTags.h"
#include "Concept.h"
#include "ConceptSlot.h"
#include "ConceptSkill.h"
#include "ConceptualObject.h"
#include "GameplayTagsManager.h"

// Initialize static tag variables
FGameplayTag FConceptSkillTags::Concept_Tier_Physical;
FGameplayTag FConceptSkillTags::Concept_Tier_Intermediate;
FGameplayTag FConceptSkillTags::Concept_Tier_Advanced;
FGameplayTag FConceptSkillTags::Concept_Tier_Abstract;

FGameplayTag FConceptSkillTags::Concept_Element_Fire;
FGameplayTag FConceptSkillTags::Concept_Element_Water;
FGameplayTag FConceptSkillTags::Concept_Element_Earth;
FGameplayTag FConceptSkillTags::Concept_Element_Air;

FGameplayTag FConceptSkillTags::Concept_Property_Heat;
FGameplayTag FConceptSkillTags::Concept_Property_Cold;
FGameplayTag FConceptSkillTags::Concept_Property_Flow;
FGameplayTag FConceptSkillTags::Concept_Property_Stability;

FGameplayTag FConceptSkillTags::Concept_Abstract_Energy;
FGameplayTag FConceptSkillTags::Concept_Abstract_Balance;
FGameplayTag FConceptSkillTags::Concept_Abstract_Harmony;
FGameplayTag FConceptSkillTags::Concept_Abstract_Creation;

FGameplayTag FConceptSkillTags::BodyPart_Head;
FGameplayTag FConceptSkillTags::BodyPart_Body;
FGameplayTag FConceptSkillTags::BodyPart_Arms;
FGameplayTag FConceptSkillTags::BodyPart_Feet;

FGameplayTag FConceptSkillTags::Skill_Manifestation_Active;
FGameplayTag FConceptSkillTags::Skill_Manifestation_Passive;
FGameplayTag FConceptSkillTags::Skill_Manifestation_Crafting;
FGameplayTag FConceptSkillTags::Skill_Manifestation_Proficiency;

FGameplayTag FConceptSkillTags::Skill_Effect_Damage;
FGameplayTag FConceptSkillTags::Skill_Effect_Healing;
FGameplayTag FConceptSkillTags::Skill_Effect_Buff;
FGameplayTag FConceptSkillTags::Skill_Effect_Debuff;
FGameplayTag FConceptSkillTags::Skill_Effect_Utility;

FGameplayTag FConceptSkillTags::Object_Quality_Poor;
FGameplayTag FConceptSkillTags::Object_Quality_Common;
FGameplayTag FConceptSkillTags::Object_Quality_Uncommon;
FGameplayTag FConceptSkillTags::Object_Quality_Rare;
FGameplayTag FConceptSkillTags::Object_Quality_Epic;
FGameplayTag FConceptSkillTags::Object_Quality_Legendary;

void FConceptSkillTags::InitializeTags()
{
	UGameplayTagsManager& TagManager = UGameplayTagsManager::Get();

	// Concept Tier Tags
	Concept_Tier_Physical = TagManager.AddNativeGameplayTag(TEXT("Concept.Tier.Physical"), TEXT("Physical tier concepts representing basic elements and forces"));
	Concept_Tier_Intermediate = TagManager.AddNativeGameplayTag(TEXT("Concept.Tier.Intermediate"), TEXT("Intermediate tier concepts representing properties and characteristics"));
	Concept_Tier_Advanced = TagManager.AddNativeGameplayTag(TEXT("Concept.Tier.Advanced"), TEXT("Advanced tier concepts representing complex phenomena"));
	Concept_Tier_Abstract = TagManager.AddNativeGameplayTag(TEXT("Concept.Tier.Abstract"), TEXT("Abstract tier concepts representing philosophical ideas"));

	// Concept Element Tags
	Concept_Element_Fire = TagManager.AddNativeGameplayTag(TEXT("Concept.Element.Fire"), TEXT("The elemental force of heat and combustion"));
	Concept_Element_Water = TagManager.AddNativeGameplayTag(TEXT("Concept.Element.Water"), TEXT("The elemental force of fluidity and life"));
	Concept_Element_Earth = TagManager.AddNativeGameplayTag(TEXT("Concept.Element.Earth"), TEXT("The elemental force of solidity and stability"));
	Concept_Element_Air = TagManager.AddNativeGameplayTag(TEXT("Concept.Element.Air"), TEXT("The elemental force of movement and breath"));

	// Concept Property Tags
	Concept_Property_Heat = TagManager.AddNativeGameplayTag(TEXT("Concept.Property.Heat"), TEXT("The transfer of thermal energy"));
	Concept_Property_Cold = TagManager.AddNativeGameplayTag(TEXT("Concept.Property.Cold"), TEXT("The absence of thermal energy"));
	Concept_Property_Flow = TagManager.AddNativeGameplayTag(TEXT("Concept.Property.Flow"), TEXT("Continuous movement in a direction"));
	Concept_Property_Stability = TagManager.AddNativeGameplayTag(TEXT("Concept.Property.Stability"), TEXT("Resistance to change or disturbance"));

	// Concept Abstract Tags
	Concept_Abstract_Energy = TagManager.AddNativeGameplayTag(TEXT("Concept.Abstract.Energy"), TEXT("The capacity to do work and cause change"));
	Concept_Abstract_Balance = TagManager.AddNativeGameplayTag(TEXT("Concept.Abstract.Balance"), TEXT("The state of equilibrium between opposing forces"));
	Concept_Abstract_Harmony = TagManager.AddNativeGameplayTag(TEXT("Concept.Abstract.Harmony"), TEXT("The state of perfect agreement and unity"));
	Concept_Abstract_Creation = TagManager.AddNativeGameplayTag(TEXT("Concept.Abstract.Creation"), TEXT("The act of bringing something into existence"));

	// Body Part Tags
	BodyPart_Head = TagManager.AddNativeGameplayTag(TEXT("BodyPart.Head"), TEXT("The head body part"));
	BodyPart_Body = TagManager.AddNativeGameplayTag(TEXT("BodyPart.Body"), TEXT("The body/torso body part"));
	BodyPart_Arms = TagManager.AddNativeGameplayTag(TEXT("BodyPart.Arms"), TEXT("The arms body part"));
	BodyPart_Feet = TagManager.AddNativeGameplayTag(TEXT("BodyPart.Feet"), TEXT("The feet body part"));

	// Skill Manifestation Tags
	Skill_Manifestation_Active = TagManager.AddNativeGameplayTag(TEXT("Skill.Manifestation.Active"), TEXT("Active skills that can be used directly"));
	Skill_Manifestation_Passive = TagManager.AddNativeGameplayTag(TEXT("Skill.Manifestation.Passive"), TEXT("Passive abilities that provide constant benefits"));
	Skill_Manifestation_Crafting = TagManager.AddNativeGameplayTag(TEXT("Skill.Manifestation.Crafting"), TEXT("Crafting knowledge for creating items"));
	Skill_Manifestation_Proficiency = TagManager.AddNativeGameplayTag(TEXT("Skill.Manifestation.Proficiency"), TEXT("Character proficiencies that improve capabilities"));

	// Skill Effect Tags
	Skill_Effect_Damage = TagManager.AddNativeGameplayTag(TEXT("Skill.Effect.Damage"), TEXT("Skills that deal damage"));
	Skill_Effect_Healing = TagManager.AddNativeGameplayTag(TEXT("Skill.Effect.Healing"), TEXT("Skills that provide healing"));
	Skill_Effect_Buff = TagManager.AddNativeGameplayTag(TEXT("Skill.Effect.Buff"), TEXT("Skills that provide positive effects"));
	Skill_Effect_Debuff = TagManager.AddNativeGameplayTag(TEXT("Skill.Effect.Debuff"), TEXT("Skills that apply negative effects"));
	Skill_Effect_Utility = TagManager.AddNativeGameplayTag(TEXT("Skill.Effect.Utility"), TEXT("Skills that provide utility functions"));

	// Object Quality Tags
	Object_Quality_Poor = TagManager.AddNativeGameplayTag(TEXT("Object.Quality.Poor"), TEXT("Poor quality objects"));
	Object_Quality_Common = TagManager.AddNativeGameplayTag(TEXT("Object.Quality.Common"), TEXT("Common quality objects"));
	Object_Quality_Uncommon = TagManager.AddNativeGameplayTag(TEXT("Object.Quality.Uncommon"), TEXT("Uncommon quality objects"));
	Object_Quality_Rare = TagManager.AddNativeGameplayTag(TEXT("Object.Quality.Rare"), TEXT("Rare quality objects"));
	Object_Quality_Epic = TagManager.AddNativeGameplayTag(TEXT("Object.Quality.Epic"), TEXT("Epic quality objects"));
	Object_Quality_Legendary = TagManager.AddNativeGameplayTag(TEXT("Object.Quality.Legendary"), TEXT("Legendary quality objects"));

	// Make sure all tags are registered
	TagManager.RegisterNativeTags();
}

FGameplayTag FConceptSkillTags::GetConceptTierTag(EConceptTier Tier)
{
	switch (Tier)
	{
	case EConceptTier::Physical:
		return Concept_Tier_Physical;
	case EConceptTier::Intermediate:
		return Concept_Tier_Intermediate;
	case EConceptTier::Advanced:
		return Concept_Tier_Advanced;
	case EConceptTier::Abstract:
		return Concept_Tier_Abstract;
	default:
		return FGameplayTag::EmptyTag;
	}
}

FGameplayTag FConceptSkillTags::GetBodyPartTag(EBodyPartType BodyPart)
{
	switch (BodyPart)
	{
	case EBodyPartType::Head:
		return BodyPart_Head;
	case EBodyPartType::Body:
		return BodyPart_Body;
	case EBodyPartType::Arms:
		return BodyPart_Arms;
	case EBodyPartType::Feet:
		return BodyPart_Feet;
	default:
		return FGameplayTag::EmptyTag;
	}
}

FGameplayTag FConceptSkillTags::GetSkillManifestationTag(ESkillManifestationType Type)
{
	switch (Type)
	{
	case ESkillManifestationType::Active:
		return Skill_Manifestation_Active;
	case ESkillManifestationType::Passive:
		return Skill_Manifestation_Passive;
	case ESkillManifestationType::Crafting:
		return Skill_Manifestation_Crafting;
	case ESkillManifestationType::Proficiency:
		return Skill_Manifestation_Proficiency;
	default:
		return FGameplayTag::EmptyTag;
	}
}

FGameplayTag FConceptSkillTags::GetObjectQualityTag(EObjectQuality Quality)
{
	switch (Quality)
	{
	case EObjectQuality::Poor:
		return Object_Quality_Poor;
	case EObjectQuality::Common:
		return Object_Quality_Common;
	case EObjectQuality::Uncommon:
		return Object_Quality_Uncommon;
	case EObjectQuality::Rare:
		return Object_Quality_Rare;
	case EObjectQuality::Epic:
		return Object_Quality_Epic;
	case EObjectQuality::Legendary:
		return Object_Quality_Legendary;
	default:
		return FGameplayTag::EmptyTag;
	}
}
