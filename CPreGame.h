#include "SDL.h"
#include "CSemiDefHandler.h"
#include "CSemiLodHandler.h"
//struct AnimatedPic
//{
//	std::vector<SDL_Surface*> frames;
//	~AnimatedPic()
//	{
//		for (int i=0;i<frames.size();i++) 
//		{
//			SDL_FreeSurface(frames[i]);
//			delete frames[i];
//		}
//	}
//};
class CPreGame
{
public:
	enum EState { //where are we?
		mainMenu, ScenarioList
	} state;
	struct menuItems { 
		SDL_Surface * background;
		CSemiDefHandler *newGame, *loadGame, *highScores,*credits, *quit;
		SDL_Rect lNewGame, lLoadGame, lHighScores, lCredits, lQuit;
		int highlighted;//0=none; 1=new game; 2=load game; 3=high score; 4=credits; 5=quit
	} * ourMainMenu, * newGameManu;
	std::string map; //selected map
	std::vector<CSemiLodHandler *> handledLods; 
	CPreGame(); //c-tor
	void showMainMenu();  
	void runLoop(); // runs mainloop of PreGame
	void initMainMenu(); //loads components for main menu
	void highlightButton(int which, int on);
};