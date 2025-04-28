// Copyright Koorogi Games, Inc. All Rights Reserved.

#include "ConceptRegistry.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"

void UConceptRegistry::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Load all concepts and skills when the subsystem initializes
	LoadAllConcepts();
	LoadAllSkills();
}

void UConceptRegistry::Deinitialize()
{
	Super::Deinitialize();

	// Clear all data
	AllConcepts.Empty();
	AllSkills.Empty();
	ConceptsByTier.Empty();
	SkillsByType.Empty();
}

void UConceptRegistry::LoadAllConcepts()
{
	// Clear existing concepts
	AllConcepts.Empty();

	// Use asset manager to find all concept assets
	UAssetManager& AssetManager = UAssetManager::Get();
	TArray<FAssetData> AssetData;
	FARFilter Filter;
	Filter.ClassNames.Add(UConcept::StaticClass()->GetFName());
	Filter.bRecursiveClasses = true;
	AssetManager.GetAssetRegistry().GetAssets(Filter, AssetData);

	// Add all found concepts to the array
	for (const FAssetData& Asset : AssetData)
	{
		TSoftObjectPtr<UConcept> ConceptPtr(Asset.GetAsset());
		if (ConceptPtr.IsValid())
		{
			AllConcepts.Add(ConceptPtr);
		}
	}

	// Organize concepts by tier
	OrganizeConceptsByTier();
}

void UConceptRegistry::LoadAllSkills()
{
	// Clear existing skills
	AllSkills.Empty();

	// Use asset manager to find all skill assets
	UAssetManager& AssetManager = UAssetManager::Get();
	TArray<FAssetData> AssetData;
	FARFilter Filter;
	Filter.ClassNames.Add(UConceptSkill::StaticClass()->GetFName());
	Filter.bRecursiveClasses = true;
	AssetManager.GetAssetRegistry().GetAssets(Filter, AssetData);

	// Add all found skills to the array
	for (const FAssetData& Asset : AssetData)
	{
		TSoftObjectPtr<UConceptSkill> SkillPtr(Asset.GetAsset());
		if (SkillPtr.IsValid())
		{
			AllSkills.Add(SkillPtr);
		}
	}

	// Organize skills by manifestation type
	OrganizeSkillsByType();
}

TArray<TSoftObjectPtr<UConcept>> UConceptRegistry::GetConceptsByTier(EConceptTier Tier) const
{
	if (ConceptsByTier.Contains(Tier))
	{
		return ConceptsByTier[Tier];
	}

	return TArray<TSoftObjectPtr<UConcept>>();
}

TArray<TSoftObjectPtr<UConceptSkill>> UConceptRegistry::GetSkillsByType(ESkillManifestationType Type) const
{
	if (SkillsByType.Contains(Type))
	{
		return SkillsByType[Type];
	}

	return TArray<TSoftObjectPtr<UConceptSkill>>();
}

UConcept* UConceptRegistry::FindConceptByName(const FString& ConceptName) const
{
	for (const auto& ConceptPtr : AllConcepts)
	{
		UConcept* Concept = ConceptPtr.Get();
		if (Concept && Concept->GetName().Equals(ConceptName, ESearchCase::IgnoreCase))
		{
			return Concept;
		}
	}

	return nullptr;
}

UConceptSkill* UConceptRegistry::FindSkillByName(const FString& SkillName) const
{
	for (const auto& SkillPtr : AllSkills)
	{
		UConceptSkill* Skill = SkillPtr.Get();
		if (Skill && Skill->GetName().Equals(SkillName, ESearchCase::IgnoreCase))
		{
			return Skill;
		}
	}

	return nullptr;
}

TArray<TSoftObjectPtr<UConceptSkill>> UConceptRegistry::GetSkillsRequiringConcept(UConcept* Concept) const
{
	TArray<TSoftObjectPtr<UConceptSkill>> MatchingSkills;

	if (!Concept)
	{
		return MatchingSkills;
	}

	for (const auto& SkillPtr : AllSkills)
	{
		UConceptSkill* Skill = SkillPtr.Get();
		if (Skill)
		{
			for (const auto& RequiredConcept : Skill->RequiredConcepts)
			{
				if (RequiredConcept.Get() == Concept)
				{
					MatchingSkills.Add(SkillPtr);
					break;
				}
			}
		}
	}

	return MatchingSkills;
}

UConceptRegistry* UConceptRegistry::GetConceptRegistry(const UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	if (!GameInstance)
	{
		return nullptr;
	}

	return GameInstance->GetSubsystem<UConceptRegistry>();
}

void UConceptRegistry::OrganizeConceptsByTier()
{
	// Clear existing map
	ConceptsByTier.Empty();

	// Initialize arrays for each tier
	ConceptsByTier.Add(EConceptTier::Physical, TArray<TSoftObjectPtr<UConcept>>());
	ConceptsByTier.Add(EConceptTier::Intermediate, TArray<TSoftObjectPtr<UConcept>>());
	ConceptsByTier.Add(EConceptTier::Advanced, TArray<TSoftObjectPtr<UConcept>>());
	ConceptsByTier.Add(EConceptTier::Abstract, TArray<TSoftObjectPtr<UConcept>>());

	// Categorize each concept by its tier
	for (const auto& ConceptPtr : AllConcepts)
	{
		UConcept* Concept = ConceptPtr.Get();
		if (Concept)
		{
			ConceptsByTier[Concept->Tier].Add(ConceptPtr);
		}
	}
}

void UConceptRegistry::OrganizeSkillsByType()
{
	// Clear existing map
	SkillsByType.Empty();

	// Initialize arrays for each type
	SkillsByType.Add(ESkillManifestationType::Active, TArray<TSoftObjectPtr<UConceptSkill>>());
	SkillsByType.Add(ESkillManifestationType::Passive, TArray<TSoftObjectPtr<UConceptSkill>>());
	SkillsByType.Add(ESkillManifestationType::Crafting, TArray<TSoftObjectPtr<UConceptSkill>>());
	SkillsByType.Add(ESkillManifestationType::Proficiency, TArray<TSoftObjectPtr<UConceptSkill>>());

	// Categorize each skill by its manifestation type
	for (const auto& SkillPtr : AllSkills)
	{
		UConceptSkill* Skill = SkillPtr.Get();
		if (Skill)
		{
			SkillsByType[Skill->ManifestationType].Add(SkillPtr);
		}
	}
}
