// Copyright Koorogi Games, Inc. All Rights Reserved.

#include "ConceptComponent.h"
#include "Abilities/ConceptAbilitySystemComponent.h"
#include "ConceptSkillTags.h"

UConceptComponent::UConceptComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Initialize default max slots per body part
	MaxSlotsPerBodyPart.Add(EBodyPartType::Head, 3);
	MaxSlotsPerBodyPart.Add(EBodyPartType::Body, 4);
	MaxSlotsPerBodyPart.Add(EBodyPartType::Arms, 2);
	MaxSlotsPerBodyPart.Add(EBodyPartType::Feet, 2);
}

void UConceptComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeSlots();

	// Find or create the ability system component
	AActor* Owner = GetOwner();
	if (Owner)
	{
		AbilitySystemComponent = Owner->FindComponentByClass<UAbilitySystemComponent>();
		if (!AbilitySystemComponent)
		{
			// Create a new ability system component if one doesn't exist
			AbilitySystemComponent = NewObject<UConceptAbilitySystemComponent>(Owner, TEXT("ConceptAbilitySystemComponent"));
			AbilitySystemComponent->RegisterComponent();
		}
	}
}

void UConceptComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

UAbilitySystemComponent* UConceptComponent::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void UConceptComponent::InitializeSlots()
{
	// Create initial slots for each body part based on MaxSlotsPerBodyPart
	for (const auto& Pair : MaxSlotsPerBodyPart)
	{
		EBodyPartType BodyPart = Pair.Key;
		int32 MaxSlots = Pair.Value;
		
		TArray<FConceptSlot> Slots;
		for (int32 i = 0; i < MaxSlots; ++i)
		{
			FConceptSlot NewSlot;
			NewSlot.BodyPart = BodyPart;
			
			// Only unlock the first slot for each body part by default
			NewSlot.bIsUnlocked = (i == 0);
			
			// Set max tier based on body part
			switch (BodyPart)
			{
			case EBodyPartType::Head:
				NewSlot.MaxTier = EConceptTier::Abstract; // Head can hold the highest tier concepts
				break;
			case EBodyPartType::Body:
				NewSlot.MaxTier = EConceptTier::Advanced;
				break;
			case EBodyPartType::Arms:
				NewSlot.MaxTier = EConceptTier::Intermediate;
				break;
			case EBodyPartType::Feet:
				NewSlot.MaxTier = EConceptTier::Physical;
				break;
			default:
				NewSlot.MaxTier = EConceptTier::Physical;
				break;
			}
			
			Slots.Add(NewSlot);
		}
		
		BodyPartSlots.Add(BodyPart, Slots);
	}
}

bool UConceptComponent::ObserveConcept(UConcept* Concept, float ObservationQuality)
{
	if (!Concept)
	{
		return false;
	}
	
	// Add to observed concepts
	ObservedConcepts.Add(Concept);
	
	// Calculate acquisition chance based on concept difficulty and observation quality
	float AcquisitionChance = Concept->BaseAcquisitionChance * ObservationQuality;
	AcquisitionChance = FMath::Clamp(AcquisitionChance, 0.0f, 1.0f);
	
	// Random chance to immediately acquire the concept
	if (FMath::FRand() <= AcquisitionChance)
	{
		// Try to find a suitable body part based on the concept's tier
		EBodyPartType TargetBodyPart;
		
		switch (Concept->Tier)
		{
		case EConceptTier::Abstract:
			TargetBodyPart = EBodyPartType::Head;
			break;
		case EConceptTier::Advanced:
			TargetBodyPart = EBodyPartType::Body;
			break;
		case EConceptTier::Intermediate:
			TargetBodyPart = EBodyPartType::Arms;
			break;
		case EConceptTier::Physical:
			TargetBodyPart = EBodyPartType::Feet;
			break;
		default:
			TargetBodyPart = EBodyPartType::Body;
			break;
		}
		
		return AcquireConcept(Concept, TargetBodyPart);
	}
	
	return true; // Successfully observed, but not yet acquired
}

bool UConceptComponent::TryAcquireConcept(UConcept* Concept)
{
	if (!Concept || !HasObservedConcept(Concept) || HasAcquiredConcept(Concept))
	{
		return false;
	}
	
	// Try to find a suitable body part based on the concept's tier
	EBodyPartType TargetBodyPart;
	
	switch (Concept->Tier)
	{
	case EConceptTier::Abstract:
		TargetBodyPart = EBodyPartType::Head;
		break;
	case EConceptTier::Advanced:
		TargetBodyPart = EBodyPartType::Body;
		break;
	case EConceptTier::Intermediate:
		TargetBodyPart = EBodyPartType::Arms;
		break;
	case EConceptTier::Physical:
		TargetBodyPart = EBodyPartType::Feet;
		break;
	default:
		TargetBodyPart = EBodyPartType::Body;
		break;
	}
	
	return AcquireConcept(Concept, TargetBodyPart);
}

