#include "GameDataRepository.hpp"
#include "ActionList.hpp"
#include "Bitmap.hpp"
#include "GauntletApp.hpp"

using namespace gauntlet;

void GameDataRepository::Initialize()
{
	CreatePlayerCharacterTypes();
	CreateAnimationTypes();
	CreateAttackTypes();
	CreateMonsterTypes();
	CreateSpawnerTypes();
	CreateFonts();
}

void GameDataRepository::Destroy()
{
	playerCharacterTypes.clear();
	spriteSheetAnimationTypes.clear();
	attackTypes.clear();
	monsterTypes.clear();
	fonts.clear();
}

void GameDataRepository::CreatePlayerCharacterTypes()
{
	PlayerCharacterType temp;
	playerCharacterTypes.insert(playerCharacterTypes.begin(), NUM_OF_PLAYER_CHARACTER_TYPES, temp);

	PlayerCharacterType warrior(
		"Strawberry Surprise",
		"Warrior",
		Bitmap::Load("Warrior.png"),
		"An earth pony with incredibly strength and insatiable apetite for violence",
		"Strong melee attacks and heavy armor",
		200, 50, 50, 100,
		Action(ATTACK_ACTION_TYPE, HAMMER_SWING, 0),
		XYPair<double>(96, 96), 50, 24);
	warrior.secondaryInherentActions.push_back(Action(ATTACK_ACTION_TYPE, GROUND_POUND, 1));
	warrior.secondaryInherentActions.push_back(Action(ATTACK_ACTION_TYPE, HAMMER_SMASH, 2));


	PlayerCharacterType wizard(
		"Golden Bolt",
		"Wizard",
		Bitmap::Load("Wizard.png"),
		"A reserved unicorn with power and the wisdom to use it responsibly",
		"Ranged and guided attacks",
		50, 200, 100, 50,
		Action(ATTACK_ACTION_TYPE, BALL_LIGHTNING, 3),
		XYPair<double>(96, 96), 50, 24);
	wizard.secondaryInherentActions.push_back(Action(ATTACK_ACTION_TYPE, LASER_BLAST, 4));
	wizard.secondaryInherentActions.push_back(Action(ATTACK_ACTION_TYPE, GUIDED_LIGHTNING, 5));

	//valkyrie: 75, 75, 75, 175,
	PlayerCharacterType valkyrie(
		"Blue Mist",
		"Valkyrie",
		Bitmap::Load("Valkyrie.png"),
		"A swift pegasus dedicated to protecting the throne and kingdom",
		"Fast attacks, movement and flight",
		75, 75, 75, 175,
		Action(ATTACK_ACTION_TYPE, HALBERD_SWING, 6),
		XYPair<double>(96, 96), 50, 24);
	valkyrie.secondaryInherentActions.push_back(Action(ATTACK_ACTION_TYPE, HALBERD_PIERCE, 7));
	valkyrie.secondaryInherentActions.push_back(Action(OTHER_ACTION_TYPE, FLIGHT, 8));

	PlayerCharacterType archer(
		"Swift Leaf",
		"Archer",
		Bitmap::Load("Archer.png"),
		"A charismatic unicorn with impecable aim",
		"Fast movement, long-ranged attacks and guided long-ranged attacks",
		50, 100, 150, 50,
		Action(ATTACK_ACTION_TYPE, FIRED_ARROW, 9),
		XYPair<double>(96, 96), 50, 24);
	archer.secondaryInherentActions.push_back(Action(TRIPLE_SHOT_ACTION, FIRED_ARROW, 10));
	archer.secondaryInherentActions.push_back(Action(ATTACK_ACTION_TYPE, GUIDED_ARROW, 11));


	playerCharacterTypes[PC_WARRIOR] = warrior;
	playerCharacterTypes[PC_WIZARD] = wizard;
	playerCharacterTypes[PC_VALKYRIE] = valkyrie;
	playerCharacterTypes[PC_ARCHER] = archer;
}

