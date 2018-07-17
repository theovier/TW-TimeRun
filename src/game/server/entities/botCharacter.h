#ifndef GAME_SERVER_BOTCHARACTER_H
#define GAME_SERVER_BOTCHARACTER_H

#include "character.h"

class CBotCharacter : public CCharacter {
public:
	CBotCharacter(CGameWorld *pWorld);
	void Tick() override;
private:
	void OnBotDeath(CPlayer* Killer, int Weapon);
	void Handle();
	void Aim(vec2 Target);
};

#endif