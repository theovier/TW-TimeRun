#ifndef GAME_SERVER_BOTPLAYER_H
#define GAME_SERVER_BOTPLAYER_H

#include "player.h"
#include "entities/botcharacter.h"

class CBotPlayer : public CPlayer {
public:
	CBotPlayer(CGameContext *pGameServer, int ClientID, int Team);

	void InitBot(struct CBotSpawn *pSpawn);
	int m_BotType;
	struct CBotSpawn *m_BotSpawn;
};

#endif
