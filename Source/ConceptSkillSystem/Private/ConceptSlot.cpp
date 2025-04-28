// Copyright Koorogi Games, Inc. All Rights Reserved.

#include "ConceptSlot.h"

FConceptSlot::FConceptSlot()
{
	BodyPart = EBodyPartType::None;
	bIsUnlocked = false;
	MasteryLevel = 0;
	MaxTier = EConceptTier::Physical;
	SlotId = FGuid::NewGuid();
}

bool FConceptSlot::IsEmpty() const
{
	return !HeldConcept.IsValid();
}

bool FConceptSlot::CanHoldConcept(const UConcept* Concept) const
{
	if (!bIsUnlocked || !Concept)
	{
		return false;
	}

	// Check if the concept's tier is within the slot's capacity
	return static_cast<int32>(Concept->Tier) <= static_cast<int32>(MaxTier);
}

bool FConceptSlot::SetConcept(TSoftObjectPtr<UConcept> NewConcept)
{
	if (!bIsUnlocked)
	{
		return false;
	}

	if (NewConcept.IsValid())
	{
		UConcept* ConceptObj = NewConcept.Get();
		if (!CanHoldConcept(ConceptObj))
		{
			return false;
		}
	}

	HeldConcept = NewConcept;
	MasteryLevel = 0; // Reset mastery when a new concept is set
	return true;
}

void FConceptSlot::ClearConcept()
{
	HeldConcept = nullptr;
	MasteryLevel = 0;
}

void FConceptSlot::IncreaseMastery(int32 Amount)
{
	if (!IsEmpty())
	{
		MasteryLevel = FMath::Clamp(MasteryLevel + Amount, 0, 100);
	}
}
