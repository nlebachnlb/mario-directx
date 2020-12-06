#pragma once
const int GAME_TIME = 300 * 1000;

struct GameData
{
	GameData();

	void ModifyCoin(int coin, bool relative = false);
	void ModifyLife(int life, bool relative = false);
	void ModifyScore(long score, bool relative = false);
	
	void ResetCombo();
	void GainCombo();
	int GetCombo();

	int world, life, coin;
	long score;

private:
	int combo;
};

