// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ConceptAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GETTER(PropertyName) \
SETTER(PropertyName)

#define GETTER(PropertyName) \
FGameplayAttribute Get##PropertyName##Attribute() const \
{ \
	return ClassName::Get##PropertyName##Attribute(); \
}

#define SETTER(PropertyName) \
void Set##PropertyName(float NewVal) \
{ \
	PropertyName.SetCurrentValue(NewVal); \
}

/**
 * UConceptAttributeSet - Defines attributes for the Concept Skill System
 * These attributes can be modified by gameplay effects and used by gameplay abilities
 */
UCLASS()
class CONCEPTSKILLSYSTEM_API UConceptAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UConceptAttributeSet();

	// Replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Called after attribute change
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// Called before attribute change
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// Concept-related attributes
	// ConceptPower - Represents the overall power of concepts a character has mastered
	UPROPERTY(BlueprintReadOnly, Category = "Concept Attributes", ReplicatedUsing = OnRep_ConceptPower)
	FGameplayAttributeData ConceptPower;
	ATTRIBUTE_ACCESSORS(UConceptAttributeSet, ConceptPower)

	// ConceptCapacity - Represents how many concepts a character can hold
	UPROPERTY(BlueprintReadOnly, Category = "Concept Attributes", ReplicatedUsing = OnRep_ConceptCapacity)
	FGameplayAttributeData ConceptCapacity;
	ATTRIBUTE_ACCESSORS(UConceptAttributeSet, ConceptCapacity)

	// ConceptMastery - Represents how quickly a character can master new concepts
	UPROPERTY(BlueprintReadOnly, Category = "Concept Attributes", ReplicatedUsing = OnRep_ConceptMastery)
	FGameplayAttributeData ConceptMastery;
	ATTRIBUTE_ACCESSORS(UConceptAttributeSet, ConceptMastery)

	// ObservationRange - Determines how far a character can observe concepts
	UPROPERTY(BlueprintReadOnly, Category = "Concept Attributes", ReplicatedUsing = OnRep_ObservationRange)
	FGameplayAttributeData ObservationRange;
	ATTRIBUTE_ACCESSORS(UConceptAttributeSet, ObservationRange)

	// ObservationSpeed - Determines how quickly a character can observe concepts
	UPROPERTY(BlueprintReadOnly, Category = "Concept Attributes", ReplicatedUsing = OnRep_ObservationSpeed)
	FGameplayAttributeData ObservationSpeed;
	ATTRIBUTE_ACCESSORS(UConceptAttributeSet, ObservationSpeed)

	// SkillPotency - Affects the power of skills
	UPROPERTY(BlueprintReadOnly, Category = "Concept Attributes", ReplicatedUsing = OnRep_SkillPotency)
	FGameplayAttributeData SkillPotency;
	ATTRIBUTE_ACCESSORS(UConceptAttributeSet, SkillPotency)

	// CraftingProficiency - Affects crafting success and quality
	UPROPERTY(BlueprintReadOnly, Category = "Concept Attributes", ReplicatedUsing = OnRep_CraftingProficiency)
	FGameplayAttributeData CraftingProficiency;
	ATTRIBUTE_ACCESSORS(UConceptAttributeSet, CraftingProficiency)

protected:
	// Replication callbacks
	UFUNCTION()
	virtual void OnRep_ConceptPower(const FGameplayAttributeData& OldConceptPower);

	UFUNCTION()
	virtual void OnRep_ConceptCapacity(const FGameplayAttributeData& OldConceptCapacity);

	UFUNCTION()
	virtual void OnRep_ConceptMastery(const FGameplayAttributeData& OldConceptMastery);

	UFUNCTION()
	virtual void OnRep_ObservationRange(const FGameplayAttributeData& OldObservationRange);

	UFUNCTION()
	virtual void OnRep_ObservationSpeed(const FGameplayAttributeData& OldObservationSpeed);

	UFUNCTION()
	virtual void OnRep_SkillPotency(const FGameplayAttributeData& OldSkillPotency);

	UFUNCTION()
	virtual void OnRep_CraftingProficiency(const FGameplayAttributeData& OldCraftingProficiency);
};
