#include "character.h"
#include <game/server/player.h>
#include "flag.h"
#include "checkpoint.h"

CCheckpoint::CCheckpoint(CGameWorld *pGameWorld, int Team, vec2 StandPos, int Index) : CFlag(pGameWorld, Team, StandPos) {
	m_Index = Index;
}

void CCheckpoint::Tick() {
	CCharacter* apCloseChars[MAX_CLIENTS];
	int closestChars = GameServer()->m_World.FindEntities(m_Pos, ms_PhysSize, (CEntity**)apCloseChars, MAX_CLIENTS, CGameWorld::ENTTYPE_CHARACTER);
	for (int i = 0; i < closestChars; i++) {
		CCharacter* character = apCloseChars[i];
		CPlayer* controller = character->GetPlayer();

		if (character->IsAlive() && !controller->IsBot()) {
			SaveProgress(character, controller);
			Regenerate(character, controller);
		}
	}	
}

void CCheckpoint::SaveProgress(CCharacter* beneficiary, CPlayer* controller) {
	int lastCheckpointIndex = controller->m_GameExp.m_LastFlag;
	if (lastCheckpointIndex != m_Index) {
		controller->m_GameExp.m_LastFlag = m_Index;
		GameServer()->SendChatTarget(controller->GetCID(), "Checkpoint reached.");
	}
}

void CCheckpoint::Regenerate(CCharacter* beneficiary, CPlayer* controller) {
	if(Server()->Tick() > controller->m_GameExp.m_RegenTimer + Server()->TickSpeed() * GameServer()->Tuning()->m_RegenTimer) {
		if (!RegenerateHealth(beneficiary)) {
			RegenerateAmmo(beneficiary);
		}
		controller->m_GameExp.m_RegenTimer = Server()->Tick();
	}
}

bool CCheckpoint::RegenerateHealth(CCharacter* beneficiary) {
	return beneficiary->IncreaseHealth(1);
}

void CCheckpoint::RegenerateAmmo(CCharacter* beneficiary) {
	int weapon = beneficiary->m_ActiveWeapon;
	if (beneficiary->m_aWeapons[weapon].m_Ammo != -1) {
		int MaxAmmo = g_pData->m_Weapons.m_aId[weapon].m_Maxammo;
		if (beneficiary->m_aWeapons[weapon].m_Ammo < MaxAmmo) {
			beneficiary->m_aWeapons[weapon].m_Ammo++;
		}
	}
}

