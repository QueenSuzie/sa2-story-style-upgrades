/**************************************************************************************
 *   main.cpp  --  This file is part of Story Style Upgrades.                         *
 *                                                                                    *
 *   Copyright (C) 2023 Queen Suzie                                                   *
 *                                                                                    *
 *   Story Style Upgrades is free software: you can redistribute it and/or modify     *
 *   it under the terms of the GNU General Public License as published                *
 *   by the Free Software Foundation, either version 3 of the License,                *
 *   or (at your option) any later version.                                           *
 *                                                                                    *
 *   Story Style Upgrades is distributed in the hope that it will be useful,          *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty                      *
 *   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                          *
 *   See the GNU General Public License for more details.                             *
 *                                                                                    *
 *   You should have received a copy of the GNU General Public License                *
 *   along with this program.  If not, see http://www.gnu.org/licenses/.              *
 *                                                                                    *
 *************************************************************************************/

#include "pch.h"

StoryStyleUpgradeHandler UpgradeHandler;

extern "C" {
	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions) {
		const IniFile* config = new IniFile(std::string(path) + "\\config.ini");
		bool includeCurrentLevelUpgrade = config->getBool("General", "IncludeCurrentLevelUpgrade", false);
		bool includeCurrentHuntingLevelUpgrade = config->getBool("General", "IncludeCurrentHuntingLevelUpgrade", false);
		bool disableAllShadowUpgrades = config->getBool("General", "DisableAllShadowUpgrades", false);
		bool disableSonicFlameRing = config->getBool("General", "DisableSonicFlameRing", false);
		bool enableUpgradeRestoreOnRestart = config->getBool("General", "EnableUpgradeRestoreOnRestart", false);
		std::string upgradeResetButton = config->getString("General", "UpgradeResetButton", "Y");
		delete config;

		UpgradeHandler.init(includeCurrentLevelUpgrade, includeCurrentHuntingLevelUpgrade, disableAllShadowUpgrades, disableSonicFlameRing, enableUpgradeRestoreOnRestart, upgradeResetButton);
	}

	__declspec(dllexport) void __cdecl OnInput() {
		if (GameState == GameStates_Pause) {
			UpgradeHandler.setUpgradeResetButtonState();
		}
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer }; // This is needed for the Mod Loader to recognize the DLL.
}