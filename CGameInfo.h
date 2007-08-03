#ifndef CGAMEINFO_H
#define CGAMEINFO_H

#include "CPreGame.h"
#include "StartInfo.h"
#include "CSpellHandler.h"
#include "CAbilityHandler.h"
#include "CCreaturehandler.h"
#include "CArtHandler.h"
#include "CHeroHandler.h"
#include "CAmbarCendamo.h"
#include "CBuildingHandler.h"
#include "CObjectHandler.h"
#include "CMusicHandler.h"
#include "CSemiLodHandler.h"
#include "CDefObjInfoHandler.h"
#include "CLodHandler.h"
#include "CTownHandler.h"
#include "CGeneralTextHandler.h"
#include "CGameInterface.h"
#include "SDL.h"

#include <vector>
/*
	CGameInfo class
	for allowing different functions for modifying game informations
*/
class CGameInfo
{
public:
	static CGameInfo * mainObj; //pointer to main CGameInfo object
	CArtHandler * arth;
	CHeroHandler * heroh;
	CCreatureHandler * creh;
	CAbilityHandler * abilh;
	CSpellHandler * spellh;
	CAmbarCendamo * ac;
	CBuildingHandler * buildh;
	CObjectHandler * objh;
	CMusicHandler * mush;
	CSemiLodHandler * sspriteh;
	CDefObjInfoHandler * dobjinfo;
	CTownHandler * townh;
	CLodHandler * spriteh;
	CLodHandler * bitmaph;
	CGeneralTextHandler * generaltexth;
	CPlayerInterface * playerint;
	std::vector<SDL_Color> playerColors;
	SDL_Color neutralColor;
	StartInfo scenarioOps;
};

#endif //CGAMEINFO_H