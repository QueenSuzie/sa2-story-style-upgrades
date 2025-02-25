/**************************************************************************************
 *   StoryStyleUpgradeHandler.cpp  --  This file is part of Story Style Upgrades.     *
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

FunctionHook<int, int> hUpgradeGet((intptr_t)LevelItem_Main);
FunctionHook<void> hRestartLevel((intptr_t)0x43C370);

void StoryStyleUpgradeHandler::init(bool includeCurrentLevelUpgrade, bool includeCurrentHuntingLevelUpgrade, bool disableAllShadowUpgrades, bool disableSonicFlameRing, bool enableUpgradeRestoreOnRestart, std::string upgradeResetButton, bool alwaysRestoreOnRestart) {
	this->includeCurrentLevelUpgrade = includeCurrentLevelUpgrade;
	this->includeCurrentHuntingLevelUpgrade = includeCurrentHuntingLevelUpgrade;
	this->disableAllShadowUpgrades = disableAllShadowUpgrades;
	this->disableSonicFlameRing = disableSonicFlameRing;
	this->enableUpgradeRestoreOnRestart = enableUpgradeRestoreOnRestart;
	this->alwaysRestoreOnRestart = alwaysRestoreOnRestart;
	this->setUpgradeResetButton(upgradeResetButton);

	this->initUpgradeBitsToIndexes();
	this->initCharacterUpgrades();
	this->initSonicUpgrades();
	this->initTailsUpgrades();
	this->initKnucklesUpgrades();
	this->initShadowUpgrades();
	this->initEggmanUpgrades();
	this->initRougeUpgrades();

	this->overwriteUpgradeItemComparison();

	hUpgradeGet.Hook(UpgradeHook);
	SetPhysicsAndGiveUpgrades.Hook(SetCharacterPhysicsAndUpgrades);
	hRestartLevel.Hook(RestartLevel);
}

void StoryStyleUpgradeHandler::initUpgradeBitsToIndexes() {
	if (!this->upgradesBitToIndexMap.empty()) {
		return;
	}

	this->upgradesBitToIndexMap[Upgrades_SonicLightShoes] = UpgradeBits_SonicLightShoes;
	this->upgradesBitToIndexMap[Upgrades_SonicAncientLight] = UpgradeBits_SonicAncientLight;
	this->upgradesBitToIndexMap[Upgrades_SonicMagicGloves] = UpgradeBits_SonicMagicGloves;
	this->upgradesBitToIndexMap[Upgrades_SonicFlameRing] = UpgradeBits_SonicFlameRing;
	this->upgradesBitToIndexMap[Upgrades_SonicBounceBracelet] = UpgradeBits_SonicBounceBracelet;
	this->upgradesBitToIndexMap[Upgrades_SonicMysticMelody] = UpgradeBits_SonicMysticMelody;
	this->upgradesBitToIndexMap[Upgrades_TailsBooster] = UpgradeBits_TailsBooster;
	this->upgradesBitToIndexMap[Upgrades_TailsBazooka] = UpgradeBits_TailsBazooka;
	this->upgradesBitToIndexMap[Upgrades_TailsLaserBlaster] = UpgradeBits_TailsLaserBlaster;
	this->upgradesBitToIndexMap[Upgrades_TailsMysticMelody] = UpgradeBits_TailsMysticMelody;
	this->upgradesBitToIndexMap[Upgrades_KnucklesShovelClaw] = UpgradeBits_KnucklesShovelClaw;
	this->upgradesBitToIndexMap[Upgrades_KnucklesSunglasses] = UpgradeBits_KnucklesSunglasses;
	this->upgradesBitToIndexMap[Upgrades_KnucklesHammerGloves] = UpgradeBits_KnucklesHammerGloves;
	this->upgradesBitToIndexMap[Upgrades_KnucklesAirNecklace] = UpgradeBits_KnucklesAirNecklace;
	this->upgradesBitToIndexMap[Upgrades_KnucklesMysticMelody] = UpgradeBits_KnucklesMysticMelody;
	this->upgradesBitToIndexMap[Upgrades_ShadowAirShoes] = UpgradeBits_ShadowAirShoes;
	this->upgradesBitToIndexMap[Upgrades_ShadowAncientLight] = UpgradeBits_ShadowAncientLight;
	this->upgradesBitToIndexMap[Upgrades_ShadowFlameRing] = UpgradeBits_ShadowFlameRing;
	this->upgradesBitToIndexMap[Upgrades_ShadowMysticMelody] = UpgradeBits_ShadowMysticMelody;
	this->upgradesBitToIndexMap[Upgrades_EggmanJetEngine] = UpgradeBits_EggmanJetEngine;
	this->upgradesBitToIndexMap[Upgrades_EggmanLargeCannon] = UpgradeBits_EggmanLargeCannon;
	this->upgradesBitToIndexMap[Upgrades_EggmanLaserBlaster] = UpgradeBits_EggmanLaserBlaster;
	this->upgradesBitToIndexMap[Upgrades_EggmanProtectiveArmor] = UpgradeBits_EggmanProtectiveArmor;
	this->upgradesBitToIndexMap[Upgrades_EggmanMysticMelody] = UpgradeBits_EggmanMysticMelody;
	this->upgradesBitToIndexMap[Upgrades_RougePickNails] = UpgradeBits_RougePickNails;
	this->upgradesBitToIndexMap[Upgrades_RougeTreasureScope] = UpgradeBits_RougeTreasureScope;
	this->upgradesBitToIndexMap[Upgrades_RougeIronBoots] = UpgradeBits_RougeIronBoots;
	this->upgradesBitToIndexMap[Upgrades_RougeMysticMelody] = UpgradeBits_RougeMysticMelody;
}

void StoryStyleUpgradeHandler::initCharacterUpgrades() {
	std::vector<Upgrades> sonic_upgrades = {
		Upgrades_SonicLightShoes,
		Upgrades_SonicAncientLight,
		Upgrades_SonicMagicGloves,
		Upgrades_SonicFlameRing,
		Upgrades_SonicBounceBracelet,
		Upgrades_SonicMysticMelody
	};

	std::vector<Upgrades> tails_upgrades = {
		Upgrades_TailsBooster,
		Upgrades_TailsBazooka,
		Upgrades_TailsLaserBlaster,
		Upgrades_TailsMysticMelody
	};

	std::vector<Upgrades> knuckles_upgrades = {
		Upgrades_KnucklesShovelClaw,
		Upgrades_KnucklesSunglasses,
		Upgrades_KnucklesHammerGloves,
		Upgrades_KnucklesAirNecklace,
		Upgrades_KnucklesMysticMelody,
	};

	std::vector<Upgrades> shadow_upgrades = {
		Upgrades_ShadowAirShoes,
		Upgrades_ShadowAncientLight,
		Upgrades_ShadowFlameRing,
		Upgrades_ShadowMysticMelody
	};

	std::vector<Upgrades> eggman_upgrades = {
		Upgrades_EggmanJetEngine,
		Upgrades_EggmanLargeCannon,
		Upgrades_EggmanLaserBlaster,
		Upgrades_EggmanProtectiveArmor,
		Upgrades_EggmanMysticMelody
	};

	std::vector<Upgrades> rouge_upgrades = {
		Upgrades_RougePickNails,
		Upgrades_RougeTreasureScope,
		Upgrades_RougeIronBoots,
		Upgrades_RougeMysticMelody
	};

	this->characterUpgrades[Characters_Sonic] = sonic_upgrades;
	this->characterUpgrades[Characters_MechTails] = tails_upgrades;
	this->characterUpgrades[Characters_Knuckles] = knuckles_upgrades;
	this->characterUpgrades[Characters_Shadow] = shadow_upgrades;
	this->characterUpgrades[Characters_MechEggman] = eggman_upgrades;
	this->characterUpgrades[Characters_Rouge] = rouge_upgrades;
}

void StoryStyleUpgradeHandler::initSonicUpgrades() {
	std::unordered_map<Upgrades, bool> city_escape;
	city_escape[Upgrades_SonicLightShoes] = false;
	city_escape[Upgrades_SonicAncientLight] = false;
	city_escape[Upgrades_SonicMagicGloves] = false;
	city_escape[Upgrades_SonicFlameRing] = false;
	city_escape[Upgrades_SonicBounceBracelet] = false;
	city_escape[Upgrades_SonicMysticMelody] = false;

	this->levelUpgrades[LevelIDs_CityEscape] = city_escape;
	this->levelUpgrades[LevelIDs_BigFoot] = city_escape;

	if (!this->includeCurrentLevelUpgrade) {
		this->levelUpgrades[LevelIDs_MetalHarbor] = city_escape;
	}

	std::unordered_map<Upgrades, bool> green_forest;
	green_forest[Upgrades_SonicLightShoes] = true;
	green_forest[Upgrades_SonicAncientLight] = false;
	green_forest[Upgrades_SonicMagicGloves] = false;
	green_forest[Upgrades_SonicFlameRing] = false;
	green_forest[Upgrades_SonicBounceBracelet] = false;
	green_forest[Upgrades_SonicMysticMelody] = false;

	if (this->includeCurrentLevelUpgrade) {
		this->levelUpgrades[LevelIDs_MetalHarbor] = green_forest;
	}

	this->levelUpgrades[LevelIDs_SonicVsShadow1] = green_forest;
	this->levelUpgrades[LevelIDs_GreenForest] = green_forest;

	if (!this->includeCurrentLevelUpgrade) {
		this->levelUpgrades[LevelIDs_PyramidCave] = green_forest;
	}

	std::unordered_map<Upgrades, bool> crazy_gadget;
	crazy_gadget[Upgrades_SonicLightShoes] = true;
	crazy_gadget[Upgrades_SonicAncientLight] = false;
	crazy_gadget[Upgrades_SonicMagicGloves] = false;
	crazy_gadget[Upgrades_SonicFlameRing] = false;
	crazy_gadget[Upgrades_SonicBounceBracelet] = true;
	crazy_gadget[Upgrades_SonicMysticMelody] = false;

	this->levelUpgrades[LevelIDs_EggGolemS] = crazy_gadget;

	if (!this->includeCurrentLevelUpgrade) {
		this->levelUpgrades[LevelIDs_CrazyGadget] = crazy_gadget;
	}

	if (this->includeCurrentLevelUpgrade) {
		this->levelUpgrades[LevelIDs_PyramidCave] = crazy_gadget;
	}

	std::unordered_map<Upgrades, bool> final_rush;
	final_rush[Upgrades_SonicLightShoes] = true;
	final_rush[Upgrades_SonicAncientLight] = false;
	final_rush[Upgrades_SonicMagicGloves] = false;
	final_rush[Upgrades_SonicFlameRing] = !this->disableSonicFlameRing;
	final_rush[Upgrades_SonicBounceBracelet] = true;
	final_rush[Upgrades_SonicMysticMelody] = false;

	this->levelUpgrades[LevelIDs_FinalRush] = final_rush;
	this->levelUpgrades[LevelIDs_SonicVsShadow2] = final_rush;
	this->levelUpgrades[LevelIDs_CannonsCoreS] = final_rush;

	if (this->includeCurrentLevelUpgrade) {
		this->levelUpgrades[LevelIDs_CrazyGadget] = final_rush;
	}

	std::unordered_map<Upgrades, bool> green_hill;
	green_hill[Upgrades_SonicLightShoes] = false;
	green_hill[Upgrades_SonicAncientLight] = false;
	green_hill[Upgrades_SonicMagicGloves] = false;
	green_hill[Upgrades_SonicFlameRing] = false;
	green_hill[Upgrades_SonicBounceBracelet] = true;
	green_hill[Upgrades_SonicMysticMelody] = false;

	this->levelUpgrades[LevelIDs_GreenHill] = green_hill;
}

void StoryStyleUpgradeHandler::initTailsUpgrades() {
	std::unordered_map<Upgrades, bool> prison_lane;
	prison_lane[Upgrades_TailsBooster] = false;
	prison_lane[Upgrades_TailsBazooka] = false;
	prison_lane[Upgrades_TailsLaserBlaster] = false;
	prison_lane[Upgrades_TailsMysticMelody] = false;

	this->levelUpgrades[LevelIDs_TailsVsEggman1] = prison_lane;
	this->levelUpgrades[LevelIDs_PrisonLane] = prison_lane;

	if (!this->includeCurrentLevelUpgrade) {
		this->levelUpgrades[LevelIDs_MissionStreet] = prison_lane;
	}

	std::unordered_map<Upgrades, bool> hidden_base;
	hidden_base[Upgrades_TailsBooster] = true;
	hidden_base[Upgrades_TailsBazooka] = false;
	hidden_base[Upgrades_TailsLaserBlaster] = false;
	hidden_base[Upgrades_TailsMysticMelody] = false;

	this->levelUpgrades[LevelIDs_HiddenBase] = hidden_base;
	this->levelUpgrades[LevelIDs_EternalEngine] = hidden_base;
	this->levelUpgrades[LevelIDs_TailsVsEggman2] = hidden_base;
	this->levelUpgrades[LevelIDs_CannonsCoreT] = hidden_base;

	if (this->includeCurrentLevelUpgrade) {
		this->levelUpgrades[LevelIDs_MissionStreet] = hidden_base;
	}
}

void StoryStyleUpgradeHandler::initKnucklesUpgrades() {
	std::unordered_map<Upgrades, bool> wild_canyon;
	wild_canyon[Upgrades_KnucklesShovelClaw] = false;
	wild_canyon[Upgrades_KnucklesSunglasses] = false;
	wild_canyon[Upgrades_KnucklesHammerGloves] = false;
	wild_canyon[Upgrades_KnucklesAirNecklace] = false;
	wild_canyon[Upgrades_KnucklesMysticMelody] = false;

	this->levelUpgrades[LevelIDs_WildCanyon] = wild_canyon;

	if (!this->includeCurrentLevelUpgrade && !this->includeCurrentHuntingLevelUpgrade) {
		this->levelUpgrades[LevelIDs_PumpkinHill] = wild_canyon;
	}

	std::unordered_map<Upgrades, bool> aquatic_mine;
	aquatic_mine[Upgrades_KnucklesShovelClaw] = true;
	aquatic_mine[Upgrades_KnucklesSunglasses] = false;
	aquatic_mine[Upgrades_KnucklesHammerGloves] = false;
	aquatic_mine[Upgrades_KnucklesAirNecklace] = false;
	aquatic_mine[Upgrades_KnucklesMysticMelody] = false;

	this->levelUpgrades[LevelIDs_AquaticMine] = aquatic_mine;

	if (!this->includeCurrentLevelUpgrade && !this->includeCurrentHuntingLevelUpgrade) {
		this->levelUpgrades[LevelIDs_DeathChamber] = aquatic_mine;
	}

	if (this->includeCurrentLevelUpgrade || this->includeCurrentHuntingLevelUpgrade) {
		this->levelUpgrades[LevelIDs_PumpkinHill] = aquatic_mine;
	}

	std::unordered_map<Upgrades, bool> meteor_herd;
	meteor_herd[Upgrades_KnucklesShovelClaw] = true;
	meteor_herd[Upgrades_KnucklesSunglasses] = false;
	meteor_herd[Upgrades_KnucklesHammerGloves] = true;
	meteor_herd[Upgrades_KnucklesAirNecklace] = false;
	meteor_herd[Upgrades_KnucklesMysticMelody] = false;

	this->levelUpgrades[LevelIDs_KingBoomBoo] = meteor_herd;
	this->levelUpgrades[LevelIDs_MeteorHerd] = meteor_herd;
	this->levelUpgrades[LevelIDs_KnucklesVsRouge] = meteor_herd;
	this->levelUpgrades[LevelIDs_CannonsCoreK] = meteor_herd;

	if (this->includeCurrentLevelUpgrade || this->includeCurrentHuntingLevelUpgrade) {
		this->levelUpgrades[LevelIDs_DeathChamber] = meteor_herd;
	}
}

void StoryStyleUpgradeHandler::initShadowUpgrades() {
	std::unordered_map<Upgrades, bool> radical_highway;
	radical_highway[Upgrades_ShadowAirShoes] = false;
	radical_highway[Upgrades_ShadowAncientLight] = false;
	radical_highway[Upgrades_ShadowFlameRing] = false;
	radical_highway[Upgrades_ShadowMysticMelody] = false;

	this->levelUpgrades[LevelIDs_HotShot] = radical_highway;
	this->levelUpgrades[LevelIDs_RadicalHighway] = radical_highway;

	if (this->disableAllShadowUpgrades) {
		this->levelUpgrades[LevelIDs_WhiteJungle] = radical_highway;
		this->levelUpgrades[LevelIDs_SkyRail] = radical_highway;
		this->levelUpgrades[LevelIDs_FinalChase] = radical_highway;
		this->levelUpgrades[LevelIDs_Biolizard] = radical_highway;

		this->levelUpgrades[LevelIDs_SonicVsShadow1][Upgrades_ShadowAirShoes] = false;
		this->levelUpgrades[LevelIDs_SonicVsShadow1][Upgrades_ShadowAncientLight] = false;
		this->levelUpgrades[LevelIDs_SonicVsShadow1][Upgrades_ShadowFlameRing] = false;
		this->levelUpgrades[LevelIDs_SonicVsShadow1][Upgrades_ShadowMysticMelody] = false;
		this->levelUpgrades[LevelIDs_SonicVsShadow2][Upgrades_ShadowAirShoes] = false;
		this->levelUpgrades[LevelIDs_SonicVsShadow2][Upgrades_ShadowAncientLight] = false;
		this->levelUpgrades[LevelIDs_SonicVsShadow2][Upgrades_ShadowFlameRing] = false;
		this->levelUpgrades[LevelIDs_SonicVsShadow2][Upgrades_ShadowMysticMelody] = false;
		return;
	}
	
	if (!this->includeCurrentLevelUpgrade) {
		this->levelUpgrades[LevelIDs_WhiteJungle] = radical_highway;
	}

	std::unordered_map<Upgrades, bool> sky_rail;
	sky_rail[Upgrades_ShadowAirShoes] = true;
	sky_rail[Upgrades_ShadowAncientLight] = false;
	sky_rail[Upgrades_ShadowFlameRing] = false;
	sky_rail[Upgrades_ShadowMysticMelody] = false;

	this->levelUpgrades[LevelIDs_SkyRail] = sky_rail;
	this->levelUpgrades[LevelIDs_FinalChase] = sky_rail;
	this->levelUpgrades[LevelIDs_Biolizard] = sky_rail;

	if (this->includeCurrentLevelUpgrade) {
		this->levelUpgrades[LevelIDs_WhiteJungle] = sky_rail;
	}
	
	this->levelUpgrades[LevelIDs_SonicVsShadow1][Upgrades_ShadowAirShoes] = true;
	this->levelUpgrades[LevelIDs_SonicVsShadow1][Upgrades_ShadowAncientLight] = false;
	this->levelUpgrades[LevelIDs_SonicVsShadow1][Upgrades_ShadowFlameRing] = false;
	this->levelUpgrades[LevelIDs_SonicVsShadow1][Upgrades_ShadowMysticMelody] = false;
	this->levelUpgrades[LevelIDs_SonicVsShadow2][Upgrades_ShadowAirShoes] = true;
	this->levelUpgrades[LevelIDs_SonicVsShadow2][Upgrades_ShadowAncientLight] = false;
	this->levelUpgrades[LevelIDs_SonicVsShadow2][Upgrades_ShadowFlameRing] = false;
	this->levelUpgrades[LevelIDs_SonicVsShadow2][Upgrades_ShadowMysticMelody] = false;
}

void StoryStyleUpgradeHandler::initEggmanUpgrades() {
	std::unordered_map<Upgrades, bool> iron_gate;
	iron_gate[Upgrades_EggmanJetEngine] = false;
	iron_gate[Upgrades_EggmanLargeCannon] = false;
	iron_gate[Upgrades_EggmanLaserBlaster] = false;
	iron_gate[Upgrades_EggmanProtectiveArmor] = false;
	iron_gate[Upgrades_EggmanMysticMelody] = false;

	this->levelUpgrades[LevelIDs_IronGate] = iron_gate;

	std::unordered_map<Upgrades, bool> weapons_bed;
	weapons_bed[Upgrades_EggmanJetEngine] = true;
	weapons_bed[Upgrades_EggmanLargeCannon] = false;
	weapons_bed[Upgrades_EggmanLaserBlaster] = false;
	weapons_bed[Upgrades_EggmanProtectiveArmor] = false;
	weapons_bed[Upgrades_EggmanMysticMelody] = true;

	this->levelUpgrades[LevelIDs_WeaponsBed] = weapons_bed;
	this->levelUpgrades[LevelIDs_CosmicWall] = weapons_bed;
	this->levelUpgrades[LevelIDs_EggGolemE] = weapons_bed;
	this->levelUpgrades[LevelIDs_CannonsCoreE] = weapons_bed;

	if (this->includeCurrentLevelUpgrade) {
		std::unordered_map<Upgrades, bool> sand_ocean;
		sand_ocean[Upgrades_EggmanJetEngine] = false;
		sand_ocean[Upgrades_EggmanLargeCannon] = false;
		sand_ocean[Upgrades_EggmanLaserBlaster] = false;
		sand_ocean[Upgrades_EggmanProtectiveArmor] = false;
		sand_ocean[Upgrades_EggmanMysticMelody] = true;

		this->levelUpgrades[LevelIDs_SandOcean] = sand_ocean;
		this->levelUpgrades[LevelIDs_LostColony] = weapons_bed;
	} else {
		this->levelUpgrades[LevelIDs_SandOcean] = iron_gate;

		std::unordered_map<Upgrades, bool> lost_colony;
		lost_colony[Upgrades_EggmanJetEngine] = false;
		lost_colony[Upgrades_EggmanLargeCannon] = false;
		lost_colony[Upgrades_EggmanLaserBlaster] = false;
		lost_colony[Upgrades_EggmanProtectiveArmor] = false;
		lost_colony[Upgrades_EggmanMysticMelody] = true;

		this->levelUpgrades[LevelIDs_LostColony] = lost_colony;
	}

	this->levelUpgrades[LevelIDs_TailsVsEggman1][Upgrades_EggmanJetEngine] = true;
	this->levelUpgrades[LevelIDs_TailsVsEggman1][Upgrades_EggmanLargeCannon] = false;
	this->levelUpgrades[LevelIDs_TailsVsEggman1][Upgrades_EggmanLaserBlaster] = false;
	this->levelUpgrades[LevelIDs_TailsVsEggman1][Upgrades_EggmanProtectiveArmor] = false;
	this->levelUpgrades[LevelIDs_TailsVsEggman1][Upgrades_EggmanMysticMelody] = true;
	this->levelUpgrades[LevelIDs_TailsVsEggman2][Upgrades_EggmanJetEngine] = true;
	this->levelUpgrades[LevelIDs_TailsVsEggman2][Upgrades_EggmanLargeCannon] = false;
	this->levelUpgrades[LevelIDs_TailsVsEggman2][Upgrades_EggmanLaserBlaster] = false;
	this->levelUpgrades[LevelIDs_TailsVsEggman2][Upgrades_EggmanProtectiveArmor] = false;
	this->levelUpgrades[LevelIDs_TailsVsEggman2][Upgrades_EggmanMysticMelody] = true;
}

void StoryStyleUpgradeHandler::initRougeUpgrades() {
	std::unordered_map<Upgrades, bool> dry_lagoon;
	dry_lagoon[Upgrades_RougePickNails] = false;
	dry_lagoon[Upgrades_RougeTreasureScope] = false;
	dry_lagoon[Upgrades_RougeIronBoots] = false;
	dry_lagoon[Upgrades_RougeMysticMelody] = false;

	this->levelUpgrades[LevelIDs_DryLagoon] = dry_lagoon;

	if (!this->includeCurrentLevelUpgrade && !this->includeCurrentHuntingLevelUpgrade) {
		this->levelUpgrades[LevelIDs_EggQuarters] = dry_lagoon;
	}

	std::unordered_map<Upgrades, bool> security_hall;
	security_hall[Upgrades_RougePickNails] = true;
	security_hall[Upgrades_RougeTreasureScope] = false;
	security_hall[Upgrades_RougeIronBoots] = false;
	security_hall[Upgrades_RougeMysticMelody] = false;

	this->levelUpgrades[LevelIDs_SecurityHall] = security_hall;
	this->levelUpgrades[LevelIDs_FlyingDog] = security_hall;

	this->levelUpgrades[LevelIDs_KnucklesVsRouge][Upgrades_RougePickNails] = true;
	this->levelUpgrades[LevelIDs_KnucklesVsRouge][Upgrades_RougeTreasureScope] = false;
	this->levelUpgrades[LevelIDs_KnucklesVsRouge][Upgrades_RougeIronBoots] = true;
	this->levelUpgrades[LevelIDs_KnucklesVsRouge][Upgrades_RougeMysticMelody] = false;

	std::unordered_map<Upgrades, bool> mad_space;
	mad_space[Upgrades_RougePickNails] = true;
	mad_space[Upgrades_RougeTreasureScope] = false;
	mad_space[Upgrades_RougeIronBoots] = true;
	mad_space[Upgrades_RougeMysticMelody] = false;

	if (this->includeCurrentLevelUpgrade || this->includeCurrentHuntingLevelUpgrade) {
		this->levelUpgrades[LevelIDs_EggQuarters] = security_hall;
		this->levelUpgrades[LevelIDs_MadSpace] = mad_space;
	} else {
		this->levelUpgrades[LevelIDs_MadSpace] = security_hall;
	}

	this->levelUpgrades[LevelIDs_CannonsCoreR] = mad_space;

	if (StoryStyleUpgradeHandler::CCODetected) {
		this->levelUpgrades[LevelIDs_WildCanyon].insert(dry_lagoon.begin(), dry_lagoon.end());
		this->levelUpgrades[LevelIDs_AquaticMine].insert(security_hall.begin(), security_hall.end());
		this->levelUpgrades[LevelIDs_MeteorHerd].insert(mad_space.begin(), mad_space.end());

		if (!this->includeCurrentLevelUpgrade && !this->includeCurrentHuntingLevelUpgrade) {
			this->levelUpgrades[LevelIDs_PumpkinHill].insert(dry_lagoon.begin(), dry_lagoon.end());
			this->levelUpgrades[LevelIDs_DeathChamber].insert(security_hall.begin(), security_hall.end());
		} else {
			this->levelUpgrades[LevelIDs_PumpkinHill].insert(security_hall.begin(), security_hall.end());
			this->levelUpgrades[LevelIDs_DeathChamber].insert(mad_space.begin(), mad_space.end());
		}
	}
}

void StoryStyleUpgradeHandler::setUpgradeResetButton(std::string upgradeResetButton) {
	if (upgradeResetButton == "Y") {
		return; // Default, do nothing.
	}

	if (upgradeResetButton == "Z") {
		this->upgradeResetButton = Buttons_Z;
	} else if (upgradeResetButton == "Left Trigger") {
		this->upgradeResetButton = Buttons_L;
	} else if (upgradeResetButton == "Right Trigger") {
		this->upgradeResetButton = Buttons_R;
	}
}

int StoryStyleUpgradeHandler::handleCCOUpgrades(Upgrades upgrade, bool ignore_level_upgrades = false) {
	if (
		!StoryStyleUpgradeHandler::CCODetected
		|| CurrentCharacter != Characters_Sonic
		|| (!this->levelUpgrades[CurrentLevel][upgrade] && !ignore_level_upgrades)
	) {
		return 0;
	}

	switch (upgrade) {
		case Upgrades_SonicLightShoes:
			return Upgrades_ShadowAirShoes;

		case Upgrades_SonicAncientLight:
			return Upgrades_ShadowAncientLight;

		case Upgrades_SonicFlameRing:
			return Upgrades_ShadowFlameRing;

		case Upgrades_SonicMysticMelody:
			return Upgrades_ShadowMysticMelody;

		case Upgrades_SonicMagicGloves:
		case Upgrades_SonicBounceBracelet:
		case Upgrades_KnucklesAirNecklace:
			return upgrade;
	}

	return 0;
}

void StoryStyleUpgradeHandler::setLevelUpgrades() {
	if (this->levelUpgrades.count(CurrentLevel) <= 0 || MainCharObj2[0] == NULL || MainCharObj2[1] != NULL) {
		return; // Invalid Level
	}

	int upgrades = 0;
	for (Upgrades upgrade : this->characterUpgrades[CurrentCharacter]) {
		upgrades |= this->levelUpgrades[CurrentLevel][upgrade] ? upgrade : 0;
		upgrades |= this->handleCCOUpgrades(upgrade);
		StoryStyleUpgradeHandler::CharacterUpgradesGot[this->upgradesBitToIndexMap[upgrade]] = this->levelUpgrades[CurrentLevel][upgrade];
	}

	this->originalUpgrades = upgrades;
	MainCharObj2[0]->Upgrades = upgrades;
}

void StoryStyleUpgradeHandler::restoreLevelUpgrades() {
	MainCharObj2[0]->Upgrades = this->originalUpgrades;
	for (Upgrades upgrade : this->characterUpgrades[CurrentCharacter]) {
		StoryStyleUpgradeHandler::CharacterUpgradesGot[this->upgradesBitToIndexMap[upgrade]] = this->levelUpgrades[CurrentLevel][upgrade];
	}
}

void StoryStyleUpgradeHandler::setUpgradeResetButtonState() {
	this->isUpgradeResetButtonHeld = Controllers[0].on & this->upgradeResetButton;
}

void StoryStyleUpgradeHandler::checkRestartUpgradeReset() {
	if (!this->alwaysRestoreOnRestart && (!this->isUpgradeResetButtonHeld || !this->enableUpgradeRestoreOnRestart)) {
		return;
	}

	UpgradeHandler.restoreLevelUpgrades();
}

void StoryStyleUpgradeHandler::overwriteUpgradeItemComparison() {
	WriteJump((void*)0x6D8649, &StoryStyleUpgradeHandler::UpgradeItemComparison);
	for (unsigned short i = 0; i < 5; i++) {
		WriteData<1>((void*)(0x6D864E + i), 0x90u);
	}
}

bool StoryStyleUpgradeHandler::CharacterHasUpgrade(unsigned int upgrade) {
	if (upgrade >= StoryStyleUpgradeHandler::CharacterUpgradesGotSize) {
		return false;
	}

	return StoryStyleUpgradeHandler::CharacterUpgradesGot[upgrade];
}

__declspec(naked) void StoryStyleUpgradeHandler::UpgradeItemComparison() {
	__asm {
		push	eax
		call	StoryStyleUpgradeHandler::CharacterHasUpgrade
		cmp		eax, 0
		pop		eax
		pop		edi
		jz		UPGRADE_RETURN
		jmp		StoryStyleUpgradeHandler::JumpBackToLevelItemLoad

UPGRADE_RETURN:
		jmp		StoryStyleUpgradeHandler::JumpBackToLevelItemUpgradeLoad
	}
}

int UpgradeHook(int upgrade) {
	int upgrades = MainCharObj2[0]->Upgrades;
	int ret = hUpgradeGet.Original(upgrade);

	if (upgrades != MainCharObj2[0]->Upgrades) {
		int upgrade_got = MainCharObj2[0]->Upgrades - upgrades;
		StoryStyleUpgradeHandler::CharacterUpgradesGot[UpgradeHandler.upgradesBitToIndexMap[upgrade_got]] = true;
		MainCharObj2[0]->Upgrades |= UpgradeHandler.handleCCOUpgrades((Upgrades)upgrade_got, true);
	}

	return ret;
}

void SetCharacterPhysicsAndUpgrades(ObjectMaster* character, int a2) {
	SetPhysicsAndGiveUpgrades.Original(character, a2);
	UpgradeHandler.setLevelUpgrades();
}

void RestartLevel() {
	hRestartLevel.Original();

	// This is the only gamestate when restarting.
	if (GameState == GameStates_LoadFinished) {
		UpgradeHandler.checkRestartUpgradeReset();
	}
}