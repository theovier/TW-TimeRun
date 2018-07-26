#ifndef GAME_SERVER_FLAGBOT_H
#define GAME_SERVER_FLAGBOT_H

#include <game/server/gamecontext.h>
#include <game/server/entities/bots/botcharacter.h>
#include <game/server/entities/checkpoint.h>

class CFlagBot : public CBotCharacter {

	MACRO_ALLOC_POOL_ID()

public:
	CFlagBot(CGameWorld *pWorld);
protected:
	void Handle() override;
	void OnDeath(CPlayer* Killer) override;
private:
	CCheckpoint* m_Flag;
};

#endif