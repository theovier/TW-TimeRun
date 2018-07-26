#ifndef BOTSPAWN_H
#define BOTSPAWN_H

#include <game/server/gamecontext.h>

class CGameControllerEXP;

class CBotSpawn : public CEntity {

public:
	CBotSpawn(CGameWorld *pWorld, vec2 Pos, CGameControllerEXP *pController);
	void Tick() override;
	void Reset() override;
	virtual vec2 GetPos();
	int GetBotType();
	virtual void OnSpawnlingsDeath();

protected:
	int m_RespawnTimer;
	int m_RespawnTick;
	int m_TriggerRadius;
	int m_BotType;

private:
	bool m_AlreadySpawned;
	bool CanSpawn();
	bool IsPlayerNearby();
	bool IsSpawnOccupied();
	void Spawn();
	CGameControllerEXP *m_Controller;
};

#endif
