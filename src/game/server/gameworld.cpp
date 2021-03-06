/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */

#include "gameworld.h"
#include "entity.h"
#include "gamecontext.h"
#include "player.h"
#include <game/server/entities/bots/botCharacter.h>

//////////////////////////////////////////////////
// game world
//////////////////////////////////////////////////
CGameWorld::CGameWorld()
{
	m_pGameServer = 0x0;
	m_pServer = 0x0;

	m_Paused = false;
	m_ResetRequested = false;
	for(int i = 0; i < NUM_ENTTYPES; i++)
		m_apFirstEntityTypes[i] = 0;
}

CGameWorld::~CGameWorld()
{
	// delete all entities
	for(int i = 0; i < NUM_ENTTYPES; i++)
		while(m_apFirstEntityTypes[i])
			delete m_apFirstEntityTypes[i];
}

void CGameWorld::SetGameServer(CGameContext *pGameServer)
{
	m_pGameServer = pGameServer;
	m_pServer = m_pGameServer->Server();
}

CEntity *CGameWorld::FindFirst(int Type)
{
	return Type < 0 || Type >= NUM_ENTTYPES ? 0 : m_apFirstEntityTypes[Type];
}

int CGameWorld::FindEntities(vec2 Pos, float Radius, CEntity **ppEnts, int Max, int Type)
{
	if(Type < 0 || Type >= NUM_ENTTYPES)
		return 0;

	int Num = 0;
	for(CEntity *pEnt = m_apFirstEntityTypes[Type];	pEnt; pEnt = pEnt->m_pNextTypeEntity)
	{
		if(distance(pEnt->m_Pos, Pos) < Radius+pEnt->m_ProximityRadius)
		{
			if(ppEnts)
				ppEnts[Num] = pEnt;
			Num++;
			if(Num == Max)
				break;
		}
	}

	return Num;
}

int CGameWorld::FindEntities(vec2 Pos, CEntity **ppEnts, int Max, int Type) {
	if (Type < 0 || Type >= NUM_ENTTYPES)
		return 0;

	int Num = 0;
	for (CEntity *pEnt = m_apFirstEntityTypes[Type]; pEnt; pEnt = pEnt->m_pNextTypeEntity) {
		if (ppEnts)
			ppEnts[Num] = pEnt;
		Num++;
		if (Num == Max)
			break;
	}

	return Num;
}

void CGameWorld::InsertEntity(CEntity *pEnt)
{
#ifdef CONF_DEBUG
	for(CEntity *pCur = m_apFirstEntityTypes[pEnt->m_ObjType]; pCur; pCur = pCur->m_pNextTypeEntity)
		dbg_assert(pCur != pEnt, "err");
#endif

	// insert it
	if(m_apFirstEntityTypes[pEnt->m_ObjType])
		m_apFirstEntityTypes[pEnt->m_ObjType]->m_pPrevTypeEntity = pEnt;
	pEnt->m_pNextTypeEntity = m_apFirstEntityTypes[pEnt->m_ObjType];
	pEnt->m_pPrevTypeEntity = 0x0;
	m_apFirstEntityTypes[pEnt->m_ObjType] = pEnt;
}

void CGameWorld::DestroyEntity(CEntity *pEnt)
{
	pEnt->m_MarkedForDestroy = true;
}

void CGameWorld::RemoveEntity(CEntity *pEnt)
{
	// not in the list
	if(!pEnt->m_pNextTypeEntity && !pEnt->m_pPrevTypeEntity && m_apFirstEntityTypes[pEnt->m_ObjType] != pEnt)
		return;

	// remove
	if(pEnt->m_pPrevTypeEntity)
		pEnt->m_pPrevTypeEntity->m_pNextTypeEntity = pEnt->m_pNextTypeEntity;
	else
		m_apFirstEntityTypes[pEnt->m_ObjType] = pEnt->m_pNextTypeEntity;
	if(pEnt->m_pNextTypeEntity)
		pEnt->m_pNextTypeEntity->m_pPrevTypeEntity = pEnt->m_pPrevTypeEntity;

	// keep list traversing valid
	if(m_pNextTraverseEntity == pEnt)
		m_pNextTraverseEntity = pEnt->m_pNextTypeEntity;

	pEnt->m_pNextTypeEntity = 0;
	pEnt->m_pPrevTypeEntity = 0;
}

