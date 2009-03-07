#ifndef __NETPACKS_H__
#define __NETPACKS_H__
#include "../global.h"
#include "BattleAction.h"
#include "HeroBonus.h"

struct IPack
{
	virtual ui16 getType()const = 0 ;
	//template<ui16 Type>
	//static bool isType(const IPack * ip)
	//{
	//	return Type == ip->getType();
	//}
	template<ui16 Type>
	static bool isType(IPack * ip)
	{
		return Type == ip->getType();
	}
	//template<ui16 Type>
	//static bool isType(const IPack & ip)
	//{
	//	return Type == ip.getType();
	//}
};
template <typename T> struct CPack
	:public IPack
{
	ui16 type; 
	ui16 getType() const{return type;}
	T* This(){return static_cast<T*>(this);};
};
template <typename T> struct Query
	:public CPack<T>
{
	ui32 id;
};

struct MetaString : public CPack<MetaString> //2001 helper for object scrips
{
	std::vector<std::string> strings;
	std::vector<std::pair<ui8,ui32> > texts; //pairs<text handler type, text number>; types: 1 - generaltexthandler->all; 2 - objh->xtrainfo; 3 - objh->names; 4 - objh->restypes; 5 - arth->artifacts[id].name; 6 - generaltexth->arraytxt; 7 - creh->creatures[os->subID].namePl; 8 - objh->creGens; 9 - objh->mines[ID].first; 10 - objh->mines[ID].second; 11 - objh->advobtxt
	std::vector<si32> message;
	std::vector<std::string> replacements;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & strings & texts & message & replacements;
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

struct SetResources : public CPack<SetResources> //104
{
	SetResources(){res.resize(RESOURCE_QUANTITY);type = 104;};
	ui8 player;
	std::vector<si32> res; //res[resid] => res amount

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & player & res;
	}
};
struct SetResource : public CPack<SetResource> //102
{
	SetResource(){type = 102;};

	ui8 player, resid;
	si32 val;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & player & resid & val;
	}
}; 
struct SetPrimSkill : public CPack<SetPrimSkill> //105
{
	SetPrimSkill(){type = 105;};
	ui8 abs; //0 - changes by value; 1 - sets to value
	si32 id;
	ui16 which, val;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & abs & id & which & val;
	}
}; 
struct SetSecSkill : public CPack<SetSecSkill> //106
{
	SetSecSkill(){type = 106;};
	ui8 abs; //0 - changes by value; 1 - sets to value
	si32 id;
	ui16 which, val;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & abs & id & which & val;
	}
}; 
struct HeroVisitCastle : public CPack<HeroVisitCastle> //108
{
	HeroVisitCastle(){flags=0;type = 108;};
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
struct ChangeSpells : public CPack<ChangeSpells> //109
{
	ChangeSpells(){type = 109;};
	ui8 learn; //1 - gives spell, 0 - takes
	ui32 hid;
	std::set<ui32> spells;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & learn & hid & spells;
	}
}; 

struct SetMana : public CPack<SetMana> //110
{
	SetMana(){type = 110;};
	ui32 hid, val;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & val & hid;
	}
}; 
struct SetMovePoints : public CPack<SetMovePoints> //111
{
	SetMovePoints(){type = 111;};
	ui32 hid, val;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & val & hid;
	}
}; 
struct FoWChange : public CPack<FoWChange> //112
{
	FoWChange(){type = 112;};
	std::set<int3> tiles;
	ui8 player, mode; //mode==0 - hide, mode==1 - reveal
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & tiles & player;
	}
}; 

struct SetAvailableHeroes : public CPack<SetAvailableHeroes> //113
{
	SetAvailableHeroes(){type = 113;flags=0;};
	ui8 player;
	si32 hid1, hid2;
	ui8 flags; //1 - reset army of hero1; 2 - reset army of hero 2
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & player & hid1 & hid2 & flags;
	}
};

struct GiveBonus :  public CPack<GiveBonus> //115
{
	GiveBonus(){type = 115;};

	ui32 hid;
	HeroBonus bonus;
	MetaString bdescr;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & bonus & hid & bdescr;
	}
};

struct ChangeObjPos : public CPack<ChangeObjPos> //116
{
	ChangeObjPos(){type = 116;};

	ui32 objid;
	int3 nPos;
	ui8 flags; //bit flags: 1 - redraw

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & objid & nPos & flags;
	}
};

