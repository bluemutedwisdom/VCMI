#ifndef __NETPACKS_H__
#define __NETPACKS_H__
#include "../global.h"
#include "../CGameState.h"
#include "BattleAction.h"
#include "HeroBonus.h"
#include <set>

class CClient;
class CGameState;
class CGameHandler;
class CConnection;

struct CPack
{
	ui16 type;

	CPack(){};
	virtual ~CPack(){};
	ui16 getType() const{return type;}
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		tlog1 << "CPack serialized... this should not happen!\n";
	}
	DLL_EXPORT void applyGs(CGameState *gs)
	{};
};

struct CPackForClient : public CPack
{
	CPackForClient(){type = 1;};

	CGameState* GS(CClient *cl);
	void applyFirstCl(CClient *cl)//called before applying to gs
	{};
	void applyCl(CClient *cl)//called after applying to gs
	{}; 
};

struct CPackForServer : public CPack
{
	CConnection *c;
	CGameState* GS(CGameHandler *gh);
	CPackForServer()
	{
		type = 2; 
		c = NULL;
	};

	void applyGh(CGameHandler *gh)//called after applying to gs
	{}; 
};


struct Query : public CPackForClient
{
	ui32 id;
};

struct MetaString : public CPack //2001 helper for object scrips
{
	enum {GENERAL_TXT=1, XTRAINFO_TXT, OBJ_NAMES, RES_NAMES, ART_NAMES, ARRAY_TXT, CRE_PL_NAMES, CREGENS, MINE_NAMES, 
		MINE_EVNTS, ADVOB_TXT, ART_EVNTS, SPELL_NAME};
	std::vector<std::string> strings;
	std::vector<std::pair<ui8,ui32> > texts; //pairs<text handler type, text number>; types: 1 - generaltexthandler->all; 2 - objh->xtrainfo; 3 - objh->names; 4 - objh->restypes; 5 - arth->artifacts[id].name; 6 - generaltexth->arraytxt; 7 - creh->creatures[os->subID].namePl; 8 - objh->creGens; 9 - objh->mines[ID].first; 10 - objh->mines[ID].second; 11 - objh->advobtxt
	std::vector<si32> message;
	std::vector<std::string> replacements;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & strings & texts & message & replacements;
	}
	void addTxt(ui8 type, ui32 serial)
	{
		*this << std::make_pair(type,serial);
	}
	MetaString& operator<<(const std::pair<ui8,ui32> &txt)
	{
		message.push_back(-((si32)texts.size())-1);
		texts.push_back(txt);
		return *this;
	}
	MetaString& operator<<(const std::string &txt)
	{
		message.push_back(strings.size()+1);
		strings.push_back(txt);
		return *this;
	}
	void clear()
	{
		strings.clear();
		texts.clear();
		message.clear();
	}

	MetaString(){type = 2001;};
}; 

/***********************************************************************************************************/

struct SystemMessage : public CPackForClient //95
{
	SystemMessage(const std::string Text) : text(Text){type = 95;};
	SystemMessage(){type = 95;};
	void applyCl(CClient *cl);

	std::string text;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & text;
	}
};

struct YourTurn : public CPackForClient //100
{
	YourTurn(){type = 100;};
	void applyCl(CClient *cl);
	DLL_EXPORT void applyGs(CGameState *gs);

	ui8 player;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & player;
	}
};

struct SetResource : public CPackForClient //102
{
	SetResource(){type = 102;};
	void applyCl(CClient *cl);
	DLL_EXPORT void applyGs(CGameState *gs);

	ui8 player, resid;
	si32 val;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & player & resid & val;
	}
}; 
struct SetResources : public CPackForClient //104
{
	SetResources(){res.resize(RESOURCE_QUANTITY);type = 104;};
	void applyCl(CClient *cl);
	DLL_EXPORT void applyGs(CGameState *gs);

