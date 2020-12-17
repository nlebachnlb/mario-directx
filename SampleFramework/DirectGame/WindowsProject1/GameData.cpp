#include "GameData.h"
#include "Mathf.h"

GameData::GameData()
{
	world = 1; life = 4; coin = 0; score = 0;
	cards = new std::vector<int>();
	tempData.lastNodeID = 0;
	tempData.currentNodeID = 0;
	tempData.status = GameplayStatus::None;
}

GameData::~GameData()
{
	delete cards;
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

void GameData::ModifyScore(long score, bool relative)
{
	this->score = Mathf::Min(relative ? this->score + score : score, 9999999);
}

void GameData::ResetCombo()
{
	if (combo > -1) combo = -1;
}

void GameData::GainCombo()
{
	combo = Mathf::Clamp(combo + 1, 0, 8);
}

int GameData::GetCombo()
{
	return this->combo;
}

WorldMapTempData GameData::GetWorldMapTempData()
{
	return tempData;
}

void GameData::SetWorldMapTempData(WorldMapTempData data)
{
	this->tempData = data;
}

std::vector<int>* GameData::GetCards()
{
	return cards;
}


void GameData::SetPowerup(std::string powerup)
{
	this->marioPowerup = powerup;
}

std::string GameData::GetPowerup()
{
	if (marioPowerup.empty()) marioPowerup = "SmallMario";
	return marioPowerup;
}