void GameDataRepository::CreateAnimationTypes()
{
	//Create the basic animations, setting if they loop, using their height to calculate
	//frames, then setting how much time is spent in each frame

	auto groundPound = Bitmap::Load("ground pound.png");
	SpriteSheetAnimationType groundPoundAnim(groundPound, false, 0.2, groundPound->Height(),
		groundPound->Width()/groundPound->Height(), false, true);

	auto warhammerSwing = Bitmap::Load("warhammer swing.png");
	SpriteSheetAnimationType hammerSwingAnim(warhammerSwing, true, 
		2.0, warhammerSwing->Height(), warhammerSwing->Width()/warhammerSwing->Height(), true, false);


	auto hammerSmash = Bitmap::Load("hammer smash.png");
	SpriteSheetAnimationType hammerSmashAnim(hammerSmash, false, 0.15, hammerSmash->Height(),
		hammerSmash->Width()/hammerSmash->Height(), true, false);

	auto ballLightning = Bitmap::Load("ball lightning.png");
	SpriteSheetAnimationType ballLightningAnim(ballLightning, true, 0.5, ballLightning->Height(),
		ballLightning->Width()/ballLightning->Height());

	auto laserBlast = Bitmap::Load("laser.png");
	SpriteSheetAnimationType laserBlastAnim(laserBlast, true, 1.0, laserBlast->Height(),
		laserBlast->Width()/laserBlast->Height());

	auto halberdSwing = Bitmap::Load("halberd.png");
	SpriteSheetAnimationType halberdAnim(halberdSwing, true, 2.0, halberdSwing->Height(),
		halberdSwing->Width()/halberdSwing->Height());

	auto arrow = Bitmap::Load("arrow.png");
	SpriteSheetAnimationType arrowAnim(arrow, true, 1.0, arrow->Height(),
		arrow->Width()/arrow->Height());

	auto glowingArrow = Bitmap::Load("glowing arrow.png");
	SpriteSheetAnimationType glowingArrowAnim(glowingArrow, true, 1.0, glowingArrow->Height(),
		glowingArrow->Width()/glowingArrow->Height());

	auto crackle = Bitmap::Load("crackle.png");
	SpriteSheetAnimationType crackleAnim(crackle, false, 0.2, crackle->Height(),
		crackle->Width()/crackle->Height(), false);

	auto flash = Bitmap::Load("flash.png");
	SpriteSheetAnimationType flashAnim(flash, false, 0.1, flash->Height(), 
		flash->Width()/flash->Height(), false, false);

	auto slash = Bitmap::Load("slash.png");
	SpriteSheetAnimationType slashAnim(slash, false, 0.2, slash->Height(),
		slash->Width()/slash->Height(), true);

	auto smallFlash = Bitmap::Load("small flash.png");
	SpriteSheetAnimationType smallFlashAnim(smallFlash, false, 0.1, smallFlash->Height(),
		smallFlash->Width()/smallFlash->Height());

	auto swingArc = Bitmap::Load("swing arc.png");
	SpriteSheetAnimationType swingArcAnim(swingArc, false, 0.2, swingArc->Height(),
		swingArc->Width()/swingArc->Height(), true);

	auto wallop = Bitmap::Load("wallop.png");
	SpriteSheetAnimationType wallopAnim(wallop, false, 0.2, wallop->Height(),
		wallop->Width()/wallop->Height());


	auto rustlingGrave = Bitmap::Load("shallow grave.png");
	SpriteSheetAnimationType graveAnim(rustlingGrave, false, 0.35, rustlingGrave->Height(),
		rustlingGrave->Width()/rustlingGrave->Height());


	SpriteSheetAnimationType temp;
	spriteSheetAnimationTypes.insert(spriteSheetAnimationTypes.begin(), NUM_OF_ANIMATIONS, temp);
	spriteSheetAnimationTypes[ANIM_GROUND_POUND] = groundPoundAnim;
	spriteSheetAnimationTypes[ANIM_HAMMER] = hammerSwingAnim;
	spriteSheetAnimationTypes[ANIM_CRACKLE] = crackleAnim;
	spriteSheetAnimationTypes[ANIM_FLASH] = flashAnim;
	spriteSheetAnimationTypes[ANIM_HAMMER_SMASH] = hammerSmashAnim;
	spriteSheetAnimationTypes[ANIM_BALL_LIGHTNING] = ballLightningAnim;
	spriteSheetAnimationTypes[ANIM_LASER] = laserBlastAnim;
	spriteSheetAnimationTypes[ANIM_HALBERD] = halberdAnim;
	spriteSheetAnimationTypes[ANIM_SLASH] = slashAnim;
	spriteSheetAnimationTypes[ANIM_ARROW] = arrowAnim;
	spriteSheetAnimationTypes[ANIM_GLOWING_ARROW] = glowingArrowAnim;
	spriteSheetAnimationTypes[ANIM_SMALL_FLASH] = smallFlashAnim;
	spriteSheetAnimationTypes[ANIM_SWING_ARC] = swingArcAnim;
	spriteSheetAnimationTypes[ANIM_WALLOP] = wallopAnim;
	spriteSheetAnimationTypes[ANIM_GRAVE] = graveAnim;

}

