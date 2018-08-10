#include <game/server/gamecontext.h>
#include "movingbosspickup.h"

CMovingBossPickup::CMovingBossPickup(CGameWorld *pGameWorld, vec2 From, vec2 To, float Duration, int Type, int SubType) : CMovingPickup(pGameWorld, From, To, Duration, Type, SubType) {

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