// Copyright Koorogi Games, Inc. All Rights Reserved.

#include "ConceptualObject.h"

AConceptualObject::AConceptualObject()
{
	PrimaryActorTick.bCanEverTick = true;

	Quality = EObjectQuality::Common;
	Durability = 100;
	DegradationChance = 0.2f;
}

void AConceptualObject::BeginPlay()
{
	Super::BeginPlay();
	InitializeSlots();
}

void AConceptualObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AConceptualObject::InitializeSlots()
{
	// Clear existing slots
	ConceptSlots.Empty();
	
	// Create slots based on quality
	int32 NumSlots = GetMaxSlots();
	EConceptTier MaxTier = GetMaxConceptTier();
	
	for (int32 i = 0; i < NumSlots; ++i)
	{
		FConceptSlot NewSlot;
		NewSlot.BodyPart = EBodyPartType::None; // Object slots aren't tied to body parts
		NewSlot.bIsUnlocked = (i == 0); // Only the first slot is unlocked by default
		NewSlot.MaxTier = MaxTier;
		NewSlot.SlotId = FGuid::NewGuid();
		
		ConceptSlots.Add(NewSlot);
	}
}

bool AConceptualObject::AddConcept(UConcept* Concept, bool bRiskDegradation)
{
	if (!Concept)
	{
		return false;
	}
	
	// Check if the concept is already an intrinsic concept
	for (const auto& IntrinsicConcept : IntrinsicConcepts)
	{
		if (IntrinsicConcept.Get() == Concept)
		{
			return false; // Already an intrinsic concept
		}
	}
	
	// Find an empty slot
	FConceptSlot EmptySlot;
	if (!FindEmptySlotForConcept(Concept, EmptySlot))
	{
		return false; // No suitable slot found
	}
	
	// Set the concept in the slot
	EmptySlot.SetConcept(Concept);
	
	// Update the slot in the array
	for (int32 i = 0; i < ConceptSlots.Num(); ++i)
	{
		if (ConceptSlots[i].SlotId == EmptySlot.SlotId)
		{
			ConceptSlots[i] = EmptySlot;
			break;
		}
	}
	
	// Risk of degradation when adding concepts
	if (bRiskDegradation && FMath::FRand() <= DegradationChance)
	{
		// Higher tier concepts cause more degradation
		int32 DegradationAmount = 5 * (static_cast<int32>(Concept->Tier) + 1);
		ApplyDegradation(DegradationAmount);
	}
	
	// Broadcast delegate
	OnObjectConceptAdded.Broadcast(Concept, EmptySlot);
	
	return true;
}

bool AConceptualObject::RemoveConcept(const FGuid& SlotId)
{
	FConceptSlot FoundSlot;
	int32 SlotIndex;
	
	if (!FindSlotById(SlotId, FoundSlot, SlotIndex) || FoundSlot.IsEmpty())
	{
		return false;
	}
	
	// Clear the concept from the slot
	FoundSlot.ClearConcept();
	
	// Update the slot in the array
	ConceptSlots[SlotIndex] = FoundSlot;
	
	return true;
}

bool AConceptualObject::UnlockConceptSlot(EConceptTier MaxTier)
{
	// Find the first locked slot
	for (int32 i = 0; i < ConceptSlots.Num(); ++i)
	{
		if (!ConceptSlots[i].bIsUnlocked)
		{
			ConceptSlots[i].bIsUnlocked = true;
			ConceptSlots[i].MaxTier = MaxTier;
			
			// Broadcast delegate
			OnObjectSlotUnlocked.Broadcast(ConceptSlots[i]);
			
			return true;
		}
	}
	
	return false; // No locked slots found
}

bool AConceptualObject::UpgradeQuality()
{
	// Check if already at maximum quality
	if (Quality == EObjectQuality::Legendary)
	{
		return false;
	}
	
	// Upgrade to next quality level
	Quality = static_cast<EObjectQuality>(static_cast<uint8>(Quality) + 1);
	
	// Reinitialize slots for the new quality
	InitializeSlots();
	
	// Broadcast delegate
	OnObjectQualityChanged.Broadcast(static_cast<int32>(Quality));
	
	return true;
}

bool AConceptualObject::HasConcept(UConcept* Concept) const
{
	if (!Concept)
	{
		return false;
	}
	
	// Check intrinsic concepts
	for (const auto& IntrinsicConcept : IntrinsicConcepts)
	{
		if (IntrinsicConcept.Get() == Concept)
		{
			return true;
		}
	}
	
	// Check slots
	for (const FConceptSlot& Slot : ConceptSlots)
	{
		if (!Slot.IsEmpty() && Slot.HeldConcept.Get() == Concept)
		{
			return true;
		}
	}
	
	return false;
}

