#include "grenadebotspawn.h"

CGrenadeBotSpawn::CGrenadeBotSpawn(CGameWorld *pGameWorld, vec2 Pos, CGameControllerTimeRun *pController) : CBotSpawn(pGameWorld, Pos, pController) {
	m_BotType = BOTTYPE_GRENADE;
	m_RespawnTimer = GameServer()->Tuning()->m_GrenadeBotRespawnTime;
}