bool UConceptComponent::AcquireConcept(UConcept* Concept, EBodyPartType TargetBodyPart)
{
	if (!Concept || HasAcquiredConcept(Concept))
	{
		return false;
	}
	
	// Find an empty slot that can hold this concept
	FConceptSlot EmptySlot;
	if (!FindEmptySlotForConcept(Concept, TargetBodyPart, EmptySlot))
	{
		// If no suitable slot found in the target body part, try other body parts
		bool FoundSlot = false;
		
		for (const auto& Pair : BodyPartSlots)
		{
			if (Pair.Key != TargetBodyPart)
			{
				if (FindEmptySlotForConcept(Concept, Pair.Key, EmptySlot))
				{
					FoundSlot = true;
					TargetBodyPart = Pair.Key;
					break;
				}
			}
		}
		
		if (!FoundSlot)
		{
			return false; // No suitable slot found in any body part
		}
	}
	
	// Set the concept in the slot
	EmptySlot.SetConcept(Concept);
	
	// Update the slot in the map
	UpdateSlot(EmptySlot, TargetBodyPart);
	
	// Add to acquired concepts
	AcquiredConcepts.Add(Concept);
	
	// Apply gameplay tags for this concept if we have an ability system component
	if (AbilitySystemComponent)
	{
		// Add the concept's tier tag
		FGameplayTag TierTag = FConceptSkillTags::GetConceptTierTag(Concept->Tier);
		if (TierTag.IsValid())
		{
			AbilitySystemComponent->AddLooseGameplayTag(TierTag);
		}

		// Add the concept's tags
		for (const FGameplayTag& Tag : Concept->ConceptTags)
		{
			AbilitySystemComponent->AddLooseGameplayTag(Tag);
		}

		// Add the body part tag
		FGameplayTag BodyPartTag = FConceptSkillTags::GetBodyPartTag(TargetBodyPart);
		if (BodyPartTag.IsValid())
		{
			AbilitySystemComponent->AddLooseGameplayTag(BodyPartTag);
		}
	}
	
	// Broadcast delegate
	OnConceptAcquired.Broadcast(Concept, EmptySlot);
	
	return true;
}

bool UConceptComponent::UnlockConceptSlot(EBodyPartType BodyPart, EConceptTier MaxTier)
{
	if (!BodyPartSlots.Contains(BodyPart))
	{
		return false;
	}
	
	TArray<FConceptSlot>& Slots = BodyPartSlots[BodyPart];
	
	// Find the first locked slot
	for (int32 i = 0; i < Slots.Num(); ++i)
	{
		if (!Slots[i].bIsUnlocked)
		{
			Slots[i].bIsUnlocked = true;
			Slots[i].MaxTier = MaxTier;
			
			// Broadcast delegate
			OnSlotUnlocked.Broadcast(Slots[i]);
			
			return true;
		}
	}
	
	return false; // No locked slots found
}

TArray<FConceptSlot> UConceptComponent::GetSlotsForBodyPart(EBodyPartType BodyPart) const
{
	if (BodyPartSlots.Contains(BodyPart))
	{
		return BodyPartSlots[BodyPart];
	}
	
	return TArray<FConceptSlot>();
}

bool UConceptComponent::FindEmptySlotForConcept(UConcept* Concept, EBodyPartType BodyPart, FConceptSlot& OutSlot)
{
	if (!Concept || !BodyPartSlots.Contains(BodyPart))
	{
		return false;
	}
	
	const TArray<FConceptSlot>& Slots = BodyPartSlots[BodyPart];
	
	for (const FConceptSlot& Slot : Slots)
	{
		if (Slot.IsEmpty() && Slot.bIsUnlocked && Slot.CanHoldConcept(Concept))
		{
			OutSlot = Slot;
			return true;
		}
	}
	
	return false;
}

bool UConceptComponent::IncreaseMastery(const FGuid& SlotId, int32 Amount)
{
	FConceptSlot FoundSlot;
	EBodyPartType BodyPart;
	
	if (!FindSlotById(SlotId, FoundSlot, BodyPart) || FoundSlot.IsEmpty())
	{
		return false;
	}
	
	// Increase mastery
	FoundSlot.IncreaseMastery(Amount);
	
	// Update the slot
	UpdateSlot(FoundSlot, BodyPart);
	
	// Update ability system component if we have one
	if (AbilitySystemComponent && FoundSlot.HeldConcept.IsValid())
	{
		// If this is a UConceptAbilitySystemComponent, update ability levels based on new mastery
		UConceptAbilitySystemComponent* ConceptASC = Cast<UConceptAbilitySystemComponent>(AbilitySystemComponent);
		if (ConceptASC)
		{
			ConceptASC->UpdateAbilityLevelsFromConceptMastery();
		}
	}
	
	// Broadcast delegate
	if (FoundSlot.HeldConcept.IsValid())
	{
		OnConceptMasteryChanged.Broadcast(FoundSlot.HeldConcept.Get(), FoundSlot.MasteryLevel);
	}
	
	return true;
}

bool UConceptComponent::HasAcquiredConcept(UConcept* Concept) const
{
	return Concept && AcquiredConcepts.Contains(Concept);
}

bool UConceptComponent::HasObservedConcept(UConcept* Concept) const
{
	return Concept && ObservedConcepts.Contains(Concept);
}

bool UConceptComponent::FindSlotById(const FGuid& SlotId, FConceptSlot& OutSlot, EBodyPartType& OutBodyPart)
{
	for (const auto& Pair : BodyPartSlots)
	{
		EBodyPartType BodyPart = Pair.Key;
		const TArray<FConceptSlot>& Slots = Pair.Value;
		
		for (const FConceptSlot& Slot : Slots)
		{
			if (Slot.SlotId == SlotId)
			{
				OutSlot = Slot;
				OutBodyPart = BodyPart;
				return true;
			}
		}
	}
	
	return false;
}

void UConceptComponent::UpdateSlot(const FConceptSlot& Slot, EBodyPartType BodyPart)
{
	if (!BodyPartSlots.Contains(BodyPart))
	{
		return;
	}
	
	TArray<FConceptSlot>& Slots = BodyPartSlots[BodyPart];
	
	for (int32 i = 0; i < Slots.Num(); ++i)
	{
		if (Slots[i].SlotId == Slot.SlotId)
		{
			Slots[i] = Slot;
			break;
		}
	}
}
