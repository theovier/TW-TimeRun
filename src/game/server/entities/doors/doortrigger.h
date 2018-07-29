#ifndef GAME_SERVER_ENTITIES_DOORS_DOORTRIGGER_H
#define GAME_SERVER_ENTITIES_DOORS_DOORTRIGGER_H

class CDoorTrigger : public CEntity {

public:
	CDoorTrigger(CGameWorld *pGameWorld, vec2 Pos, int DoorIndex);

	virtual void Tick();

private:
	int m_DoorIndex;
	float m_AnimationTick;
	float m_AnimationTime = 1.0f;

};

#endif