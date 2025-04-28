// Copyright Koorogi Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Concept.h"
#include "ConceptSkill.h"
#include "ConceptRegistry.generated.h"

/**
 * UConceptRegistry - Subsystem that manages all concepts and skills in the game
 */
UCLASS()
class CONCEPTSKILLSYSTEM_API UConceptRegistry : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem

	// All concepts available in the game
	UPROPERTY(BlueprintReadOnly, Category = "Concept System")
	TArray<TSoftObjectPtr<UConcept>> AllConcepts;

	// All skills available in the game
	UPROPERTY(BlueprintReadOnly, Category = "Concept System")
	TArray<TSoftObjectPtr<UConceptSkill>> AllSkills;

	// Concepts organized by tier
	UPROPERTY(BlueprintReadOnly, Category = "Concept System")
	TMap<EConceptTier, TArray<TSoftObjectPtr<UConcept>>> ConceptsByTier;

	// Skills organized by manifestation type
	UPROPERTY(BlueprintReadOnly, Category = "Concept System")
	TMap<ESkillManifestationType, TArray<TSoftObjectPtr<UConceptSkill>>> SkillsByType;

	// Load all concepts from the content directory
	UFUNCTION(BlueprintCallable, Category = "Concept System")
	void LoadAllConcepts();

	// Load all skills from the content directory
	UFUNCTION(BlueprintCallable, Category = "Concept System")
	void LoadAllSkills();

	// Get all concepts of a specific tier
	UFUNCTION(BlueprintCallable, Category = "Concept System")
	TArray<TSoftObjectPtr<UConcept>> GetConceptsByTier(EConceptTier Tier) const;

	// Get all skills of a specific manifestation type
	UFUNCTION(BlueprintCallable, Category = "Concept System")
	TArray<TSoftObjectPtr<UConceptSkill>> GetSkillsByType(ESkillManifestationType Type) const;

	// Find a concept by name
	UFUNCTION(BlueprintCallable, Category = "Concept System")
	UConcept* FindConceptByName(const FString& ConceptName) const;

	// Find a skill by name
	UFUNCTION(BlueprintCallable, Category = "Concept System")
	UConceptSkill* FindSkillByName(const FString& SkillName) const;

	// Get all skills that require a specific concept
	UFUNCTION(BlueprintCallable, Category = "Concept System")
	TArray<TSoftObjectPtr<UConceptSkill>> GetSkillsRequiringConcept(UConcept* Concept) const;

	// Get the singleton instance
	UFUNCTION(BlueprintCallable, Category = "Concept System", meta = (WorldContext = "WorldContextObject"))
	static UConceptRegistry* GetConceptRegistry(const UObject* WorldContextObject);

private:
	// Organize concepts by tier
	void OrganizeConceptsByTier();

	// Organize skills by manifestation type
	void OrganizeSkillsByType();
};
