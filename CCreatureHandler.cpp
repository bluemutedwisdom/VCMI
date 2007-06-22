#include "stdafx.h"
#include "CCreatureHandler.h"

void CCreatureHandler::loadCreatures()
{
	std::ifstream inp("H3bitmap.lod\\ZCRTRAIT.TXT", std::ios::in|std::ios::binary);
	inp.seekg(0,std::ios::end); // na koniec
	int andame = inp.tellg();  // read length
	inp.seekg(0,std::ios::beg); // wracamy na poczatek
	char * bufor = new char[andame]; // allocate memory 
	inp.read((char*)bufor, andame); // read map file to buffer
	std::string buf = std::string(bufor);
	delete [andame] bufor;
	int i=0; //buf iterator
	int hmcr=0;
	for(i; i<andame; ++i)
	{
		if(buf[i]=='\r')
			++hmcr;
		if(hmcr==2)
			break;
	}
	i+=2;

	while(i<buf.size())
	{
		if(creatures.size()>190 && buf.substr(i, buf.size()-i).find('\r')==std::string::npos)
		{
			loadAnimationInfo();
			break;
		}

		CCreature ncre;

		int befi=i;
		for(i; i<andame; ++i)
		{
			if(buf[i]=='\t')
				break;
		}
		ncre.nameSing = buf.substr(befi, i-befi);
		++i;

		befi=i;
		for(i; i<andame; ++i)
		{
			if(buf[i]=='\t')
				break;
		}
		ncre.namePl = buf.substr(befi, i-befi);
		++i;

		befi=i;
		for(i; i<andame; ++i)
		{
			if(buf[i]=='\t')
				break;
		}
		ncre.wood = atoi(buf.substr(befi, i-befi).c_str());
		++i;

		befi=i;
		for(i; i<andame; ++i)
		{
			if(buf[i]=='\t')
				break;
		}
		ncre.mercury = atoi(buf.substr(befi, i-befi).c_str());
		++i;

		befi=i;
		for(i; i<andame; ++i)
		{
			if(buf[i]=='\t')
				break;
		}
		ncre.ore = atoi(buf.substr(befi, i-befi).c_str());
		++i;

		befi=i;
		for(i; i<andame; ++i)
		{
			if(buf[i]=='\t')
				break;
		}
		ncre.sulfur = atoi(buf.substr(befi, i-befi).c_str());
		++i;

		befi=i;
		for(i; i<andame; ++i)
		{
			if(buf[i]=='\t')
				break;
		}
		ncre.crystal = atoi(buf.substr(befi, i-befi).c_str());
		++i;

		befi=i;
		for(i; i<andame; ++i)
		{
			if(buf[i]=='\t')
				break;
		}
		ncre.gems = atoi(buf.substr(befi, i-befi).c_str());
		++i;

		befi=i;
		for(i; i<andame; ++i)
		{
			if(buf[i]=='\t')
				break;
		}
		ncre.gold = atoi(buf.substr(befi, i-befi).c_str());
		++i;

		befi=i;
		for(i; i<andame; ++i)
		{
			if(buf[i]=='\t')
				break;
		}
		ncre.fightValue = atoi(buf.substr(befi, i-befi).c_str());
		++i;

		befi=i;
		for(i; i<andame; ++i)
		{
			if(buf[i]=='\t')
				break;
		}
		ncre.AIValue = atoi(buf.substr(befi, i-befi).c_str());
		++i;

		befi=i;
		for(i; i<andame; ++i)
		{
			if(buf[i]=='\t')
				break;
		}
		ncre.growth = atoi(buf.substr(befi, i-befi).c_str());
		++i;

		befi=i;
		for(i; i<andame; ++i)
		{
			if(buf[i]=='\t')
				break;
		}
		ncre.hordeGrowth = atoi(buf.substr(befi, i-befi).c_str());
		++i;

		befi=i;
		for(i; i<andame; ++i)
		{
			if(buf[i]=='\t')
				break;
		}
		ncre.hitPoints = atoi(buf.substr(befi, i-befi).c_str());
		++i;

		befi=i;
		for(i; i<andame; ++i)
		{
			if(buf[i]=='\t')
				break;
		}
		ncre.speed = atoi(buf.substr(befi, i-befi).c_str());
		++i;

		befi=i;
		for(i; i<andame; ++i)
		{
			if(buf[i]=='\t')
				break;
		}
		ncre.attack = atoi(buf.substr(befi, i-befi).c_str());
		++i;

		befi=i;
		for(i; i<andame; ++i)
		{
			if(buf[i]=='\t')
				break;
		}
		ncre.defence = atoi(buf.substr(befi, i-befi).c_str());
		++i;

		befi=i;
		for(i; i<andame; ++i)
		{
			if(buf[i]=='\t')
				break;
		}
		ncre.low1 = atoi(buf.substr(befi, i-befi).c_str());
		++i;

		befi=i;
		for(i; i<andame; ++i)
		{
			if(buf[i]=='\t')
				break;
		}
		ncre.high1 = atoi(buf.substr(befi, i-befi).c_str());
		++i;

		befi=i;
		for(i; i<andame; ++i)
		{
			if(buf[i]=='\t')
				break;
		}
		ncre.shots = atoi(buf.substr(befi, i-befi).c_str());
		++i;

		befi=i;
		for(i; i<andame; ++i)
		{
			if(buf[i]=='\t')
				break;
		}
		ncre.spells = atoi(buf.substr(befi, i-befi).c_str());
		++i;

		befi=i;
		for(i; i<andame; ++i)
		{
			if(buf[i]=='\t')
				break;
		}
		ncre.low2 = atoi(buf.substr(befi, i-befi).c_str());
		++i;

		befi=i;
		for(i; i<andame; ++i)
		{
			if(buf[i]=='\t')
				break;
		}
		ncre.high2 = atoi(buf.substr(befi, i-befi).c_str());
		++i;

		befi=i;
		for(i; i<andame; ++i)
		{
			if(buf[i]=='\t')
				break;
		}
		ncre.abilityText = buf.substr(befi, i-befi);
		++i;

		befi=i;
		for(i; i<andame; ++i)
		{
			if(buf[i]=='\r')
				break;
		}
		ncre.abilityRefs = buf.substr(befi, i-befi);
		i+=2;
		if(ncre.nameSing!=std::string("") && ncre.namePl!=std::string(""))
		{
			ncre.idNumber = creatures.size();
			creatures.push_back(ncre);
		}
	}
}

