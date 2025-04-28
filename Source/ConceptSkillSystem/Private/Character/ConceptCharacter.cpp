// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/ConceptCharacter.h"
#include "ConceptComponent.h"
#include "ConceptSkillManager.h"
#include "Abilities/ConceptAbilitySystemComponent.h"
#include "Abilities/ConceptAttributeSet.h"
#include "ConceptObservable.h"
#include "ConceptSkillFunctionLibrary.h"

AConceptCharacter::AConceptCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Create the concept component
	ConceptComponent = CreateDefaultSubobject<UConceptComponent>(TEXT("ConceptComponent"));

	// Create the concept skill manager
	ConceptSkillManager = CreateDefaultSubobject<UConceptSkillManager>(TEXT("ConceptSkillManager"));

	// Create the ability system component
	AbilitySystemComponent = CreateDefaultSubobject<UConceptAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	// Create the attribute set
	AttributeSet = CreateDefaultSubobject<UConceptAttributeSet>(TEXT("AttributeSet"));
}

void AConceptCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AConceptCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Initialize ability system when possessed
	InitializeAbilitySystem();
}

void AConceptCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Initialize ability system when player state is replicated
	InitializeAbilitySystem();
}

UAbilitySystemComponent* AConceptCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UConceptComponent* AConceptCharacter::GetConceptComponent() const
{
	return ConceptComponent;
}

UConceptSkillManager* AConceptCharacter::GetConceptSkillManager() const
{
	return ConceptSkillManager;
}

UConceptAttributeSet* AConceptCharacter::GetAttributeSet() const
{
	return AttributeSet;
}

bool AConceptCharacter::ObserveConcept(AActor* ObservableObject)
{
	if (!ObservableObject || !ObservableObject->GetClass()->ImplementsInterface(UConceptObservable::StaticClass()))
	{
		return false;
	}

	// Check if the object can be observed
	if (!IConceptObservable::Execute_CanBeObserved(ObservableObject))
	{
		return false;
	}

	// Trigger the observation
	IConceptObservable::Execute_OnObserved(ObservableObject, this);

	return true;
}

bool AConceptCharacter::AcquireConcept(UConcept* Concept, EBodyPartType BodyPart)
{
	if (!ConceptComponent || !Concept)
	{
		return false;
	}

	return ConceptComponent->AcquireConcept(Concept, BodyPart);
}

bool AConceptCharacter::UnlockSkill(UConceptSkill* Skill)
{
	if (!ConceptSkillManager || !Skill)
	{
		return false;
	}

	return ConceptSkillManager->UnlockSkill(Skill);
}

void AConceptCharacter::CheckForNewSkills()
{
	if (ConceptSkillManager)
	{
		ConceptSkillManager->CheckForNewSkills();
	}
}

void AConceptCharacter::InitializeAbilitySystem()
{
	// Make sure the ability system component is initialized
	if (AbilitySystemComponent)
	{
		// Set the owner and avatar for the ability system component
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		// Update concept power attribute based on acquired concepts
		if (ConceptComponent && AttributeSet)
		{
			// Calculate total concept power from all acquired concepts
			float TotalConceptPower = 0.0f;
			for (const auto& ConceptPair : ConceptComponent->GetAcquiredConcepts())
			{
				UConcept* Concept = ConceptPair.Value.Concept;
				if (Concept)
				{
					TotalConceptPower += Concept->Power * ConceptPair.Value.MasteryLevel;
				}
			}

			// Set the concept power attribute
			AttributeSet->SetConceptPower(TotalConceptPower);

			// Set concept capacity based on available slots
			AttributeSet->SetConceptCapacity(static_cast<float>(ConceptComponent->GetTotalSlots()));
		}

		// Grant abilities for all unlocked skills
		if (ConceptSkillManager)
		{
			ConceptSkillManager->GrantAbilitiesForUnlockedSkills();
			ConceptSkillManager->ApplyPassiveEffects();
		}
	}
}
