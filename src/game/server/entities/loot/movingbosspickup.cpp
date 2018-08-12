#include <game/server/gamecontext.h>
#include "movingbosspickup.h"

CMovingBossPickup::CMovingBossPickup(CGameWorld *pGameWorld, vec2 From, CEntity *Target, float Duration, int Type, int SubType) : CMovingPickup(pGameWorld, From, Target->GetPos(), Duration, Type, SubType) {
	m_Target = Target;
	GameServer()->CreateExplosion(From, -1, -1, true);
}

void CMovingBossPickup::Tick() {
	vec2 targetPos = m_Target->GetPos();
	m_Dir = vec2(targetPos.x - m_From.x, targetPos.y - m_From.y);
	m_Destination = targetPos;
	CMovingPickup::Tick();
}

void CMovingBossPickup::TickPickup() {
	CCharacter *pChr = GameServer()->m_World.ClosestBotCharacter(m_Pos, 20.0f, 0);
	if (pChr && pChr->IsAlive()) {
		switch (m_Type) {
			case POWERUP_HEALTH: {
				pChr->IncreaseHealth(GameServer()->Tuning()->m_BossSelfHeal);
				break;
			}
		}
		Reset();
	}
}