	ui8 player;
	std::vector<si32> res; //res[resid] => res amount

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & player & res;
	}
};

struct SetPrimSkill : public CPackForClient //105
{
	SetPrimSkill(){type = 105;};
	void applyCl(CClient *cl);
	DLL_EXPORT void applyGs(CGameState *gs);

	ui8 abs; //0 - changes by value; 1 - sets to value
	si32 id;
	ui16 which, val;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & abs & id & which & val;
	}
}; 
struct SetSecSkill : public CPackForClient //106
{
	SetSecSkill(){type = 106;};
	void applyCl(CClient *cl);
	DLL_EXPORT void applyGs(CGameState *gs);

	ui8 abs; //0 - changes by value; 1 - sets to value
	si32 id;
	ui16 which, val;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & abs & id & which & val;
	}
}; 
struct HeroVisitCastle : public CPackForClient //108
{
	HeroVisitCastle(){flags=0;type = 108;};
	void applyCl(CClient *cl);
	DLL_EXPORT void applyGs(CGameState *gs);

	ui8 flags; //1 - start, 2 - garrison
	ui32 tid, hid;

	bool start() //if hero is entering castle (if false - leaving)
	{
		return flags & 1;
	}
	bool garrison() //if hero is entering/leaving garrison (if false - it's only visiting hero)
	{
		return flags & 2;
	}
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & flags & tid & hid;
	}
}; 
struct ChangeSpells : public CPackForClient //109
{
	ChangeSpells(){type = 109;};
	void applyCl(CClient *cl);
	DLL_EXPORT void applyGs(CGameState *gs);

	ui8 learn; //1 - gives spell, 0 - takes
	ui32 hid;
	std::set<ui32> spells;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & learn & hid & spells;
	}
}; 

struct SetMana : public CPackForClient //110
{
	SetMana(){type = 110;};
	void applyCl(CClient *cl);
	DLL_EXPORT void applyGs(CGameState *gs);


	ui32 hid, val;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & val & hid;
	}
}; 
struct SetMovePoints : public CPackForClient //111
{
	SetMovePoints(){type = 111;};
	void applyCl(CClient *cl);
	DLL_EXPORT void applyGs(CGameState *gs);

	ui32 hid, val;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & val & hid;
	}
}; 
struct FoWChange : public CPackForClient //112
{
	FoWChange(){type = 112;};
	void applyCl(CClient *cl);
	DLL_EXPORT void applyGs(CGameState *gs);

	std::set<int3> tiles;
	ui8 player, mode; //mode==0 - hide, mode==1 - reveal
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & tiles & player & mode;
	}
}; 

struct SetAvailableHeroes : public CPackForClient //113
{
	SetAvailableHeroes(){type = 113;flags=0;};
	void applyCl(CClient *cl);
	DLL_EXPORT void applyGs(CGameState *gs);

	ui8 player;
	si32 hid1, hid2;
	ui8 flags; //1 - reset army of hero1; 2 - reset army of hero 2
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & player & hid1 & hid2 & flags;
	}
};

struct GiveBonus :  public CPackForClient //115
{
	GiveBonus(){type = 115;};
	void applyCl(CClient *cl);
	DLL_EXPORT void applyGs(CGameState *gs);

	ui32 hid;
	HeroBonus bonus;
	MetaString bdescr;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & bonus & hid & bdescr;
	}
};

struct ChangeObjPos : public CPackForClient //116
{
	ChangeObjPos(){type = 116;};
	void applyFirstCl(CClient *cl);
	void applyCl(CClient *cl);
	DLL_EXPORT void applyGs(CGameState *gs);

	ui32 objid;
	int3 nPos;
	ui8 flags; //bit flags: 1 - redraw

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & objid & nPos & flags;
	}
};

struct RemoveObject : public CPackForClient //500
{
	RemoveObject(){type = 500;};
	RemoveObject(si32 ID){id = ID;type = 500;};
	void applyFirstCl(CClient *cl);
	void applyCl(CClient *cl);
	DLL_EXPORT void applyGs(CGameState *gs);

