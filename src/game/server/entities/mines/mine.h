#ifndef GAME_SERVER_ENTITIES_MINE_H
#define GAME_SERVER_ENTITIES_MINE_H

#include <game/server/gamecontext.h>

class CMine : public CTrap {

public:
	CMine(CGameWorld *pWorld, vec2 Pos);

	virtual void Tick() override;

protected:
	int m_RespawnTick = -1;
	float m_RespawnTime = 5.0f;
	float m_RadiusEmote = 300.0f;
	float m_RadiusSound = 400.0f;

	virtual void WarnNearbyPlayers();
	virtual void MakeTickingNoise();
	virtual void SetEmotes();
	virtual void SetEmote(CCharacter* passenger);
	virtual void Fire(CCharacter* at);
	virtual void Respawn();
};
#endif