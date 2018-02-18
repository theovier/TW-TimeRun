#ifndef GAME_SERVER_BOTPLAYER_H
#define GAME_SERVER_BOTPLAYER_H

#include "player.h"

class CBotPlayer : public CPlayer {
public:
	CBotPlayer(CGameContext *pGameServer, int ClientID, int Team);
};

#endif
