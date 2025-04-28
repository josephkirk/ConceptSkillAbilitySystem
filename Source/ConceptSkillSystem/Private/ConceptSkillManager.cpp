// Copyright Koorogi Games, Inc. All Rights Reserved.

#include "ConceptSkillManager.h"
#include "Abilities/ConceptAbilitySystemComponent.h"
#include "Abilities/ConceptAbility.h"
#include "Abilities/ConceptGameplayEffect.h"
#include "ConceptSkillTags.h"
#include "GameplayCueManager.h"

UConceptSkillManager::UConceptSkillManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UConceptSkillManager::BeginPlay()
{
	Super::BeginPlay();

	// Get the concept component from the same actor
	ConceptComponent = GetOwner()->FindComponentByClass<UConceptComponent>();

	// Get the ability system component
	if (ConceptComponent)
	{
		AbilitySystemComponent = ConceptComponent->GetAbilitySystemComponent();
	}
	else
	{
		AbilitySystemComponent = GetOwner()->FindComponentByClass<UAbilitySystemComponent>();
	}

	// Check for skills that can be unlocked with starting concepts
	CheckForNewSkills();

	// Grant abilities for already unlocked skills
	GrantAbilitiesForUnlockedSkills();

	// Apply passive effects
	ApplyPassiveEffects();
}

void UConceptSkillManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UConceptSkillManager::CheckForNewSkills()
{
	if (!ConceptComponent)
	{
		return;
	}

	// Check each available skill to see if it can be unlocked
	for (const auto& SkillPtr : AvailableSkills)
	{
		UConceptSkill* Skill = SkillPtr.Get();
		if (Skill && !UnlockedSkills.Contains(Skill))
		{
			if (CanUnlockSkill(Skill))
			{
				UnlockSkill(Skill);
			}
		}
	}
}

bool UConceptSkillManager::UnlockSkill(UConceptSkill* Skill)
{
	if (!Skill || UnlockedSkills.Contains(Skill))
	{
		return false;
	}

	// Add to unlocked skills
	UnlockedSkills.Add(Skill);

	// Categorize the skill based on its manifestation type
	CategorizeSkill(Skill);

	// Grant the ability if it's an active skill
	if (Skill->ManifestationType == ESkillManifestationType::Active && Skill->GrantedAbility)
	{
		GrantAbilityForSkill(Skill);
	}
	// Apply passive effects if it's a passive ability
	else if (Skill->ManifestationType == ESkillManifestationType::Passive && Skill->GrantedEffects.Num() > 0)
	{
		ApplyPassiveEffects();
	}

	// Broadcast delegate
	OnSkillUnlocked.Broadcast(Skill);

	return true;
}

bool UConceptSkillManager::RemoveSkill(UConceptSkill* Skill)
{
	if (!Skill || !UnlockedSkills.Contains(Skill))
	{
		return false;
	}

	// Remove from unlocked skills
	UnlockedSkills.Remove(Skill);

	// Remove from category
	RemoveSkillFromCategory(Skill);

	// Remove the ability if it's an active skill
	if (Skill->ManifestationType == ESkillManifestationType::Active && Skill->GrantedAbility)
	{
		RemoveAbilityForSkill(Skill);
	}
	// Remove passive effects if it's a passive ability
	else if (Skill->ManifestationType == ESkillManifestationType::Passive && Skill->GrantedEffects.Num() > 0)
	{
		ApplyPassiveEffects();
	}

	// Broadcast delegate
	OnSkillRemoved.Broadcast(Skill);

	return true;
}

bool UConceptSkillManager::CanUnlockSkill(UConceptSkill* Skill) const
{
	if (!Skill || !ConceptComponent)
	{
		return false;
	}

	// Check if all required concepts are acquired with sufficient mastery
	for (const auto& RequiredConcept : Skill->RequiredConcepts)
	{
		UConcept* Concept = RequiredConcept.Get();
		if (!Concept || !ConceptComponent->HasAcquiredConcept(Concept))
		{
			return false; // Missing a required concept
		}

		// Check mastery level
		bool HasSufficientMastery = false;

		// Check each body part for the concept with sufficient mastery
		for (const auto& Pair : ConceptComponent->BodyPartSlots)
		{
			const TArray<FConceptSlot>& Slots = Pair.Value;

			for (const FConceptSlot& Slot : Slots)
			{
				if (!Slot.IsEmpty() && Slot.HeldConcept.Get() == Concept && Slot.MasteryLevel >= Skill->RequiredMasteryLevel)
				{
					HasSufficientMastery = true;
					break;
				}
			}

			if (HasSufficientMastery)
			{
				break;
			}
		}

		if (!HasSufficientMastery)
		{
			return false; // Insufficient mastery for a required concept
		}
	}

	return true; // All requirements met
}

