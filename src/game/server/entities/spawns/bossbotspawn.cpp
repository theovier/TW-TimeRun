#include "bossbotspawn.h"

CBossBotSpawn::CBossBotSpawn(CGameWorld *pGameWorld, vec2 Pos, CGameControllerEXP *pController) : CBotSpawn(pGameWorld, Pos, pController) {
	m_BotType = BOTTYPE_ENDBOSS;
}