/**************************************************************************************
 *   StoryStyleUpgradeHandler.h  --  This file is part of Story Style Upgrades.       *
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

#pragma once
#include <unordered_map>

UsercallFuncVoid(SetPhysicsAndGiveUpgrades, (ObjectMaster* character, int a2), (character, a2), (intptr_t)0x4599C0, rEAX, rECX);

class StoryStyleUpgradeHandler {
	private:
		std::unordered_map<short, std::unordered_map<Upgrades, bool>> levelUpgrades;
		std::unordered_map<int, std::vector<Upgrades>> characterUpgrades;
		bool includeCurrentLevelUpgrade = false;
		bool includeCurrentHuntingLevelUpgrade = true;
		bool disableAllShadowUpgrades = false;
		bool disableSonicFlameRing = false;
		bool enableUpgradeRestoreOnRestart = true;
		bool isUpgradeResetButtonHeld = false;
		int upgradeResetButton = Buttons_Y;
		int originalUpgrades = -1;

		void initUpgradeBitsToIndexes();
		void initCharacterUpgrades();
		void initSonicUpgrades();
		void initTailsUpgrades();
		void initKnucklesUpgrades();
		void initShadowUpgrades();
		void initEggmanUpgrades();
		void initRougeUpgrades();
		void setUpgradeResetButton(std::string);
		void overwriteUpgradeItemComparison();

	public:
		void init(bool, bool, bool, bool, bool, std::string);
		void setLevelUpgrades();
		void restoreLevelUpgrades();
		void setUpgradeResetButtonState();
		void checkRestartUpgradeReset();
		static void UpgradeItemComparison();
		static bool CharacterHasUpgrade(unsigned int);
		inline static bool CharacterUpgradesGot[28] = {};
		inline static unsigned short CharacterUpgradesGotSize = 28;
		std::unordered_map<unsigned int, unsigned short> upgradesBitToIndexMap;
		static inline const void* JumpBackToLevelItemLoad = (void*)0x6D8653;
		static inline const void* JumpBackToLevelItemUpgradeLoad = (void*)0x6D865F;
};

int UpgradeHook(int);
void SetCharacterPhysicsAndUpgrades(ObjectMaster*, int);
void RestartLevel();