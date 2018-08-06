#include "kamikazebotspawn.h"

CKamikazeBotSpawn::CKamikazeBotSpawn(CGameWorld *pGameWorld, vec2 Pos, CGameControllerTimeRun *pController) : CBotSpawn(pGameWorld, Pos, pController) {
	m_BotType = BOTTYPE_KAMIKAZE;
	m_RespawnTimer = GameServer()->Tuning()->m_KamikazeBotRespawnTime;
}