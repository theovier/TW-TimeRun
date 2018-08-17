#ifndef GAME_SERVER_BOSSBOT_H
#define GAME_SERVER_BOSSBOT_H

#include <game/server/gamecontext.h>
#include <game/server/entities/bots/botcharacter.h>

class CBossBot : public CBotCharacter {

	MACRO_ALLOC_POOL_ID()

public:
	CBossBot(CGameWorld *pWorld);
	const char *GetDisplayName() override;
	virtual void Tick() override;
	virtual bool TakeDamage(vec2 Force, int Dmg, int From, int Weapon) override;
	
protected:
	virtual void Handle() {};
	virtual bool ShouldDespawn() { return false; };
	const int GetBotType() { return BOTTYPE_ENDBOSS; };

private:
	enum PHASES {
		PHASE_IDLE,
		PHASE_SIMPLE,
		PHASE_MINIONS,
		PHASE_HEAL,
		PHASE_ENRAGE
	};
	int m_CurrentPhase;

	void TickCurrentPhase();
	void TickIdlePhase();
	void TickSimplePhase();
	void TickMinionPhase();
	void TickHealPhase();
	void TickEnrage();

	int m_MinionPhaseHPThreshold; //when the HP fall below this point, the boss enteres the minion phase
	int m_HealPhaseHPThreshold; //when the HP fall below this point, the boss enteres the heal phase
	int m_EnragePhaseHPThreshold; //when the HP fall below this point, the boss enrages

	void OnEnterFight();
	void EnterNextPhase();
	void EnterSimplePhase();
	void EnterMinionPhase();
	void EnterHealPhase();
	void EnterEnragePhase();
	
	void SummonMinions(int amount);
	void FreezeAllPlayers(float time);
	void HealSelf();
	void SpawnLightning();
};

#endif