void CCreatureHandler::loadAnimationInfo()
{
	std::ifstream inp("H3bitmap.lod\\CRANIM.TXT", std::ios::in|std::ios::binary);
	inp.seekg(0,std::ios::end); // na koniec
	int andame = inp.tellg();  // read length
	inp.seekg(0,std::ios::beg); // wracamy na poczatek
	char * bufor = new char[andame]; // allocate memory 
	inp.read((char*)bufor, andame); // read map file to buffer
	inp.close();
	std::string buf = std::string(bufor);
	delete [andame] bufor;
	int i=0; //buf iterator
	int hmcr=0;
	for(i; i<andame; ++i)
	{
		if(buf[i]=='\r')
			++hmcr;
		if(hmcr==2)
			break;
	}
	i+=2;
	for(int dd=0; dd<creatures.size(); ++dd)
	{
		loadUnitAnimInfo(creatures[dd], buf, i);
	}
	return;
}

void CCreatureHandler::loadUnitAnimInfo(CCreature & unit, std::string & src, int & i)
{
	int befi=i;
	for(i; i<src.size(); ++i)
	{
		if(src[i]=='\t')
			break;
	}
	unit.timeBetweenFidgets = atof(src.substr(befi, i-befi).c_str());
	++i;

	while(unit.timeBetweenFidgets == 0.0)
	{
		for(i; i<src.size(); ++i)
		{
			if(src[i]=='\r')
				break;
		}
		i+=2;
		befi=i;
		for(i; i<src.size(); ++i)
		{
			if(src[i]=='\t')
				break;
		}
		unit.timeBetweenFidgets = atof(src.substr(befi, i-befi).c_str());
		++i;
	}

	befi=i;
	for(i; i<src.size(); ++i)
	{
		if(src[i]=='\t')
			break;
	}
	unit.walkAnimationTime = atof(src.substr(befi, i-befi).c_str());
	++i;

	befi=i;
	for(i; i<src.size(); ++i)
	{
		if(src[i]=='\t')
			break;
	}
	unit.attackAnimationTime = atof(src.substr(befi, i-befi).c_str());
	++i;

	befi=i;
	for(i; i<src.size(); ++i)
	{
		if(src[i]=='\t')
			break;
	}
	unit.flightAnimationDistance = atof(src.substr(befi, i-befi).c_str());
	++i;

	///////////////////////

	befi=i;
	for(i; i<src.size(); ++i)
	{
		if(src[i]=='\t')
			break;
	}
	unit.upperRightMissleOffsetX = atoi(src.substr(befi, i-befi).c_str());
	++i;

	befi=i;
	for(i; i<src.size(); ++i)
	{
		if(src[i]=='\t')
			break;
	}
	unit.upperRightMissleOffsetY = atoi(src.substr(befi, i-befi).c_str());
	++i;

	befi=i;
	for(i; i<src.size(); ++i)
	{
		if(src[i]=='\t')
			break;
	}
	unit.rightMissleOffsetX = atoi(src.substr(befi, i-befi).c_str());
	++i;

	befi=i;
	for(i; i<src.size(); ++i)
	{
		if(src[i]=='\t')
			break;
	}
	unit.rightMissleOffsetY = atoi(src.substr(befi, i-befi).c_str());
	++i;

	befi=i;
	for(i; i<src.size(); ++i)
	{
		if(src[i]=='\t')
			break;
	}
	unit.lowerRightMissleOffsetX = atoi(src.substr(befi, i-befi).c_str());
	++i;

	befi=i;
	for(i; i<src.size(); ++i)
	{
		if(src[i]=='\t')
			break;
	}
	unit.lowerRightMissleOffsetY = atoi(src.substr(befi, i-befi).c_str());
	++i;

	///////////////////////

	for(int jjj=0; jjj<12; ++jjj)
	{
		befi=i;
		for(i; i<src.size(); ++i)
		{
			if(src[i]=='\t')
				break;
		}
		unit.missleFrameAngles[jjj] = atof(src.substr(befi, i-befi).c_str());
		++i;
	}

	befi=i;
	for(i; i<src.size(); ++i)
	{
		if(src[i]=='\t')
			break;
	}
	unit.troopCountLocationOffset= atoi(src.substr(befi, i-befi).c_str());
	++i;

	befi=i;
	for(i; i<src.size(); ++i)
	{
		if(src[i]=='\t')
			break;
	}
	unit.attackClimaxFrame = atoi(src.substr(befi, i-befi).c_str());
	++i;

	for(i; i<src.size(); ++i)
	{
		if(src[i]=='\r')
			break;
	}
	i+=2;
}
