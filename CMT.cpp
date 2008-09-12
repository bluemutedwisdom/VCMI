// CMT.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <cmath>
#include <string>
#include <vector>
#include <queue>
#include <cmath>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/thread.hpp>
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "SDL_Extensions.h"
#include "SDL_framerate.h"
#include "CGameInfo.h"
#include "mapHandler.h"
#include "global.h"
#include "CPreGame.h"
#include "CConsoleHandler.h"
#include "CCursorHandler.h"
#include "CPathfinder.h"
#include "CGameState.h"
#include "CCallback.h"
#include "CPlayerInterface.h"
#include "CLuaHandler.h"
#include "CAdvmapInterface.h"
#include "hch/CBuildingHandler.h"
#include "hch/CVideoHandler.h"
#include "hch/CAbilityHandler.h"
#include "hch/CHeroHandler.h"
#include "hch/CCreatureHandler.h"
#include "hch/CSpellHandler.h"
#include "hch/CMusicHandler.h"
#include "hch/CLodHandler.h"
#include "hch/CDefHandler.h"
#include "hch/CAmbarCendamo.h"
#include "hch/CGeneralTextHandler.h"
#include "client/Graphics.h"
#include "client/Client.h"
#include "lib/Connection.h"
#include "lib/Interprocess.h"
#include "lib/VCMI_Lib.h"
std::string NAME = NAME_VER + std::string(" (client)");
DLL_EXPORT void initDLL(CLodHandler *b);
SDL_Surface * screen, * screen2;
extern SDL_Surface * CSDL_Ext::std32bppSurface;
std::queue<SDL_Event> events;
boost::mutex eventsM;
TTF_Font * TNRB16, *TNR, *GEOR13, *GEORXX, *GEORM, *GEOR16;
namespace intpr = boost::interprocess;
#ifndef __GNUC__
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char** argv)
#endif
{ 
	int port;
	if(argc > 1)
	{
#ifdef _MSC_VER
		port = _tstoi(argv[1]);
#else
		port = _ttoi(argv[1]);
#endif
	}
	else
	{
		port = 3030;
		std::cout << "Port " << port << " will be used." << std::endl;
	}
	std::cout.flags(ios::unitbuf);
	std::cout << NAME << std::endl;
	srand ( time(NULL) );
	CPG=NULL;
	atexit(SDL_Quit);
	CGameInfo * cgi = CGI = new CGameInfo; //contains all global informations about game (texts, lodHandlers, map handler itp.)
	//CLuaHandler luatest;
	//luatest.test(); 
		//CBIKHandler cb;
		//cb.open("CSECRET.BIK");
	std::cout << "Starting... " << std::endl;
	THC timeHandler tmh, total, pomtime;
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO/*|SDL_INIT_EVENTTHREAD*/)==0)
	{
		screen = SDL_SetVideoMode(800,600,24,SDL_SWSURFACE|SDL_DOUBLEBUF/*|SDL_FULLSCREEN*/);  //initializing important global surface
		THC std::cout<<"\tInitializing screen: "<<pomtime.getDif()<<std::endl;
		SDL_WM_SetCaption(NAME.c_str(),""); //set window title
		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			int rmask = 0xff000000;int gmask = 0x00ff0000;int bmask = 0x0000ff00;int amask = 0x000000ff;
		#else
			int rmask = 0x000000ff;	int gmask = 0x0000ff00;	int bmask = 0x00ff0000;	int amask = 0xff000000;
		#endif
		CSDL_Ext::std32bppSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, 1, 1, 32, rmask, gmask, bmask, amask);
		THC std::cout<<"\tInitializing minors: "<<pomtime.getDif()<<std::endl;
		TTF_Init();
		TNRB16 = TTF_OpenFont("Fonts" PATHSEPARATOR "tnrb.ttf",16);
		GEOR13 = TTF_OpenFont("Fonts" PATHSEPARATOR "georgia.ttf",13);
		GEOR16 = TTF_OpenFont("Fonts" PATHSEPARATOR "georgia.ttf",16);
		GEORXX = TTF_OpenFont("Fonts" PATHSEPARATOR "tnrb.ttf",22);
		GEORM = TTF_OpenFont("Fonts" PATHSEPARATOR "georgia.ttf",10);
		atexit(TTF_Quit);
		THC std::cout<<"\tInitializing fonts: "<<pomtime.getDif()<<std::endl;
		CMusicHandler * mush = new CMusicHandler;  //initializing audio
		mush->initMusics();
		//audio initialized 
		cgi->consoleh = new CConsoleHandler;
		cgi->mush = mush;
		THC std::cout<<"\tInitializing sound: "<<pomtime.getDif()<<std::endl;
		THC std::cout<<"Initializing screen, fonts and sound handling: "<<tmh.getDif()<<std::endl;
		CDefHandler::Spriteh = cgi->spriteh = new CLodHandler();
		cgi->spriteh->init("Data" PATHSEPARATOR "H3sprite.lod","Sprites");
		BitmapHandler::bitmaph = cgi->bitmaph = new CLodHandler;
		cgi->bitmaph->init("Data" PATHSEPARATOR "H3bitmap.lod","Data");
		THC std::cout<<"Loading .lod files: "<<tmh.getDif()<<std::endl;
		initDLL(cgi->bitmaph);
		CGI->arth = VLC->arth;
		CGI->creh = VLC->creh;
		CGI->townh = VLC->townh;
		CGI->heroh = VLC->heroh;
		CGI->objh = VLC->objh;
		CGI->spellh = VLC->spellh;
		CGI->dobjinfo = VLC->dobjinfo;
		CGI->buildh = VLC->buildh;
		THC std::cout<<"Initializing VCMI_Lib: "<<tmh.getDif()<<std::endl;
		//cgi->curh->initCursor();
		//cgi->curh->showGraphicCursor();
		pomtime.getDif();
		cgi->curh = new CCursorHandler; 
		cgi->curh->initCursor();
		//cgi->screenh = new CScreenHandler;
		//cgi->screenh->initScreen();
		THC std::cout<<"\tScreen handler: "<<pomtime.getDif()<<std::endl;
		CAbilityHandler * abilh = new CAbilityHandler;
		abilh->loadAbilities();
		cgi->abilh = abilh;
		THC std::cout<<"\tAbility handler: "<<pomtime.getDif()<<std::endl;
		THC std::cout<<"Preparing first handlers: "<<tmh.getDif()<<std::endl;
		pomtime.getDif();
		graphics = new Graphics();
		THC std::cout<<"\tMain graphics: "<<tmh.getDif()<<std::endl;
		std::vector<CDefHandler **> animacje;
		for(std::vector<CHeroClass *>::iterator i = cgi->heroh->heroClasses.begin();i!=cgi->heroh->heroClasses.end();i++)
			animacje.push_back(&((*i)->*(&CHeroClass::moveAnim)));
		graphics->loadHeroAnim(animacje);
		THC std::cout<<"\tHero animations: "<<tmh.getDif()<<std::endl;
		THC std::cout<<"Initializing game graphics: "<<tmh.getDif()<<std::endl;
		CMessage::init();
		cgi->generaltexth = new CGeneralTextHandler;
		cgi->generaltexth->load();
		THC std::cout<<"Preparing more handlers: "<<tmh.getDif()<<std::endl;
		CPreGame * cpg = new CPreGame(); //main menu and submenus
		THC std::cout<<"Initialization CPreGame (together): "<<tmh.getDif()<<std::endl;
		THC std::cout<<"Initialization of VCMI (togeter): "<<total.getDif()<<std::endl;
		cpg->mush = mush;

		StartInfo *options = new StartInfo(cpg->runLoop());
		tmh.getDif();
	////////////////////////SERVER STARTING/////////////////////////////////////////////////
		char portc[10]; SDL_itoa(port,portc,10);
		intpr::shared_memory_object smo(intpr::open_or_create,"vcmi_memory",intpr::read_write);
		smo.truncate(sizeof(ServerReady));
		intpr::mapped_region mr(smo,intpr::read_write);
		ServerReady *sr = new(mr.get_address())ServerReady();
		std::string comm = std::string(SERVER_NAME) + " " + portc + " > server_log.txt";
		boost::thread servthr(boost::bind(system,comm.c_str())); //runs server executable; 	//TODO: will it work on non-windows platforms?
		THC std::cout<<"Preparing shared memory and starting server: "<<tmh.getDif()<<std::endl;
	///////////////////////////////////////////////////////////////////////////////////////
		THC tmh.getDif();pomtime.getDif();//reset timers
		cgi->pathf = new CPathfinder();
		THC std::cout<<"\tPathfinder: "<<pomtime.getDif()<<std::endl;
		if(argc>2)
		{
			std::cout << "Special mode without support for console!" << std::endl;
		}
		else
		{
			cgi->consoleh->runConsole();
		}
		THC std::cout<<"\tCallback and console: "<<pomtime.getDif()<<std::endl;
		THC std::cout<<"Handlers initialization (together): "<<tmh.getDif()<<std::endl;
		std::ofstream lll("client_log.txt");

		CConnection *c=NULL;
		//wait until server is ready
		std::cout<<"Waiting for server... " << std::flush;
		{
			intpr::scoped_lock<intpr::interprocess_mutex> slock(sr->mutex);
			while(!sr->ready)
			{
				sr->cond.wait(slock);
			}
		}
		intpr::shared_memory_object::remove("vcmi_memory");
		std::cout << tmh.getDif()<<std::endl;
		while(!c)
		{
			try
			{
				std::cout << "Establishing connection...\t";
				c = new CConnection("127.0.0.1",portc,NAME,lll);
				std::cout << "done!" <<std::endl;
			}
			catch(...)
			{
				std::cout << "\nCannot establish connection! Retrying within 3 seconds" <<std::endl;
				SDL_Delay(3000);
			}
		}
		THC std::cout<<"\tConnecting to the server: "<<tmh.getDif()<<std::endl;
		CClient cl(c,options);
		boost::thread t(boost::bind(&CClient::run,&cl));
		SDL_Event ev;
		while(1) //main SDL events loop
		{
			SDL_WaitEvent(&ev);
			if(ev.type==SDL_QUIT) 
			{
				cl.close();
				SDL_Delay(750);
				exit(0);
			}
			eventsM.lock();
			events.push(ev);
			eventsM.unlock();
		}
	}
	else
	{
		printf("Something was wrong: %s/n", SDL_GetError());
		return -1;
	}
}