TArray<TSoftObjectPtr<UConceptSkill>> UConceptSkillManager::GetSkillsByType(ESkillManifestationType Type) const
{
	switch (Type)
	{
	case ESkillManifestationType::Active:
		return ActiveSkills;
	case ESkillManifestationType::Passive:
		return PassiveAbilities;
	case ESkillManifestationType::Crafting:
		return CraftingKnowledge;
	case ESkillManifestationType::Proficiency:
		return CharacterProficiencies;
	default:
		return TArray<TSoftObjectPtr<UConceptSkill>>();
	}
}

TMap<TSoftObjectPtr<UConcept>, int32> UConceptSkillManager::GetConceptMasteryLevels() const
{
	TMap<TSoftObjectPtr<UConcept>, int32> MasteryLevels;

	if (!ConceptComponent)
	{
		return MasteryLevels;
	}

	// Gather mastery levels from all body part slots
	for (const auto& Pair : ConceptComponent->BodyPartSlots)
	{
		const TArray<FConceptSlot>& Slots = Pair.Value;

		for (const FConceptSlot& Slot : Slots)
		{
			if (!Slot.IsEmpty())
			{
				// If the concept already exists in the map, keep the highest mastery level
				if (!MasteryLevels.Contains(Slot.HeldConcept) || MasteryLevels[Slot.HeldConcept] < Slot.MasteryLevel)
				{
					MasteryLevels.Add(Slot.HeldConcept, Slot.MasteryLevel);
				}
			}
		}
	}

	return MasteryLevels;
}

int32 UConceptSkillManager::CalculateSkillEffectivePower(UConceptSkill* Skill) const
{
	if (!Skill)
	{
		return 0;
	}

	// Get current mastery levels
	TMap<TSoftObjectPtr<UConcept>, int32> MasteryLevels = GetConceptMasteryLevels();

	// Calculate effective power based on mastery levels
	return Skill->CalculateEffectivePower(MasteryLevels);
}

void UConceptSkillManager::CategorizeSkill(UConceptSkill* Skill)
{
	if (!Skill)
	{
		return;
	}

	// Add to the appropriate category based on manifestation type
	switch (Skill->ManifestationType)
	{
	case ESkillManifestationType::Active:
		ActiveSkills.AddUnique(Skill);
		break;
	case ESkillManifestationType::Passive:
		PassiveAbilities.AddUnique(Skill);
		break;
	case ESkillManifestationType::Crafting:
		CraftingKnowledge.AddUnique(Skill);
		break;
	case ESkillManifestationType::Proficiency:
		CharacterProficiencies.AddUnique(Skill);
		break;
	}
}

void UConceptSkillManager::RemoveSkillFromCategory(UConceptSkill* Skill)
{
	if (!Skill)
	{
		return;
	}

	// Remove from the appropriate category based on manifestation type
	switch (Skill->ManifestationType)
	{
	case ESkillManifestationType::Active:
		ActiveSkills.Remove(Skill);
		break;
	case ESkillManifestationType::Passive:
		PassiveAbilities.Remove(Skill);
		break;
	case ESkillManifestationType::Crafting:
		CraftingKnowledge.Remove(Skill);
		break;
	case ESkillManifestationType::Proficiency:
		CharacterProficiencies.Remove(Skill);
		break;
	}
}

void UConceptSkillManager::GrantAbilitiesForUnlockedSkills()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	// Grant abilities for all active skills
	for (const auto& SkillPtr : ActiveSkills)
	{
		UConceptSkill* Skill = SkillPtr.Get();
		if (Skill && Skill->GrantedAbility)
		{
			GrantAbilityForSkill(Skill);
		}
	}
}

FGameplayAbilitySpecHandle UConceptSkillManager::GrantAbilityForSkill(UConceptSkill* Skill)
{
	if (!Skill || !Skill->GrantedAbility || !AbilitySystemComponent)
	{
		return FGameplayAbilitySpecHandle();
	}

	// Calculate the effective power of the skill
	int32 EffectivePower = CalculateSkillEffectivePower(Skill);

	// Convert to a reasonable level value (1-10)
	int32 Level = FMath::Max(1, FMath::Min(10, EffectivePower / 10));

	// If we have a ConceptAbilitySystemComponent, use its specialized method
	UConceptAbilitySystemComponent* ConceptASC = Cast<UConceptAbilitySystemComponent>(AbilitySystemComponent);
	if (ConceptASC)
	{
		return ConceptASC->GrantAbilityFromConceptSkill(Skill, Level);
	}

	// Otherwise use the standard method
	FGameplayAbilitySpec AbilitySpec = Skill->GetAbilitySpec(Level);
	return AbilitySystemComponent->GiveAbility(AbilitySpec);
}

