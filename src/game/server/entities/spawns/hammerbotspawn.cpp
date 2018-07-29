#include "hammerbotspawn.h"

CHammerBotSpawn::CHammerBotSpawn(CGameWorld *pGameWorld, vec2 Pos, CGameControllerTimeRun *pController) : CBotSpawn(pGameWorld, Pos, pController) {
	m_BotType = BOTTYPE_HAMMER;
	m_RespawnTimer = GameServer()->Tuning()->m_HammerBotRespawnTime;
}