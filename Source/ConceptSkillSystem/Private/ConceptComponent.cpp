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
	UE_LOG(LogTemp, Log, TEXT("ConceptComponent initialized for actor %s"), *GetOwner()->GetName());
	
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
	
	// Initialize CoreNodeSlots with default amplification factor
	for (auto& Pair : MaxSlotsPerBodyPart)
	{
		EBodyPartType BodyPart = Pair.Key;
		FCoreNodeSlot CoreSlot;
		CoreSlot.AmplificationFactor = 1.0f;  // Default no amplification, can be modified via editor
		CoreNodeSlots.Add(BodyPart, CoreSlot);
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

void UConceptComponent::GainProgression(float Amount)
{
    if (Amount > 0.0f)
    {
        ProgressionPool += Amount;
        // Optionally, check for automatic unlocks or log progression, but keep it manual for now
    }
}

bool UConceptComponent::UnlockConceptSlot(EBodyPartType BodyPart, EConceptTier MaxTier)
{
    if (ProgressionPool >= ProgressionCostToUnlockSlot)
    {
        ProgressionPool -= ProgressionCostToUnlockSlot;  // Consume progression points
        // Existing unlock logic; find the first locked slot and unlock it
        if (BodyPartSlots.Contains(BodyPart))
        {
            TArray<FConceptSlot>& Slots = BodyPartSlots[BodyPart];
            for (auto& Slot : Slots)
            {
                if (!Slot.bIsUnlocked)
                {
                    Slot.bIsUnlocked = true;
                    Slot.MaxTier = MaxTier;  // Set the max tier for the slot
                    OnSlotUnlocked.Broadcast(Slot);  // Broadcast the event
                    return true;
                }
            }
        }
        return false;  // No locked slots found, or progression insufficient after check
    }
    return false;  // Not enough progression
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

bool UConceptComponent::ReconfigureSlot(const FGuid& SlotId, EBodyPartType NewBodyPart, EConceptTier NewMaxTier)
{
    EBodyPartType OldBodyPart;
    FConceptSlot Slot;
    if (FindSlotById(SlotId, Slot, OldBodyPart))
    {
        // Remove slot from old body part
        TArray<FConceptSlot>& OldSlots = BodyPartSlots[OldBodyPart];
        OldSlots.Remove(Slot);
        
        // Set new body part and max tier
        Slot.BodyPart = NewBodyPart;
        Slot.MaxTier = NewMaxTier;
        Slot.SlotId = SlotId;  // Preserve slot ID
        
        // Add to new body part slots
        TArray<FConceptSlot>& NewSlots = BodyPartSlots[NewBodyPart];
        NewSlots.Add(Slot);
        
        // Update CoreNode if necessary, but keep it simple for now
        return true;
    }
    return false;
}

bool UConceptComponent::IncreaseCoreNodeAmplification(EBodyPartType BodyPart, float Amount)
{
    if (CoreNodeSlots.Contains(BodyPart))
    {
        FCoreNodeSlot& CoreSlot = CoreNodeSlots[BodyPart];
        CoreSlot.AmplificationFactor += Amount;  // Increase amplification factor
        CoreSlot.AmplificationFactor = FMath::Clamp(CoreSlot.AmplificationFactor, 1.0f, 5.0f);  // Clamp to reasonable values, e.g., 1.0 to 5.0
        return true;
    }
    return false;  // Body part not found or no Core Node slot
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

float UConceptComponent::CalculateCharacterObjectSynergy(UObject* EquippedObject)
{
    if (UConceptualObject* ConceptObject = Cast<UConceptualObject>(EquippedObject))
    {
        float synergy = 0.0f;
        // Get object's concept names (assume FConceptSlot has a UConcept* Concept field)
        TArray<UConcept*> ObjectConcepts;
        for (const auto& Slot : ConceptObject->Slots)  // Assume Slots is a TArray<FConceptSlot>
        {
            if (Slot.Concept)
            {
                ObjectConcepts.Add(Slot.Concept);
            }
        }
        
        // Check against character's acquired concepts
        for (auto* CharConcept : AcquiredConcepts.Array())  // AcquiredConcepts is TSet, use Array() for iteration
        {
            for (auto* ObjConcept : ObjectConcepts)
            {
                if (CharConcept == ObjConcept)  // Simple match, can be expanded to tag similarity
                {
                    synergy += 1.0f;  // Base synergy per match
                }
            }
        }
        
        // Amplify by Core Node factors if the concept is in a Core Node Slot
        // For simplicity, apply average amplification from all Core Nodes
        float avgAmplification = 1.0f;
        if (CoreNodeSlots.Num() > 0)
        {
            float sumAmp = 0.0f;
            for (auto& CorePair : CoreNodeSlots)
            {
                sumAmp += CorePair.Value.AmplificationFactor;
            }
            avgAmplification = sumAmp / CoreNodeSlots.Num();
            synergy *= avgAmplification;
        }
        
        UE_LOG(LogTemp, Log, TEXT("Calculating synergy with object: %s, Result: %f"), *EquippedObject->GetName(), synergy);
        return synergy;  // Return synergy value as float
    }
    return 0.0f;  // No synergy if object is not a UConceptualObject or null
}

TArray<FString> UConceptComponent::GetConceptCombinationSynergies(const TArray<UConcept*>& Concepts)
{
    TArray<FString> synergies;
    if (Concepts.Num() < 2) return synergies;  // No synergies with less than 2 concepts
    
    // Simple hardcoded synergy rules based on DESIGN.md; can be data-driven later with tags or tables
    for (int32 i = 0; i < Concepts.Num(); ++i)
    {
        for (int32 j = i + 1; j < Concepts.Num(); ++j)
        {
            FString ConceptName1 = Concepts[i]->GetName();  // Assume UConcept has GetName or similar
            FString ConceptName2 = Concepts[j]->GetName();
            
            if (ConceptName1.Equals("Fire", ESearchCase::IgnoreCase) && ConceptName2.Equals("Wind", ESearchCase::IgnoreCase))
            {
                synergies.Add("Flame Whirlwind: Increased area damage from fire-based attacks.");
            }
            else if (ConceptName1.Equals("Strength", ESearchCase::IgnoreCase) && ConceptName2.Equals("Heavy", ESearchCase::IgnoreCase))
            {
                synergies.Add("Crushing Force: Higher chance to stagger enemies on impact.");
            }
            // Add more rules as needed, e.g., from a data table in future iterations
        }
    }
    
    // Remove duplicates if any
    synergies.Shrink();
    return synergies;
}

bool UConceptComponent::MediateSkill(const TArray<UConcept*>& Concepts, bool bIsActiveSkill)
{
    // Check if all concepts are acquired by the character
    for (auto* Concept : Concepts)
    {
        if (!HasAcquiredConcept(Concept))
        {
            return false;  // Cannot mediate if not all concepts are acquired
        }
    }
    
    // Create a new mediated skill
    FMediatedSkill NewSkill;
    NewSkill.Concepts = Concepts;  // Copy the array of concepts
    NewSkill.bIsActiveSkill = bIsActiveSkill;
    
    // Generate a simple description based on concepts (can be expanded with a system for emergent effects)
    FString Description = "Skill from concepts: ";
    for (auto* Concept : Concepts)
    {
        Description += Concept->GetName() + " ";  // Assume UConcept has GetName()
    }
    if (bIsActiveSkill)
    {
        Description += "(Active)";
    }
    else
    {
        Description += "(Passive)";
    }
    NewSkill.SkillDescription = Description;
    
    // Add to mediated skills array
    MediatedSkills.Add(NewSkill);
    
    // After adding the mediated skill, apply a Gameplay Tag for ability integration
    if (AbilitySystemComponent && MediatedSkills.Num() > 0)
    {
        FGameplayTagContainer TagContainer;
        FString TagName = FString::Printf(TEXT("ConceptSkill.%s"), *NewSkill.SkillDescription.Replace(TEXT(" "), TEXT(".")).ToLower());  // Generate a tag like 'conceptskill.skill.from.concepts.fire.wind'
        TagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName(*TagName)));
        AbilitySystemComponent->AddLooseGameplayTags(TagContainer);  // Apply tag to ability system
        UE_LOG(LogTemp, Log, TEXT("Applied Gameplay Tag: %s for new skill"), *TagName);
    }
    
    // Corrected logging to handle concept names array
    TArray<FString> ConceptNames;
    for (auto* Concept : Concepts)
    {
        if (Concept)
        {
            ConceptNames.Add(Concept->GetName());
        }
    }
    UE_LOG(LogTemp, Log, TEXT("Mediating skill with concepts: %s, Active: %s"), *FString::Join(ConceptNames, TEXT(", ")), bIsActiveSkill ? TEXT("true") : TEXT("false"));
    return true;  // Successfully mediated the skill
}

TArray<FString> UConceptComponent::GetMediatedSkills()
{
    TArray<FString> SkillList;
    for (const auto& Skill : MediatedSkills)
    {
        FString SkillString = Skill.SkillDescription;
        SkillList.Add(SkillString);
    }
    return SkillList;  // Return descriptions of all mediated skills
}

UFUNCTION(BlueprintCallable, Category = "Testing")
void UConceptComponent::TestConceptMechanics()
{
    // Example test for skill mediation
    TArray<UConcept*> TestConcepts;
    // Assume sample concepts are loaded or created; in practice, provide via parameters or mock data
    UConcept* Concept1 = NewObject<UConcept>(); Concept1->Name = FName("Fire");  // Mock concept for testing
    UConcept* Concept2 = NewObject<UConcept>(); Concept2->Name = FName("Wind");
    TestConcepts.Add(Concept1);
    TestConcepts.Add(Concept2);
    
    if (MediateSkill(TestConcepts, true))  // Test active skill mediation
    {
        UE_LOG(LogTemp, Log, TEXT("Test: Skill mediation succeeded."));
        if (AbilitySystemComponent)
        {
            FGameplayTagContainer TagsToCheck;
            TagsToCheck.AddTag(FGameplayTag::RequestGameplayTag(FName(TEXT("ConceptSkill.Skill.from.concepts:fire.wind"))));  // Example tag based on test concepts
            if (AbilitySystemComponent->HasAnyMatchingGameplayTags(TagsToCheck))
            {
                UE_LOG(LogTemp, Log, TEXT("Test: Gameplay Tag applied successfully."));
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Test: Gameplay Tag not applied."));
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Test: Skill mediation failed."));
    }
    
    // Test synergy calculation (assume an equipped object is available)
    // For demonstration, create a mock ConceptualObject if needed
    AConceptualObject* MockObject = Cast<AConceptualObject>(GetOwner()->FindComponentByClass(UConceptualObject::StaticClass()));
    if (MockObject)
    {
        float synergy = CalculateCharacterObjectSynergy(MockObject);
        UE_LOG(LogTemp, Log, TEXT("Test: Character-Object synergy calculated as %f"), synergy);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Test: No ConceptualObject found for synergy test."));
    }
}