	si32 id;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & id;
	}
}; 
struct TryMoveHero : public CPackForClient //501
{
	TryMoveHero(){type = 501;};
	void applyFirstCl(CClient *cl);
	void applyCl(CClient *cl);
	void applyGs(CGameState *gs);

	ui32 id, movePoints;
	ui8 result; //0 - failed; 1- succes -normal move; 2 - teleportation, 3 - instant jump
	int3 start, end;
	std::set<int3> fowRevealed; //revealed tiles

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & id & result & start & end & movePoints & fowRevealed;
	}
}; 
struct SetGarrisons : public CPackForClient //502
{
	SetGarrisons(){type = 502;};
	void applyCl(CClient *cl);
	DLL_EXPORT void applyGs(CGameState *gs);

	std::map<ui32,CCreatureSet> garrs;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & garrs;
	}
}; 
struct NewStructures : public CPackForClient //504
{
	NewStructures(){type = 504;};
	void applyCl(CClient *cl);
	DLL_EXPORT void applyGs(CGameState *gs);

	si32 tid;
	std::set<si32> bid;
	si16 builded; 

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & tid & bid & builded;
	}
}; 
struct SetAvailableCreatures : public CPackForClient //506
{
	SetAvailableCreatures(){type = 506;};
	void applyCl(CClient *cl);
	DLL_EXPORT void applyGs(CGameState *gs);

	si32 tid;
	std::map<si32,ui32> creatures;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & tid & creatures;
	}
};  
struct SetHeroesInTown : public CPackForClient //508
{
	SetHeroesInTown(){type = 508;};
	void applyCl(CClient *cl);
	DLL_EXPORT void applyGs(CGameState *gs);

	si32 tid, visiting, garrison; //id of town, visiting hero, hero in garrison

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & tid & visiting & garrison;
	}
};  
struct SetHeroArtifacts : public CPackForClient //509
{
	SetHeroArtifacts(){type = 509;};
	void applyCl(CClient *cl);
	DLL_EXPORT void applyGs(CGameState *gs);
	DLL_EXPORT void setArtAtPos(ui16 pos, int art);

	si32 hid;
	std::vector<ui32> artifacts; //hero's artifacts from bag
	std::map<ui16,ui32> artifWorn; //map<position,artifact_id>; positions: 0 - head; 1 - shoulders; 2 - neck; 3 - right hand; 4 - left hand; 5 - torso; 6 - right ring; 7 - left ring; 8 - feet; 9 - misc1; 10 - misc2; 11 - misc3; 12 - misc4; 13 - mach1; 14 - mach2; 15 - mach3; 16 - mach4; 17 - spellbook; 18 - misc5

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & hid & artifacts & artifWorn;
	}

	std::vector<HeroBonus*> gained, lost; //used locally as hlp when applying
};   

struct HeroRecruited : public CPackForClient //515
{
	HeroRecruited(){type = 515;};
	void applyCl(CClient *cl);
	DLL_EXPORT void applyGs(CGameState *gs);

	si32 hid, tid; //subID of hero
	int3 tile;
	ui8 player;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & hid & tid & tile & player;
	}
};  

struct GiveHero : public CPackForClient //516
{
	GiveHero(){type = 516;};
	void applyFirstCl(CClient *cl);
	void applyCl(CClient *cl);
	DLL_EXPORT void applyGs(CGameState *gs);

	ui32 id; //object id
	ui8 player;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & id & player;
	}
};  

struct NewTurn : public CPackForClient //101
{
	DLL_EXPORT void applyGs(CGameState *gs);

	struct Hero
	{
		ui32 id, move, mana; //id is a general serial id
		template <typename Handler> void serialize(Handler &h, const int version)
		{
			h & id & move & mana;
		}
		bool operator<(const Hero&h)const{return id < h.id;}
	};

