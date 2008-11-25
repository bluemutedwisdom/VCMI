#ifndef __GENIUS_AI__
#define __GENIUS_AI__

#define VCMI_DLL

#pragma warning (disable: 4100 4251 4245 4018 4081)
#include "../../AI_Base.h"
#include "../../CCallback.h"
#include "../../hch/CCreatureHandler.h"

#pragma warning (default: 4100 4251 4245 4018 4081)

namespace GeniusAI {

class CBattleHelper
{
public:
	CBattleHelper();
	~CBattleHelper();

	int GetBattleFieldPosition(int x, int y);
	int DecodeXPosition(int battleFieldPosition);
	int DecodeYPosition(int battleFieldPosition);

	int GetShortestDistance(int pointA, int pointB);
	int GetDistanceWithObstacles(int pointA, int pointB);

	int GetVoteForMaxDamage() const { return m_voteForMaxDamage; }
	int GetVoteForMinDamage() const { return m_voteForMinDamage; }
	int GetVoteForMaxSpeed() const { return m_voteForMaxSpeed; }
	int GetVoteForDistance() const { return m_voteForDistance; }
	int GetVoteForDistanceFromShooters() const { return m_voteForDistanceFromShooters; }
	int GetVoteForHitPoints() const { return m_voteForHitPoints; }

	const int InfiniteDistance;
	const int BattlefieldWidth;
	const int BattlefieldHeight;
private:
	int m_voteForMaxDamage;
	int m_voteForMinDamage;
	int m_voteForMaxSpeed;
	int m_voteForDistance;
	int m_voteForDistanceFromShooters;
	int m_voteForHitPoints;
};

/**
 * Maybe it is a additional proxy, but i've separated the game IA for transparent.
 */
class CBattleLogic
{
private:
	enum EMainStrategyType
	{
		strategy_super_aggresive,
		strategy_aggresive,
		strategy_neutral,
		strategy_defensive,
		strategy_super_defensive,
		strategy_stupid_attack /** only for debug and test purpose */
	};
	enum ECreatureRoleInBattle
	{
		creature_role_shooter,
		creature_role_defenser,
		creature_role_fast_attacker,
		creature_role_attacker
	};
	enum EActionType
	{
		action_cancel = 0,				// Cancel BattleAction   
		action_cast_spell = 1,			// Hero cast a spell   
		action_walk = 2,				// Walk   
		action_defend = 3,				// Defend   
		action_retreat = 4,				// Retreat from the battle   
		action_surrender = 5,			// Surrender   
		action_walk_and_attack = 6,		// Walk and Attack   
		action_shoot = 7,				// Shoot    
		action_wait = 8,				// Wait   
		action_catapult = 9,			// Catapult 
		action_monster_casts_spell = 10 // Monster casts a spell (i.e. Faerie Dragons)
	};
public:
	CBattleLogic(ICallback *cb);
	~CBattleLogic();

	void SetCurrentTurn(int turn);

	BattleAction MakeDecision(int stackID);
private:
	CBattleHelper m_battleHelper;
	int m_iCurrentTurn;
	bool m_bIsAttacker;
	ICallback *m_cb;

	void MakeStatistics(int currentCreatureId); // before decision we have to make some calculation and simulation
	// statistics
	typedef std::vector<std::pair<int, int> > creature_stat; // first - creature id, second - value
	creature_stat m_statMaxDamage;
	creature_stat m_statMinDamage;
	//
	creature_stat m_statMaxSpeed;
	creature_stat m_statDistance;
	creature_stat m_statDistanceFromShooters;
	creature_stat m_statHitPoints;

	std::vector<int> GetAvailableHexesForAttacker(CStack *defender);

	BattleAction MakeDefend(int stackID);
};

class CGeniusAI : public CGlobalAI
{
private:
	ICallback *m_cb;
	CBattleLogic *m_battleLogic;
public:
	virtual void init(ICallback * CB);
	virtual void yourTurn();
	virtual void heroKilled(const CGHeroInstance *);
	virtual void heroCreated(const CGHeroInstance *);
	virtual void heroMoved(const HeroMoveDetails &);
	virtual void heroPrimarySkillChanged(const CGHeroInstance * hero, int which, int val) {};
	virtual void showSelDialog(std::string text, std::vector<CSelectableComponent*> & components, int askID){};
	virtual void tileRevealed(int3 pos){};
	virtual void tileHidden(int3 pos){};
	virtual void heroGotLevel(const CGHeroInstance *hero, int pskill, std::vector<ui16> &skills, boost::function<void(ui32)> &callback);
	// battle
	virtual void actionFinished(const BattleAction *action);//occurs AFTER every action taken by any stack or by the hero
	virtual void actionStarted(const BattleAction *action);//occurs BEFORE every action taken by any stack or by the hero
	virtual void battleAttack(BattleAttack *ba); //called when stack is performing attack
	virtual void battleStackAttacked(BattleStackAttacked * bsa); //called when stack receives damage (after battleAttack())
	virtual void battleEnd(BattleResult *br);
	virtual void battleNewRound(int round); //called at the beggining of each turn, round=-1 is the tactic phase, round=0 is the first "normal" turn
	virtual void battleStackMoved(int ID, int dest);
	virtual void battleSpellCasted(SpellCasted *sc);
	virtual void battleStart(CCreatureSet *army1, CCreatureSet *army2, int3 tile, CGHeroInstance *hero1, CGHeroInstance *hero2, bool side); //called by engine when battle starts; side=0 - left, side=1 - right
	virtual void battlefieldPrepared(int battlefieldType, std::vector<CObstacle*> obstacles); //called when battlefield is prepared, prior the battle beginning
	//
	virtual void battleStackMoved(int ID, int dest, bool startMoving, bool endMoving);
	virtual void battleStackAttacking(int ID, int dest);
	virtual void battleStackIsAttacked(int ID, int dmg, int killed, int IDby, bool byShooting);
	virtual BattleAction activeStack(int stackID);
};

}

#endif/*__GENIUS_AI__*/