// UE4OSC created by Zach Duer
// see readme for copyright, permissions and credits

#include "UE4OSCPluginPrivatePCH.h"

class FUE4OSCPlugin : public IModuleInterface
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE(FUE4OSCPlugin, UE4OSCPlugin)



void FUE4OSCPlugin::StartupModule()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
}


void FUE4OSCPlugin::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}