TArray<TSoftObjectPtr<UConcept>> AConceptualObject::GetAllConcepts() const
{
	TArray<TSoftObjectPtr<UConcept>> AllConcepts = IntrinsicConcepts;
	
	// Add concepts from slots
	for (const FConceptSlot& Slot : ConceptSlots)
	{
		if (!Slot.IsEmpty())
		{
			AllConcepts.AddUnique(Slot.HeldConcept);
		}
	}
	
	return AllConcepts;
}

int32 AConceptualObject::GetMaxSlots() const
{
	// Maximum slots based on quality
	switch (Quality)
	{
	case EObjectQuality::Poor:
		return 1;
	case EObjectQuality::Common:
		return 2;
	case EObjectQuality::Uncommon:
		return 3;
	case EObjectQuality::Rare:
		return 4;
	case EObjectQuality::Epic:
		return 5;
	case EObjectQuality::Legendary:
		return 6;
	default:
		return 1;
	}
}

EConceptTier AConceptualObject::GetMaxConceptTier() const
{
	// Maximum concept tier based on quality
	switch (Quality)
	{
	case EObjectQuality::Poor:
		return EConceptTier::Physical;
	case EObjectQuality::Common:
		return EConceptTier::Physical;
	case EObjectQuality::Uncommon:
		return EConceptTier::Intermediate;
	case EObjectQuality::Rare:
		return EConceptTier::Intermediate;
	case EObjectQuality::Epic:
		return EConceptTier::Advanced;
	case EObjectQuality::Legendary:
		return EConceptTier::Abstract;
	default:
		return EConceptTier::Physical;
	}
}

bool AConceptualObject::ApplyDegradation(int32 Amount)
{
	// Reduce durability
	Durability = FMath::Max(0, Durability - Amount);
	
	// Check if object is destroyed
	if (Durability <= 0)
	{
		// Object is destroyed
		// In a real implementation, this would destroy the actor or trigger some event
		return true;
	}
	
	return false;
}

bool AConceptualObject::FindEmptySlotForConcept(UConcept* Concept, FConceptSlot& OutSlot)
{
	if (!Concept)
	{
		return false;
	}
	
	for (const FConceptSlot& Slot : ConceptSlots)
	{
		if (Slot.IsEmpty() && Slot.bIsUnlocked && Slot.CanHoldConcept(Concept))
		{
			OutSlot = Slot;
			return true;
		}
	}
	
	return false;
}

bool AConceptualObject::FindSlotById(const FGuid& SlotId, FConceptSlot& OutSlot, int32& OutIndex)
{
	for (int32 i = 0; i < ConceptSlots.Num(); ++i)
	{
		if (ConceptSlots[i].SlotId == SlotId)
		{
			OutSlot = ConceptSlots[i];
			OutIndex = i;
			return true;
		}
	}
	
	return false;
}

bool AConceptualObject::OverSlotConcept(UConcept* Concept)
{
    if (!Concept) return false;
    
    // Check if there is an empty slot; over-slotting should only occur when slots are full
    FConceptSlot DummySlot;
    if (FindEmptySlotForConcept(Concept, DummySlot)) return false;  // No need for over-slotting if a slot is available
    
    // Determine success chance based on concept tier and object quality
    // Lower success for higher tier concepts and lower quality objects
    float successChance = 0.5f - 0.1f * static_cast<float>(Concept->Tier) + 0.1f * static_cast<float>(static_cast<uint8>(Quality));  // Example: Base 50%, -10% per tier, +10% per quality level
    successChance = FMath::Clamp(successChance, 0.0f, 1.0f);
    
    if (FMath::FRand() <= successChance)
    {
        // Success: Add a new slot for the concept
        FConceptSlot NewSlot;
        NewSlot.bIsUnlocked = true;
        NewSlot.MaxTier = Concept->Tier;
        NewSlot.SetConcept(Concept);
        NewSlot.SlotId = FGuid::NewGuid();
        ConceptSlots.Add(NewSlot);
        OnObjectConceptAdded.Broadcast(Concept, NewSlot);
        return true;
    }
    else
    {
        // Failure: Determine if degradation or breakage
        float breakageChance = 0.1f + 0.05f * static_cast<float>(Concept->Tier);  // Base 10% breakage chance, +5% per tier
        breakageChance = FMath::Clamp(breakageChance, 0.0f, 1.0f);
        
        if (FMath::FRand() <= breakageChance)
        {
            // Breakage: Destroy the object
            Destroy();
            return false;
        }
        else
        {
            // Degradation: Reduce durability or quality
            int32 degradationAmount = 10 + 5 * static_cast<int32>(Concept->Tier);  // Example degradation amount
            ApplyDegradation(degradationAmount);
            return false;
        }
    }
}
