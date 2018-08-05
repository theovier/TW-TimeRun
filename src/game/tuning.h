/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_TUNING_H
#define GAME_TUNING_H
#undef GAME_TUNING_H // this file will be included several times

// physics tuning
MACRO_TUNING_PARAM(GroundControlSpeed, ground_control_speed, 10.0f)
MACRO_TUNING_PARAM(GroundControlAccel, ground_control_accel, 100.0f / TicksPerSecond)
MACRO_TUNING_PARAM(GroundFriction, ground_friction, 0.5f)
MACRO_TUNING_PARAM(GroundJumpImpulse, ground_jump_impulse, 13.2f)
MACRO_TUNING_PARAM(AirJumpImpulse, air_jump_impulse, 12.0f)
MACRO_TUNING_PARAM(AirControlSpeed, air_control_speed, 250.0f / TicksPerSecond)
MACRO_TUNING_PARAM(AirControlAccel, air_control_accel, 1.5f)
MACRO_TUNING_PARAM(AirFriction, air_friction, 0.95f)
MACRO_TUNING_PARAM(HookLength, hook_length, 380.0f)
MACRO_TUNING_PARAM(HookFireSpeed, hook_fire_speed, 80.0f)
MACRO_TUNING_PARAM(HookDragAccel, hook_drag_accel, 3.0f)
MACRO_TUNING_PARAM(HookDragSpeed, hook_drag_speed, 15.0f)
MACRO_TUNING_PARAM(Gravity, gravity, 0.5f)

MACRO_TUNING_PARAM(VelrampStart, velramp_start, 550)
MACRO_TUNING_PARAM(VelrampRange, velramp_range, 2000)
MACRO_TUNING_PARAM(VelrampCurvature, velramp_curvature, 1.4f)

// weapon tuning
MACRO_TUNING_PARAM(GunCurvature, gun_curvature, 1.25f)
MACRO_TUNING_PARAM(GunSpeed, gun_speed, 2200.0f)
MACRO_TUNING_PARAM(GunLifetime, gun_lifetime, 2.0f)

MACRO_TUNING_PARAM(ShotgunCurvature, shotgun_curvature, 1.25f)
MACRO_TUNING_PARAM(ShotgunSpeed, shotgun_speed, 2750.0f)
MACRO_TUNING_PARAM(ShotgunSpeeddiff, shotgun_speeddiff, 0.8f)
MACRO_TUNING_PARAM(ShotgunLifetime, shotgun_lifetime, 0.20f)

MACRO_TUNING_PARAM(GrenadeCurvature, grenade_curvature, 7.0f)
MACRO_TUNING_PARAM(GrenadeSpeed, grenade_speed, 1000.0f)
MACRO_TUNING_PARAM(GrenadeLifetime, grenade_lifetime, 2.0f)

MACRO_TUNING_PARAM(LaserReach, laser_reach, 800.0f)
MACRO_TUNING_PARAM(LaserBounceDelay, laser_bounce_delay, 150)
MACRO_TUNING_PARAM(LaserBounceNum, laser_bounce_num, 1)
MACRO_TUNING_PARAM(LaserBounceCost, laser_bounce_cost, 0)
MACRO_TUNING_PARAM(LaserDamage, laser_damage, 5)

MACRO_TUNING_PARAM(PlayerCollision, player_collision, 1)
MACRO_TUNING_PARAM(PlayerHooking, player_hooking, 1)

// custom

// misc
MACRO_TUNING_PARAM(RespawnTimer, respawn_timer, 20.0f)
MACRO_TUNING_PARAM(RegenTimer, regen_timer, 1.0f)
MACRO_TUNING_PARAM(PoisonTimer, poison_timer, 1.0f)
MACRO_TUNING_PARAM(PickupLifetime, pickup_lifetime, 20.0f)
MACRO_TUNING_PARAM(TimeBonusFlagGain, time_bonus_flag, 15)

// turrets
MACRO_TUNING_PARAM(TurretGunLife, turret_gun_life, 10)
MACRO_TUNING_PARAM(TurretGunDamage, turret_gun_damage, 1)
MACRO_TUNING_PARAM(TurretGunAttackSpeed, turret_gun_attack_speed, 0.5f)
MACRO_TUNING_PARAM(TurretGunRespawnTime, turret_gun_respawn_time, 10)

MACRO_TUNING_PARAM(TurretLaserLife, turret_laser_life, 10)
MACRO_TUNING_PARAM(TurretLaserDamage, turret_laser_damage, 3)
MACRO_TUNING_PARAM(TurretLaserAttackSpeed, turret_laser_attack_speed, 1.5f)
MACRO_TUNING_PARAM(TurretLaserReach, turret_laser_reach, 800.0f)
MACRO_TUNING_PARAM(TurretLaserRespawnTime, turret_laser_respawn_time, 10)

// mines
MACRO_TUNING_PARAM(MineRespawnTime, mine_respawn_time, 15)
MACRO_TUNING_PARAM(MineWarnRadius, mine_warn_radius, 400.0f)

//bots
MACRO_TUNING_PARAM(BotDespawnTime, bot_despawn_time, 60)

MACRO_TUNING_PARAM(GunBotRespawnTime, bot_gun_respawn_time, 20.0f)
MACRO_TUNING_PARAM(GunBotLife, bot_gun_life, 15.0f)

MACRO_TUNING_PARAM(HammerBotRespawnTime, bot_hammer_respawn_time, 10.0f)
MACRO_TUNING_PARAM(HammerBotLife, bot_hammer_life, 10.0f)

MACRO_TUNING_PARAM(FlagBotLife, bot_flag_life, 25.0f)

MACRO_TUNING_PARAM(GatekeeperBotLife, bot_door_life, 25.0f)

MACRO_TUNING_PARAM(ShotgunBotRespawnTime, bot_shotgun_respawn_time, 20.0f)
MACRO_TUNING_PARAM(ShotgunBotLife, bot_shotgun_life, 20.0f)

// boss
MACRO_TUNING_PARAM(BossLife, boss_life, 200)
MACRO_TUNING_PARAM(BossDistancelimit, boss_distancelimit, 1000.0f)
MACRO_TUNING_PARAM(BossEnrageDmgThreshold, boss_enrage_dmg, 5)

// freezing
MACRO_TUNING_PARAM(FreezerReload, freezer_reload, 1.5f)
MACRO_TUNING_PARAM(FreezerTimer, freeze_timer, 2.5f)
#endif