bool UConceptSkillManager::RemoveAbilityForSkill(UConceptSkill* Skill)
{
	if (!Skill || !AbilitySystemComponent)
	{
		return false;
	}

	// If we have a ConceptAbilitySystemComponent, use its specialized method
	UConceptAbilitySystemComponent* ConceptASC = Cast<UConceptAbilitySystemComponent>(AbilitySystemComponent);
	if (ConceptASC)
	{
		return ConceptASC->RemoveAbilityFromConceptSkill(Skill);
	}

	// Otherwise, we need to find the ability spec handle manually
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if (Spec.SourceObject == Skill)
		{
			return AbilitySystemComponent->ClearAbility(Spec.Handle);
		}
	}

	return false;
}

void UConceptSkillManager::UpdateAbilityLevels()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	// If we have a ConceptAbilitySystemComponent, use its specialized method
	UConceptAbilitySystemComponent* ConceptASC = Cast<UConceptAbilitySystemComponent>(AbilitySystemComponent);
	if (ConceptASC)
	{
		ConceptASC->UpdateAbilityLevelsFromConceptMastery();
		return;
	}

	// Otherwise update abilities manually
	for (const auto& SkillPtr : ActiveSkills)
	{
		UConceptSkill* Skill = SkillPtr.Get();
		if (!Skill || !Skill->GrantedAbility)
		{
			continue;
		}

		// Find the ability spec
		FGameplayAbilitySpec* AbilitySpec = nullptr;
		for (FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
		{
			if (Spec.SourceObject == Skill)
			{
				AbilitySpec = &Spec;
				break;
			}
		}

		if (AbilitySpec)
		{
			// Calculate the effective power of the skill
			int32 EffectivePower = CalculateSkillEffectivePower(Skill);

			// Convert to a reasonable level value (1-10)
			int32 Level = FMath::Max(1, FMath::Min(10, EffectivePower / 10));

			// Update the level if needed
			if (AbilitySpec->Level != Level)
			{
				AbilitySpec->Level = Level;
				AbilitySystemComponent->MarkAbilitySpecDirty(*AbilitySpec);
			}
		}
	}
}

void UConceptSkillManager::ApplyPassiveEffects()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	// Remove all existing passive effects first
	// This is a simplistic approach - in a real implementation, you might want to track
	// which effects are applied and only remove/update as needed
	TArray<FActiveGameplayEffectHandle> EffectsToRemove;
	for (const auto& Pair : AbilitySystemComponent->GetActiveGameplayEffects().GetActiveEffects())
	{
		const FActiveGameplayEffect& Effect = Pair.Value;
		if (Effect.Spec.Def && Effect.Spec.Def->IsA<UConceptGameplayEffect>())
		{
			EffectsToRemove.Add(Pair.Key);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : EffectsToRemove)
	{
		AbilitySystemComponent->RemoveActiveGameplayEffect(Handle);
	}

	// Apply effects for all passive abilities
	for (const auto& SkillPtr : PassiveAbilities)
	{
		UConceptSkill* Skill = SkillPtr.Get();
		if (!Skill)
		{
			continue;
		}

		// Calculate the effective power of the skill
		int32 EffectivePower = CalculateSkillEffectivePower(Skill);

		// Convert to a reasonable level value (1-10)
		float Level = FMath::Max(1.0f, FMath::Min(10.0f, EffectivePower / 10.0f));

		// Apply each effect
		for (TSubclassOf<UGameplayEffect> EffectClass : Skill->GrantedEffects)
		{
			if (EffectClass)
			{
				FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
				EffectContext.AddSourceObject(Skill);

				FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EffectClass, Level, EffectContext);
				if (SpecHandle.IsValid())
				{
					// Set the source skill as a set-by-caller magnitude
					SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Data.SkillPower")), EffectivePower);

					// Apply the effect
					AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				}
			}
		}
	}
}

