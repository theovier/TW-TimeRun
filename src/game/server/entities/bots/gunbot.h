#include <game/server/entities/bots/botcharacter.h>

class CGunbot : public CBotCharacter {
	MACRO_ALLOC_POOL_ID()
public:
	CGunbot(CGameWorld *pWorld);
private:

	void SelectAppropriateWeapon(float distanceToTarget) override;
	void Move(vec2 Target) override;
	void Hook(vec2 Target) override {};
	const int GetBotType() { return BOTTYPE_GUN; };

	//rng for standing still or moving while attacking.
	float m_StillstandTick;
	float m_StillstandTime = 1.0f; //in seconds
	float m_StillstandChance = 0.65f;
	bool m_StandStill;

};