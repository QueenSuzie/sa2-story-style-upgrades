#pragma once
#include <unordered_map>

class StoryStyleUpgradeHandler {
	private:
		std::unordered_map<short, std::unordered_map<Upgrades, bool>> levelUpgrades;
		std::unordered_map<int, std::vector<Upgrades>> characterUpgrades;
		std::unordered_map<Upgrades, bool> originalUpgradeGots;
		bool includeCurrentLevelUpgrade = false;
		bool disableAllShadowUpgrades = false;
		bool disableSonicFlameRing = false;
		int originalUpgrades = -1;

		void initCharacterUpgrades();
		void initSonicUpgrades();
		void initTailsUpgrades();
		void initKnucklesUpgrades();
		void initShadowUpgrades();
		void initEggmanUpgrades();
		void initRougeUpgrades();
		void setUpgrade(Upgrades, bool);

	public:
		void init(bool includeCurrentLevelUpgrade, bool disableAllShadowUpgrades, bool disableSonicFlameRing);
		void setLevelUpgrades();
		void restoreLevelUpgrades();
		bool hasUpgrade(Upgrades);
};

