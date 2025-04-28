# Concept Skill System Plugin for Unreal Engine 5.5

A plugin implementing a concept-based skill system where characters and objects can hold concepts that translate into abilities and skills.

> **IMPORTANT NOTE**: This plugin is still in active development. The API is subject to frequent changes and may not be stable. Use at your own risk in production environments.

## Overview

The Concept Skill System is built around the following design pillars:

1. **Concept as the Universal Building Block**: Concepts are the fundamental units of understanding and power in the world, representing the inherent nature of objects, events, and abstract forces across all tiers of reality (Physical, Intermediate, Advanced, Abstract).

2. **Understanding Through Observation and Interaction**: Character progression and concept acquisition are driven by actively observing and interacting with the environment, emphasizing curiosity and engagement with the game world beyond combat.

3. **Embodied Knowledge (Character Slots)**: A character's capacity to internalize concepts is represented by a finite, unlockable slot system tied to specific body parts (Head, Body, Arms, Feet), linking theoretical understanding and practical abilities to the character's physical and mental being.

4. **Concepts in the World (Object Slots)**: Objects possess inherent conceptual properties (intrinsic concepts) that grant passive benefits when used, and can hold additional concepts within their own quality-dependent slots, making gear an active part of a character's capabilities.

5. **Diverse Manifestation of Ability**: Concepts translate into usable abilities through multiple distinct pathways: combining concepts into active skills, forging concepts into the properties of physical objects, and mediating concepts into internalized character proficiencies.

6. **Meaningful Risk and Reward**: The system incorporates elements of risk in concept acquisition (random chance from observation/item usage) and particularly in crafting (the chance of object degradation or destruction through over-slotting), making successes feel earned and pushing players to weigh potential gains against potential losses.

7. **Synergistic and Emergent Capabilities**: The interaction and combination of concepts, both within a character's own slots and between character and object slots, are central to the system, encouraging creative experimentation and leading to the emergence of unique and powerful synergistic abilities.

8. **Player-Driven Specialization**: Character development is highly personalized, shaped by the player's choices in what to observe, which concepts to prioritize, how to allocate limited concept slots, and how to utilize tools like the Body Manual to customize their character's fundamental conceptual capacity, supporting diverse and flexible archetypes.

## Features

* **Concept-based Skill System**: Characters can acquire concepts through observation and interaction, which can be combined to create skills.
* **Slot-based Concept Storage**: Characters have slots tied to body parts that can hold concepts, with higher-tier concepts requiring more slots.
* **Observation Mechanics**: Characters can observe objects and entities to learn concepts from them.
* **Skill Creation and Management**: Create skills by combining concepts, with different manifestation types (Active, Passive, Crafting, Proficiency).
* **Object Quality System**: Objects can hold concepts based on their quality, with higher-quality objects having more slots.
* **Body Manual**: Special items that allow characters to customize their concept slots.
* **Concept Registry**: Central registry for managing all concepts and skills in the game.
* **Gameplay Ability System Integration**: Fully integrated with Unreal Engine's Gameplay Ability System for robust ability management.
* **Gameplay Tags**: Uses Gameplay Tags for concept categorization, skill requirements, and effects.
* **Attribute System**: Custom attribute set for concept-related character statistics.
* **Gameplay Cues**: Visual and audio feedback for concept abilities using Niagara particles and sound effects.

## Core Components

### Concepts

Concepts are the fundamental building blocks of the system. Each concept belongs to a tier (Physical, Intermediate, Advanced, Abstract) and has properties like power, acquisition difficulty, and related concepts.

### Concept Slots

Characters and objects have slots that can hold concepts. Character slots are tied to body parts (Head, Body, Arms, Feet), while object slots depend on the object's quality.

### Skills

Skills are formed by combining concepts and can manifest as active abilities, passive bonuses, crafting knowledge, or character proficiencies.

### Observation System

Characters can observe and learn concepts from the environment, with acquisition chance based on observation quality and concept difficulty.

## Getting Started

### Adding the Plugin to Your Project

1. Copy the plugin folder to your project's `Plugins` directory
2. Enable the plugin in the Unreal Editor through Edit > Plugins
3. Restart the editor

### Basic Setup

1. Add a `ConceptComponent` to your character
2. Add a `ConceptSkillManager` to your character
3. Create concept assets in the Content Browser
4. Create skill assets in the Content Browser
5. Make objects observable by implementing the `ConceptObservable` interface

## Example Usage

### Creating a Concept

1. Right-click in the Content Browser
2. Select Create > Concept Skill System > Concept
3. Configure the concept's properties (name, tier, power, acquisition difficulty, etc.)

### Creating a Skill

1. Right-click in the Content Browser
2. Select Create > Concept Skill System > Concept Skill
3. Configure the skill's properties (name, required concepts, manifestation type, etc.)

### Making an Object Observable

```cpp
// In your actor's header file
#include "ConceptObservable.h"

UCLASS()
class YOURGAME_API AYourActor : public AActor, public IConceptObservable
{
    GENERATED_BODY()

public:
    // IConceptObservable interface
    virtual TArray<UConcept*> GetObservableConcepts_Implementation() const override;
    virtual float GetObservationQuality_Implementation() const override;
    virtual void OnObserved_Implementation(AActor* Observer) override;
    virtual bool CanBeObserved_Implementation() const override;
};
```

### Observing a Concept

```cpp
// In your character's code
#include "ConceptSkillFunctionLibrary.h"

// Observe a concept from an object
UConceptSkillFunctionLibrary::ObserveConcept(this, ObservableObject);
```

### Unlocking a Skill

```cpp
// In your character's code
#include "ConceptSkillManager.h"

// Get the concept skill manager
UConceptSkillManager* SkillManager = FindComponentByClass<UConceptSkillManager>();
if (SkillManager)
{
    // Check for new skills that can be unlocked based on acquired concepts
    SkillManager->CheckForNewSkills();
}
```

## Development Status

This plugin is currently in active development. The API is not finalized and may change significantly between versions. Features may be added, modified, or removed without prior notice. It is recommended to thoroughly test the plugin with your project before using it in a production environment.

## License

This plugin is licensed under the terms of the MIT license. See the LICENSE file for details.
