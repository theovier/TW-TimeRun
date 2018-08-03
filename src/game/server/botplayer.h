#ifndef GAME_SERVER_BOTPLAYER_H
#define GAME_SERVER_BOTPLAYER_H

#include "player.h"
#include "entities/spawns/botspawn.h"
#include "entities/bots/botcharacter.h"

class CBotPlayer : public CPlayer {
	MACRO_ALLOC_POOL_ID()
public:
	CBotPlayer(CGameContext *pGameServer, int ClientID, int Team);

	void Tick() override;
	void Reset() override;
	void Snap(int SnappingClient) override;
	void InitBot(CBotSpawn *pSpawn);
	bool RemovePermaWeapons() override { return false; };
	CBotSpawn* m_Spawn;
	void MarkForDestroy();
	const bool IsMarkedForDestroy() { return m_MarkedForDestroy; };
	virtual void OnDisconnect(const char *pReason) override;

private:
	const char *GetDisplayName() override;
	void InitTeeInfo();
	bool m_MarkedForDestroy;
};

#endif
