#ifndef GAME_SERVER_ENTITIES_UPWARDSTRAP_H
#define GAME_SERVER_ENTITIES_UPWARDSTRAP_H

#include <game/server/gamecontext.h>

class CUpwardsTrap : public CTrap {

public:
	CUpwardsTrap(CGameWorld *pWorld, vec2 Pos);

protected:
	virtual bool VictimInVerticalRange(vec2 victimPos);
};
#endif