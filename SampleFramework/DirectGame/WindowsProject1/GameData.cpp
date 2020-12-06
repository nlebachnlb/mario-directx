#include "GameData.h"
#include "Mathf.h"

GameData::GameData()
{
	world = 1; life = 5; coin = 0; score = 0;
}

void GameData::ModifyCoin(int coin, bool relative)
{
	this->coin = relative ? this->coin + coin : coin;

	if (this->coin > 99)
	{
		ModifyLife(this->life + 1);
		this->coin = 0;
	}
}

void GameData::ModifyLife(int life, bool relative)
{
	this->life = relative ? this->life + life : life;
	this->life = Mathf::Clamp(this->life, 0, 99);
}

void GameData::ModifyScore(long score)
{
	this->score = Mathf::Min(score, 9999999);
}
