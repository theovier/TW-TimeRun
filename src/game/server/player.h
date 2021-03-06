/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_PLAYER_H
#define GAME_SERVER_PLAYER_H

// this include should perhaps be removed
#include "entities/character.h"
#include <game/server/gamemodes/timerun.h>
#include "gamecontext.h"

// player object
class CPlayer
{
	MACRO_ALLOC_POOL_ID()

public:
	CPlayer(CGameContext *pGameServer, int ClientID, int Team);
	virtual ~CPlayer();

	void Init(int CID);

	void TryRespawn();
	void Respawn();
	void SetTeam(int Team, bool DoChatMsg=true);
	int GetTeam() const { return m_Team; };
	int GetCID() const { return m_ClientID; };

	virtual void Tick();
	virtual void Reset();
	virtual void PostTick();
	virtual void Snap(int SnappingClient);

	void OnDirectInput(CNetObj_PlayerInput *NewInput);
	void OnPredictedInput(CNetObj_PlayerInput *NewInput);
	virtual void OnDisconnect(const char *pReason);

	void KillCharacter(int Weapon = WEAPON_GAME);
	CCharacter *GetCharacter();

	//---------------------------------------------------------
	// this is used for snapping so we know how we can clip the view for the player
	vec2 m_ViewPos;

	// states if the client is chatting, accessing a menu etc.
	int m_PlayerFlags;

	// used for snapping to just update latency if the scoreboard is active
	int m_aActLatency[MAX_CLIENTS];

	// used for spectator mode
	int m_SpectatorID;

	bool m_IsReady;

	//
	int m_Vote;
	int m_VotePos;
	//
	int m_LastVoteCall;
	int m_LastVoteTry;
	int m_LastChat;
	int m_LastSetTeam;
	int m_LastSetSpectatorMode;
	int m_LastChangeInfo;
	int m_LastEmote;
	int m_LastKill;

	// TODO: clean this up
	struct
	{
		char m_SkinName[64];
		int m_UseCustomColor;
		int m_ColorBody;
		int m_ColorFeet;
	} m_TeeInfos;

	int m_RespawnTick;
	int m_DieTick;
	int m_Score;
	int m_ScoreStartTick;
	bool m_ForceBalanced;
	int m_LastActionTick;
	int m_TeamChangeTick;
	struct
	{
		int m_TargetX;
		int m_TargetY;
	} m_LatestActivity;

	// network latency calculations
	struct
	{
		int m_Accum;
		int m_AccumMin;
		int m_AccumMax;
		int m_Avg;
		int m_Min;
		int m_Max;
	} m_Latency;

	// bots
	inline bool IsBot() { return m_Team == 1; };
	float m_NobodyTimer;

	struct WeaponStat
	{
		int m_StartAmmo;
		bool m_Got;
	};

	struct CItems {
		int m_Potions = 1;
	};

	struct CGame
	{
		int m_EnterTick; //ticks
		int m_Time; //seconds
		int m_Kills;
		int m_LastFlag;
		int m_Weapons;
		WeaponStat m_PermaWeapons[NUM_WEAPONS];
		int m_RegenTimer; //ticks
		int m_PoisonTimer; //ticks
		CItems m_Items;
	} m_GameStats;

	bool GiveWeaponPermanently(int Weapon, int PermaStartAmmo);
	bool HasWeaponPermanently(int Weapon);
	virtual bool RemovePermaWeapons();
	virtual void LoadPermaWeapons();
	virtual void ResetStats();

	virtual bool UseItem(int Item);
	virtual bool UsePotion();

	CCharacter *m_pCharacter;

	bool m_UseCustomColor;
	bool m_IsRainbow;
	int m_RainbowColor;
	int m_RainbowColorNumber;
	virtual void SetRainbow(bool active);
	virtual void TickRainbow();

protected:
	CGameContext *m_pGameServer;

	CGameContext *GameServer() const { return m_pGameServer; }
	IServer *Server() const;

	//
	bool m_Spawning;
	int m_ClientID;
	int m_Team;

	void SnapIngame(int SnappingClient);

	virtual const char *GetDisplayName();

};

#endif
