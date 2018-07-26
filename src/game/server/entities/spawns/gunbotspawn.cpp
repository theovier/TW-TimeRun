#include "gunbotspawn.h"

CGunBotSpawn::CGunBotSpawn(CGameWorld *pGameWorld, vec2 Pos, CGameControllerEXP *pController) : CBotSpawn(pGameWorld, Pos, pController) {
	m_BotType = BOTTYPE_GUN;
}