// Copyright Koorogi Games, Inc. All Rights Reserved.

#include "BodyManual.h"
#include "ConceptComponent.h"

ABodyManual::ABodyManual()
{
	PrimaryActorTick.bCanEverTick = true;

	TargetBodyPart = EBodyPartType::Body;
	MaxUnlockableTier = EConceptTier::Intermediate;
	NumSlotsToUnlock = 1;
	bConsumeOnUse = true;
}

void ABodyManual::BeginPlay()
{
	Super::BeginPlay();
}

void ABodyManual::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ABodyManual::UseManual(AActor* TargetCharacter)
{
	if (!TargetCharacter)
	{
		return false;
	}

	// Find the concept component on the target character
	UConceptComponent* ConceptComp = TargetCharacter->FindComponentByClass<UConceptComponent>();
	if (!ConceptComp)
	{
		return false;
	}

	// Track how many slots were successfully unlocked
	int32 SlotsUnlocked = 0;

	// Try to unlock the specified number of slots
	for (int32 i = 0; i < NumSlotsToUnlock; ++i)
	{
		if (ConceptComp->UnlockConceptSlot(TargetBodyPart, MaxUnlockableTier))
		{
			SlotsUnlocked++;
		}
		else
		{
			// No more slots to unlock
			break;
		}
	}

	// If any slots were unlocked, the manual was used successfully
	if (SlotsUnlocked > 0)
	{
		// Call the blueprint event
		OnManualUsed(TargetCharacter, SlotsUnlocked);

		// Consume the manual if needed
		if (bConsumeOnUse)
		{
			Destroy();
		}

		return true;
	}

	return false;
}
