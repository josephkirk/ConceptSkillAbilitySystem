# TODOs for Concept-Based RPG Skill System

This file lists key tasks to align the current implementation with the design document and pillars. Prioritize integration with Unreal 5 Gameplay Ability System and Gameplay Tags.

## Core Components
- [x] **Implement Core Node Slots:** [Completed] Added amplification mechanics in `ConceptComponent` to enhance concept effects within body areas. (References: DESIGN.md Section 3.2, Current gap in slot management)
- [x] **Add Body Manual Functionality:** [Completed] Created a system for slot reconfiguration and increasing Core Node Slots, enabling player-driven specialization. (References: DESIGN.md Section 3.4, Missing in current code)

## Progression Mechanics
- [x] **Enhance Mundane Activity Progression:** [Completed] Updated `ConceptComponent` to track and utilize a Progression Pool for unlocking slots through activities like sleep and training. (References: DESIGN.md Section 3.3, DESIGN Pillars, Partial implementation in mastery but not holistic)

## Synergy and Interaction
- [x] **Develop Character-Object Synergy:** [Completed] Implemented interactions between character and object concepts, including emergent skills and amplification via Core Nodes. (References: DESIGN.md Section 6.1, Underdeveloped in current synergy checks)
- [x] **Improve Concept Combination Synergy:** [Completed] Added logic in `ConceptSkillManager` or similar to handle emergent effects from combined concepts, with Core Node influence. (References: DESIGN.md Section 6.2, DESIGN Pillars "Synergistic Capabilities")

## Concept Manifestation and Risks
- [x] **Implement Over-Slotting Mechanic:** [Completed] Added risky forging process with degradation/breakage chances in `ConceptualObject`. (References: DESIGN.md Section 5.3, DESIGN Pillars "Meaningful Risk and Reward", Missing risk elements)
- [x] **Refine Skill Manifestation:** [Completed] Ensured diverse pathways (active, passive, crafting) are fully supported and tied to slot positions. (References: DESIGN.md Section 5.1, Partial in `UConceptSkill`)

## General Improvements
- [x] **Ensure Design Pillar Integration:** [Completed] Integrated with Unreal 5 Gameplay Ability System by adding Gameplay Tag application in skill manifestation; added comprehensive tests and logging for debugging. All changes align with design pillars.
- [x] **Update Documentation:** [Completed] This file has been updated.

Review and update this list as features are implemented.
