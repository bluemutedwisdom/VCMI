#ifndef MAPD_H
#define MAPD_H

#include <string>
#include <vector>
#include "global.h"
#include "CSemiDefHandler.h"
#include "CDefHandler.h"

enum ESortBy{name,playerAm,size,format, viccon,loscon};
struct Sresource
{
	std::string resName; //name of this resource
	int amount; //it can be greater and lesser than 0
};

struct TimeEvent
{
	std::string eventName;
	std::string message;
	std::vector<Sresource> decIncRes; //decreases / increases of resources
	unsigned int whichPlayers; //which players are affected by this event (+1 - first, +2 - second, +4 - third, +8 - fourth etc.)
	bool areHumansAffected;
	bool areCompsAffected;
	int firstAfterNDays; //how many days after appears this event
	int nextAfterNDays; //how many days after the epperance before appaers this event
//bajty wydarze� (59 + |teksty|)
//4 bajty na d�ugo�� nazwy zdarzenia
//nazwa zdarzenia (bajty dodatkowe)
//4 bajty na d�ugo�� wiadomo�ci
//wiadomo�� (bajty dodatkowe)
//4 bajty na zwi�kszenie si� ilosci drewna (zapis normalny) lub ff,ff,ff,ff - ilo�� drewna do odebrania (maksymalna ilo�� drewna, kt�r� mo�na da�/odebra� to 32767)
//4 bajty na zwi�kszenie si� ilosci rt�ci (zapis normalny) lub ff,ff,ff,ff - ilo�� rt�ci do odebrania (maksymalna ilo�� rt�ci, kt�r� mo�na da�/odebra� to 32767)
//4 bajty na zwi�kszenie si� ilosci rudy (zapis normalny) lub ff,ff,ff,ff - ilo�� rudy do odebrania (maksymalna ilo�� rudy, kt�r� mo�na da�/odebra� to 32767)
//4 bajty na zwi�kszenie si� ilosci siarki (zapis normalny) lub ff,ff,ff,ff - ilo�� siarki do odebrania (maksymalna ilo�� siarki, kt�r� mo�na da�/odebra� to 32767)
//4 bajty na zwi�kszenie si� ilosci kryszta�u (zapis normalny) lub ff,ff,ff,ff - ilo�� kryszta�u do odebrania (maksymalna ilo�� kryszta�u, kt�r� mo�na da�/odebra� to 32767)
//4 bajty na zwi�kszenie si� ilosci klejnot�w (zapis normalny) lub ff,ff,ff,ff - ilo�� klejnot�w do odebrania (maksymalna ilo�� klejnot�w, kt�r� mo�na da�/odebra� to 32767)
//4 bajty na zwi�kszenie si� ilosci z�ota (zapis normalny) lub ff,ff,ff,ff - ilo�� z�ota do odebrania (maksymalna ilo�� z�ota, kt�r� mo�na da�/odebra� to 32767)
//1 bajt - kt�rych graczy dotyczy zdarzenie (pole bitowe, +1 - pierwszy, +2 - drugi, +4 - trzeci, +8 - czwarty, +16 - pi�ty, +32 - sz�sty, +64 - si�dmy, +128 - �smy)
//1 bajt - czy zdarzenie odnosi si� do graczy - ludzi (00 - nie, 01 - tak)
//1 bajt - czy zdarzenie odnosi si� do graczy komputerowych (00 - nie, 01 - tak)
//2 bajty - op�nienie pierwszego wyst�pienia (w dniach, zapis normalny, maks 671)
//1 bajt - co ile dni wyst�puje zdarzenie (maks 28, 00 oznacza zdarzenie jednorazowe)
//17 bajt�w zerowych
};
struct TerrainTile
{
	EterrainType tertype; // type of terrain
	unsigned int terview; // look of terrain
	Eriver nuine; // type of Eriver (0 if there is no Eriver)
	unsigned int rivDir; // direction of Eriver
	Eroad malle; // type of Eroad (0 if there is no Eriver)
	unsigned int roadDir; // direction of Eroad
	unsigned int siodmyTajemniczyBajt; // mysterius byte // jak bedzie waidomo co to, to sie nazwie inaczej
};
struct DefInfo //information from def declaration
{
	std::string name; 
	int bytes [42];
	//CSemiDefHandler * handler;
	CDefHandler * handler;
	int printPriority;
	bool isOnDefList;
	bool isVisitable();
};
struct Location
{
	int x, y; 
	bool z; // underground
};
struct SheroName //name of starting hero
{
	int heroID;
	std::string heroName;
};
struct PlayerInfo
{
	bool canHumanPlay;
	bool canComputerPlay;
	unsigned int AITactic; //(00 - random, 01 -  warrior, 02 - builder, 03 - explorer)
	unsigned int allowedFactions; //(01 - castle; 02 - rampart; 04 - tower; 08 - inferno; 16 - necropolis; 32 - dungeon; 64 - stronghold; 128 - fortress; 256 - conflux);
	bool isFactionRandom; 
	unsigned int mainHeroPortrait; //it's ID of hero with choosen portrait; 255 if standard
	std::string mainHeroName;
	std::vector<SheroName> heroesNames;
	bool hasMainTown;
	bool generateHeroAtMainTown;
	Location posOfMainTown;
	int team;
};
struct LossCondition
{
	ElossCon typeOfLossCon;
	union
	{
		Location castlePos;
		Location heroPos;
		int timeLimit; // in days
	};
};
struct CspecificVictoryConidtions
{
	bool allowNormalVictory;
	bool appliesToAI;
};
struct VicCon0 : public CspecificVictoryConidtions //acquire artifact
{
	int ArtifactID;
};
struct VicCon1 : public CspecificVictoryConidtions //accumulate creatures
{
	int monsterID;
	int neededQuantity;
};
struct VicCon2 : public CspecificVictoryConidtions // accumulate resources
{
	int resourceID;
	int neededQuantity;
};
struct VicCon3 : public CspecificVictoryConidtions // upgrade specific town
{
	Location posOfCity;
	int councilNeededLevel; //0 - town; 1 - city; 2 - capitol
	int fortNeededLevel;// 0 - fort; 1 - citadel; 2 - castle
};
struct VicCon4 : public CspecificVictoryConidtions // build grail structure
{
	bool anyLocation;
	Location whereBuildGrail;
};
struct VicCon5 : public CspecificVictoryConidtions // defeat a specific hero
{
	Location locationOfHero;
};
struct VicCon6 : public CspecificVictoryConidtions // capture a specific town
{
	Location locationOfTown;
};
struct VicCon7 : public CspecificVictoryConidtions // defeat a specific monster
{
	Location locationOfMonster;
};
/*struct VicCon8 : public CspecificVictoryConidtions // flag all creature dwellings
{
};
struct VicCon9 : public CspecificVictoryConidtions // flag all mines
{
};*/
struct VicCona : public CspecificVictoryConidtions //transport specific artifact
{
	int artifactID;
	Location destinationPlace;
};
struct Rumor
{
	std::string name, text;
};