	std::set<Hero> heroes; //updates movement and mana points
	std::vector<SetResources> res;//resource list
	std::vector<SetAvailableCreatures> cres;//resource list
	ui32 day;
	bool resetBuilded;

	NewTurn(){type = 101;};

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & heroes & cres & res & day & resetBuilded;
	}
}; 

struct Component : public CPack //2002 helper for object scrips informations
{
	enum {PRIM_SKILL,SEC_SKILL,RESOURCE,CREATURE,ARTIFACT,EXPERIENCE,SPELL};
	ui16 id, subtype; //id uses ^^^ enums, when id==EXPPERIENCE subtype==0 means exp points and subtype==1 levels)
	si32 val; // + give; - take
	si16 when; // 0 - now; +x - within x days; -x - per x days

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & id & subtype & val & when;
	}
	Component(){type = 2002;};
	Component(ui16 Type, ui16 Subtype, si32 Val, si16 When):id(Type),subtype(Subtype),val(Val),when(When){type = 2002;};
};

struct InfoWindow : public CPackForClient //103  - displays simple info window
{
	void applyCl(CClient *cl);

	MetaString text;
	std::vector<Component> components;
	ui8 player;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & text & components & player;
	}
	InfoWindow(){type = 103;};
};

struct SetObjectProperty : public CPackForClient//1001
{
	DLL_EXPORT void applyGs(CGameState *gs);

	ui32 id;
	ui8 what; //1 - owner; 2 - blockvis; 3 - first stack count; 4 - visitors; 5 - visited; 6 - ID (if 34 then also def is replaced)
	ui32 val;
	SetObjectProperty(){type = 1001;};
	SetObjectProperty(ui32 ID, ui8 What, ui32 Val):id(ID),what(What),val(Val){type = 1001;};
	
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & id & what & val;
	}
};

struct SetHoverName : public CPackForClient//1002
{
	DLL_EXPORT void applyGs(CGameState *gs);

	ui32 id;
	MetaString name;
	SetHoverName(){type = 1002;};
	SetHoverName(ui32 ID, MetaString& Name):id(ID),name(Name){type = 1002;};
	
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & id & name;
	}
};
struct HeroLevelUp : public Query//2000
{
	void applyCl(CClient *cl);
	DLL_EXPORT void applyGs(CGameState *gs);

	si32 heroid;
	ui8 primskill, level;
	std::vector<ui16> skills;

	HeroLevelUp(){type = 2000;};
	
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & id & heroid & primskill & level & skills;
	}
};

//A dialog that requires making decision by player - it may contain components to choose between or has yes/no options
//Client responds with QueryReply, where answer: 0 - cancel pressed, choice doesn't matter; 1/2/...  - first/second/... component selected and OK pressed
//Until sending reply player won't be allowed to take any actions
struct BlockingDialog : public Query//2003
{
	enum {ALLOW_CANCEL = 1, SELECTION = 2};

	void applyCl(CClient *cl);

	MetaString text;
	std::vector<Component> components;
	ui8 player;
	ui8 flags;

	bool cancel() const
	{
		return flags & ALLOW_CANCEL;
	}
	bool selection() const
	{
		return flags & SELECTION;
	}

	BlockingDialog(bool yesno, bool Selection)
	{
		type = 2003; 
		if(yesno) flags |= ALLOW_CANCEL;
		if(Selection) flags |= SELECTION;
	}
	BlockingDialog()
	{
		type = 2003;
	};

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & id & text & components & player & flags;
	}
};

struct BattleInfo;
struct BattleStart : public CPackForClient//3000
{
	BattleStart(){type = 3000;};
	void applyCl(CClient *cl);
	DLL_EXPORT void applyGs(CGameState *gs);

	BattleInfo * info;

	
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & info;
	}
};
struct BattleNextRound : public CPackForClient//3001
{	
	BattleNextRound(){type = 3001;};
	void applyCl(CClient *cl);
	DLL_EXPORT void applyGs(CGameState *gs);