void GameDataRepository::CreateAttackTypes()
{
	const int tps = GauntletApp::TPS;

	AttackType hammerSwing(PATH_AROUND_CHARACTER, 10, 10, 10, 1.0, 3.0, ANIM_HAMMER, ANIM_SWING_ARC, ANIM_WALLOP,
		IS_MELEE | KNOCKS_BACK);
	//attackTypes.insert(std::pair<int, AttackType>(HAMMER_SWING, hammerSwing));

	AttackType groundPound(PATH_FORWARD, 10, 30, 64, 0.5, 0.0, NO_ANIMATION, ANIM_GROUND_POUND, ANIM_WALLOP,
		IS_MELEE | WILL_STUN  | PREVENTS_MOVEMENT);

	AttackType hammerSmash(PATH_FORWARD, 5, 20, 10, 3.0, 5.0, ANIM_HAMMER_SMASH, NO_ANIMATION, ANIM_WALLOP,
		IS_MELEE| WILL_STUN| PREVENTS_MOVEMENT | PREVENTS_ROTATING | DESTROYED_ON_HIT );


	AttackType ballLightning(PATH_FORWARD, -1.0, 10, 10, 1.0, 3.0, ANIM_BALL_LIGHTNING, ANIM_FLASH, ANIM_CRACKLE,
		DESTROYED_ON_HIT);

	AttackType laser(PATH_NON_MOVING_LINE, tps*3, 100, 5, 0.5, 0.0, ANIM_LASER, ANIM_FLASH, ANIM_FLASH,
		PREVENTS_MOVEMENT | PREVENTS_ROTATING);

	AttackType guidedBallLightning(PATH_FORWARD, -1.0, 10, 10, 1.0, 3.0, ANIM_BALL_LIGHTNING, ANIM_FLASH, ANIM_CRACKLE,
		DESTROYED_ON_HIT | MOVES_IN_ATTACKERS_FACING_DIRECTION);

	AttackType halberdSwing(PATH_AROUND_CHARACTER, 10, 10, 10, 1.0, 3.0, ANIM_HALBERD, ANIM_SWING_ARC, ANIM_SLASH,
		IS_MELEE);

	AttackType halberdPierce(PATH_FORWARD, 10, 20, 10, 2.0, 3.0, ANIM_HALBERD, NO_ANIMATION, ANIM_SLASH,
		IS_MELEE | DESTROYED_ON_HIT);

	AttackType arrow(PATH_FORWARD, -1, 5, 5, 1.0, 5.0, ANIM_ARROW, NO_ANIMATION, ANIM_SLASH,
		DESTROYED_ON_HIT);

	AttackType guidedArrow(PATH_FORWARD, -1, 5, 5, 1.0, 5.0, ANIM_GLOWING_ARROW, NO_ANIMATION, ANIM_SLASH,
		DESTROYED_ON_HIT | MOVES_IN_ATTACKERS_FACING_DIRECTION);

	AttackType skullChomp(PATH_FORWARD, 10, 50, 10, 1.0, 3.0, 
		NO_ANIMATION, NO_ANIMATION, ANIM_WALLOP, 
		IS_MELEE | PREVENTS_MOVEMENT | PREVENTS_ROTATING | DESTROYED_ON_HIT);

	attackTypes[HAMMER_SWING] = hammerSwing;
	attackTypes[GROUND_POUND] = groundPound;
	attackTypes[HAMMER_SMASH] = hammerSmash;
	attackTypes[BALL_LIGHTNING] = ballLightning;
	attackTypes[LASER_BLAST] = laser;
	attackTypes[GUIDED_LIGHTNING] = guidedBallLightning;
	attackTypes[HALBERD_SWING] = halberdSwing;
	attackTypes[HALBERD_PIERCE] = halberdPierce;
	attackTypes[FIRED_ARROW] = arrow;
	attackTypes[GUIDED_ARROW] = guidedArrow;
	attackTypes[SKULL_CHOMP] = skullChomp;
}

void GameDataRepository::CreateMonsterTypes()
{
	MonsterType skeleton;

	skeleton.maxHealth = 20;
	skeleton.maxEnergy = 100;
	skeleton.defaultAttackIndex = SKULL_CHOMP;
	skeleton.radius = 24;
	skeleton.height = 50;
	skeleton.startAltitude = 0;
	//skeleton.bitmap.reset();
	skeleton.bitmap = Bitmap::Load("Skeleton.png");
	skeleton.imageWidth = 96; skeleton.imageHeight = 96;
	skeleton.moveSpeed = 30;
	skeleton.thinkCooldown = 6;
	skeleton.aiTypeIndex = BASIC_MONSTER_AI;

	monsterTypes.insert(std::pair<int, MonsterType>(SPOOKY_BONES, skeleton));
}

void GameDataRepository::CreateSpawnerTypes()
{
	SpawnerType grave;
	grave.damagedAnimationIndex = ANIM_GRAVE;
	grave.spawningAnimationIndex = ANIM_GRAVE;
	grave.durability = 100;
	grave.idleAnimationIndex = ANIM_GRAVE;
	grave.isMovementImpedingWall = false;
	grave.monsterList.push_back(&monsterTypes[SPOOKY_BONES]);
	grave.monsterSelectionTechnique = SPAWNER_MONSTER_SELECT_IS_RANDOM;
	grave.spawnCooldownRate = (spriteSheetAnimationTypes[ANIM_GRAVE].NUM_OF_FRAMES+2)*
								spriteSheetAnimationTypes[ANIM_GRAVE].SECONDS_PER_FRAME;
	grave.spawnLimit = 1;

	spawnerTypes.insert(std::pair<int, SpawnerType>(SHALLOW_GRAVE, grave));

}

void GameDataRepository::CreateFonts()
{
	
	fonts.insert(std::pair<int, Font>(DEBUG_FONT, Font()));
	fonts[DEBUG_FONT].InitAsDebugFont();
	
	fonts.insert(std::pair<int, Font>(MESSAGE_FONT, Font()));
	fonts[MESSAGE_FONT].Initialize("atari full.ttf", 16);
}