//
void CGameWorld::Snap(int SnappingClient)
{
	for(int i = 0; i < NUM_ENTTYPES; i++)
		for(CEntity *pEnt = m_apFirstEntityTypes[i]; pEnt; )
		{
			m_pNextTraverseEntity = pEnt->m_pNextTypeEntity;
			pEnt->Snap(SnappingClient);
			pEnt = m_pNextTraverseEntity;
		}
}

void CGameWorld::Reset()
{
	// reset all entities
	for(int i = 0; i < NUM_ENTTYPES; i++)
		for(CEntity *pEnt = m_apFirstEntityTypes[i]; pEnt; )
		{
			m_pNextTraverseEntity = pEnt->m_pNextTypeEntity;
			pEnt->Reset();
			pEnt = m_pNextTraverseEntity;
		}
	RemoveEntities();

	GameServer()->m_pController->PostReset();
	RemoveEntities();

	m_ResetRequested = false;
}

void CGameWorld::RemoveEntities()
{
	// destroy objects marked for destruction
	for(int i = 0; i < NUM_ENTTYPES; i++)
		for(CEntity *pEnt = m_apFirstEntityTypes[i]; pEnt; )
		{
			m_pNextTraverseEntity = pEnt->m_pNextTypeEntity;
			if(pEnt->m_MarkedForDestroy)
			{
				RemoveEntity(pEnt);
				pEnt->Destroy();
			}
			pEnt = m_pNextTraverseEntity;
		}
}

void CGameWorld::Tick()
{
	if(m_ResetRequested)
		Reset();

	if(!m_Paused)
	{
		// update all objects
		for(int i = 0; i < NUM_ENTTYPES; i++)
			for(CEntity *pEnt = m_apFirstEntityTypes[i]; pEnt; )
			{
				m_pNextTraverseEntity = pEnt->m_pNextTypeEntity;
				pEnt->Tick();
				pEnt = m_pNextTraverseEntity;
			}

		for(int i = 0; i < NUM_ENTTYPES; i++)
			for(CEntity *pEnt = m_apFirstEntityTypes[i]; pEnt; )
			{
				m_pNextTraverseEntity = pEnt->m_pNextTypeEntity;
				pEnt->TickDefered();
				pEnt = m_pNextTraverseEntity;
			}
	}
	else
	{
		// update all objects
		for(int i = 0; i < NUM_ENTTYPES; i++)
			for(CEntity *pEnt = m_apFirstEntityTypes[i]; pEnt; )
			{
				m_pNextTraverseEntity = pEnt->m_pNextTypeEntity;
				pEnt->TickPaused();
				pEnt = m_pNextTraverseEntity;
			}
	}

	RemoveEntities();
}


// TODO: should be more general
CCharacter *CGameWorld::IntersectCharacter(vec2 Pos0, vec2 Pos1, float Radius, vec2& NewPos, CEntity *pNotThis)
{
	// Find other players
	float ClosestLen = distance(Pos0, Pos1) * 100.0f;
	CCharacter *pClosest = 0;

	CCharacter *p = (CCharacter *)FindFirst(ENTTYPE_CHARACTER);
	for(; p; p = (CCharacter *)p->TypeNext())
 	{
		if(p == pNotThis)
			continue;

		vec2 IntersectPos = closest_point_on_line(Pos0, Pos1, p->m_Pos);
		float Len = distance(p->m_Pos, IntersectPos);
		if(Len < p->m_ProximityRadius+Radius)
		{
			Len = distance(Pos0, IntersectPos);
			if(Len < ClosestLen)
			{
				NewPos = IntersectPos;
				ClosestLen = Len;
				pClosest = p;
			}
		}
	}

	return pClosest;
}

bool CGameWorld::IsCharacterNearby(vec2 Pos, float Radius, class CEntity *pNotThis) {
	CCharacter *p = AnyClosestCharacter(Pos, Radius, pNotThis);
	if (p && distance(Pos, p->GetPos()) < Radius) {
		return true;
	}
	return false;
}

