#pragma once
#include <string>
#include <vector>
const int GAME_TIME = 300 * 1000;

enum class GameplayStatus
{
	None, Lose, Victory
};

struct WorldMapTempData
{
	int lastNodeID, currentNodeID;
	GameplayStatus status;
	WorldMapTempData() { lastNodeID = currentNodeID = 0; status = GameplayStatus::None; }
};

struct GameData
{
	GameData();
	~GameData();

	void ResetData();

	void ModifyCoin(int coin, bool relative = false);
	void ModifyLife(int life, bool relative = false);
	void ModifyScore(long score, bool relative = false);

	void ResetCombo();
	void GainCombo();
	int GetCombo();

	WorldMapTempData GetWorldMapTempData();
	void SetWorldMapTempData(WorldMapTempData data);

	std::vector<int>* GetCards();

	void SetPowerup(std::string powerup);
	std::string GetPowerup();

	int world, life, coin;
	long score;

private:
	int combo;
	std::string marioPowerup;
	std::vector<int>* cards;

	WorldMapTempData tempData;
};

