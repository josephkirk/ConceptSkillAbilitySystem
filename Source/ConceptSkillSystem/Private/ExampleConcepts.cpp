// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExampleConcepts.h"

UExampleConcepts::UExampleConcepts()
{
}

void UExampleConcepts::CreateExampleConcepts()
{
	// Clear existing concepts
	PhysicalConcepts.Empty();
	IntermediateConcepts.Empty();
	AdvancedConcepts.Empty();
	AbstractConcepts.Empty();

	// Create example physical concepts
	UConcept* FireConcept = NewObject<UConcept>(this, UConcept::StaticClass(), TEXT("Fire"));
	FireConcept->DisplayName = FText::FromString(TEXT("Fire"));
	FireConcept->Description = FText::FromString(TEXT("The elemental force of heat and combustion."));
	FireConcept->Tier = EConceptTier::Physical;
	FireConcept->Power = 10;
	FireConcept->AcquisitionDifficulty = 20;
	FireConcept->BaseAcquisitionChance = 0.3f;
	PhysicalConcepts.Add(FireConcept);

	UConcept* WaterConcept = NewObject<UConcept>(this, UConcept::StaticClass(), TEXT("Water"));
	WaterConcept->DisplayName = FText::FromString(TEXT("Water"));
	WaterConcept->Description = FText::FromString(TEXT("The elemental force of fluidity and life."));
	WaterConcept->Tier = EConceptTier::Physical;
	WaterConcept->Power = 10;
	WaterConcept->AcquisitionDifficulty = 20;
	WaterConcept->BaseAcquisitionChance = 0.3f;
	PhysicalConcepts.Add(WaterConcept);

	UConcept* EarthConcept = NewObject<UConcept>(this, UConcept::StaticClass(), TEXT("Earth"));
	EarthConcept->DisplayName = FText::FromString(TEXT("Earth"));
	EarthConcept->Description = FText::FromString(TEXT("The elemental force of solidity and stability."));
	EarthConcept->Tier = EConceptTier::Physical;
	EarthConcept->Power = 10;
	EarthConcept->AcquisitionDifficulty = 20;
	EarthConcept->BaseAcquisitionChance = 0.3f;
	PhysicalConcepts.Add(EarthConcept);

	UConcept* AirConcept = NewObject<UConcept>(this, UConcept::StaticClass(), TEXT("Air"));
	AirConcept->DisplayName = FText::FromString(TEXT("Air"));
	AirConcept->Description = FText::FromString(TEXT("The elemental force of movement and breath."));
	AirConcept->Tier = EConceptTier::Physical;
	AirConcept->Power = 10;
	AirConcept->AcquisitionDifficulty = 20;
	AirConcept->BaseAcquisitionChance = 0.3f;
	PhysicalConcepts.Add(AirConcept);

	// Create example intermediate concepts
	UConcept* HeatConcept = NewObject<UConcept>(this, UConcept::StaticClass(), TEXT("Heat"));
	HeatConcept->DisplayName = FText::FromString(TEXT("Heat"));
	HeatConcept->Description = FText::FromString(TEXT("The transfer of thermal energy between objects."));
	HeatConcept->Tier = EConceptTier::Intermediate;
	HeatConcept->Power = 20;
	HeatConcept->AcquisitionDifficulty = 40;
	HeatConcept->BaseAcquisitionChance = 0.2f;
	HeatConcept->RelatedConcepts.Add(FireConcept);
	IntermediateConcepts.Add(HeatConcept);

	UConcept* FlowConcept = NewObject<UConcept>(this, UConcept::StaticClass(), TEXT("Flow"));
	FlowConcept->DisplayName = FText::FromString(TEXT("Flow"));
	FlowConcept->Description = FText::FromString(TEXT("The continuous movement in a direction."));
	FlowConcept->Tier = EConceptTier::Intermediate;
	FlowConcept->Power = 20;
	FlowConcept->AcquisitionDifficulty = 40;
	FlowConcept->BaseAcquisitionChance = 0.2f;
	FlowConcept->RelatedConcepts.Add(WaterConcept);
	FlowConcept->RelatedConcepts.Add(AirConcept);
	IntermediateConcepts.Add(FlowConcept);

	UConcept* StabilityConcept = NewObject<UConcept>(this, UConcept::StaticClass(), TEXT("Stability"));
	StabilityConcept->DisplayName = FText::FromString(TEXT("Stability"));
	StabilityConcept->Description = FText::FromString(TEXT("The resistance to change or disturbance."));
	StabilityConcept->Tier = EConceptTier::Intermediate;
	StabilityConcept->Power = 20;
	StabilityConcept->AcquisitionDifficulty = 40;
	StabilityConcept->BaseAcquisitionChance = 0.2f;
	StabilityConcept->RelatedConcepts.Add(EarthConcept);
	IntermediateConcepts.Add(StabilityConcept);

	// Create example advanced concepts
	UConcept* EnergyConcept = NewObject<UConcept>(this, UConcept::StaticClass(), TEXT("Energy"));
	EnergyConcept->DisplayName = FText::FromString(TEXT("Energy"));
	EnergyConcept->Description = FText::FromString(TEXT("The capacity to do work and cause change."));
	EnergyConcept->Tier = EConceptTier::Advanced;
	EnergyConcept->Power = 30;
	EnergyConcept->AcquisitionDifficulty = 60;
	EnergyConcept->BaseAcquisitionChance = 0.1f;
	EnergyConcept->RelatedConcepts.Add(HeatConcept);
	EnergyConcept->RelatedConcepts.Add(FlowConcept);
	AdvancedConcepts.Add(EnergyConcept);

	UConcept* BalanceConcept = NewObject<UConcept>(this, UConcept::StaticClass(), TEXT("Balance"));
	BalanceConcept->DisplayName = FText::FromString(TEXT("Balance"));
	BalanceConcept->Description = FText::FromString(TEXT("The state of equilibrium between opposing forces."));
	BalanceConcept->Tier = EConceptTier::Advanced;
	BalanceConcept->Power = 30;
	BalanceConcept->AcquisitionDifficulty = 60;
	BalanceConcept->BaseAcquisitionChance = 0.1f;
	BalanceConcept->RelatedConcepts.Add(StabilityConcept);
	BalanceConcept->RelatedConcepts.Add(FlowConcept);
	AdvancedConcepts.Add(BalanceConcept);

	// Create example abstract concepts
	UConcept* HarmonyConcept = NewObject<UConcept>(this, UConcept::StaticClass(), TEXT("Harmony"));
	HarmonyConcept->DisplayName = FText::FromString(TEXT("Harmony"));
	HarmonyConcept->Description = FText::FromString(TEXT("The state of perfect agreement and unity."));
	HarmonyConcept->Tier = EConceptTier::Abstract;
	HarmonyConcept->Power = 50;
	HarmonyConcept->AcquisitionDifficulty = 80;
	HarmonyConcept->BaseAcquisitionChance = 0.05f;
	HarmonyConcept->RelatedConcepts.Add(BalanceConcept);
	HarmonyConcept->RelatedConcepts.Add(EnergyConcept);
	AbstractConcepts.Add(HarmonyConcept);

	UConcept* CreationConcept = NewObject<UConcept>(this, UConcept::StaticClass(), TEXT("Creation"));
	CreationConcept->DisplayName = FText::FromString(TEXT("Creation"));
	CreationConcept->Description = FText::FromString(TEXT("The act of bringing something into existence."));
	CreationConcept->Tier = EConceptTier::Abstract;
	CreationConcept->Power = 50;
	CreationConcept->AcquisitionDifficulty = 80;
	CreationConcept->BaseAcquisitionChance = 0.05f;
	CreationConcept->RelatedConcepts.Add(EnergyConcept);
	AbstractConcepts.Add(CreationConcept);
}

