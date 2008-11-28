#pragma once
#include "global.h"
#include "CPlayerInterface.h"
#include <list>

class CCreatureSet;
class CGHeroInstance;
class CDefHandler;
class CStack;
class CCallback;
class AdventureMapButton;
class CHighlightableButton;
class CHighlightableButtonsGroup;
struct BattleResult;
struct SpellCasted;
template <typename T> struct CondSh;

class CBattleInterface;

class CBattleHero : public IShowable, public ClickableL
{
public:
	bool flip; //false if it's attacking hero, true otherwise
	CDefHandler * dh, *flag; //animation and flag
	const CGHeroInstance * myHero; //this animation's hero instance
	const CBattleInterface * myOwner; //battle interface to which this animation is assigned
	int phase; //stage of animation
	int image; //frame of animation
	unsigned char flagAnim, flagAnimCount; //for flag animation
	void show(SDL_Surface * to); //prints next frame of animation to to
	void activate();
	void deactivate();
	void setPhase(int newPhase);
	void clickLeft(boost::logic::tribool down);
	CBattleHero(const std::string & defName, int phaseG, int imageG, bool filpG, unsigned char player, const CGHeroInstance * hero, const CBattleInterface * owner); //c-tor
	~CBattleHero(); //d-tor
};

class CBattleHex : public Hoverable, public MotionInterested, public ClickableL, public ClickableR
{
private:
	bool setAlterText; //if true, this hex has set alternative text in console and will clean it
public:
	unsigned int myNumber;
	bool accesible;
	//CStack * ourStack;
	bool hovered, strictHovered;
	CBattleInterface * myInterface; //interface that owns me
	static std::pair<int, int> getXYUnitAnim(const int & hexNum, const bool & attacker, const CCreature * creature); //returns (x, y) of left top corner of animation
	//for user interactions
	void hover (bool on);
	void activate();
	void deactivate();
	void mouseMoved (const SDL_MouseMotionEvent & sEvent);
	void clickLeft(boost::logic::tribool down);
	void clickRight(boost::logic::tribool down);
	CBattleHex();
};

class CBattleObstacle
{
	std::vector<int> lockedHexes;
};

class CBattleConsole : public IShowable, public CIntObject
{
private:
	std::vector< std::string > texts; //a place where texts are stored
	int lastShown; //last shown line of text
public:
	std::string alterTxt; //if it's not empty, this text is displayed
	CBattleConsole(); //c-tor
	~CBattleConsole(); //d-tor
	void show(SDL_Surface * to = 0);
	bool addText(const std::string & text); //adds text at the last position; returns false if failed (e.g. text longer than 70 characters)
	void eraseText(unsigned int pos); //erases added text at position pos
	void changeTextAt(const std::string & text, unsigned int pos); //if we have more than pos texts, pos-th is changed to given one
	void scrollUp(unsigned int by = 1); //scrolls console up by 'by' positions
	void scrollDown(unsigned int by = 1); //scrolls console up by 'by' positions
};

class CBattleReslutWindow : public IShowable, public CIntObject, public IActivable
{
private:
	SDL_Surface * background;
	AdventureMapButton * exit;
public:
	CBattleReslutWindow(const BattleResult & br, const SDL_Rect & pos, const CBattleInterface * owner); //c-tor
	~CBattleReslutWindow(); //d-tor

	void bExitf();

	void activate();
	void deactivate();
	void show(SDL_Surface * to = 0);
};

class CBattleOptionsWindow : public IShowable, public CIntObject, public IActivable
{
private:
	CBattleInterface * myInt;
	SDL_Surface * background;
	AdventureMapButton * setToDefault, * exit;
	CHighlightableButton * viewGrid, * movementShadow, * mouseShadow;
	CHighlightableButtonsGroup * animSpeeds;
public:
	CBattleOptionsWindow(const SDL_Rect & position, CBattleInterface * owner); //c-tor
	~CBattleOptionsWindow(); //d-tor

	void bDefaultf();
	void bExitf();

	void activate();
	void deactivate();
	void show(SDL_Surface * to = 0);
};

class CBattleInterface : public CMainInterface, public MotionInterested, public KeyInterested
{
private:
	SDL_Surface * background, * menu, * amountBasic, * amountNormal, * cellBorders, * backgroundWithHexes;
	AdventureMapButton * bOptions, * bSurrender, * bFlee, * bAutofight, * bSpell,
		* bWait, * bDefence, * bConsoleUp, * bConsoleDown;
	CBattleConsole * console;
	CBattleHero * attackingHero, * defendingHero;
	CCreatureSet * army1, * army2; //fighting armies
	CGHeroInstance * attackingHeroInstance, * defendingHeroInstance;
	std::map< int, CCreatureAnimation * > creAnims; //animations of creatures from fighting armies (order by BattleInfo's stacks' ID)
	std::map< int, CDefHandler * > idToProjectile; //projectiles of creaures (creatureID, defhandler)
	std::map< int, bool > creDir; // <creatureID, if false reverse creature's animation>
	unsigned char animCount;
	int activeStack; //number of active stack; -1 - no one
	std::vector<int> shadedHexes; //hexes available for active stack
	int previouslyHoveredHex; //number of hex that was hovered by the cursor a while ago
	int currentlyHoveredHex; //number of hex that is supposed to be hovered (for a while it may be inappropriately set, but will be renewed soon)
	int animSpeed; //speed of animation; 1 - slowest, 2 - medium, 4 - fastest
	float getAnimSpeedMultiplier() const; //returns multiplier for number of frames in a group

