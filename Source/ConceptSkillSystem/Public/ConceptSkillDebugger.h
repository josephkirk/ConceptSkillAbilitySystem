// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Concept.h"
#include "ConceptSkill.h"
#include "ConceptComponent.h"
#include "ConceptSkillManager.h"
#include "ConceptSkillDebugger.generated.h"

/**
 * AConceptSkillDebugger - Utility actor for debugging and testing the Concept Skill System
 */
UCLASS()
class CONCEPTSKILLSYSTEM_API AConceptSkillDebugger : public AActor
{
	GENERATED_BODY()

public:
	AConceptSkillDebugger();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// The target actor to debug
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept Skill Debugger")
	AActor* TargetActor;

	// Whether to automatically debug on begin play
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept Skill Debugger")
	bool bDebugOnBeginPlay;

	// Debug the concept component of the target actor
	UFUNCTION(BlueprintCallable, Category = "Concept Skill Debugger")
	void DebugConceptComponent();

	// Debug the concept skill manager of the target actor
	UFUNCTION(BlueprintCallable, Category = "Concept Skill Debugger")
	void DebugConceptSkillManager();

	// Add a test concept to the target actor
	UFUNCTION(BlueprintCallable, Category = "Concept Skill Debugger")
	bool AddTestConcept(UConcept* Concept, EBodyPartType BodyPart);

	// Increase mastery of a concept in the target actor
	UFUNCTION(BlueprintCallable, Category = "Concept Skill Debugger")
	bool IncreaseMastery(UConcept* Concept, int32 Amount);

	// Check for new skills that can be unlocked
	UFUNCTION(BlueprintCallable, Category = "Concept Skill Debugger")
	void CheckForNewSkills();

	// Print all concepts in the target actor
	UFUNCTION(BlueprintCallable, Category = "Concept Skill Debugger")
	void PrintAllConcepts();

	// Print all skills in the target actor
	UFUNCTION(BlueprintCallable, Category = "Concept Skill Debugger")
	void PrintAllSkills();

private:
	// Get the concept component from the target actor
	UConceptComponent* GetConceptComponent() const;

	// Get the concept skill manager from the target actor
	UConceptSkillManager* GetConceptSkillManager() const;

	// Print debug information to the screen and log
	void PrintDebug(const FString& Message, const FColor& Color = FColor::White, float Duration = 5.0f);
};
