// Copyright Koorogi Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConceptSlot.h"
#include "BodyManual.generated.h"

/**
 * ABodyManual - An item that allows characters to customize their concept slots
 * Implements the "Player-Driven Specialization" design pillar
 */
UCLASS()
class CONCEPTSKILLSYSTEM_API ABodyManual : public AActor
{
	GENERATED_BODY()

public:
	ABodyManual();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// The body part this manual can modify
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept System")
	EBodyPartType TargetBodyPart;

	// The maximum tier of concept this manual can unlock
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept System")
	EConceptTier MaxUnlockableTier;

	// The number of slots this manual can unlock
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept System", meta = (ClampMin = "1"))
	int32 NumSlotsToUnlock;

	// Whether this manual is consumed on use
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept System")
	bool bConsumeOnUse;

	// Use the manual on a character
	UFUNCTION(BlueprintCallable, Category = "Concept System")
	bool UseManual(AActor* TargetCharacter);

	// Called when the manual is used successfully
	UFUNCTION(BlueprintImplementableEvent, Category = "Concept System")
	void OnManualUsed(AActor* TargetCharacter, int32 SlotsUnlocked);
};