	bool spellDestSelectMode; //if true, player is choosing destination for his spell
	int spellSelMode; //0 - any location, 1 - any firendly creature, 2 - any hostile creature, 3 - any creature, 4 - obstacle
	BattleAction * spellToCast; //spell for which player is choosing destination

	class CAttHelper
	{
	public:
		int ID; //attacking stack
		int IDby; //attacked stack
		int dest; //atacked hex
		int frame, maxframe; //frame of animation, number of frames of animation
		int hitCount; //for delaying animation
		bool reversing;
		int posShiftDueToDist;
		bool shooting;
		int shootingGroup; //if shooting is true, print this animation group
	} * attackingInfo;
	void attackingShowHelper();
	void redrawBackgroundWithHexes(int activeStack);
	void printConsoleAttacked(int ID, int dmg, int killed, int IDby);

	struct SProjectileInfo
	{
		int x, y; //position on the screen
		int dx, dy; //change in position in one step
		int step, lastStep; //to know when finish showing this projectile
		int creID; //ID of creature that shot this projectile
		int frameNum; //frame to display form projectile animation
		bool spin; //if true, frameNum will be increased
		int animStartDelay; //how many times projectile must be attempted to be shown till it's really show (decremented after hit)
	};
	std::list<SProjectileInfo> projectiles;
	void projectileShowHelper(SDL_Surface * to=NULL); //prints projectiles present on the battlefield
	void giveCommand(ui8 action, ui16 tile, ui32 stack, si32 additional=-1);
	bool isTileAttackable(const int & number) const; //returns true if tile 'number' is neighbouring any tile from active stack's range or is one of these tiles

	struct SBattleEffect
	{
		int x, y; //position on the screen
		int frame, maxFrame;
		CDefHandler * anim; //animation to display
	};
	std::list<SBattleEffect> battleEffects; //different animations to display on the screen like spell effects
public:
	CBattleInterface(CCreatureSet * army1, CCreatureSet * army2, CGHeroInstance *hero1, CGHeroInstance *hero2); //c-tor
	~CBattleInterface(); //d-tor

	//std::vector<TimeInterested*> timeinterested; //animation handling
	bool printCellBorders; //if true, cell borders will be printed
	void setPrintCellBorders(bool set); //set for above member
	bool printStackRange; //if true,range of active stack will be printed
	void setPrintStackRange(bool set); //set for above member
	bool printMouseShadow; //if true, hex under mouse will be shaded
	void setPrintMouseShadow(bool set); //set for above member
	void setAnimSpeed(int set); //set for animSpeed
	int getAnimSpeed() const; //get for animSpeed

	CBattleHex bfield[BFIELD_SIZE]; //11 lines, 17 hexes on each
	std::vector< CBattleObstacle * > obstacles; //vector of obstacles on the battlefield
	SDL_Surface * cellBorder, * cellShade;
	CondSh<BattleAction *> *givenCommand; //data != NULL if we have i.e. moved current unit
	bool myTurn; //if true, interface is active (commands can be ordered
	CBattleReslutWindow * resWindow; //window of end of battle
	bool showStackQueue; //if true, queue of stacks will be shown

	//button handle funcs:
	void bOptionsf();
	void bSurrenderf();
	void bFleef();
	void reallyFlee(); //performs fleeing without asking player
	void bAutofightf();
	void bSpellf();
	void bWaitf();
	void bDefencef();
	void bConsoleUpf();
	void bConsoleDownf();
	//end of button handle funcs
	//napisz tu klase odpowiadajaca za wyswietlanie bitwy i obsluge uzytkownika, polecenia ma przekazywac callbackiem
	void activate();
	void deactivate();
	void show(SDL_Surface * to = NULL);
	void keyPressed(const SDL_KeyboardEvent & key);
	void mouseMoved(const SDL_MouseMotionEvent &sEvent);
	bool reverseCreature(int number, int hex, bool wideTrick = false); //reverses animation of given creature playing animation of reversing

	struct SStackAttackedInfo
	{
		int ID; //id of attacked stack
		int dmg; //damage dealt
		int amountKilled; //how many creatures in stack has been killed
		int IDby; //ID of attacking stack
		bool byShooting; //if true, stack has been attacked by shooting
		bool killed; //if true, stack has been killed
	};

	//call-ins
	void newStack(CStack stack); //new stack appeared on battlefield
	void stackRemoved(CStack stack); //stack disappeared from batlefiled
	//void stackKilled(int ID, int dmg, int killed, int IDby, bool byShooting); //stack has been killed (but corpses remain)
	void stackActivated(int number); //active stack has been changed
	void stackMoved(int number, int destHex, bool endMoving); //stack with id number moved to destHex
	void stacksAreAttacked(std::vector<SStackAttackedInfo> attackedInfos); //called when a certain amount of stacks has been attacked
	void stackAttacking(int ID, int dest); //called when stack with id ID is attacking something on hex dest
	void newRound(int number); //caled when round is ended; number is the number of round
	void hexLclicked(int whichOne); //hex only call-in
	void stackIsShooting(int ID, int dest); //called when stack with id ID is shooting to hex dest
	void battleFinished(const BattleResult& br); //called when battle is finished - battleresult window should be printed
	void spellCasted(SpellCasted * sc); //called when a hero casts a spell
	void castThisSpell(int spellID); //called when player has chosen a spell from spellbook
	void displayEffect(ui32 effect, int destTile); //displays effect of a spell on the battlefield; affected: true - attacker. false - defender

	friend class CBattleHex;
	friend class CBattleReslutWindow;
	friend class CPlayerInterface;
};
