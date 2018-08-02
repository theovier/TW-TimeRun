#ifndef GAME_SERVER_BOSSBOT_H
#define GAME_SERVER_BOSSBOT_H

#include <game/server/gamecontext.h>
#include <game/server/entities/bots/botcharacter.h>

class CBossBot : public CBotCharacter {

	MACRO_ALLOC_POOL_ID()

public:
	CBossBot(CGameWorld *pWorld);
	const char *GetDisplayName() override;
	virtual bool TakeDamage(vec2 Force, int Dmg, int From, int Weapon) override;
protected:
	const int GetBotType() { return BOTTYPE_ENDBOSS; };
private:
	//after this amount of dmg taken the boss enrages
	int m_EnrageDmgThreshold;
	bool m_IsEnraged;
	void Enrage();
};

#endif