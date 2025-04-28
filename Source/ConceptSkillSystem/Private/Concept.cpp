// Copyright Koorogi Games, Inc. All Rights Reserved.

#include "Concept.h"

UConcept::UConcept()
{
	Tier = EConceptTier::Physical;
	Power = 1;
	AcquisitionDifficulty = 10;
	BaseAcquisitionChance = 0.1f;
}

FPrimaryAssetId UConcept::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(TEXT("Concept"), GetFName());
}