vec2 CGameWorld::FindEmptySpot(vec2 Pos, float Radius, float MinDistance, int MaxTries) {
	int tries = 0;
	while (true) {
		int x = frandom() * Radius;
		int y = frandom() * Radius;
		if (frandom() < 0.5f) x *= -1;
		if (frandom() < 0.5f) y *= -1;
		vec2 possibleSpot = Pos + vec2(x, y);

		bool charNearBy = IsCharacterNearby(possibleSpot, MinDistance);
		if (IsCharacterNearby(possibleSpot, MinDistance) ||
			GameServer()->Collision()->GetCollisionAt(possibleSpot.x + MinDistance / 3.f, possibleSpot.y - MinDistance / 3.f)&CCollision::COLFLAG_SOLID ||
			GameServer()->Collision()->GetCollisionAt(possibleSpot.x + MinDistance / 3.f, possibleSpot.y + MinDistance / 3.f)&CCollision::COLFLAG_SOLID ||
			GameServer()->Collision()->GetCollisionAt(possibleSpot.x - MinDistance / 3.f, possibleSpot.y - MinDistance / 3.f)&CCollision::COLFLAG_SOLID ||
			GameServer()->Collision()->GetCollisionAt(possibleSpot.x - MinDistance / 3.f, possibleSpot.y + MinDistance / 3.f)&CCollision::COLFLAG_SOLID)
		{
			if (++tries == MaxTries) {
				return vec2(0, 0);
			}
		}
		else {
			return possibleSpot;
		}
	}
}

CCharacter *CGameWorld::ClosestCharacter(vec2 Pos, float Radius, CEntity *pNotThis)
{
	// Find other players
	float ClosestRange = Radius*2;
	CCharacter *pClosest = 0;

	CCharacter *p = (CCharacter *)GameServer()->m_World.FindFirst(ENTTYPE_CHARACTER);
	for(; p; p = (CCharacter *)p->TypeNext())
 	{
		if(p == pNotThis)
			continue;

		bool isBot = dynamic_cast<const CBotCharacter*>(p) != nullptr;
		if (isBot)
			continue;

		float Len = distance(Pos, p->m_Pos);
		if(Len < p->m_ProximityRadius+Radius)
		{
			if(Len < ClosestRange)
			{
				ClosestRange = Len;
				pClosest = p;
			}
		}
	}

	return pClosest;
}

CCharacter *CGameWorld::ClosestBotCharacter(vec2 Pos, float Radius, CEntity *pNotThis) {
	float ClosestRange = Radius * 2;
	CCharacter *pClosest = 0;

	CCharacter *p = (CCharacter *)GameServer()->m_World.FindFirst(ENTTYPE_CHARACTER);
	for (; p; p = (CCharacter *)p->TypeNext())
	{
		if (p == pNotThis)
			continue;

		bool isBot = dynamic_cast<const CBotCharacter*>(p) != nullptr;
		if (!isBot)
			continue;

		float Len = distance(Pos, p->m_Pos);
		if (Len < p->m_ProximityRadius + Radius)
		{
			if (Len < ClosestRange)
			{
				ClosestRange = Len;
				pClosest = p;
			}
		}
	}

	return pClosest;
}

CCharacter *CGameWorld::AnyClosestCharacter(vec2 Pos, float Radius, CEntity *pNotThis) {
	float ClosestRange = Radius * 2;
	CCharacter *pClosest = 0;

	CCharacter *p = (CCharacter *)GameServer()->m_World.FindFirst(ENTTYPE_CHARACTER);
	for (; p; p = (CCharacter *)p->TypeNext())
	{
		if (p == pNotThis)
			continue;

		float Len = distance(Pos, p->m_Pos);
		if (Len < p->m_ProximityRadius + Radius)
		{
			if (Len < ClosestRange)
			{
				ClosestRange = Len;
				pClosest = p;
			}
		}
	}

	return pClosest;
}

CCharacter *CGameWorld::ClosestCharacterInLOS(vec2 Pos, float Radius, CEntity *pNotThis) {
	CCharacter *pClosest = 0;
	CCharacter *p = (CCharacter *)GameServer()->m_World.ClosestCharacter(Pos, Radius, pNotThis);
	for (; p; p = (CCharacter *)p->TypeNext()) {
		if (GameServer()->Collision()->HasLineOfSight(Pos, p->GetPos())) {
			pClosest = p;
			break;
		}
		else {
			continue;
		}
	}
	return pClosest;
}

CEntity *CGameWorld::ClosestEntity(int EntityType, vec2 Pos, CEntity *pNotThis) {
	float ClosestRange = FLT_MAX;
	CCharacter *pClosest = 0;

	CCharacter *p = (CCharacter *)GameServer()->m_World.FindFirst(EntityType);
	for (; p; p = (CCharacter *)p->TypeNext())
	{
		if (p == pNotThis)
			continue;

		float Len = distance(Pos, p->m_Pos);
		if (Len < ClosestRange)
		{
			ClosestRange = Len;
			pClosest = p;
		}
	}

	return pClosest;
}
