#ifndef GAME_SERVER_BOTPLAYER_H
#define GAME_SERVER_BOTPLAYER_H

#include "player.h"
#include "entities/bots/botcharacter.h"

class CBotPlayer : public CPlayer {
	MACRO_ALLOC_POOL_ID()
public:
	CBotPlayer(CGameContext *pGameServer, int ClientID, int Team);

	void Tick() override;
	void Snap(int SnappingClient) override;
	void InitBot(struct CBotSpawn *pSpawn);
	void RemovePermaWeapons() override {};
	int m_BotType;
	struct CBotSpawn *m_BotSpawn;
	bool m_MarkedForDestroy;

private:
	const char *GetDisplayName() override;

};

#endif
