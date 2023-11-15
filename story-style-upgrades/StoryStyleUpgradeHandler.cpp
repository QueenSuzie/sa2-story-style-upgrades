#include "pch.h"
#include <chrono>
#include <thread>

FunctionHook<void, task*> hStageLoad((intptr_t)0x47BB50);
FunctionHook<void, task*> hloadResultScreen((intptr_t)LoadResultScreenObjects);
FunctionHook<void, task*> hStageLoadUnloadHandler((intptr_t)0x43D510);

void StageLoadHook(task* tp) {
	hStageLoad.Original(tp);

	UpgradeHandler.setLevelUpgrades();
}

void StageUnloadHook(task* tp) {
	UpgradeHandler.restoreLevelUpgrades();

	hloadResultScreen.Original(tp);
}

void StageLoadUnloadHook(task* tp) {
	// Only on exit game or game over.
	if (GameState == GameStates_NormalExit || GameState == GameStates_Pause) {
		UpgradeHandler.restoreLevelUpgrades();
	}

	hStageLoadUnloadHandler.Original(tp);
}

void StoryStyleUpgradeHandler::init(bool includeCurrentLevelUpgrade, bool disableAllShadowUpgrades, bool disableSonicFlameRing) {
	this->includeCurrentLevelUpgrade = includeCurrentLevelUpgrade;
	this->disableAllShadowUpgrades = disableAllShadowUpgrades;
	this->disableSonicFlameRing = disableSonicFlameRing;

	this->initCharacterUpgrades();
	this->initSonicUpgrades();
	this->initTailsUpgrades();
	this->initKnucklesUpgrades();
	this->initShadowUpgrades();
	this->initEggmanUpgrades();
	this->initRougeUpgrades();

	hStageLoad.Hook(StageLoadHook);
	hloadResultScreen.Hook(StageUnloadHook);
	hStageLoadUnloadHandler.Hook(StageLoadUnloadHook);
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

	if (!this->includeCurrentLevelUpgrade) {
		this->levelUpgrades[LevelIDs_PumpkinHill] = wild_canyon;
	}

	std::unordered_map<Upgrades, bool> aquatic_mine;
	aquatic_mine[Upgrades_KnucklesShovelClaw] = true;
	aquatic_mine[Upgrades_KnucklesSunglasses] = false;
	aquatic_mine[Upgrades_KnucklesHammerGloves] = false;
	aquatic_mine[Upgrades_KnucklesAirNecklace] = false;
	aquatic_mine[Upgrades_KnucklesMysticMelody] = false;

	this->levelUpgrades[LevelIDs_AquaticMine] = aquatic_mine;

	if (!this->includeCurrentLevelUpgrade) {
		this->levelUpgrades[LevelIDs_DeathChamber] = aquatic_mine;
	}

	if (this->includeCurrentLevelUpgrade) {
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

	if (this->includeCurrentLevelUpgrade) {
		this->levelUpgrades[LevelIDs_DeathChamber] = meteor_herd;
	}
}

void StoryStyleUpgradeHandler::initShadowUpgrades() {
	if (this->disableAllShadowUpgrades) {
		return;
	}

	std::unordered_map<Upgrades, bool> radical_highway;
	radical_highway[Upgrades_ShadowAirShoes] = false;
	radical_highway[Upgrades_ShadowAncientLight] = false;
	radical_highway[Upgrades_ShadowFlameRing] = false;
	radical_highway[Upgrades_ShadowMysticMelody] = false;

	this->levelUpgrades[LevelIDs_RadicalHighway] = radical_highway;
	
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

	if (this->includeCurrentLevelUpgrade) {
		this->levelUpgrades[LevelIDs_WhiteJungle] = sky_rail;
	}
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
}

void StoryStyleUpgradeHandler::initRougeUpgrades() {
	std::unordered_map<Upgrades, bool> dry_lagoon;
	dry_lagoon[Upgrades_RougePickNails] = false;
	dry_lagoon[Upgrades_RougeTreasureScope] = false;
	dry_lagoon[Upgrades_RougeIronBoots] = false;
	dry_lagoon[Upgrades_RougeMysticMelody] = false;

	this->levelUpgrades[LevelIDs_DryLagoon] = dry_lagoon;

	if (!this->includeCurrentLevelUpgrade) {
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

	if (this->includeCurrentLevelUpgrade) {
		this->levelUpgrades[LevelIDs_EggQuarters] = security_hall;

		std::unordered_map<Upgrades, bool> mad_space;
		mad_space[Upgrades_RougePickNails] = true;
		mad_space[Upgrades_RougeTreasureScope] = false;
		mad_space[Upgrades_RougeIronBoots] = true;
		mad_space[Upgrades_RougeMysticMelody] = false;

		this->levelUpgrades[LevelIDs_MadSpace] = mad_space;
	} else {
		this->levelUpgrades[LevelIDs_MadSpace] = security_hall;
	}
}

void StoryStyleUpgradeHandler::setLevelUpgrades() {
	if (this->levelUpgrades.count(CurrentLevel) <= 0 || MainCharObj2[0] == NULL || MainCharObj2[1] != NULL) {
		return; // Invalid Level
	}

	int upgrades = 0;
	this->originalUpgradeGots.clear();
	for (Upgrades upgrade : this->characterUpgrades[CurrentCharacter]) {
		this->originalUpgradeGots[upgrade] = this->hasUpgrade(upgrade);
		upgrades |= this->levelUpgrades[CurrentLevel][upgrade] ? upgrade : 0;
		this->setUpgrade(upgrade, this->levelUpgrades[CurrentLevel][upgrade]);
	}

	this->originalUpgrades = MainCharObj2[0]->Upgrades;
	MainCharObj2[0]->Upgrades = upgrades;
}

void StoryStyleUpgradeHandler::restoreLevelUpgrades() {
	if (this->levelUpgrades.count(CurrentLevel) <= 0 || this->originalUpgradeGots.size() <= 0 || this->originalUpgrades < 0) {
		return;
	}

	for (Upgrades upgrade : this->characterUpgrades[CurrentCharacter]) {
		this->setUpgrade(upgrade, this->originalUpgradeGots[upgrade]);
	}
	
	MainCharObj2[0]->Upgrades = this->originalUpgrades;
	this->originalUpgradeGots.clear();
	this->originalUpgrades = -1;
}

void StoryStyleUpgradeHandler::setUpgrade(Upgrades upgrade, bool enabled) {
	switch (upgrade) {
		case Upgrades_SonicLightShoes:
			SonicLightShoesGot = enabled;
		case Upgrades_SonicAncientLight:
			SonicAncientLightGot = enabled;
		case Upgrades_SonicMagicGloves:
			SonicMagicGlovesGot = enabled;
		case Upgrades_SonicFlameRing:
			SonicFlameRingGot = enabled;
		case Upgrades_SonicBounceBracelet:
			SonicBounceBraceletGot = enabled;
		case Upgrades_SonicMysticMelody:
			SonicMysticMelodyGot = enabled;
		case Upgrades_TailsBooster:
			TailsBoosterGot = enabled;
		case Upgrades_TailsBazooka:
			TailsBazookaGot = enabled;
		case Upgrades_TailsLaserBlaster:
			TailsLaserBlasterGot = enabled;
		case Upgrades_TailsMysticMelody:
			TailsMysticMelodyGot = enabled;
		case Upgrades_KnucklesShovelClaw:
			KnucklesShovelClawGot = enabled;
		case Upgrades_KnucklesSunglasses:
			KnucklesSunglassesGot = enabled;
		case Upgrades_KnucklesHammerGloves:
			KnucklesHammerGlovesGot = enabled;
		case Upgrades_KnucklesAirNecklace:
			KnucklesAirNecklaceGot = enabled;
		case Upgrades_KnucklesMysticMelody:
			KnucklesMysticMelodyGot = enabled;
		case Upgrades_ShadowAirShoes:
			ShadowAirShoesGot = enabled;
		case Upgrades_ShadowAncientLight:
			ShadowAncientLightGot = enabled;
		case Upgrades_ShadowFlameRing:
			ShadowFlameRingGot = enabled;
		case Upgrades_ShadowMysticMelody:
			ShadowMysticMelodyGot = enabled;
		case Upgrades_EggmanJetEngine:
			EggmanJetEngineGot = enabled;
		case Upgrades_EggmanLargeCannon:
			EggmanLargeCannonGot = enabled;
		case Upgrades_EggmanLaserBlaster:
			EggmanLaserBlasterGot = enabled;
		case Upgrades_EggmanProtectiveArmor:
			EggmanProtectiveArmorGot = enabled;
		case Upgrades_EggmanMysticMelody:
			EggmanMysticMelodyGot = enabled;
		case Upgrades_RougePickNails:
			RougePickNailsGot = enabled;
		case Upgrades_RougeTreasureScope:
			RougeTreasureScopeGot = enabled;
		case Upgrades_RougeIronBoots:
			RougeIronBootsGot = enabled;
		case Upgrades_RougeMysticMelody:
			RougeMysticMelodyGot = enabled;
	}
}

bool StoryStyleUpgradeHandler::hasUpgrade(Upgrades upgrade) {
	if (MainCharObj2[0] == NULL) {
		return false;
	}

	return (MainCharObj2[0]->Upgrades & upgrade) == upgrade;
}