TArray<UConceptSkill*> UConceptSkillManager::GetSkillsWithTag(const FGameplayTag& Tag) const
{
	TArray<UConceptSkill*> MatchingSkills;

	for (const auto& SkillPtr : UnlockedSkills)
	{
		UConceptSkill* Skill = SkillPtr.Get();
		if (Skill && Skill->HasSkillTag(Tag))
		{
			MatchingSkills.Add(Skill);
		}
	}

	return MatchingSkills;
}

void UConceptSkillManager::ExecuteCue(const FGameplayTag& Tag, const FGameplayCueParameters& Parameters)
{
	// Get the gameplay cue manager
	UGameplayCueManager* GameplayCueManager = UAbilitySystemGlobals::Get().GetGameplayCueManager();
	if (!GameplayCueManager)
	{
		return;
	}

	// Execute the gameplay cue
	GameplayCueManager->HandleGameplayCue(GetOwner(), Tag, EGameplayCueEvent::Executed, Parameters);
}

void UConceptSkillManager::ForwardGameplayCueToParent()
{
	// Default implementation does nothing
}

void UConceptSkillManager::ExecuteGameplayCueForSkill(UConceptSkill* Skill, AActor* Target)
{
	if (!Skill || !Target)
	{
		return;
	}

	// Create gameplay cue parameters
	FGameplayCueParameters Parameters;
	Parameters.EffectCauser = GetOwner();
	Parameters.AggregatedSourceTags = Skill->SkillTags;

	// Add concept tier and element tags to the parameters
	FGameplayTagContainer AdditionalTags;

	// Add tier tag based on the highest tier concept in the skill
	EConceptTier HighestTier = EConceptTier::Physical;
	for (const auto& ConceptPair : Skill->RequiredConcepts)
	{
		if (ConceptPair.Key.Get() && ConceptPair.Key.Get()->Tier > HighestTier)
		{
			HighestTier = ConceptPair.Key.Get()->Tier;
		}
	}

	// Add the appropriate tier tag
	switch (HighestTier)
	{
	case EConceptTier::Physical:
		AdditionalTags.AddTag(FConceptSkillTags::Get().Tier_Physical);
		break;
	case EConceptTier::Intermediate:
		AdditionalTags.AddTag(FConceptSkillTags::Get().Tier_Intermediate);
		break;
	case EConceptTier::Advanced:
		AdditionalTags.AddTag(FConceptSkillTags::Get().Tier_Advanced);
		break;
	case EConceptTier::Abstract:
		AdditionalTags.AddTag(FConceptSkillTags::Get().Tier_Abstract);
		break;
	}

	// Add element tags if present in the skill tags
	if (Skill->SkillTags.HasTag(FConceptSkillTags::Get().Element_Fire))
	{
		AdditionalTags.AddTag(FConceptSkillTags::Get().Element_Fire);
	}
	else if (Skill->SkillTags.HasTag(FConceptSkillTags::Get().Element_Water))
	{
		AdditionalTags.AddTag(FConceptSkillTags::Get().Element_Water);
	}
	else if (Skill->SkillTags.HasTag(FConceptSkillTags::Get().Element_Earth))
	{
		AdditionalTags.AddTag(FConceptSkillTags::Get().Element_Earth);
	}
	else if (Skill->SkillTags.HasTag(FConceptSkillTags::Get().Element_Air))
	{
		AdditionalTags.AddTag(FConceptSkillTags::Get().Element_Air);
	}
	else if (Skill->SkillTags.HasTag(FConceptSkillTags::Get().Element_Light))
	{
		AdditionalTags.AddTag(FConceptSkillTags::Get().Element_Light);
	}
	else if (Skill->SkillTags.HasTag(FConceptSkillTags::Get().Element_Dark))
	{
		AdditionalTags.AddTag(FConceptSkillTags::Get().Element_Dark);
	}

	// Add the additional tags to the parameters
	Parameters.EffectContext.AddOwnedTags(AdditionalTags);

	// Get or create a gameplay cue tag for this skill
	FGameplayTag CueTag;
	if (SkillGameplayCueTags.Contains(Skill))
	{
		CueTag = SkillGameplayCueTags[Skill];
	}
	else
	{
		// Create a new gameplay cue tag based on the skill name
		FString CueTagString = FString::Printf(TEXT("GameplayCue.Concept.Skill.%s"), *Skill->GetName());
		CueTag = FGameplayTag::RequestGameplayTag(FName(*CueTagString));
		SkillGameplayCueTags.Add(Skill, CueTag);
	}

	// Execute the gameplay cue
	ExecuteCue(CueTag, Parameters);
}
