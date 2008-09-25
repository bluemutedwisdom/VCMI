#pragma once
#include "CPlayerInterface.h"
#include "client/FunctionList.h"
#include <boost/bind.hpp>
class AdventureMapButton 
	: public ClickableL, public ClickableR, public Hoverable, public KeyInterested, public CButtonBase
{
public:
	std::map<int,std::string> hoverTexts; //state -> text for statusbar
	std::string helpBox; //for right-click help
	char key; //key shortcut
	CFunctionList<void()> callback;
	bool colorChange, blocked,
		actOnDown; //runs when mouse is pressed down over it, not when up

	void clickRight (tribool down);
	virtual void clickLeft (tribool down);
	void hover (bool on);
	void block(bool on); //if button is blocked then it'll change it's graphic to inactive (offset==2) and won't react on l-clicks
	void keyPressed (const SDL_KeyboardEvent & key);
	void activate(); // makes button active
	void deactivate(); // makes button inactive (but doesn't delete)

	AdventureMapButton(); //c-tor
	AdventureMapButton( const std::map<int,std::string> &, const std::string &HelpBox, const CFunctionList<void()> &Callback, int x, int y, const std::string &defName, bool activ=false,  std::vector<std::string> * add = NULL, bool playerColoredButton = false );//c-tor
	AdventureMapButton( const std::string &Name, const std::string &HelpBox, const CFunctionList<void()> &Callback, int x, int y, const std::string &defName, bool activ=false,  std::vector<std::string> * add = NULL, bool playerColoredButton = false );//c-tor
	//AdventureMapButton( std::string Name, std::string HelpBox, boost::function<void()> Callback, int x, int y, std::string defName, bool activ=false,  std::vector<std::string> * add = NULL, bool playerColoredButton = false );//c-tor

	void init(const CFunctionList<void()> &Callback, const std::map<int,std::string> &Name, const std::string &HelpBox, bool playerColoredButton, const std::string &defName, std::vector<std::string> * add, int x, int y, bool activ );
};

class CHighlightableButton 
	: public AdventureMapButton
{
public:
	CHighlightableButton(const CFunctionList<void()> &onSelect, const CFunctionList<void()> &onDeselect, const std::map<int,std::string> &Name, const std::string &HelpBox, bool playerColoredButton, const std::string &defName, std::vector<std::string> * add, int x, int y, bool activ );
	bool selected, onlyOn;
	CFunctionList<void()> callback2; //when disselecting
	void select(bool on);
	void clickLeft (tribool down);
};

class CHighlightableButtonsGroup
{
public:
	CFunctionList2<void(int)> onChange; //called when changing selected button with new button's id
	std::vector<CHighlightableButton*> buttons;

	//void addButton(const std::map<int,std::string> &tooltip, const std::string &HelpBox, const std::string &defName, int x, int y, int uid);
	void addButton(CHighlightableButton* bt);//add existing button, it'll be deleted by CHighlightableButtonsGroup destructor
	void addButton(const std::map<int,std::string> &tooltip, const std::string &HelpBox, const std::string &defName, int x, int y, int uid, const CFunctionList<void()> &OnSelect=0); //creates new button
	CHighlightableButtonsGroup(const CFunctionList2<void(int)> &OnChange);
	~CHighlightableButtonsGroup();
	void activate();
	void deactivate();
	void select(int id, bool mode); //mode==0: id is serial; mode==1: id is unique button id
	void selectionChanged(int to);
	void show(SDL_Surface * to = NULL);
};


class CSlider : public IShowable, public MotionInterested, public ClickableL
{
public:
	AdventureMapButton left, right, slider; //if vertical then left=up
	int capacity,//how many elements can be active at same time
		amount, //how many elements
		value; //first active element
	bool horizontal, moving;
	CDefEssential *imgs ;

	boost::function<void(int)> moved;
	//void(T::*moved)(int to);
	//T* owner;

	void redrawSlider(); 

	void sliderClicked();
	void moveLeft();
	void clickLeft (tribool down);
	void mouseMoved (const SDL_MouseMotionEvent & sEvent);
	void moveRight();
	void moveTo(int to);
	void block(bool on);
	void activate(); // makes button active
	void deactivate(); // makes button inactive (but doesn't delete)
	void show(SDL_Surface * to = NULL);
	CSlider(int x, int y, int totalw, boost::function<void(int)> Moved, int Capacity, int Amount, 
		int Value=0, bool Horizontal=true);
	~CSlider();
};	
