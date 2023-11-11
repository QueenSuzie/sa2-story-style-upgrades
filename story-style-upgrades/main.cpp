#include "pch.h"

StoryStyleUpgradeHandler UpgradeHandler;
bool canSetUpgrades = true;

extern "C" {
	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions) {
		const IniFile* config = new IniFile(std::string(path) + "\\config.ini");
		bool includeCurrentLevelUpgrade = config->getBool("General", "IncludeCurrentLevelUpgrade", false);
		bool disableAllShadowUpgrades = config->getBool("General", "DisableAllShadowUpgrades", false);
		bool disableSonicFlameRing = config->getBool("General", "DisableSonicFlameRing", false);
		delete config;

		UpgradeHandler.init(includeCurrentLevelUpgrade, disableAllShadowUpgrades, disableSonicFlameRing);
	}

	__declspec(dllexport) void __cdecl OnFrame() {
		if ((GameState == GameStates_Loading || GameState == GameStates_LoadItems) && canSetUpgrades) {
			UpgradeHandler.setLevelUpgrades();
			canSetUpgrades = false;
		}

		if (GameState == GameStates_LoadFinished) {
			canSetUpgrades = true;
		}

		if (
			GameState == GameStates_ReturnToMenu_1 ||
			GameState == GameStates_ReturnToMenu_2 ||
			GameState == GameStates_Exit_1 ||
			GameState == GameStates_Exit_2 ||
			GameState == GameStates_Exit_3 ||
			GameState == GameStates_NormalExit ||
			GameState == GameStates_GoToNextLevel
		) {
			UpgradeHandler.restoreLevelUpgrades();
			canSetUpgrades = true;
		}
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer }; // This is needed for the Mod Loader to recognize the DLL.
}