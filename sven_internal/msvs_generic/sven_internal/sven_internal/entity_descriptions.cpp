#include "StdAfx.h"

CEntityDescription::CEntityDescription(_In_z_ const char* _ModelName, _In_z_ const char* _HumanReadableName, _In_ EEntityRelationship _Relationship, _In_ EEntityClass _Class, _In_ EEntitySubclass _Subclass, _In_ EEntitySubtype _Subtype) {
	m_pszModelName = _ModelName;
	m_pszHumanReadableName = _HumanReadableName;
	m_eRelationship = _Relationship;
	m_eClass = _Class;
	m_eSubclass = _Subclass;
	m_eSubtype = _Subtype;
}

CEntityDescriptions::CEntityDescriptions() {
	//TODO: Add something like "kMinorEnemy" relationship because ragebot attacks hornets and xen trees (lol) - xWhitey

	m_pVecDescriptions = Q_new(CVector<CEntityDescription*>)({});

	PushDescription(Q_new(CEntityDescription)("any", "Unknown", EEntityRelationship::kNone, EEntityClass::kUnknown, EEntitySubclass::kUnspecified, EEntitySubtype::kUnknown));

	PushDescription(Q_new(CEntityDescription)("player.mdl", "Player", EEntityRelationship::kFriend, EEntityClass::kPlayer, EEntitySubclass::kUnspecified, EEntitySubtype::kPlayer));

	PushDescription(Q_new(CEntityDescription)("scientist.mdl", "Scientist", EEntityRelationship::kFriend, EEntityClass::kScientist, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("scientist2.mdl", "Scientist", EEntityRelationship::kFriend, EEntityClass::kScientist, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("gordon_scientist.mdl", "Gordon Freeman", EEntityRelationship::kFriend, EEntityClass::kScientist, EEntitySubclass::kGordonFreemanScientist, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("cleansuit_scientist.mdl", "Cleansuit Scientist", EEntityRelationship::kFriend, EEntityClass::kScientist, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("scientist_rosenberg.mdl", "Doctor Rosenberg", EEntityRelationship::kFriend, EEntityClass::kScientist, EEntitySubclass::kRosenbergScientist, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("wheelchair_sci.mdl", "Doctor Keller", EEntityRelationship::kFriend, EEntityClass::kScientist, EEntitySubclass::kKellerScientist, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("civ_scientist.mdl", "Scientist", EEntityRelationship::kFriend, EEntityClass::kScientist, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("civ_paper_scientist.mdl", "Scientist", EEntityRelationship::kFriend, EEntityClass::kScientist, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("console_civ_scientist.mdl", "Scientist", EEntityRelationship::kFriend, EEntityClass::kScientist, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("sc2sci.mdl", "Scientist", EEntityRelationship::kFriend, EEntityClass::kScientist, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("scigun.mdl", "Scientist's Shotgun", EEntityRelationship::kFriend, EEntityClass::kScientist, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("civ.mdl", "Evil Scientist", EEntityRelationship::kEnemy, EEntityClass::kScientist, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("civ_coat_scientist.mdl", "Scientist", EEntityRelationship::kFriend, EEntityClass::kScientist, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("gordon_suit.mdl", "Gordon Freeman", EEntityRelationship::kFriend, EEntityClass::kScientist, EEntitySubclass::kGordonFreemanScientist, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("barney.mdl", "Barney", EEntityRelationship::kFriend, EEntityClass::kSecurityGuard, EEntitySubclass::kBarney, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("wrangler.mdl", "Ammunation Barney", EEntityRelationship::kFriend, EEntityClass::kSecurityGuard, EEntitySubclass::kBarney, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("intro_barney.mdl", "Barney", EEntityRelationship::kFriend, EEntityClass::kSecurityGuard, EEntitySubclass::kBarney, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("dead_barney.mdl", "Barney", EEntityRelationship::kFriend, EEntityClass::kSecurityGuard, EEntitySubclass::kBarney, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("otis.mdl", "Otis", EEntityRelationship::kFriend, EEntityClass::kSecurityGuard, EEntitySubclass::kOtis, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("intro_otis.mdl", "Otis", EEntityRelationship::kFriend, EEntityClass::kSecurityGuard, EEntitySubclass::kOtis, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("barniel.mdl", "Barniel", EEntityRelationship::kFriend, EEntityClass::kSecurityGuard, EEntitySubclass::kBarney, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("agruntf.mdl", "Alien Grunt", EEntityRelationship::kFriend, EEntityClass::kAlienGrunt, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("sc2grunt.mdl", "Alien Grunt", EEntityRelationship::kFriend, EEntityClass::kAlienGrunt, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("hungerbarney.mdl", "Barnabus", EEntityRelationship::kEnemy, EEntityClass::kSecurityGuard, EEntitySubclass::kOtis /* uses deagle */, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("otisf.mdl", "Otto", EEntityRelationship::kEnemy, EEntityClass::kSecurityGuard, EEntitySubclass::kOtis, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("hungerotis.mdl", "Otto", EEntityRelationship::kEnemy, EEntityClass::kSecurityGuard, EEntitySubclass::kOtis, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("barnabus.mdl", "Barnabus", EEntityRelationship::kEnemy, EEntityClass::kSecurityGuard, EEntitySubclass::kBarney, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("sheriff.mdl", "Sheriff", EEntityRelationship::kEnemy, EEntityClass::kSecurityGuard, EEntitySubclass::kTheyHungerSheriff, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("headcrab.mdl", "Headcrab", EEntityRelationship::kEnemy, EEntityClass::kHeadcrab, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("dissected_headcrab.mdl", "Dissected Headcrab", EEntityRelationship::kNeutral, EEntityClass::kHeadcrab, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("kingheadcrab.mdl", "King Headcrab", EEntityRelationship::kEnemy, EEntityClass::kHeadcrab, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("baby_headcrab.mdl", "Baby Headcrab", EEntityRelationship::kEnemy, EEntityClass::kHeadcrab, EEntitySubclass::kBabyHeadcrab, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("hungercrab.mdl", "Baby Headcrab", EEntityRelationship::kEnemy, EEntityClass::kHeadcrab, EEntitySubclass::kBabyHeadcrab, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("thehand.mdl", "Hand", EEntityRelationship::kEnemy, EEntityClass::kHeadcrab, EEntitySubclass::kBabyHeadcrab, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("zombie.mdl", "Zombie", EEntityRelationship::kEnemy, EEntityClass::kZombie, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("zombie_soldier.mdl", "Zombie Soldier", EEntityRelationship::kEnemy, EEntityClass::kZombie, EEntitySubclass::kZombieSoldier, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("zombie_barney.mdl", "Zombie Barney", EEntityRelationship::kEnemy, EEntityClass::kZombie, EEntitySubclass::kZombieBarney, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("hungerzombie.mdl", "Hungry Zombie", EEntityRelationship::kEnemy, EEntityClass::kZombie, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("zombie2.mdl", "Hungry Zombie", EEntityRelationship::kEnemy, EEntityClass::kZombie, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("zombie3.mdl", "Hungry Zombie", EEntityRelationship::kEnemy, EEntityClass::kZombie, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("nursezombie.mdl", "Zombie Nurse", EEntityRelationship::kEnemy, EEntityClass::kZombie, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("lpzombie.mdl", "Hungry Zombie", EEntityRelationship::kEnemy, EEntityClass::kZombie, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("headless_zombie.mdl", "Headless Zombie", EEntityRelationship::kNeutral, EEntityClass::kZombie, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("bullsquid.mdl", "Bullsquid", EEntityRelationship::kEnemy, EEntityClass::kBullsquirt, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("bs_experiment.mdl", "Bullsquid", EEntityRelationship::kNeutral, EEntityClass::kBullsquirt, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("zombiebull.mdl", "Zombie Bull", EEntityRelationship::kEnemy, EEntityClass::kBullsquirt, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("houndeye.mdl", "Houndeye", EEntityRelationship::kEnemy, EEntityClass::kHoundeye, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("hungerhound.mdl", "Hungry Dog", EEntityRelationship::kEnemy, EEntityClass::kHoundeye, EEntitySubclass::kTheyHungerHoundeye, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("barnacle.mdl", "Barnacle", EEntityRelationship::kEnemy, EEntityClass::kBarnacle, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("islave.mdl", "Alien Slave", EEntityRelationship::kEnemy, EEntityClass::kAlienSlave, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("islavef.mdl", "Alien Slave", EEntityRelationship::kFriend, EEntityClass::kAlienSlave, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("dead_islave.mdl", "Deceased Alien Slave", EEntityRelationship::kNeutral, EEntityClass::kAlienSlave, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("skeleton.mdl", "Skeleton", EEntityRelationship::kNeutral, EEntityClass::kAlienSlave, EEntitySubclass::kSkeleton, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("sslave.mdl", "Alien Slave", EEntityRelationship::kEnemy, EEntityClass::kAlienSlave, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("hungerslave.mdl", "Skeleton", EEntityRelationship::kEnemy, EEntityClass::kAlienSlave, EEntitySubclass::kSkeleton, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("collar_test.mdl", "Alien Slave's Chains", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kAlienSlaveChains, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("hgrunt.mdl", "Human Grunt", EEntityRelationship::kEnemy, EEntityClass::kHumanGrunt, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("hgruntf.mdl", "Human Grunt", EEntityRelationship::kFriend, EEntityClass::kHumanGrunt, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("hgrunt_sniper.mdl", "Human Grunt", EEntityRelationship::kEnemy, EEntityClass::kHumanGrunt, EEntitySubclass::kHumanGruntSniper, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("agrunt.mdl", "Alien Grunt", EEntityRelationship::kEnemy, EEntityClass::kAlienGrunt, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("zork.mdl", "Zork", EEntityRelationship::kEnemy, EEntityClass::kAlienGrunt, EEntitySubclass::kTheyHungerAlienGruntZork, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("franklin2.mdl", "Hungry Alien Grunt", EEntityRelationship::kEnemy, EEntityClass::kAlienGrunt, EEntitySubclass::kTheyHungerAlienGruntBoss, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("tentacle2.mdl", "Tentacle", EEntityRelationship::kEnemy, EEntityClass::kTentacle, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("tentacle2T.mdl", "Tentacle", EEntityRelationship::kEnemy, EEntityClass::kTentacle, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("tentacle3.mdl", "Tentacle", EEntityRelationship::kEnemy, EEntityClass::kTentacle, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("stukabat.mdl", "Stukabat", EEntityRelationship::kEnemy, EEntityClass::kStukabat, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("turret.mdl", "Turret", EEntityRelationship::kEnemy, EEntityClass::kTurret, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("sentry.mdl", "Sentry Turret", EEntityRelationship::kEnemy, EEntityClass::kTurret, EEntitySubclass::kSentry, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("miniturret.mdl", "Mini Turret", EEntityRelationship::kEnemy, EEntityClass::kTurret, EEntitySubclass::kMiniTurret, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("deployable_sentry.mdl", "Deployable Turret", EEntityRelationship::kNeutral, EEntityClass::kTurret, EEntitySubclass::kSentry, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("leech.mdl", "Leech", EEntityRelationship::kEnemy, EEntityClass::kLeech, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("gman.mdl", "Government Man", EEntityRelationship::kEnemy, EEntityClass::kGovernmentMan, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("hassassin.mdl", "Female Assassin", EEntityRelationship::kEnemy, EEntityClass::kBlackOps, EEntitySubclass::kFemaleAssasin, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("hassassinf.mdl", "Female Assassin", EEntityRelationship::kFriend, EEntityClass::kBlackOps, EEntitySubclass::kFemaleAssasin, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("santas_helper.mdl", "Santa's Helper", EEntityRelationship::kFriend, EEntityClass::kBlackOps, EEntitySubclass::kFemaleAssasin, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("massn.mdl", "Male Assassin", EEntityRelationship::kEnemy, EEntityClass::kBlackOps, EEntitySubclass::kMaleAssasin, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("massnf.mdl", "Male Assassin", EEntityRelationship::kFriend, EEntityClass::kBlackOps, EEntitySubclass::kMaleAssasin, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("piranha.mdl", "Piranha", EEntityRelationship::kEnemy, EEntityClass::kPiranha, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("w_squeak.mdl", "Squeak", EEntityRelationship::kEnemy, EEntityClass::kSqueakGrenade, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("zombierat.mdl", "Zombie Rat", EEntityRelationship::kEnemy, EEntityClass::kSqueakGrenade, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("chubby.mdl", "Chumtoad", EEntityRelationship::kFriend, EEntityClass::kChumtoad, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("chumtoad.mdl", "Chumtoad", EEntityRelationship::kFriend, EEntityClass::kChumtoad, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("controller.mdl", "Alien Controller", EEntityRelationship::kEnemy, EEntityClass::kAlienController, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("icky.mdl", "Ichthyosaur", EEntityRelationship::kEnemy, EEntityClass::kIchthyosaur, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("garg.mdl", "Gargantua", EEntityRelationship::kEnemy, EEntityClass::kGargantua, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("babygarg.mdl", "Baby Gargantua", EEntityRelationship::kEnemy, EEntityClass::kGargantua, EEntitySubclass::kBabyGargantua, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("big_mom.mdl", "Gonarch", EEntityRelationship::kEnemy, EEntityClass::kGonarch, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("gonome.mdl", "Gonome", EEntityRelationship::kEnemy, EEntityClass::kGonome, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("hungergonome.mdl", "Hungry Gonome", EEntityRelationship::kEnemy, EEntityClass::kGonome, EEntitySubclass::kTheyHungerGonome, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("pit_drone.mdl", "Pit Drone", EEntityRelationship::kEnemy, EEntityClass::kPitDrone, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("strooper.mdl", "Shock Trooper", EEntityRelationship::kEnemy, EEntityClass::kShockTrooper, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("w_shock_rifle.mdl", "Shock Rifle", EEntityRelationship::kEnemy, EEntityClass::kShockRifle, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("mantaray.mdl", "Manta Ray", EEntityRelationship::kEnemy, EEntityClass::kMantaRay, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("flyer.mdl", "Manta Ray", EEntityRelationship::kEnemy, EEntityClass::kMantaRay, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("flyer_gibs.mdl", "Deceased Manta Ray", EEntityRelationship::kNeutral, EEntityClass::kMantaRay, EEntitySubclass::kDeceasedMantaRay, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("voltigore.mdl", "Voltigore", EEntityRelationship::kEnemy, EEntityClass::kVoltigore, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("baby_voltigore.mdl", "Baby Voltigore", EEntityRelationship::kEnemy, EEntityClass::kVoltigore, EEntitySubclass::kBabyVoltigore, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("pit_worm_up.mdl", "Pit Worm", EEntityRelationship::kEnemy, EEntityClass::kPitWorm, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("geneworm.mdl", "Gene Worm", EEntityRelationship::kEnemy, EEntityClass::kGeneWorm, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("osprey.mdl", "Osprey", EEntityRelationship::kEnemy, EEntityClass::kOsprey, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("osprey2.mdl", "Osprey", EEntityRelationship::kEnemy, EEntityClass::kOsprey, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("ospreyf.mdl", "Osprey", EEntityRelationship::kFriend, EEntityClass::kOsprey, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("dead_osprey.mdl", "Deceased Osprey", EEntityRelationship::kEnemy, EEntityClass::kOsprey, EEntitySubclass::kDeceasedOsprey, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("crashed_osprey.mdl", "Deceased Osprey", EEntityRelationship::kEnemy, EEntityClass::kOsprey, EEntitySubclass::kDeceasedOsprey, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("blkop_osprey.mdl", "Black Ops Osprey", EEntityRelationship::kEnemy, EEntityClass::kOsprey, EEntitySubclass::kBlackOpsOsprey, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("apache.mdl", "Apache", EEntityRelationship::kEnemy, EEntityClass::kApache, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("blkop_apache.mdl", "Black Ops Apache", EEntityRelationship::kEnemy, EEntityClass::kApache, EEntitySubclass::kBlackOpsApache, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("boss.mdl", "Helicopter", EEntityRelationship::kEnemy, EEntityClass::kApache, EEntitySubclass::kTheyHungerBoss, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("huey_apache.mdl", "Apache", EEntityRelationship::kEnemy, EEntityClass::kApache, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("mini_apache.mdl", "Apache", EEntityRelationship::kFriend, EEntityClass::kApache, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("nihilanth.mdl", "Nihilanth", EEntityRelationship::kEnemy, EEntityClass::kNihilanth, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("aflock.mdl", "Boid", EEntityRelationship::kNeutral, EEntityClass::kBoid, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("boid.mdl", "Boid", EEntityRelationship::kNeutral, EEntityClass::kBoid, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("hgrunt_opfor.mdl", "Human Grunt", EEntityRelationship::kFriend, EEntityClass::kHumanGrunt, EEntitySubclass::kHumanGruntAlly, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("hgrunt_opforf.mdl", "Human Grunt", EEntityRelationship::kFriend, EEntityClass::kHumanGrunt, EEntitySubclass::kHumanGruntAlly, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("hgrunt_medic.mdl", "Medic Human Grunt", EEntityRelationship::kFriend, EEntityClass::kHumanGrunt, EEntitySubclass::kHumanGruntAllyMedic, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("hgrunt_medicf.mdl", "Medic Human Grunt", EEntityRelationship::kFriend, EEntityClass::kHumanGrunt, EEntitySubclass::kHumanGruntAllyMedic, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("hgrunt_torch.mdl", "Torch Human Grunt", EEntityRelationship::kFriend, EEntityClass::kHumanGrunt, EEntitySubclass::kHumanGruntAllyTorch, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("hgrunt_torchf.mdl", "Torch Human Grunt", EEntityRelationship::kFriend, EEntityClass::kHumanGrunt, EEntitySubclass::kHumanGruntAllyTorch, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("mortar.mdl", "Mortar", EEntityRelationship::kEnemy, EEntityClass::kMortar, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("mortarshell.mdl", "Mortar Shell", EEntityRelationship::kEnemy, EEntityClass::kMortar, EEntitySubclass::kMortarShell, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("hwgrunt.mdl", "Heavy weapons Human Grunt", EEntityRelationship::kEnemy, EEntityClass::kHumanGrunt, EEntitySubclass::kHumanGruntHeavyStuff, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("hassault.mdl", "Heavy weapons Human Grunt", EEntityRelationship::kEnemy, EEntityClass::kHumanGrunt, EEntitySubclass::kHumanGruntHeavyStuff, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("hwgruntf.mdl", "Heavy weapons Human Grunt", EEntityRelationship::kFriend, EEntityClass::kHumanGrunt, EEntitySubclass::kHumanGruntHeavyStuff, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("rgrunt.mdl", "Robo Grunt", EEntityRelationship::kEnemy, EEntityClass::kHumanGrunt, EEntitySubclass::kRoboGrunt, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("rgruntf.mdl", "Robo Grunt", EEntityRelationship::kFriend, EEntityClass::kHumanGrunt, EEntitySubclass::kRoboGrunt, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("kingpin.mdl", "Kingpin", EEntityRelationship::kEnemy, EEntityClass::kKingpin, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("tor.mdl", "Tor", EEntityRelationship::kEnemy, EEntityClass::kXenCommander, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("torf.mdl", "Tor", EEntityRelationship::kFriend, EEntityClass::kXenCommander, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("lambda.mdl", "Respawn-Point", EEntityRelationship::kFriend, EEntityClass::kCheckpoint, EEntitySubclass::kUnspecified, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("umbrella.mdl", "Umbrella (Respawn-Point)", EEntityRelationship::kFriend, EEntityClass::kCheckpoint, EEntitySubclass::kTheyHungerUmbrellaCheckpoint, EEntitySubtype::kItem));

	PushDescription(Q_new(CEntityDescription)("bgman.mdl", "Bodyguard", EEntityRelationship::kNeutral, EEntityClass::kBodyguard, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("bgman.mdl", "Bodyguard", EEntityRelationship::kNeutral, EEntityClass::kBodyguard, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("bigrat.mdl", "Rat", EEntityRelationship::kFriend, EEntityClass::kRat, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("can.mdl", "Soda Can", EEntityRelationship::kFriend, EEntityClass::kSodaCan, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("chair.mdl", "Chair", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kChair, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("obj_chair.mdl", "Chair", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kChair, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("filecabinet.mdl", "File Cabinet", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kFileCabinet, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("forklift.mdl", "Gus", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kGusTheForkliftDriver, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("forklift_static.mdl", "Forklift", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kStaticForklift, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("crystal.mdl", "Crystal", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kCrystal, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("DecayCrystals.mdl", "Crystal", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kCrystal, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("deadhaz.mdl", "Deceased Scientist", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kDeadScientist, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("key.mdl", "Key", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kKey, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("tree.mdl", "Xen Tree", EEntityRelationship::kMinorEnemy, EEntityClass::kFurniture, EEntitySubclass::kXenTree, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("tree_palm.mdl", "Palm", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kTree, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("snow_tree.mdl", "Snow Tree", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kTree, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("tool_box.mdl", "Tool Box", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kToolBox, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("wheelchair.mdl", "Wheelchair", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kWheelchair, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("sandbags.mdl", "Sandbags", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kSandbags, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("drill.mdl", "Drill Instructor", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kDrillInstructor, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("gordon.mdl", "Gordon Freeman", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kGordonFreemanFurniture, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("holo.mdl", "Gina Cross", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kGinaCrossHologram, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("hair.mdl", "Xen Hair", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kXenHair, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("light.mdl", "Xen Plantlight", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kXenPlantlight, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("fungus(large).mdl", "Fungus", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kFungus, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("fungus(small).mdl", "Fungus", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kFungus, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("fungus.mdl", "Fungus", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kFungus, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("bush1.mdl", "Bush", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kBush, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("bush2.mdl", "Bush", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kBush, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("shrub1.mdl", "Dead Bush", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kBush, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("arc_bush.mdl", "Bush", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kBush, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("outz_tree1.mdl", "Tree", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kTree, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("arc_xer_tree1.mdl", "Tree", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kTree, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("arc_xer_tree2.mdl", "Tree", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kTree, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("tree1.mdl", "Tree", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kTree, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("tree2.mdl", "Tree", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kTree, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("zalec_tree1.mdl", "Tree", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kTree, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("ouitz_tree1.mdl", "Tree", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kTree, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("fern1.mdl", "Fern", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kFern, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("fern2.mdl", "Fern", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kFern, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("arc_fern.mdl", "Fern", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kFern, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("arc_flower.mdl", "Flower", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kFlower, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("uplant1.mdl", "Plant", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kPlant, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("uplant2.mdl", "Plant", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kPlant, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("uplant1_2x.mdl", "Plant", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kPlant, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("uplant3.mdl", "Plant", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kPlant, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("gib_skull.mdl", "Skull", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kGib, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("gib_legbone.mdl", "Leg", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kGib, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("zombiegibs1.mdl", "Leg", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kGib, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("ribcage.mdl", "Bone", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kGib, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("riblet1.mdl", "Bone", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kGib, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("hgibs.mdl", "Meat", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kGib, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("pit_drone_gibs.mdl", "Meat", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kGib, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("shock_effect.mdl", "Lightning Bolt", EEntityRelationship::kEnemy, EEntityClass::kFurniture, EEntitySubclass::kLightningBolt, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("pit_drone_spike.mdl", "Spike", EEntityRelationship::kEnemy, EEntityClass::kFurniture, EEntitySubclass::kSpike, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("vgibs.mdl", "Meat", EEntityRelationship::kEnemy, EEntityClass::kFurniture, EEntitySubclass::kGib, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("strooper_gibs.mdl", "Meat", EEntityRelationship::kEnemy, EEntityClass::kFurniture, EEntitySubclass::kGib, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("qmeal.mdl", "Meal", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kGib, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("base_flag.mdl", "Flag", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kFlag, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("flag.mdl", "Flag", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kFlag, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("ball.mdl", "Ball", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kBall, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("teleporter.mdl", "Teleporter", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kTeleporter, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("keycard.mdl", "Keycard", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kKeycard, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("dispenser.mdl", "Dispenser", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kDispenser, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("food_tray.mdl", "Food Tray", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kFoodTray, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("w_flashlight.mdl", "Flashlight", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kFlashlight, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("med_crategibs.mdl", "Technical Parts", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kCrateGibs, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("tech_crategibs.mdl", "Technical Parts", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kCrateGibs, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("computergibs.mdl", "Technical Parts", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kCrateGibs, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("bookgibs.mdl", "Book", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kBook, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("metalgibs.mdl", "Metal", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kCrateGibs, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("metalplategibs.mdl", "Metal Plate", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kCrateGibs, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("webgibs.mdl", "Cobweb", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kCrateGibs, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("stick.mdl", "Stick", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kCrateGibs, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("wetfloorsign_gibs.mdl", "Plastic", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kCrateGibs, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("ventgibs.mdl", "Broken Ventilation", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kCrateGibs, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("concrete_gibs.mdl", "Concrete", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kCrateGibs, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("woodgibs.mdl", "Wood", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kCrateGibs, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("glassgibs.mdl", "Glass", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kCrateGibs, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("garbagegibs.mdl", "Garbage", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kCrateGibs, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("office_gibs.mdl", "Papers", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kCrateGibs, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("gun_mag.mdl", "Magazine", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kCrateGibs, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("tool_box_sm.mdl", "Tool Box", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kToolboxSM, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("stretcher.mdl", "Stretcher", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kStretcher, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("pipe_1_straight.mdl", "Pipe", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kPipe, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("obj_pipe_1_straight.mdl", "Pipe", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kPipe, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("eye_scanner.mdl", "Eye Scanner", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kEyeScanner, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("eye_scanner_broken.mdl", "Broken Eye Scanner", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kBrokenEyeScanner, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("rengine.mdl", "Rocket Engine", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kRocketEngine, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("focus_emitter.mdl", "Focus Emitter", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kFocusEmitter, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("pool_equipment.mdl", "Pool Equipment", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kPoolEquipment, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("base.mdl", "Deceased Tentacle", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kGib, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("rip.mdl", "Deceased Tentacle", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kGib, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("pipe_bubbles.mdl", "Bubbles", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kBubbles, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("spit.mdl", "Spit", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kSpit, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("d0.mdl", "0", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kCharacter, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("d1.mdl", "1", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kCharacter, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("d2.mdl", "2", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kCharacter, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("d3.mdl", "3", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kCharacter, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("d4.mdl", "4", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kCharacter, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("d5.mdl", "5", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kCharacter, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("d6.mdl", "6", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kCharacter, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("d7.mdl", "7", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kCharacter, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("d8.mdl", "8", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kCharacter, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("d9.mdl", "9", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kCharacter, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("blood66p.mdl", "66%", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kCharacter, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("percent.mdl", "%", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kCharacter, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("arrow2d.mdl", "Arrow", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kCharacter, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("spore_ammo.mdl", "Spore Spawner", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kSporeSpawner, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("plant_01.mdl", "Plant", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kPlant, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("sat_globe.mdl", "Hologram", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kHologram, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("loader.mdl", "Loader", EEntityRelationship::kNeutral, EEntityClass::kFurniture, EEntitySubclass::kLoader, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("hvr.mdl", "HVR Rocket", EEntityRelationship::kEnemy, EEntityClass::kRocket, EEntitySubclass::kHVRRocket, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("rpgrocket.mdl", "RPG Rocket", EEntityRelationship::kEnemy, EEntityClass::kRocket, EEntitySubclass::kRPGRocket, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("friendly.mdl", "Mr. Friendly", EEntityRelationship::kNeutral, EEntityClass::kMrFriendly, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("roach.mdl", "Cockroach", EEntityRelationship::kNeutral, EEntityClass::kCockroach, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("egg.mdl", "Voltigore's Egg", EEntityRelationship::kNeutral, EEntityClass::kEgg, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("floater.mdl", "Flocking Floater", EEntityRelationship::kNeutral, EEntityClass::kFlockingFloater, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("cross.mdl", "Gravestone (Cross)", EEntityRelationship::kNeutral, EEntityClass::kGravestone, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("grave.mdl", "Gravestone", EEntityRelationship::kNeutral, EEntityClass::kGravestone, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("grave1.mdl", "Gravestone", EEntityRelationship::kNeutral, EEntityClass::kGravestone, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("grave2.mdl", "Gravestone", EEntityRelationship::kNeutral, EEntityClass::kGravestone, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("grave3.mdl", "Gravestone", EEntityRelationship::kNeutral, EEntityClass::kGravestone, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("grave4.mdl", "Gravestone", EEntityRelationship::kNeutral, EEntityClass::kGravestone, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("backpack.mdl", "Loot Backpack", EEntityRelationship::kNeutral, EEntityClass::kBackpack, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("item_carbattery.mdl", "Car Battery", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kCarBattery, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("item_gascan.mdl", "Gas Can", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kGasCan, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("item_toolbox.mdl", "Tool Box", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kToolBox, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("recruit.mdl", "Recon", EEntityRelationship::kNeutral, EEntityClass::kRecon, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("protozoa.mdl", "Protozoan", EEntityRelationship::kNeutral, EEntityClass::kProtozoan, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("grenade.mdl", "Grenade", EEntityRelationship::kNeutral, EEntityClass::kGrenade, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("w_tnt.mdl", "TNT", EEntityRelationship::kNeutral, EEntityClass::kGrenade, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("w_suit.mdl", "H.E.V. Suit", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kHEVSuit, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_blue_suit.mdl", "H.E.V. Suit", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kHEVSuit, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("suit2.mdl", "P.C.V. Suit", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kHEVSuit, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("gasmask.mdl", "Gas Mask", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kHEVSuit, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_2uzis.mdl", "2 Uzi's", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_2uzis_gold.mdl", "2 Golden Uzi's", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_9mmAR.mdl", "9mm AR", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_9mmhandgun.mdl", "9mm Handgun", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_357.mdl", ".357 Magnum", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_hunger357.mdl", ".357 Magnum", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_adrenaline.mdl", "Adrenaline Box", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_antidote.mdl", "Antidote Box", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_argrenade.mdl", "AR Grenades", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_ARgrenade.mdl", "AR Grenades", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_9mmarclip.mdl", "9mm AR Clip", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_9mmclip.mdl", "9mm Clip", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_1911_mag.mdl", "9mm Clip", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_1911.mdl", "9mm Handgun", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_357ammo.mdl", ".357 Ammo", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_357ammobox.mdl", ".357 Ammo Box", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_battery.mdl", "Suit Battery", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_bgrap.mdl", "Barnacle Grapple", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_chainammo.mdl", "5.56mm Ammo Box", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_crossbow.mdl", "Crossbow", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_crossbow_clip.mdl", "Crossbow Ammo", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_crowbar.mdl", "Crowbar", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("outro_crowbar.mdl", "Crowbar", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_desert_eagle.mdl", "Desert Eagle", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_displacer.mdl", "Displacer", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_egon.mdl", "Gluon Gun", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_gauss.mdl", "Gauss Gun", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_megagauss.mdl", "Mega Gauss Gun", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_gaussammo.mdl", "Gauss Ammo", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_grenade.mdl", "Grenade", EEntityRelationship::kEnemy, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_hgun.mdl", "Hivehand", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_isotopebox.mdl", "Isotope Box", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_longjump.mdl", "Longjump", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_greasegun.mdl", "Greasegun", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_greasegun_mag.mdl", "Greasegun Ammo", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_tesla.mdl", "Tesla Gun", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_tommygun.mdl", "Colt M1921A (Tommy Gun)", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_tommygun_mag.mdl", "Colt M1921A Ammo", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_m16.mdl", "M16", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_m16_mag.mdl", "M16 Ammo", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_spanner.mdl", "Spanner", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_m40a1.mdl", "M40A1 (Sniper Rifle)", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_m40a1clip.mdl", "M40A1 Ammo", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_m14.mdl", "M14 (Sniper Rifle)", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_m14_mag.mdl", "M14 Ammo", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_medkit.mdl", "Medkit", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_minigun.mdl", "Minigun", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_mp5.mdl", "MP5", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_mp5_clip.mdl", "MP5 Ammo", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_oxygen.mdl", "Oxygen Box", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_pipe_wrench.mdl", "Pipe Wrench", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_hungercrowbar.mdl", "Umbrella", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_shovel.mdl", "Shovel", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_pmedkit.mdl", "Medkit Ammo", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_syringebox.mdl", "Medkit Ammo", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("th_medkit.mdl", "Medkit", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_rad.mdl", "Rad Bottle", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_rpg.mdl", "RPG", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_rpgammo.mdl", "RPG Ammo", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_satchel.mdl", "Satchel Charge", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_saw.mdl", "M249", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_saw_clip.mdl", "M249 Ammo", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_shock.mdl", "Shock Rifle", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_shotbox.mdl", "Shotgun Ammo", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_shotgun.mdl", "Shotgun", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_dbarrel.mdl", "Double-barreled Shotgun", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_spore_launcher.mdl", "Spore Launcher", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_silencer.mdl", "Pistol with Silencer", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_sqknest.mdl", "Squeak Nest", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_svencoop_pickup.mdl", "Weapon Box", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_weaponbox.mdl", "Weapon Box", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_tripmine.mdl", "Tripmine", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_uzi.mdl", "Uzi", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_uzi_gold.mdl", "Golden Uzi", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_uzi_clip.mdl", "Uzi Ammo", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("barney_vest.mdl", "Barney's Vest", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kHEVSuit, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("barney_helmet.mdl", "Barney's Helmet", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kAmmo, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("knife.mdl", "Knife", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));
	PushDescription(Q_new(CEntityDescription)("w_knife.mdl", "Knife", EEntityRelationship::kNeutral, EEntityClass::kItem, EEntitySubclass::kWeapon, EEntitySubtype::kItem));

	PushDescription(Q_new(CEntityDescription)("spore.mdl", "Spore", EEntityRelationship::kEnemy, EEntityClass::kGrenade, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("crossbow_bolt.mdl", "Crossbow Bolt", EEntityRelationship::kEnemy, EEntityClass::kCrossbowBolt, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("hornet.mdl", "Hornet", EEntityRelationship::kMinorEnemy, EEntityClass::kHornet, EEntitySubclass::kUnspecified, EEntitySubtype::kOtherEntity));

	PushDescription(Q_new(CEntityDescription)("baby_strooper.mdl", "Spore Launcher", EEntityRelationship::kNeutral, EEntityClass::kShockTrooper, EEntitySubclass::kWeapon, EEntitySubtype::kItem));

	PushDescription(Q_new(CEntityDescription)("hev.mdl", "H.E.V. Charger", EEntityRelationship::kNeutral, EEntityClass::kCharger, EEntitySubclass::kHEVCharger, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("health_charger_body.mdl", "Health Charger", EEntityRelationship::kNeutral, EEntityClass::kCharger, EEntitySubclass::kHealthCharger, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("health_charger_both.mdl", "Health Charger Drink", EEntityRelationship::kNeutral, EEntityClass::kCharger, EEntitySubclass::kHealthChargerDrink, EEntitySubtype::kOtherEntity));
	PushDescription(Q_new(CEntityDescription)("hevglass.mdl", "H.E.V. Charger Glass", EEntityRelationship::kNeutral, EEntityClass::kCharger, EEntitySubclass::kHEVGlass, EEntitySubtype::kOtherEntity));
}

void CEntityDescriptions::PushDescription(_In_ CEntityDescription* _Description) {
	m_pVecDescriptions->push_back(_Description);
}

CEntityDescription* CEntityDescriptions::FindDescription(_In_z_ const char* _ModelName) {
	for (int idx = 0; idx < m_pVecDescriptions->size(); idx++) {
		CEntityDescription* lpDescription = m_pVecDescriptions->at(idx);
		if (!_stricmp(lpDescription->m_pszModelName, _ModelName))
			return lpDescription;
	}

	return nullptr;
}
