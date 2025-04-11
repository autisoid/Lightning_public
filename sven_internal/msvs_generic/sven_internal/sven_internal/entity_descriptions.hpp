/**
 * Copyright - xWhitey, 2024.
 * entity_descriptions.hpp - description
 *
 * Project (Project desc) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef ENTITY_DESCRIPTIONS_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in entity_descriptions.hpp
#else //ENTITY_DESCRIPTIONS_HPP_RECURSE_GUARD

#define ENTITY_DESCRIPTIONS_HPP_RECURSE_GUARD

#ifndef ENTITY_DESCRIPTIONS_HPP_GUARD
#define ENTITY_DESCRIPTIONS_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef enum struct EEntityRelationship : signed char {
	kNone = SCHAR_MIN,
	kNeutral,
	kFriend,
	kEnemy,
	kMinorEnemy
} EEntityRelationship;

typedef enum struct EEntityClass : signed char {
	kUnknown = SCHAR_MIN,

	//Player?
	kPlayer,

	//Respawn-Point
	kCheckpoint,

	//Science team
	kScientist,

	//Security Guard
	kSecurityGuard,

	//Bodyguard
	kBodyguard,

	//Rats
	kRat,

	//Soda can
	kSodaCan,

	//Furniture
	kFurniture,

	//Egg
	kEgg,

	//Mr. Friendly
	kMrFriendly,

	//Xen fauna
	kAlienGrunt,
	kBarnacle,
	kBullsquirt,
	kChumtoad,
	kIchthyosaur,
	kHoundeye,
	kHeadcrab,
	kSqueakGrenade,
	kZombie,
	kGonarch,
	kAlienController,
	kAlienSlave,
	kBoid,
	kGargantua,
	kGonome,
	kKingpin,
	kTentacle,
	kNihilanth,
	kXenCommander,
	kFlockingFloater,
	kStukabat,
	kProtozoan,
	kMantaRay,

	//Grenade
	kGrenade,

	//Leech (I believe it's a Xen creature)
	kLeech,

	//HECU
	kHumanGrunt,

	//Black Ops
	kBlackOps,

	//G-Man
	kGovernmentMan,

	//Race X
	kShockTrooper,
	kShockRifle,
	kGeneWorm,
	kPitDrone,
	kVoltigore,
	kPitWorm,

	//Turrets
	kTurret,

	//Leech-like piranha
	kPiranha,

	//Osprey
	kOsprey,

	//Apache
	kApache,

	//Mortar
	kMortar,

	//Cockroach
	kCockroach,

	//Gravestone
	kGravestone,

	//Backpack
	kBackpack,

	//Rocket
	kRocket,

	//Recon
	kRecon,

	//Crossbow Bolt
	kCrossbowBolt,

	//Hornet
	kHornet,

	//Items
	kItem,

	//Charger
	kCharger
} EEntityClass;

typedef enum struct EEntitySubclass : signed char {
	kUnspecified = SCHAR_MIN,

	//Xen fauna
	kBabyHeadcrab,
	kTheyHungerAlienGruntBoss,
	kTheyHungerGonome,
	kTheyHungerHoundeye,
	kTheyHungerAlienGruntZork,
	kDeceasedMantaRay,

	//Checkpoint
	kTheyHungerUmbrellaCheckpoint,

	//Black Ops,
	kFemaleAssasin,
	kMaleAssasin,

	//Scientists
	kRosenbergScientist,
	kGordonFreemanScientist,
	kKellerScientist,

	//Security guards
	kOtis,
	kBarney,
	kTheyHungerSheriff,

	//Race X
	kBabyVoltigore,

	//Rockets
	kHVRRocket, //High Velocity Aircraft Rocket
	kRPGRocket,

	//Furniture
	kGusTheForkliftDriver,
	kChair,
	kFileCabinet,
	kStaticForklift,
	kCrystal,
	kDeadScientist,
	kKey,
	kXenTree,
	kTree,
	kToolBox,
	kWheelchair,
	kSandbags,
	kDrillInstructor,
	kGordonFreemanFurniture,
	kGinaCrossHologram,
	kXenHair,
	kXenPlantlight,
	kBush,
	kFern,
	kFlower,
	kGib,
	kPlant,
	kFlag,
	kBall,
	kTeleporter,
	kKeycard,
	kDispenser,
	kFoodTray,
	kFlashlight,
	kCrateGibs,
	kToolboxSM,
	kStretcher,
	kPipe,
	kEyeScanner,
	kRocketEngine,
	kBook,
	kFungus,
	kFocusEmitter,
	kPoolEquipment,
	kAlienSlaveChains,
	kBrokenEyeScanner,
	kBubbles,
	kCharacter,
	kSporeSpawner,
	kPadlock,
	kLightningBolt,
	kSpike,
	kSpit,
	kHologram,
	kLoader,

	//HECU
	kHumanGruntSniper,
	kHumanGruntAlly,
	kHumanGruntAllyMedic,
	kHumanGruntAllyTorch,
	kHumanGruntHeavyStuff,
	kRoboGrunt,

	//Gargantua
	kBabyGargantua,

	//Turrets
	kSentry,
	kWallTurret,
	kMiniTurret,

	//Mortar
	kMortarShell,

	//Osprey
	kDeceasedOsprey,
	kBlackOpsOsprey,

	//Apache
	kBlackOpsApache,
	kTheyHungerBoss,

	//Alien Slave
	kSkeleton,

	//Zombie
	kZombieBarney,
	kZombieSoldier,

	//Items
	kHEVSuit,
	kWeapon,
	kAmmo,
	kCarBattery,
	kGasCan,

	//Chargers
	kHEVCharger,
	kHealthCharger,
	kHealthChargerDrink,
	kHEVGlass
} EEntitySubclass;

typedef enum struct EEntitySubtype : signed char {
	kUnknown = SCHAR_MIN,
	kPlayer,
	kItem,
	kOtherEntity
} EEntitySubtype;

typedef struct CEntityDescription {
	CEntityDescription(_In_z_ const char* _ModelName, _In_z_ const char* _HumanReadableName,
		_In_ EEntityRelationship _Relationship, _In_ EEntityClass _Class, _In_ EEntitySubclass _Subclass, _In_ EEntitySubtype _Subtype);

	EEntityRelationship m_eRelationship;
	EEntityClass m_eClass;
	EEntitySubclass m_eSubclass;
	EEntitySubtype m_eSubtype;
	const char* m_pszModelName;
	const char* m_pszHumanReadableName;
} CEntityDescription;

using CEntityDescription = struct CEntityDescription;

typedef struct CEntityDescriptions {
	CEntityDescriptions();

	void PushDescription(_In_ CEntityDescription* _Description);

	CEntityDescription* FindDescription(_In_z_ const char* _ModelName);

	CVector<CEntityDescription*>* m_pVecDescriptions;
} CEntityDescriptions;

using CEntityDescriptions = struct CEntityDescriptions;

#else //!__cplusplus
#error C++ compiler required to compile entity_descriptions.hpp
#endif //__cplusplus

#endif //ENTITY_DESCRIPTIONS_HPP_GUARD

#undef ENTITY_DESCRIPTIONS_HPP_RECURSE_GUARD
#endif //ENTITY_DESCRIPTIONS_HPP_RECURSE_GUARD