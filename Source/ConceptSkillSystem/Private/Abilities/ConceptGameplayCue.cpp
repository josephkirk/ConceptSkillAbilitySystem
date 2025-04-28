// Copyright Epic Games, Inc. All Rights Reserved.

#include "Abilities/ConceptGameplayCue.h"
#include "Concept.h"
#include "ConceptSkillTags.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

UConceptGameplayCue::UConceptGameplayCue()
{
	// Initialize default values
}

bool UConceptGameplayCue::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	// Call parent implementation
	Super::OnExecute_Implementation(MyTarget, Parameters);

	// Get concept tier and element from parameters
	EConceptTier ConceptTier = GetConceptTierFromParameters(Parameters);
	FName ConceptElement = GetConceptElementFromParameters(Parameters);

	// Get color based on element
	FLinearColor Color = FLinearColor::White;
	if (ElementColors.Contains(ConceptElement))
	{
		Color = ElementColors[ConceptElement];
	}

	// Spawn particle effect based on tier
	if (TierParticleEffects.Contains(ConceptTier))
	{
		UParticleSystem* ParticleSystem = TierParticleEffects[ConceptTier];
		SpawnParticleEffect(MyTarget, ParticleSystem, Color);
	}

	// Spawn niagara effect based on tier
	if (TierNiagaraEffects.Contains(ConceptTier))
	{
		UNiagaraSystem* NiagaraSystem = TierNiagaraEffects[ConceptTier];
		SpawnNiagaraEffect(MyTarget, NiagaraSystem, Color);
	}

	return true;
}

bool UConceptGameplayCue::OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	// Call parent implementation
	Super::OnActive_Implementation(MyTarget, Parameters);

	// Get concept tier from parameters
	EConceptTier ConceptTier = GetConceptTierFromParameters(Parameters);

	// Play sound effect based on tier
	if (TierSoundEffects.Contains(ConceptTier))
	{
		USoundBase* Sound = TierSoundEffects[ConceptTier];
		PlaySoundEffect(MyTarget, Sound);
	}

	return true;
}

EConceptTier UConceptGameplayCue::GetConceptTierFromParameters(const FGameplayCueParameters& Parameters) const
{
	// Default to Physical tier
	EConceptTier ConceptTier = EConceptTier::Physical;

	// Check for tier tags
	FGameplayTagContainer Tags;
	Parameters.EffectContext.GetOwnedGameplayTags(Tags);

	// Check for specific tier tags
	if (Tags.HasTag(FConceptSkillTags::Get().Tier_Physical))
	{
		ConceptTier = EConceptTier::Physical;
	}
	else if (Tags.HasTag(FConceptSkillTags::Get().Tier_Intermediate))
	{
		ConceptTier = EConceptTier::Intermediate;
	}
	else if (Tags.HasTag(FConceptSkillTags::Get().Tier_Advanced))
	{
		ConceptTier = EConceptTier::Advanced;
	}
	else if (Tags.HasTag(FConceptSkillTags::Get().Tier_Abstract))
	{
		ConceptTier = EConceptTier::Abstract;
	}

	return ConceptTier;
}

FName UConceptGameplayCue::GetConceptElementFromParameters(const FGameplayCueParameters& Parameters) const
{
	// Default element
	FName ConceptElement = TEXT("None");

	// Check for element tags
	FGameplayTagContainer Tags;
	Parameters.EffectContext.GetOwnedGameplayTags(Tags);

	// Check for specific element tags
	if (Tags.HasTag(FConceptSkillTags::Get().Element_Fire))
	{
		ConceptElement = TEXT("Fire");
	}
	else if (Tags.HasTag(FConceptSkillTags::Get().Element_Water))
	{
		ConceptElement = TEXT("Water");
	}
	else if (Tags.HasTag(FConceptSkillTags::Get().Element_Earth))
	{
		ConceptElement = TEXT("Earth");
	}
	else if (Tags.HasTag(FConceptSkillTags::Get().Element_Air))
	{
		ConceptElement = TEXT("Air");
	}
	else if (Tags.HasTag(FConceptSkillTags::Get().Element_Light))
	{
		ConceptElement = TEXT("Light");
	}
	else if (Tags.HasTag(FConceptSkillTags::Get().Element_Dark))
	{
		ConceptElement = TEXT("Dark");
	}

	return ConceptElement;
}

void UConceptGameplayCue::SpawnParticleEffect(AActor* Target, UParticleSystem* ParticleSystem, FLinearColor Color)
{
	if (!Target || !ParticleSystem)
	{
		return;
	}

	// Spawn particle system at target location
	UParticleSystemComponent* ParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(
		Target->GetWorld(),
		ParticleSystem,
		Target->GetActorLocation(),
		Target->GetActorRotation());

	// Set color parameter if available
	if (ParticleComponent)
	{
		ParticleComponent->SetColorParameter(TEXT("Color"), Color);
	}
}

void UConceptGameplayCue::SpawnNiagaraEffect(AActor* Target, UNiagaraSystem* NiagaraSystem, FLinearColor Color)
{
	if (!Target || !NiagaraSystem)
	{
		return;
	}

	// Spawn niagara system at target location
	UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		Target->GetWorld(),
		NiagaraSystem,
		Target->GetActorLocation(),
		Target->GetActorRotation());

	// Set color parameter if available
	if (NiagaraComponent)
	{
		NiagaraComponent->SetVariableLinearColor(TEXT("Color"), Color);
	}
}

void UConceptGameplayCue::PlaySoundEffect(AActor* Target, USoundBase* Sound)
{
	if (!Target || !Sound)
	{
		return;
	}

	// Play sound at target location
	UGameplayStatics::PlaySoundAtLocation(
		Target->GetWorld(),
		Sound,
		Target->GetActorLocation());
}
