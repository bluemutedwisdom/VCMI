#ifndef __CSPELLWINDOW_H__
#define __CSPELLWINDOW_H__


#include "../global.h"
#include "GUIBase.h"
#include "boost/function.hpp"

/*
 * CSpellWindow.h, part of VCMI engine
 *
 * Authors: listed in file AUTHORS in main folder
 *
 * License: GNU General Public License v2.0 or later
 * Full text of license available in license.txt file, in main folder
 *
 */

struct SDL_Surface;
class CDefHandler;
struct SDL_Rect;
class CGHeroInstance;
class CStatusBar;

class SpellbookInteractiveArea : public CIntObject
{
private:
	boost::function<void()> onLeft;
	std::string textOnRclick;
	boost::function<void()> onHoverOn;
	boost::function<void()> onHoverOff;
	CPlayerInterface * myInt;
public:
	void clickLeft(tribool down, bool previousState);
	void clickRight(tribool down, bool previousState);
	void hover(bool on);
	void activate();
	void deactivate();

	SpellbookInteractiveArea(const SDL_Rect & myRect, boost::function<void()> funcL, const std::string & textR,
		boost::function<void()> funcHon, boost::function<void()> funcHoff, CPlayerInterface * _myInt);//c-tor
};

class CSpellWindow : public CIntObject
{
private:
	class SpellArea : public CIntObject
	{
	public:
		int mySpell;
		CSpellWindow * owner;

		SpellArea(SDL_Rect pos, CSpellWindow * owner);
		void clickLeft(tribool down, bool previousState);
		void clickRight(tribool down, bool previousState);
		void hover(bool on);
		void activate();
		void deactivate();
	};

	SDL_Surface * background, * leftCorner, * rightCorner;
	CDefHandler * spells, //pictures of spells
		* spellTab, //school select
		* schools, //schools' pictures
		* schoolBorders [4]; //schools' 'borders': [0]: air, [1]: fire, [2]: water, [3]: earth

	SpellbookInteractiveArea * exitBtn, * battleSpells, * adventureSpells, * manaPoints;
	SpellbookInteractiveArea * selectSpellsA, * selectSpellsE, * selectSpellsF, * selectSpellsW, * selectSpellsAll;
	SpellbookInteractiveArea * lCorner, * rCorner;
	SpellArea * spellAreas[12];
	CStatusBar * statusBar;

	Uint8 sitesPerTabAdv[5];
	Uint8 sitesPerTabBattle[5];

	bool battleSpellsOnly; //if true, only battle spells are displayed; if false, only adventure map spells are displayed
	Uint8 selectedTab; // 0 - air magic, 1 - fire magic, 2 - water magic, 3 - earth magic, 4 - all schools
	Uint8 spellSite; //changes when corners are clicked
	std::set<ui32> mySpells; //all spels in this spellbook
	Uint8 schoolLvls[4]; //levels of magic for different schools: [0]: air, [1]: fire, [2]: water, [3]: earth; 0 - none, 1 - beginner, 2 - medium, 3 - expert

	const CGHeroInstance * myHero; //hero whose spells are presented

	void computeSpellsPerArea(); //recalculates spellAreas::mySpell

	void turnPageLeft();
	void turnPageRight();

	CPlayerInterface * myInt;

public:

	CSpellWindow(const SDL_Rect & myRect, const CGHeroInstance * _myHero, CPlayerInterface * _myInt, bool openOnBattleSpells = true); //c-tor
	~CSpellWindow(); //d-tor

	void fexitb();
	void fadvSpellsb();
	void fbattleSpellsb();
	void fmanaPtsb();

	void fspellsAb();
	void fspellsEb();
	void fspellsFb();
	void fspellsWb();
	void fspellsAllb();

	void fLcornerb();
	void fRcornerb();

	void keyPressed(const SDL_KeyboardEvent & key);
	void activate();
	void deactivate();
	void show(SDL_Surface * to);
};

#endif // __CSPELLWINDOW_H__
