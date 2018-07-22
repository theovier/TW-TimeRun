#include <game/server/gamecontext.h>
#include "upwardstrap.h"

CUpwardsTrap::CUpwardsTrap(CGameWorld *pGameWorld, vec2 Pos) : CTrap(pGameWorld, Pos) {
	m_Offset = vec2(0, 14.0f);
	m_Dir = vec2(0, -1);
}

bool CUpwardsTrap::VictimInVerticalRange(vec2 victimPos) {
	return victimPos.y < m_Pos.y;
}