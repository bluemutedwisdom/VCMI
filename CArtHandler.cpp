#include "CArtHandler.h"
#include <fstream>
#include <iostream>

void CArtHandler::loadArtifacts()
{
	std::ifstream inp("ARTRAITS.TXT", std::ios::in);
	std::string dump;
	for(int i=0; i<44; ++i)
	{
		inp>>dump;
	}
	inp.ignore();
	int numberlet = 0; //numer of artifact
	while(!inp.eof())
	{
		CArtifact nart;
		nart.number=numberlet++;
		char * read = new char[10000]; //here we'll have currently read character
		inp.getline(read, 10000);
		int eol=0; //end of looking
		std::string ss = std::string(read);
		if(ss==std::string("") && inp.eof())
			return;
		for(int i=0; i<200; ++i)
		{
			if(ss[i]=='\t')
			{
				nart.name = ss.substr(0, i);
				eol=i+1;
				break;
			}
		}
		for(int i=eol; i<eol+200; ++i)
		{
			if(ss[i]=='\t')
			{
				nart.price = atoi(ss.substr(eol, i).c_str());
				eol=i+1;
				break;
			}
		}
		if(ss[eol]=='x')
			nart.spellBook = true;
		else
			nart.spellBook = false;
		if(ss[eol++]!='\t')
			eol++;
		if(ss[eol]=='x')
			nart.warMachine4 = true;
		else
			nart.warMachine4 = false;
		if(ss[eol++]!='\t')
			eol++;
		if(ss[eol]=='x')
			nart.warMachine3 = true;
		else
			nart.warMachine3 = false;
		if(ss[eol++]!='\t')
			eol++;
		if(ss[eol]=='x')
			nart.warMachine2 = true;
		else
			nart.warMachine2 = false;
		if(ss[eol++]!='\t')
			eol++;
		if(ss[eol]=='x')
			nart.warMachine1 = true;
		else
			nart.warMachine1 = false;
		if(ss[eol++]!='\t')
			eol++;
		if(ss[eol]=='x')
			nart.misc5 = true;
		else
			nart.misc5 = false;
		if(ss[eol++]!='\t')
			eol++;
		if(ss[eol]=='x')
			nart.misc4 = true;
		else
			nart.misc4 = false;
		if(ss[eol++]!='\t')
			eol++;
		if(ss[eol]=='x')
			nart.misc3 = true;
		else
			nart.misc3 = false;
		if(ss[eol++]!='\t')
			eol++;
		if(ss[eol]=='x')
			nart.misc2 = true;
		else
			nart.misc2 = false;
		if(ss[eol++]!='\t')
			eol++;
		if(ss[eol]=='x')
			nart.misc1 = true;
		else
			nart.misc1 = false;
		if(ss[eol++]!='\t')
			eol++;
		if(ss[eol]=='x')
			nart.feet = true;
		else
			nart.feet = false;
		if(ss[eol++]!='\t')
			eol++;
		if(ss[eol]=='x')
			nart.lRing = true;
		else
			nart.lRing = false;
		if(ss[eol++]!='\t')
			eol++;
		if(ss[eol]=='x')
			nart.rRing = true;
		else
			nart.rRing = false;
		if(ss[eol++]!='\t')
			eol++;
		if(ss[eol]=='x')
			nart.torso = true;
		else
			nart.torso = false;
		if(ss[eol++]!='\t')
			eol++;
		if(ss[eol]=='x')
			nart.lHand = true;
		else
			nart.lHand = false;
		if(ss[eol++]!='\t')
			eol++;
		if(ss[eol]=='x')
			nart.rHand = true;
		else
			nart.rHand = false;
		if(ss[eol++]!='\t')
			eol++;
		if(ss[eol]=='x')
			nart.neck = true;
		else
			nart.neck = false;
		if(ss[eol++]!='\t')
			eol++;
		if(ss[eol]=='x')
			nart.shoulders = true;
		else
			nart.shoulders = false;
		if(ss[eol++]!='\t')
			eol++;
		if(ss[eol]=='x')
			nart.head = true;
		else
			nart.head = false;
		if(ss[eol++]!='\t')
			eol++;
		switch(ss[eol])
		{
		case 'J':
			nart.aClass = EartClass::JartClass;
			break;
			
		case 'N':
			nart.aClass = EartClass::NartClass;
			break;
			
		case 'R':
			nart.aClass = EartClass::RartClass;
			break;
			
		case 'S':
			nart.aClass = EartClass::SartClass;
			break;
			
		case 'T':
			nart.aClass = EartClass::TartClass;
			break;
		}
		if(ss[eol++]!='\t')
			eol++;
		nart.description = ss.substr(eol, ss.size());
		inp.getline(read, 10000);
		int bowCounter=0;
		nart.desc2 = "";
		do //do - while begin
		{
			++bowCounter;
			inp.getline(read, 10000);
			//inp.ignore();
			ss = std::string(read);
			nart.desc2 += ss;
		}
		while(nart.desc2[nart.desc2.size()-1]!='"' || (nart.name==std::string("�uk Penetracji") && bowCounter<4) ); //do - while end
		this->artifacts.push_back(nart);
	}
}