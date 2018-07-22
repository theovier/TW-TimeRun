#include <game/server/gamecontext.h>

class CTrap : public CEntity {
public:
	CTrap(CGameWorld *pWorld, vec2 Pos);

	virtual void Tick();

protected:
	bool m_Active;
	float m_AttackSpeed;

};