struct RemoveObject : public CPack<RemoveObject> //500
{
	RemoveObject(){type = 500;};
	RemoveObject(si32 ID){id = ID;type = 500;};
	si32 id;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & id;
	}
}; 
struct TryMoveHero : public CPack<TryMoveHero> //501
{
	TryMoveHero(){type = 501;};

	ui32 id, movePoints;
	ui8 result; //0 - failed; 1- succes -normal move; 2 - teleportation, 3 - instant jump
	int3 start, end;
	std::set<int3> fowRevealed; //revealed tiles

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & id & result & start & end & movePoints & fowRevealed;
	}
}; 
struct SetGarrisons : public CPack<SetGarrisons> //502
{
	SetGarrisons(){type = 502;};
	std::map<ui32,CCreatureSet> garrs;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & garrs;
	}
}; 
struct NewStructures : public CPack<NewStructures> //504
{
	NewStructures(){type = 504;};
	si32 tid;
	std::set<si32> bid;
	si16 builded; 

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & tid & bid & builded;
	}
}; 
struct SetAvailableCreatures : public CPack<SetAvailableCreatures> //506
{
	SetAvailableCreatures(){type = 506;};
	si32 tid;
	std::map<si32,ui32> creatures;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & tid & creatures;
	}
};  
struct SetHeroesInTown : public CPack<SetHeroesInTown> //508
{
	SetHeroesInTown(){type = 508;};
	si32 tid, visiting, garrison; //id of town, visiting hero, hero in garrison

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & tid & visiting & garrison;
	}
};  
struct SetHeroArtifacts : public CPack<SetHeroArtifacts> //509
{
	SetHeroArtifacts(){type = 509;};
	si32 hid;
	std::vector<ui32> artifacts; //hero's artifacts from bag
	std::map<ui16,ui32> artifWorn; //map<position,artifact_id>; positions: 0 - head; 1 - shoulders; 2 - neck; 3 - right hand; 4 - left hand; 5 - torso; 6 - right ring; 7 - left ring; 8 - feet; 9 - misc1; 10 - misc2; 11 - misc3; 12 - misc4; 13 - mach1; 14 - mach2; 15 - mach3; 16 - mach4; 17 - spellbook; 18 - misc5

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & hid & artifacts & artifWorn;
	}
};  

struct SetSelection : public CPack<SetSelection> //514
{
	SetSelection(){type = 514;};
	ui8 player;
	ui32 id;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & id & player;
	}
};  

struct HeroRecruited : public CPack<HeroRecruited> //515
{
	HeroRecruited(){type = 515;};
	si32 hid, tid; //subID of hero
	int3 tile;
	ui8 player;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & hid & tid & tile & player;
	}
};  

struct GiveHero : public CPack<GiveHero> //516
{
	GiveHero(){type = 516;};
	ui32 id; //object id
	ui8 player;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & id & player;
	}
};  

struct NewTurn : public CPack<NewTurn> //101
{
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
//struct SetStrInfo : public CPack<SetStrInfo> //503
//{
//	SetStrInfo(){type = 503;};
//	SetAvailableCreatures sac;
//
//	template <typename Handler> void serialize(Handler &h, const int version)
//	{
//		h & sac;
//	}
//};  
struct Component : public CPack<Component> //2002 helper for object scrips informations
{
	ui16 id, subtype; //ids: 0 - primskill; 1 - secskill; 2 - resource; 3 - creature; 4 - artifact; 5 - experience (sub==0 exp points; sub==1 levels)
	si32 val; // + give; - take
	si16 when; // 0 - now; +x - within x days; -x - per x days

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & id & subtype & val & when;
	}
	Component(){type = 2002;};
	Component(ui16 Type, ui16 Subtype, si32 Val, si16 When):id(Type),subtype(Subtype),val(Val),when(When){type = 2002;};
};

struct InfoWindow : public CPack<InfoWindow> //103  - displays simple info window
{
	MetaString text;
	std::vector<Component> components;
	ui8 player;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & text & components & player;
	}
	InfoWindow(){type = 103;};
};

struct SetObjectProperty : public CPack<SetObjectProperty>//1001
{
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

struct SetHoverName : public CPack<SetHoverName>//1002
{
	ui32 id;
	MetaString name;
	SetHoverName(){type = 1002;};
	SetHoverName(ui32 ID, MetaString& Name):id(ID),name(Name){type = 1002;};
	
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & id & name;
	}
};
struct HeroLevelUp : public Query<HeroLevelUp>//2000
{
	si32 heroid;
	ui8 primskill, level;
	std::vector<ui16> skills;