	si32 round;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & round;
	}
};
struct BattleSetActiveStack : public CPackForClient//3002
{
	BattleSetActiveStack(){type = 3002;};
	void applyCl(CClient *cl);
	DLL_EXPORT void applyGs(CGameState *gs);

	ui32 stack;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & stack;
	}
};
struct BattleResult : public CPackForClient//3003
{
	BattleResult(){type = 3003;};
	void applyFirstCl(CClient *cl);
	void applyGs(CGameState *gs);

	ui8 result; //0 - normal victory; 1 - escape; 2 - surrender
	ui8 winner; //0 - attacker, 1 - defender, [2 - draw (should be possible?)]
	std::set<std::pair<ui32,si32> > casualties[2]; //first => casualties of attackers - set of pairs crid<>number
	ui32 exp[2]; //exp for attacker and defender
	std::set<ui32> artifacts; //artifacts taken from loser to winner



	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & result & winner & casualties[0] & casualties[1] & exp & artifacts;
	}
};

struct BattleStackMoved : public CPackForClient//3004
{
	ui32 stack, tile;
	ui8 ending, distance;
	BattleStackMoved(){type = 3004;};
	void applyFirstCl(CClient *cl);
	void applyGs(CGameState *gs);
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & stack & tile & ending & distance;
	}
};

struct BattleStackAttacked : public CPackForClient//3005
{
	BattleStackAttacked(){flags = 0; type = 3005;};
	void applyCl(CClient *cl);
	DLL_EXPORT void applyGs(CGameState *gs);

	ui32 stackAttacked;
	ui32 newAmount, newHP, killedAmount, damageAmount;
	ui8 flags; //1 - is stack killed; 2 - is there special effect to be shown;
	ui32 effect; //set only if flag 2 is present

	bool killed() const//if target stack was killed
	{
		return flags & 1;
	}
	bool isEffect() const//if target stack was killed
	{
		return flags & 2;
	}
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & stackAttacked & newAmount & newHP & flags & killedAmount & damageAmount & effect;
	}
	bool operator<(const BattleStackAttacked &b) const
	{
		return stackAttacked < b.stackAttacked;
	}
};

struct BattleAttack : public CPackForClient//3006
{
	BattleAttack(){flags = 0; type = 3006;};
	void applyFirstCl(CClient *cl);
	DLL_EXPORT void applyGs(CGameState *gs);
	void applyCl(CClient *cl);

	std::set<BattleStackAttacked> bsa;
	ui32 stackAttacking;
	ui8 flags;

	bool shot()//distance attack - decrease number of shots
	{
		return flags & 1;
	}
	bool counter()//is it counterattack?
	{
		return flags & 2;
	}
	bool lucky()
	{
		return flags & 4;
	}
	bool unlucky()
	{
		//TODO: support?
		return flags & 8;
	}
	//bool killed() //if target stack was killed
	//{
	//	return bsa.killed();
	//}
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & bsa & stackAttacking & flags;
	}
};

struct StartAction : public CPackForClient//3007
{
	StartAction(){type = 3007;};
	StartAction(const BattleAction &act){ba = act; type = 3007;};
	void applyFirstCl(CClient *cl);
	DLL_EXPORT void applyGs(CGameState *gs);

	BattleAction ba;
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & ba;
	}
};

struct EndAction : public CPackForClient//3008
{
	EndAction(){type = 3008;};
	void applyCl(CClient *cl);

	template <typename Handler> void serialize(Handler &h, const int version)
	{
	}
};

struct SpellCasted : public CPackForClient//3009
{
	SpellCasted(){type = 3009;};
	DLL_EXPORT void applyGs(CGameState *gs);
	void applyCl(CClient *cl);

