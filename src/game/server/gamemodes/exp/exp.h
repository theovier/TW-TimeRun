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
#include <engine/console.h>

#include "environment.h"
#include "bots.h"

enum Items {
	POTION
};

struct CItems
{
	int m_Potions;
};

const int MAX_BOT_SPAWNS = 256;
const int MAX_TURRETS = 256;
const int MAX_MINES = 256;
const int MAX_TRAPS = 256;
const int MAX_DOORS = 128;
const int MAX_CHECKPOINTS = 32;

class CGameControllerEXP : public IGameController
{
public:

	CGameControllerEXP(class CGameContext *pGameServer);
	~CGameControllerEXP();

	bool m_BossDefeated = false;

	int m_CurTurret = 0, m_CurFlag = 0, m_CurMine = 0, m_CurTrap = 0, m_CurDoor = 0;
	CTurret *m_Turrets[MAX_TURRETS];
	CTrap *m_Traps[MAX_TRAPS];
	CMine *m_Mines[MAX_MINES];
	CDoor m_aDoors[MAX_DOORS];
	CCheckpoint *m_Checkpoints[MAX_CHECKPOINTS];

	// bots variables
	CBotSpawn m_aaBotSpawns[NUM_BOTTYPES][MAX_BOT_SPAWNS];
	int m_aNumBotSpawns[NUM_BOTTYPES];

	virtual void Tick();
	virtual bool OnEntity(int Index, vec2 Pos);
	virtual void StartRound() override;
	virtual void DoWincheck() override;
	virtual void PostReset() override;
	virtual int OnCharacterDeath(class CCharacter *pVictim, class CPlayer *pKiller, int Weapon) override;


	bool OnBotEntity(int BotType, vec2 pos);
	void BuildDoor(int d);
	void TickEnvironment();
	void TickTeleport(CPlayer* player);
	void TickWeaponStrip(CPlayer* player);
	void TickZones(CPlayer* player);
	void TickHealingZone(CCharacter* character, CPlayer* player);
	void TickPoisonZone(CCharacter* character, CPlayer* player);
	void TickBots();
	void RemoveBotsMarkedForDestroy();
	int BotCanSpawn();
	void BotSpawn(CBotSpawn *pSpawn);
	void RemoveBot(int ID, bool Killed);
	CCheckpoint * RegisterNewCheckpoint(vec2 Pos);
};
#endif
