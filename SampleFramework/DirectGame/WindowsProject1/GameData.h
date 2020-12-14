#pragma once
#include <string>
#include <vector>
const int GAME_TIME = 300 * 1000;

struct GameData
{
	GameData();
	~GameData();

	void ModifyCoin(int coin, bool relative = false);
	void ModifyLife(int life, bool relative = false);
	void ModifyScore(long score, bool relative = false);
	
	void ResetCombo();
	void GainCombo();
	int GetCombo();

	std::vector<int>* GetCards();

	void SetPowerup(std::string powerup);
	std::string GetPowerup();

	int world, life, coin;
	long score;

private:
	int combo;
	std::string marioPowerup;
	std::vector<int>* cards;
};

