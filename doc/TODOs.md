# TODOs for Concept-Based RPG Skill System

This file lists key tasks to align the current implementation with the design document and pillars. Prioritize integration with Unreal 5 Gameplay Ability System and Gameplay Tags.

## Core Components
- **Implement Core Node Slots:** Add amplification mechanics in `ConceptComponent` to enhance concept effects within body areas. (References: DESIGN.md Section 3.2, Current gap in slot management)
- **Add Body Manual Functionality:** Create a system for slot reconfiguration and increasing Core Node Slots, enabling player-driven specialization. (References: DESIGN.md Section 3.4, Missing in current code)

## Progression Mechanics
- **Enhance Mundane Activity Progression:** Update `ConceptComponent` to track and utilize a Progression Pool for unlocking slots through activities like sleep and training. (References: DESIGN.md Section 3.3, DESIGN Pillars, Partial implementation in mastery but not holistic)

## Synergy and Interaction
- **Develop Character-Object Synergy:** Implement interactions between character and object concepts, including emergent skills and amplification via Core Nodes. (References: DESIGN.md Section 6.1, Underdeveloped in current synergy checks)
- **Improve Concept Combination Synergy:** Add logic in `ConceptSkillManager` or similar to handle emergent effects from combined concepts, with Core Node influence. (References: DESIGN.md Section 6.2, DESIGN Pillars "Synergistic Capabilities")

## Concept Manifestation and Risks
- **Implement Over-Slotting Mechanic:** Add risky forging process with degradation/breakage chances in `ConceptualObject`. (References: DESIGN.md Section 5.3, DESIGN Pillars "Meaningful Risk and Reward", Missing risk elements)
- **Refine Skill Manifestation:** Ensure diverse pathways (active, passive, crafting) are fully supported and tied to slot positions. (References: DESIGN.md Section 5.1, Partial in `UConceptSkill`)

## General Improvements
- **Ensure Design Pillar Integration:** Verify all changes align with pillars like "Understanding Through Observation" and "Player-Driven Specialization". Add tests and logging for debugging.
- **Update Documentation:** Revise DESIGN.md and other docs as needed based on implementation feedback.

Review and update this list as features are implemented.
