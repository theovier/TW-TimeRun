#include "flagbotspawn.h"

CFlagBotSpawn::CFlagBotSpawn(CGameWorld *pGameWorld, vec2 Pos, CGameControllerEXP *pController) : CBotSpawn(pGameWorld, Pos, pController) {

	m_BotType = BOTTYPE_FLAGBEARER;
}