	ui8 side; //which hero casted spell: 0 - attacker, 1 - defender
	ui32 id;
	ui8 skill;
	ui16 tile; //destination tile (may not be set in some global/mass spells
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & side & id & skill & tile;
	}
};

struct SetStackEffect : public CPackForClient //3010
{
	SetStackEffect(){type = 3010;};
	DLL_EXPORT void applyGs(CGameState *gs);
	void applyCl(CClient *cl);

	std::set<ui32> stacks;
	CStack::StackEffect effect;
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & stacks & effect;
	}
};

struct ShowInInfobox : public CPackForClient //107
{
	ShowInInfobox(){type = 107;};
	ui8 player;
	Component c;
	MetaString text;

	void applyCl(CClient *cl);
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & player & c & text;
	}
};

/***********************************************************************************************************/

struct CloseServer : public CPackForServer
{
	void applyGh(CGameHandler *gh);
	template <typename Handler> void serialize(Handler &h, const int version)
	{}
};

struct EndTurn : public CPackForServer
{
	void applyGh(CGameHandler *gh);
	template <typename Handler> void serialize(Handler &h, const int version)
	{}
};

struct DismissHero : public CPackForServer
{
	DismissHero(){};
	DismissHero(si32 HID) : hid(HID) {};
	si32 hid;

	void applyGh(CGameHandler *gh);
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & hid;
	}
};

struct MoveHero : public CPackForServer
{
	MoveHero(){};
	MoveHero(const int3 &Dest, si32 HID) : dest(Dest), hid(HID){};
	int3 dest;
	si32 hid;

	void applyGh(CGameHandler *gh);
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & dest & hid;
	}
};

struct ArrangeStacks : public CPackForServer
{
	ArrangeStacks(){};
	ArrangeStacks(ui8 W, ui8 P1, ui8 P2, si32 ID1, si32 ID2, si32 VAL)
		:what(W),p1(P1),p2(P2),id1(ID1),id2(ID2),val(VAL) {};

	ui8 what; //1 - swap; 2 - merge; 3 - split
	ui8 p1, p2; //positions of first and second stack
	si32 id1, id2; //ids of objects with garrison
	si32 val;
	void applyGh(CGameHandler *gh);
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & what & p1 & p2 & id1 & id2 & val;
	}
};

struct DisbandCreature : public CPackForServer
{
	DisbandCreature(){};
	DisbandCreature(ui8 Pos, si32 ID):pos(Pos),id(ID){};
	ui8 pos; //stack pos
	si32 id; //object id

	void applyGh(CGameHandler *gh);
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & pos & id;
	}
};

struct BuildStructure : public CPackForServer
{
	BuildStructure(){};
	BuildStructure(si32 TID, si32 BID):tid(TID),bid(BID){};
	si32 bid, tid; //structure and town ids

	void applyGh(CGameHandler *gh);
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & tid & bid;
	}
};

struct RecruitCreatures : public CPackForServer
{
	RecruitCreatures(){};
	RecruitCreatures(si32 TID, si32 CRID, si32 Amount):tid(TID),crid(CRID),amount(Amount){};
	si32 tid; //town id
	ui32 crid, amount;//creature ID and amount

	void applyGh(CGameHandler *gh);
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & tid & crid & amount;
	}
};

struct UpgradeCreature : public CPackForServer
{
	UpgradeCreature(){};
	UpgradeCreature(ui8 Pos, si32 ID, si32 CRID):pos(Pos),id(ID), cid(CRID){};
	ui8 pos; //stack pos
	si32 id; //object id
	si32 cid; //id of type to which we want make upgrade

	void applyGh(CGameHandler *gh);
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & pos & id & cid;
	}
};

struct GarrisonHeroSwap : public CPackForServer
{
	GarrisonHeroSwap(){};
	GarrisonHeroSwap(si32 TID):tid(TID){};
	si32 tid; 

	void applyGh(CGameHandler *gh);
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & tid;
	}
};

