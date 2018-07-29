#include <game/server/botplayer.h>
#include <game/server/entities/bots/gatekeeperbot.h>
#include "gatekeeperbotspawn.h"

CGatekeeperBotSpawn::CGatekeeperBotSpawn(CGameWorld *pGameWorld, vec2 Pos, CGameControllerTimeRun *pController, int DoorIndex) : CBotSpawn(pGameWorld, Pos, pController) {

	m_BotType = BOTTYPE_GATEKEEPER;
	m_DoorIndex = DoorIndex;
}

int CGatekeeperBotSpawn::Spawn() {
	int clientId = CBotSpawn::Spawn();
	CPlayer* mySpawnedBot = GameServer()->m_apPlayers[clientId];
	CGatekeeperBot* gatekeeper = (CGatekeeperBot*)mySpawnedBot->GetCharacter();
	if (gatekeeper) {
		gatekeeper->SetDoorIndex(m_DoorIndex);
	}
	return clientId;
}