#include "gunbotspawn.h"

CGunBotSpawn::CGunBotSpawn(CGameWorld *pGameWorld, vec2 Pos, CGameControllerTimeRun *pController) : CBotSpawn(pGameWorld, Pos, pController) {
	m_BotType = BOTTYPE_GUN;
	m_RespawnTimer = GameServer()->Tuning()->m_GunBotRespawnTime;
}