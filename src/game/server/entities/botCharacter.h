#ifndef GAME_SERVER_BOTCHARACTER_H
#define GAME_SERVER_BOTCHARACTER_H

#include "character.h"

class CBotCharacter : public CCharacter {
public:
	CBotCharacter(CGameWorld *pWorld);
	void Tick() override;
protected:
	virtual void Handle();
	void Aim(vec2 Target);
private:
	void OnBotDeath(CPlayer* Killer, int Weapon);
};

#endif