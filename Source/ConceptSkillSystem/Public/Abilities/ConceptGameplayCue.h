// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "ConceptGameplayCue.generated.h"

/**
 * UConceptGameplayCue - Base class for concept-related gameplay cues
 * Provides visual and audio feedback for concept abilities and effects
 */
UCLASS()
class CONCEPTSKILLSYSTEM_API UConceptGameplayCue : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	UConceptGameplayCue();

	// Override to provide custom visual effects based on concept tier
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;

	// Override to provide custom audio effects based on concept tier
	virtual bool OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;

	// Get the concept tier from the gameplay cue parameters
	EConceptTier GetConceptTierFromParameters(const FGameplayCueParameters& Parameters) const;

	// Get the concept element from the gameplay cue parameters
	FName GetConceptElementFromParameters(const FGameplayCueParameters& Parameters) const;

protected:
	// Different particle systems for different concept tiers
	UPROPERTY(EditDefaultsOnly, Category = "Concept Gameplay Cue|VFX")
	TMap<EConceptTier, UParticleSystem*> TierParticleEffects;

	// Different sounds for different concept tiers
	UPROPERTY(EditDefaultsOnly, Category = "Concept Gameplay Cue|Audio")
	TMap<EConceptTier, USoundBase*> TierSoundEffects;

	// Different niagara systems for different concept tiers
	UPROPERTY(EditDefaultsOnly, Category = "Concept Gameplay Cue|VFX")
	TMap<EConceptTier, UNiagaraSystem*> TierNiagaraEffects;

	// Different colors for different concept elements
	UPROPERTY(EditDefaultsOnly, Category = "Concept Gameplay Cue|VFX")
	TMap<FName, FLinearColor> ElementColors;

	// Spawn particle effect at target location
	void SpawnParticleEffect(AActor* Target, UParticleSystem* ParticleSystem, FLinearColor Color);

	// Spawn niagara effect at target location
	void SpawnNiagaraEffect(AActor* Target, UNiagaraSystem* NiagaraSystem, FLinearColor Color);

	// Play sound at target location
	void PlaySoundEffect(AActor* Target, USoundBase* Sound);
};
