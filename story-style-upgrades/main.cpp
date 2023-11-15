#include "pch.h"

StoryStyleUpgradeHandler UpgradeHandler;

extern "C" {
	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions) {
		int t = GameState;
		const IniFile* config = new IniFile(std::string(path) + "\\config.ini");
		bool includeCurrentLevelUpgrade = config->getBool("General", "IncludeCurrentLevelUpgrade", false);
		bool disableAllShadowUpgrades = config->getBool("General", "DisableAllShadowUpgrades", false);
		bool disableSonicFlameRing = config->getBool("General", "DisableSonicFlameRing", false);
		delete config;

		UpgradeHandler.init(includeCurrentLevelUpgrade, disableAllShadowUpgrades, disableSonicFlameRing);
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer }; // This is needed for the Mod Loader to recognize the DLL.
}