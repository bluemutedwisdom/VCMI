#include "CGameState.h"
#include "CGameInterface.h"
#include "CPlayerInterface.h"
#include <algorithm>
#include "SDL_Thread.h"
#include "SDL_Extensions.h"

class CStack
{
public:
	int ID;
	CCreature * creature;
	int amount;
	int owner;
	int position;
	bool alive;
	CStack(CCreature * C, int A, int O, int I):creature(C),amount(A),owner(O), alive(true), position(-1), ID(I){};
};

class CMP_stack
{
public:
	bool operator ()(const CStack* a, const CStack* b)
	{
		return (a->creature->speed)<(b->creature->speed);
	}
} cmpst ;

void CGameState::battle(CCreatureSet * army1, CCreatureSet * army2, int3 tile, CArmedInstance *hero1, CArmedInstance *hero2)
{
	curB = new BattleInfo();
	std::vector<CStack*> & stacks = (curB->stacks);

	curB->army1=army1;
	curB->army2=army2;
	curB->hero1=dynamic_cast<CGHeroInstance*>(hero1);
	curB->hero2=dynamic_cast<CGHeroInstance*>(hero2);
	curB->side1=(hero1)?(hero1->tempOwner):(-1);
	curB->side2=(hero2)?(hero2->tempOwner):(-1);
	curB->round = -2;
	for(std::map<int,std::pair<CCreature*,int> >::iterator i = army1->slots.begin(); i!=army1->slots.end(); i++)
		stacks.push_back(new CStack(i->second.first,i->second.second,0, stacks.size()));
	for(std::map<int,std::pair<CCreature*,int> >::iterator i = army2->slots.begin(); i!=army2->slots.end(); i++)
		stacks.push_back(new CStack(i->second.first,i->second.second,1, stacks.size()));
	std::stable_sort(stacks.begin(),stacks.end(),cmpst);

	//for start inform players about battle
	for(std::map<int, PlayerState>::iterator j=CGI->state->players.begin(); j!=CGI->state->players.end(); ++j)//CGI->state->players.size(); ++j) //for testing
	{
		if (j->first > PLAYER_LIMIT)
			break;
		if(j->second.fogOfWarMap[tile.x][tile.y][tile.z])
		{ //player should be notified
			tribool side = tribool::indeterminate_value;
			if(j->first == curB->side1) //player is attacker
				side = false;
			else if(j->first == curB->side2) //player is defender
				side = true;
			else 
				return; //no witnesses
			if(CGI->playerint[j->second.serial]->human)
			{
				((CPlayerInterface*)( CGI->playerint[j->second.serial] ))->battleStart(army1, army2, tile, curB->hero1, curB->hero2, side);
			}
			else
			{
				//CGI->playerint[j->second.serial]->battleStart(army1, army2, tile, curB->hero1, curB->hero2, side);
			}
		}
	}

	curB->round++;
	if( (curB->hero1 && curB->hero1->getSecSkillLevel(19)>=0) || ( curB->hero2 && curB->hero2->getSecSkillLevel(19)>=0)  )//someone has tactics
	{
		//TODO: wywolania dla rundy -1, ograniczenie pola ruchu, etc
	}

	curB->round++;

	//SDL_Thread * eventh = SDL_CreateThread(battleEventThread, NULL);

	while(true) //do zwyciestwa jednej ze stron
	{
		for(int i=0;i<stacks.size();i++)
		{
			curB->activeStack = i;
			if(stacks[i]->alive) //niech interfejs ruszy oddzialem
			{
				unsigned char owner = (stacks[i]->owner)?(hero2->tempOwner):(hero1->tempOwner);
				unsigned char serialOwner = -1;
				for(int g=0; g<CGI->playerint.size(); ++g)
				{
					if(CGI->playerint[g]->playerID == owner)
					{
						serialOwner = g;
						break;
					}
				}
				if(CGI->playerint[serialOwner]->human)
				{
					((CPlayerInterface*)CGI->playerint[serialOwner])->activeStack(stacks[i]->ID);
				}
				else
				{
					//CGI->playerint[serialOwner]->activeStack(stacks[i]->ID);
				}
			}
			//sprawdzic czy po tej akcji ktoras strona nie wygrala bitwy
		}
		SDL_Delay(50);
	}

	for(int i=0;i<stacks.size();i++)
		delete stacks[i];
	delete curB;
	curB = NULL;
}