// Copyright Koorogi Games, Inc. All Rights Reserved.

#include "ConceptSkillSystemModule.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_MODULE(FConceptSkillSystemModule, ConceptSkillSystem);

void FConceptSkillSystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	UE_LOG(LogTemp, Log, TEXT("Concept Skill System Module has been loaded"));
}

void FConceptSkillSystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	UE_LOG(LogTemp, Log, TEXT("Concept Skill System Module has been unloaded"));
}
