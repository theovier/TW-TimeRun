#include "shotgunbotspawn.h"

CShotgunBotSpawn::CShotgunBotSpawn(CGameWorld *pGameWorld, vec2 Pos, CGameControllerTimeRun *pController) : CBotSpawn(pGameWorld, Pos, pController) {
	m_BotType = BOTTYPE_SHOTGUN;
	m_RespawnTimer = GameServer()->Tuning()->m_ShotgunBotRespawnTime;
}