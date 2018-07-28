/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_GAMEMODES_EXP_H
#define GAME_SERVER_GAMEMODES_EXP_H
#include <game/server/gamecontroller.h>
#include <game/server/entities/flag.h>
#include <game/server/entities/checkpoint.h>
#include <game/server/entities/traps/trap.h>
#include <game/server/entities/traps/upwardstrap.h>
#include <game/server/entities/turrets/turret.h>
#include <game/server/entities/turrets/gunturret.h>
#include <game/server/entities/turrets/laserturret.h>
#include <game/server/entities/mines/mine.h>
#include <game/server/entities/spawns/botspawn.h>
#include <engine/console.h>
#include "bots.h"

class CBotSpawn;

enum Items {
	POTION
};

struct CItems {
	int m_Potions;
};

const int MAX_BOT_SPAWNS = 256;
const int MAX_TURRETS = 256;
const int MAX_MINES = 256;
const int MAX_TRAPS = 256;
const int MAX_DOORS = 32;
const int MAX_CHECKPOINTS = 32;

class CGameControllerEXP : public IGameController {
public:

	CGameControllerEXP(class CGameContext *pGameServer);

	bool m_BossDefeated = false;

	int m_CurTurret = 0, m_CurFlag = 0, m_CurMine = 0, m_CurTrap = 0, m_CurDoor = 0, m_CurBotSpawn = 0;
	CTurret *m_Turrets[MAX_TURRETS];
	CTrap *m_Traps[MAX_TRAPS];
	CMine *m_Mines[MAX_MINES];
	CCheckpoint *m_Checkpoints[MAX_CHECKPOINTS];
	CBotSpawn *m_BotSpawns[MAX_BOT_SPAWNS];
	struct CDoorTemp {
		int m_State;
	} m_Door[MAX_DOORS];

	virtual void Tick() override;
	virtual void TickBots();
	virtual bool OnEntity(int Index, vec2 Pos) override;
	virtual void StartRound() override;
	virtual int OnCharacterDeath(class CCharacter *pVictim, class CPlayer *pKiller, int Weapon) override;
	virtual void DoWincheck() override;
	virtual void PostReset() override;
	CCheckpoint *RegisterNewCheckpoint(vec2 Pos);
	int GetFreePlayerSlotID();
	void RemoveBotsMarkedForDestroy();
	void RemoveBot(int ClientID);
	void KickBotsWhenServerEmpty();


	int GetDoorState(int Index);
	void SetDoorState(int Index, int State);
	void ResetDoorState();


};
#endif