	HeroLevelUp(){type = 2000;};
	
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & id & heroid & primskill & level & skills;
	}
};

struct SelectionDialog : public Query<SelectionDialog>//2001
{
	MetaString text;
	std::vector<Component> components;
	ui8 player;

	SelectionDialog(){type = 2001;};
	
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & id & text & components & player;
	}
};

struct YesNoDialog : public Query<YesNoDialog>//2002
{
	MetaString text;
	std::vector<Component> components;
	ui8 player;

	YesNoDialog(){type = 2002;};

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & id & text & components & player;
	}
};

struct BattleInfo;
struct BattleStart : public CPack<BattleStart>//3000
{
	BattleInfo * info;

	BattleStart(){type = 3000;};
	
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & info;
	}
};
struct BattleNextRound : public CPack<BattleNextRound>//3001
{
	si32 round;

	BattleNextRound(){type = 3001;};
	
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & round;
	}
};
struct BattleSetActiveStack : public CPack<BattleSetActiveStack>//3002
{
	ui32 stack;

	BattleSetActiveStack(){type = 3002;};
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & stack;
	}
};
struct BattleResult : public CPack<BattleResult>//3003
{
	ui8 result; //0 - normal victory; 1 - escape; 2 - surrender
	ui8 winner; //0 - attacker, 1 - defender, [2 - draw (should be possible?)]
	std::set<std::pair<ui32,si32> > casualties[2]; //first => casualties of attackers - set of pairs crid<>number
	ui32 exp[2]; //exp for attacker and defender
	std::set<ui32> artifacts; //artifacts taken from loser to winner



	BattleResult(){type = 3003;};
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & result & winner & casualties[0] & casualties[1] & exp & artifacts;
	}
};

struct BattleStackMoved : public CPack<BattleStackMoved>//3004
{
	ui32 stack, tile, distance;
	BattleStackMoved(){type = 3004;};
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & stack & tile & distance;
	}
};

struct BattleStackAttacked : public CPack<BattleStackAttacked>//3005
{
	ui32 stackAttacked;
	ui32 newAmount, newHP, killedAmount, damageAmount;
	ui8 flags; //1 - is stack killed; 2 - is there special effect to be shown; 4 - lucky hit
	ui32 effect; //set only if flag 2 is present


	BattleStackAttacked(){flags = 0; type = 3005;};
	bool killed() //if target stack was killed
	{
		return flags & 1;
	}
	bool isEffect() //if target stack was killed
	{
		return flags & 2;
	}
	bool lucky()
	{
		return flags & 4;
	}
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & stackAttacked & newAmount & newHP & flags & killedAmount & damageAmount & effect;
	}
};

struct BattleAttack : public CPack<BattleAttack>//3006
{
	BattleStackAttacked bsa;
	ui32 stackAttacking;
	ui8 flags;



	BattleAttack(){flags = 0; type = 3006;};
	bool shot()//distance attack - decrease number of shots
	{
		return flags & 1;
	}
	bool counter()//is it counterattack?
	{
		return flags & 2;
	}
	bool killed() //if target stack was killed
	{
		return bsa.killed();
	}
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & bsa & stackAttacking & flags;
	}
};

struct StartAction : public CPack<StartAction>//3007
{
	BattleAction ba;
	StartAction(){type = 3007;};
	StartAction(const BattleAction &act){ba = act; type = 3007;};
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & ba;
	}
};

struct SpellCasted : public CPack<SpellCasted>//3009
{
	ui8 side; //which hero casted spell: 0 - attacker, 1 - defender
	ui32 id;
	ui8 skill;
	ui16 tile; //destination tile (may not be set in some global/mass spells
	SpellCasted(){type = 3009;};
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & side & id & skill & tile;
	}
};

struct SetStackEffect : public CPack<SetStackEffect> //3010
{
	ui32 stack;
	CStack::StackEffect effect;
	SetStackEffect(){type = 3010;};
	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & stack & effect;
	}
};

struct ShowInInfobox : public CPack<ShowInInfobox> //107
{
	ShowInInfobox(){type = 107;};
	ui8 player;
	Component c;
	MetaString text;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & player & c & text;
	}
};
#endif //__NETPACKS_H__