#include "loothandler.h"
#include "player.h"
#include "../generated/protocol.h"
#include <map>
#include <utility>
#include "loothandler.h"

void CLootHandler::HandleLoot(CGameWorld *pGameWorld, vec2 Pos, int BotType) {
	DropRandomLoot(pGameWorld, Pos, BotType);
}

void CLootHandler::DropRandomLoot(CGameWorld *pGameWorld, vec2 Pos, int BotType) {
	//todo put in .cfg
	//need to be sorted atm?
	//todo update droprates

	std::map<int, float> droprates;
	switch (BotType) {	
		case BOTTYPE_HAMMER:
			droprates[LOOT_GUN] = 0.05f;
			droprates[LOOT_GRENADE] = 0.05f;
			droprates[LOOT_ARMOR] = 0.20f;
			droprates[LOOT_POTION] = 0.20f;
			droprates[LOOT_HEALTH] = 0.50f;
			droprates[LOOT_LASER] = 0.00f;
			droprates[LOOT_NINJA] = 0.00f;
			droprates[LOOT_SHOTGUN] = 0.00f;
			break;
		case BOTTYPE_GUN:
			droprates[LOOT_GRENADE] = 0.01f;
			droprates[LOOT_SHOTGUN] = 0.01f;
			droprates[LOOT_POTION] = 0.10f;
			droprates[LOOT_GUN] = 0.25f;
			droprates[LOOT_ARMOR] = 0.29f;
			droprates[LOOT_HEALTH] = 0.29f;
			droprates[LOOT_LASER] = 0.00f;
			droprates[LOOT_NINJA] = 0.00f;
			break;
		case BOTTYPE_SHOTGUN:
			droprates[LOOT_POTION] = 0.10f;
			droprates[LOOT_HEALTH] = 0.30f;
			droprates[LOOT_ARMOR] = 0.30f;
			droprates[LOOT_SHOTGUN] = 0.30f;
			droprates[LOOT_GUN] = 0.0f;
			droprates[LOOT_GRENADE] = 0.0f;
			droprates[LOOT_LASER] = 0.00f;
			droprates[LOOT_NINJA] = 0.00f;
			break;
		case BOTTYPE_KAMIKAZE:
			droprates[LOOT_HEALTH] = 0.5f;
			droprates[LOOT_ARMOR] = 0.5f;
			droprates[LOOT_POTION] = 0.00f;
			droprates[LOOT_GUN] = 0.00f;
			droprates[LOOT_GRENADE] = 0.00f;
			droprates[LOOT_SHOTGUN] = 0.00f;
			droprates[LOOT_LASER] = 0.00f;
			droprates[LOOT_NINJA] = 0.00f;
			break;
		default:
			droprates[LOOT_HEALTH] = 1.0f;
			droprates[LOOT_ARMOR] = 0.0f;
			droprates[LOOT_POTION] = 0.0f;
			droprates[LOOT_GUN] = 0.0f;
			droprates[LOOT_GRENADE] = 0.0f;
			droprates[LOOT_SHOTGUN] = 0.0f;
			droprates[LOOT_LASER] = 0.0f;
			droprates[LOOT_NINJA] = 0.0f;
			break;
	}

	int lootId = GetRandomLoot(droprates);
	std::pair<int, int> pickupType = LootToPickup(lootId);
	DropLoot(pGameWorld, Pos, pickupType);
}

int CLootHandler::GetRandomLoot(std::map<int, float> Droprates) {
	int lootId = DefaultLootId;
	float rnd = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	for (std::map<int, float>::iterator it = Droprates.begin(); it != Droprates.end(); ++it) {
		float rate = it->second;
		if (rnd < rate) {
			lootId = it->first;
			break;
		}
		rnd -= rate;
	}
	return lootId;
}

const std::pair<int, int> CLootHandler::LootToPickup(const int LootId) {
	switch (LootId) {
	case LOOT_HEALTH:
		return std::pair<int, int>(0, 0);
	case LOOT_ARMOR:
		return std::pair<int, int>(1, 0);
	case LOOT_POTION:
		return std::pair<int, int>(4, 0); //= Potion
	case LOOT_GUN:
		return std::pair<int, int>(2, 1);
	case LOOT_GRENADE:
		return std::pair<int, int>(2, 3);
	case LOOT_SHOTGUN:
		return std::pair<int, int>(2, 2);
	case LOOT_LASER:
		return std::pair<int, int>(2, 4);
	case LOOT_NINJA:
		return std::pair<int, int>(3, 5); // e.g. Type = POWERUP_NINJA; SubType = WEAPON_NINJA; /game/server/generated line 258 initial respawn must be 0.
	default:
		return std::pair<int, int>(0, 0);
	}
}

void CLootHandler::DropLoot(CGameWorld *pGameWorld, vec2 Pos, std::pair<int, int> Type) {
	new CDrop(pGameWorld, Pos, Type.first, Type.second);
}

void CLootHandler::DropWeapon(CGameWorld *pGameWorld, vec2 Pos, int Type) {
	new CDrop(pGameWorld, Pos, POWERUP_WEAPON, Type);
}

void CLootHandler::DropItem(CGameWorld *pGameWorld, vec2 Pos, int Type) {
	new CDrop(pGameWorld, Pos, Type, 0);
}