void UExampleConcepts::CreateExampleSkills()
{
	// Clear existing skills
	ExampleSkills.Empty();

	// Make sure we have concepts to work with
	if (PhysicalConcepts.IsEmpty() && IntermediateConcepts.IsEmpty() && 
		AdvancedConcepts.IsEmpty() && AbstractConcepts.IsEmpty())
	{
		CreateExampleConcepts();
	}

	// Create example active skills
	UConceptSkill* FireballSkill = NewObject<UConceptSkill>(this, UConceptSkill::StaticClass(), TEXT("Fireball"));
	FireballSkill->DisplayName = FText::FromString(TEXT("Fireball"));
	FireballSkill->Description = FText::FromString(TEXT("Launch a ball of fire at your target."));
	FireballSkill->ManifestationType = ESkillManifestationType::Active;
	FireballSkill->RequiredConcepts.Add(PhysicalConcepts[0]); // Fire
	FireballSkill->RequiredConcepts.Add(IntermediateConcepts[0]); // Heat
	FireballSkill->RequiredMasteryLevel = 30;
	FireballSkill->Power = 25;
	FireballSkill->Cooldown = 3.0f;
	ExampleSkills.Add(FireballSkill);

	UConceptSkill* WaterShieldSkill = NewObject<UConceptSkill>(this, UConceptSkill::StaticClass(), TEXT("WaterShield"));
	WaterShieldSkill->DisplayName = FText::FromString(TEXT("Water Shield"));
	WaterShieldSkill->Description = FText::FromString(TEXT("Create a protective shield of water around yourself."));
	WaterShieldSkill->ManifestationType = ESkillManifestationType::Active;
	WaterShieldSkill->RequiredConcepts.Add(PhysicalConcepts[1]); // Water
	WaterShieldSkill->RequiredConcepts.Add(IntermediateConcepts[2]); // Stability
	WaterShieldSkill->RequiredMasteryLevel = 30;
	WaterShieldSkill->Power = 20;
	WaterShieldSkill->Cooldown = 5.0f;
	ExampleSkills.Add(WaterShieldSkill);

	// Create example passive abilities
	UConceptSkill* ElementalResistanceSkill = NewObject<UConceptSkill>(this, UConceptSkill::StaticClass(), TEXT("ElementalResistance"));
	ElementalResistanceSkill->DisplayName = FText::FromString(TEXT("Elemental Resistance"));
	ElementalResistanceSkill->Description = FText::FromString(TEXT("Gain resistance to elemental damage."));
	ElementalResistanceSkill->ManifestationType = ESkillManifestationType::Passive;
	ElementalResistanceSkill->RequiredConcepts.Add(AdvancedConcepts[1]); // Balance
	ElementalResistanceSkill->RequiredMasteryLevel = 50;
	ElementalResistanceSkill->Power = 30;
	ExampleSkills.Add(ElementalResistanceSkill);

	// Create example crafting knowledge
	UConceptSkill* ElementalForgeSkill = NewObject<UConceptSkill>(this, UConceptSkill::StaticClass(), TEXT("ElementalForge"));
	ElementalForgeSkill->DisplayName = FText::FromString(TEXT("Elemental Forge"));
	ElementalForgeSkill->Description = FText::FromString(TEXT("Knowledge of how to imbue items with elemental properties."));
	ElementalForgeSkill->ManifestationType = ESkillManifestationType::Crafting;
	ElementalForgeSkill->RequiredConcepts.Add(AdvancedConcepts[0]); // Energy
	ElementalForgeSkill->RequiredConcepts.Add(PhysicalConcepts[0]); // Fire
	ElementalForgeSkill->RequiredMasteryLevel = 40;
	ElementalForgeSkill->Power = 35;
	ExampleSkills.Add(ElementalForgeSkill);

	// Create example character proficiency
	UConceptSkill* ElementalHarmonySkill = NewObject<UConceptSkill>(this, UConceptSkill::StaticClass(), TEXT("ElementalHarmony"));
	ElementalHarmonySkill->DisplayName = FText::FromString(TEXT("Elemental Harmony"));
	ElementalHarmonySkill->Description = FText::FromString(TEXT("Increased affinity with all elemental forces."));
	ElementalHarmonySkill->ManifestationType = ESkillManifestationType::Proficiency;
	ElementalHarmonySkill->RequiredConcepts.Add(AbstractConcepts[0]); // Harmony
	ElementalHarmonySkill->RequiredMasteryLevel = 70;
	ElementalHarmonySkill->Power = 50;
	ExampleSkills.Add(ElementalHarmonySkill);
}

TArray<UConcept*> UExampleConcepts::GetAllExampleConcepts() const
{
	TArray<UConcept*> AllConcepts;
	AllConcepts.Append(PhysicalConcepts);
	AllConcepts.Append(IntermediateConcepts);
	AllConcepts.Append(AdvancedConcepts);
	AllConcepts.Append(AbstractConcepts);
	return AllConcepts;
}

TArray<UConcept*> UExampleConcepts::GetExampleConceptsByTier(EConceptTier Tier) const
{
	switch (Tier)
	{
	case EConceptTier::Physical:
		return PhysicalConcepts;
	case EConceptTier::Intermediate:
		return IntermediateConcepts;
	case EConceptTier::Advanced:
		return AdvancedConcepts;
	case EConceptTier::Abstract:
		return AbstractConcepts;
	default:
		return TArray<UConcept*>();
	}
}