struct ExchangeArtifacts : public CPackForServer
{
	ExchangeArtifacts(){};
	ExchangeArtifacts(si32 H1, si32 H2, ui16 S1, ui16 S2)
		:hid1(H1),hid2(H2),slot1(S1),slot2(S2){};
	si32 hid1, hid2;
	ui16 slot1, slot2;

	void applyGh(CGameHandler *gh);
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & hid1 & hid2 & slot1 & slot2;
	}
};

struct BuyArtifact : public CPackForServer
{
	BuyArtifact(){};
	BuyArtifact(si32 HID, si32 AID):hid(HID),aid(AID){};
	si32 hid, aid; //hero and artifact id

	void applyGh(CGameHandler *gh);
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & hid & aid;
	}
};

struct TradeOnMarketplace : public CPackForServer
{
	TradeOnMarketplace(){};
	TradeOnMarketplace(ui8 Player, ui8 Mode, /*si32 ID, */ui32 R1, ui32 R2, ui32 Val)
		:player(Player),mode(Mode),/*id(ID),*/r1(R1),r2(R2),val(Val){};
	ui8 player;
	ui8 mode;//0 - res<->res; 
	//si32 id; //object id
	ui32 r1, r2; //mode 0: r1 - sold resource, r2 - bought res
	ui32 val; //units of sold resource

	void applyGh(CGameHandler *gh);
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & player & mode & /*id & */r1 & r2 & val;
	}
};

struct SetFormation : public CPackForServer
{
	SetFormation(){};
	SetFormation(si32 HID, ui8 Formation):hid(HID),formation(Formation){};
	si32 hid;
	ui8 formation;

	void applyGh(CGameHandler *gh);
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & hid & formation;
	}
};

struct HireHero : public CPackForServer
{
	HireHero(){};
	HireHero(si32 HID, si32 TID):hid(HID),tid(TID){};
	si32 hid, tid; //available hero serial and town id

	void applyGh(CGameHandler *gh);
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & hid & tid;
	}
};

struct QueryReply : public CPackForServer
{
	QueryReply(){};
	QueryReply(ui32 QID, ui32 Answer):qid(QID),answer(Answer){};
	ui32 qid, answer; //hero and artifact id

	void applyGh(CGameHandler *gh);
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & qid & answer;
	}
};

struct MakeAction : public CPackForServer
{
	MakeAction(){};
	MakeAction(const BattleAction &BA):ba(BA){};
	BattleAction ba;

	void applyGh(CGameHandler *gh);
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & ba;
	}
};

struct MakeCustomAction : public CPackForServer
{
	MakeCustomAction(){};
	MakeCustomAction(const BattleAction &BA):ba(BA){};
	BattleAction ba;

	void applyGh(CGameHandler *gh);
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & ba;
	}
};

/***********************************************************************************************************/

struct SaveGame : public CPackForClient, public CPackForServer
{
	SaveGame(){};
	SaveGame(const std::string &Fname) :fname(Fname){};
	std::string fname;

	void applyCl(CClient *cl);
	void applyGs(CGameState *gs){};
	void applyGh(CGameHandler *gh);
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & fname;
	}
};

struct PlayerMessage : public CPackForClient, public CPackForServer //513
{
	PlayerMessage(){CPackForClient::type = 513;};
	PlayerMessage(ui8 Player, const std::string &Text)
		:player(Player),text(Text)
	{CPackForClient::type = 513;};
	void applyCl(CClient *cl);
	void applyGs(CGameState *gs){};
	void applyGh(CGameHandler *gh);

	ui8 player;
	std::string text;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & text & player;
	}
}; 


struct SetSelection : public CPackForClient, public CPackForServer //514
{
	SetSelection(){CPackForClient::type = 514;};
	DLL_EXPORT void applyGs(CGameState *gs);
	void applyGh(CGameHandler *gh);

	ui8 player;
	ui32 id;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & id & player;
	}
};  

#endif //__NETPACKS_H__