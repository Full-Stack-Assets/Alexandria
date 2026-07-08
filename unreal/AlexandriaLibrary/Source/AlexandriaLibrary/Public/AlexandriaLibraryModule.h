// AlexandriaLibraryModule.h
// Minimal runtime module wrapper so AlexandriaLibrary can be dropped into a
// project's Plugins/ directory and enabled like any other plugin.

#pragma once

#include "Modules/ModuleManager.h"

class FAlexandriaLibraryModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
