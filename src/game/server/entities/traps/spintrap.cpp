#include <game/server/gamecontext.h>
#include <math.h>
#include "spintrap.h"

#define DEG2RAD( a ) ( (a) * (pi/180.0f) )

CSpinTrap::CSpinTrap(CGameWorld *pGameWorld, vec2 Pos) : CTrap(pGameWorld, Pos) {
	m_FireSpeed = 0.25f;
	m_Angle = 4.0f;
}

void CSpinTrap::Tick() {
	CTrap::Tick();
	Spin();
}

void CSpinTrap::Spin() {
	float theta = DEG2RAD(m_Angle);
	float cs = cos(theta);
	float sn = sin(theta);
	float px = m_Dir.x * cs - m_Dir.y * sn;
	float py = m_Dir.x * sn + m_Dir.y * cs;
	m_Dir.x = px;
	m_Dir.y = py;
}

bool CSpinTrap::VictimInRange(vec2 victimPos) {
	return true;
}