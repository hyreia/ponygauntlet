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

	//LoadNeededBitmaps();
}

void GameDataRepository::Destroy()
{
	playerCharacterTypes.clear();
	spriteSheetAnimationTypes.clear();
	attackTypes.clear();
}

void GameDataRepository::CreatePlayerCharacterTypes()
{
	PlayerCharacterType temp;
	playerCharacterTypes.insert(playerCharacterTypes.begin(), NUM_OF_PLAYER_CHARACTER_TYPES, temp);

	PlayerCharacterType warrior(
		"Strawberry Surprise",
		"Warrior",
		Bitmap::Load("WarriorUpdate.png"),
		"An earth pony with incredibly strength and insatiable apetite for violence",
		"Strong melee attacks and heavy armor",
		200, 50, 50, 100,
		HAMMER_SWING);
	warrior.secondaryInherentActions.push_back(GROUND_POUND);
	warrior.secondaryInherentActions.push_back(HAMMER_SMASH);


	PlayerCharacterType wizard(
		"Golden Bolt",
		"Wizard",
		Bitmap::Load("WizardUpdate.png"),
		"A reserved unicorn with power and the wisdom to use it responsibly",
		"Ranged and guided attacks",
		50, 200, 100, 50,
		BALL_LIGHTNING);
	wizard.secondaryInherentActions.push_back(LASER_BLAST);
	wizard.secondaryInherentActions.push_back(GUIDED_LIGHTNING);

	//valkyrie: 75, 75, 75, 175,
	PlayerCharacterType valkyrie(
		"Blue Mist",
		"Valkyrie",
		Bitmap::Load("ValkyrieUpdate.png"),
		"A swift pegasus dedicated to protecting the throne and kingdom",
		"Fast attacks, movement and flight",
		75, 75, 75, 175,
		HALBERD_SWING);
	valkyrie.secondaryInherentActions.push_back(HALBERD_PIERCE);
	valkyrie.secondaryInherentActions.push_back(FLIGHT);

	PlayerCharacterType archer(
		"Swift Leaf",
		"Archer",
		Bitmap::Load("ArcherUpdate.png"),
		"A charismatic pegasus with impecable aim",
		"Fast movement, long-ranged attacks and flight",
		50, 150, 150, 50,
		FIRED_ARROW);
	archer.secondaryInherentActions.push_back(TRIPLE_SHOT);
	archer.secondaryInherentActions.push_back(GUIDED_ARROW);


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
		groundPound->Width()/groundPound->Height());

	auto warhammerSwing = Bitmap::Load("warhammer swing.png");
	SpriteSheetAnimationType hammerSwingAnim(warhammerSwing, true, 
		2.0, warhammerSwing->Height(), warhammerSwing->Width()/warhammerSwing->Height());


	auto hammerSmash = Bitmap::Load("hammer smash.png");
	SpriteSheetAnimationType hammerSmashAnim(hammerSmash, false, 0.15, hammerSmash->Height(),
		hammerSmash->Width()/hammerSmash->Height());

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
		crackle->Width()/crackle->Height());

	auto flash = Bitmap::Load("flash.png");
	SpriteSheetAnimationType flashAnim(flash, false, 0.1, flash->Height(), 
		flash->Width()/flash->Height());

	auto slash = Bitmap::Load("slash.png");
	SpriteSheetAnimationType slashAnim(slash, false, 0.2, slash->Height(),
		slash->Width()/slash->Height());

	auto smallFlash = Bitmap::Load("small flash.png");
	SpriteSheetAnimationType smallFlashAnim(smallFlash, false, 0.1, smallFlash->Height(),
		smallFlash->Width()/smallFlash->Height());

	auto swingArc = Bitmap::Load("swing arc.png");
	SpriteSheetAnimationType swingArcAnim(swingArc, false, 0.2, swingArc->Height(),
		swingArc->Width()/swingArc->Height());

	auto wallop = Bitmap::Load("wallop.png");
	SpriteSheetAnimationType wallopAnim(wallop, false, 0.2, wallop->Height(),
		wallop->Width()/wallop->Height());

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

}

void GameDataRepository::CreateAttackTypes()
{
	const int tps = GauntletApp::TPS;

	AttackType hammerSwing(PATH_AROUND_CHARACTER, 10, 10, 10, 1.0, 1.0, ANIM_HAMMER, ANIM_SWING_ARC, ANIM_WALLOP,
		IS_MELEE | KNOCKS_BACK);
	attackTypes.insert(std::pair<int, AttackType>(HAMMER_SWING, hammerSwing));

	AttackType groundPound(PATH_FORWARD, 10, 30, 64, 0.5, 0.0, NO_ANIMATION, ANIM_GROUND_POUND, ANIM_WALLOP,
		IS_MELEE | WILL_STUN  | PREVENTS_MOVEMENT);

	AttackType hammerSmash(PATH_FORWARD, 10, 20, 10, 3.0, 1.0, NO_ANIMATION, ANIM_HAMMER_SMASH, ANIM_WALLOP,
		IS_MELEE| WILL_STUN| PREVENTS_MOVEMENT | PREVENTS_ROTATING | DESTROYED_ON_HIT );

	AttackType ballLightning(PATH_FORWARD, -1.0, 10, 10, 1.0, 1.0, ANIM_BALL_LIGHTNING, ANIM_FLASH, ANIM_CRACKLE,
		DESTROYED_ON_HIT);

	AttackType laser(PATH_NON_MOVING_LINE, tps*3, 100, 5, 0.5, 0.0, ANIM_LASER, ANIM_FLASH, ANIM_FLASH,
		PREVENTS_MOVEMENT | PREVENTS_ROTATING);

	AttackType guidedBallLightning(PATH_FORWARD, -1.0, 10, 10, 1.0, 1.0, ANIM_BALL_LIGHTNING, ANIM_FLASH, ANIM_CRACKLE,
		DESTROYED_ON_HIT | MOVES_IN_ATTACKERS_FACING_DIRECTION);

	AttackType halberdSwing(PATH_AROUND_CHARACTER, 10, 10, 10, 1.0, 1.0, ANIM_HALBERD, ANIM_SWING_ARC, ANIM_SLASH,
		IS_MELEE);

	AttackType halberdPierce(PATH_FORWARD, 10, 20, 10, 2.0, 1.0, ANIM_HALBERD, NO_ANIMATION, ANIM_SLASH,
		IS_MELEE | DESTROYED_ON_HIT);

	AttackType arrow(PATH_FORWARD, -1, 5, 5, 1.0, 1.5, ANIM_ARROW, NO_ANIMATION, ANIM_SLASH,
		DESTROYED_ON_HIT);

	AttackType guidedArrow(PATH_FORWARD, -1, 5, 5, 1.0, 1.5, ANIM_GLOWING_ARROW, NO_ANIMATION, ANIM_SLASH,
		DESTROYED_ON_HIT | MOVES_IN_ATTACKERS_FACING_DIRECTION);

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
}

void GameDataRepository::LoadNeededBitmaps()
{
	for(auto charType = playerCharacterTypes.begin(); 
		charType != playerCharacterTypes.end(); charType++)
	{
		loadedBitmaps.push_back(charType->bitmap);
	}

	for(auto animationType = spriteSheetAnimationTypes.begin(); animationType != spriteSheetAnimationTypes.end();
		animationType++)
	{
		loadedBitmaps.push_back(animationType->bitmap);
	}

	for(auto attackType = attackTypes.begin(); attackType != attackTypes.end(); attackType++)
	{
	}
}