class CMapEvent
{
public:
	std::string name, message;
	int wood, mercury, ore, sulfur, crystal, gems, gold; //gained / taken resources
	unsigned char players; //affected players
	bool humanAffected;
	bool computerAffected;
	int firstOccurence;
	int nextOccurence; //after nextOccurance day event will occure; if it it 0, event occures only one time;
};

struct Mapa
{
	Eformat version; // version of map Eformat
	bool twoLevel; // if map has underground level
	int difficulty; // 0 easy - 4 impossible
	int levelLimit;
	bool areAnyPLayers; // if there are any playable players on map
	std::string name;  //name of map
	std::string description;  //and description
	int height, width; 
	TerrainTile** terrain; 
	TerrainTile** undergroungTerrain; // used only if there is underground level
	std::vector<Rumor> rumors;
	std::vector<DefInfo> defy; // list of .def files
	PlayerInfo players[8]; // info about players
	std::vector<int> teams;  // teams[i] = team of player no i 
	LossCondition lossCondition;
	EvictoryConditions victoryCondition; //victory conditions
	CspecificVictoryConidtions * vicConDetails; // used only if vistory conditions aren't standard
	int howManyTeams;
	std::vector<CMapEvent> events;
};
class CMapHeader
{
public:
	Eformat version; // version of map Eformat
	bool areAnyPLayers; // if there are any playable players on map
	int height, width; 
	bool twoLevel; // if map has underground level
	std::string name;  //name of map
	std::string description;  //and description
	int difficulty; // 0 easy - 4 impossible
	int levelLimit;
	LossCondition lossCondition;
	EvictoryConditions victoryCondition; //victory conditions
	CspecificVictoryConidtions * vicConDetails; // used only if vistory conditions aren't standard
	PlayerInfo players[8]; // info about players
	std::vector<int> teams;  // teams[i] = team of player no i 
	int howManyTeams;
	CMapHeader(unsigned char *map); //an argument is a reference to string described a map (unpacked)
};
class CMapInfo : public CMapHeader
{
public:
	std::string filename;
	int playerAmnt, humenPlayers;
	CMapInfo(std::string fname, unsigned char *map):CMapHeader(map),filename(fname)
	{
		playerAmnt=humenPlayers=0;
		for (int i=0;i<8;i++)
		{
			if (players[i].canHumanPlay) {playerAmnt++;humenPlayers++;}
			else if (players[i].canComputerPlay) {playerAmnt++;}
		}
	};
};


class mapSorter
{
public:
	ESortBy sortBy;
	bool operator()(CMapHeader & a, CMapHeader& b)
	{
		switch (sortBy)
		{
		case ESortBy::format:
			return (a.version<b.version);
			break;
		case ESortBy::loscon:
			return (a.lossCondition.typeOfLossCon<b.lossCondition.typeOfLossCon);
			break;
		case ESortBy::playerAm:
			//TODO
			break;
		case ESortBy::size:
			return (a.width<b.width);
			break;
		case ESortBy::viccon:
			return (a.victoryCondition<b.victoryCondition);
			break;
		case ESortBy::name:
			return (a.name<b.name);
			break;
		default:
			return (a.name<b.name);
			break;
		}
	};
	mapSorter(ESortBy es):sortBy(es){};
};
#endif //MAPD_H
