#include "stdafx.h"
#include "CDefHandler.h"
#include "SDL_image.h"
#include <sstream>

long long pow(long long a, int b)
{
	if (!b) return 1;
	long c = a;
	while (--b)
		a*=c;
	return a;
}
void BMPHeader::print(std::ostream & out)
{
	CDefHandler::print(out,fullSize,4);
	CDefHandler::print(out,_h1,4);
	CDefHandler::print(out,_c1,4);
	CDefHandler::print(out,_c2,4);
	CDefHandler::print(out,x,4);
	CDefHandler::print(out,y,4);
	CDefHandler::print(out,_c3,2);
	CDefHandler::print(out,_c4,2);
	CDefHandler::print(out,_h2,4);
	CDefHandler::print(out,_h3,4);
	CDefHandler::print(out,dataSize1,4);
	CDefHandler::print(out,dataSize2,4);
	for (int i=0;i<8;i++)
		out << _c5[i];
	out.flush();
}
CDefHandler::CDefHandler()
{
	FDef = NULL;
	RWEntries = NULL;
	RLEntries = NULL;
	notFreeImgs = true;
}
CDefHandler::~CDefHandler()
{
	if (FDef)
		delete FDef;
	if (RWEntries)
		delete RWEntries;
	if (RLEntries)
		delete RLEntries;
	if (notFreeImgs)
		return;
	for (int i=0; i<ourImages.size(); i++)
	{
		if (ourImages[i].bitmap)
		{
			SDL_FreeSurface(ourImages[i].bitmap);
			ourImages[i].bitmap=NULL;
		}
	}
}
void CDefHandler::openDef(std::string name)
{
	int i,j, totalInBlock;
	char Buffer[13];
	defName=name;

	int andame;
	std::ifstream * is = new std::ifstream();
	is -> open(name.c_str(),std::ios::binary);
	is->seekg(0,std::ios::end); // na koniec
	andame = is->tellg();  // read length
	is->seekg(0,std::ios::beg); // wracamy na poczatek
	FDef = new unsigned char[andame]; // allocate memory 
	is->read((char*)FDef, andame); // read map file to buffer
	is->close();
	delete is;
	i = 0;
	DEFType = readNormalNr(i,4); i+=4;
	fullWidth = readNormalNr(i,4); i+=4;
	fullHeight = readNormalNr(i,4); i+=4;
	i=0xc;
	totalBlocks = readNormalNr(i,4); i+=4;

	i=0x10;
	for (int it=0;it<256;it++)
	{
		palette[it].R = FDef[i++];
		palette[it].G = FDef[i++];
		palette[it].B = FDef[i++];
		palette[it].F = 0;
	}
	i=0x310;
	totalEntries=0;
	for (int z=0; z<totalBlocks; z++)
	{
		i+=4;
		totalInBlock = readNormalNr(i,4); i+=4;
		for (j=SEntries.size(); j<totalEntries+totalInBlock; j++)
			SEntries.push_back(SEntry());
		i+=8;
		for (j=0; j<totalInBlock; j++)
		{
			for (int k=0;k<13;k++) Buffer[k]=FDef[i+k]; 
			i+=13;
			SEntries[totalEntries+j].name=Buffer;
		}
		for (j=0; j<totalInBlock; j++)
		{ 
			SEntries[totalEntries+j].offset = readNormalNr(i,4);
			i+=4;
		}
		//totalEntries+=totalInBlock;
		for(int hh=0; hh<totalInBlock; ++hh)
		{
			SEntries[totalEntries].group = z;
			++totalEntries;
		}
	}
	for(j=0; j<SEntries.size(); ++j)
	{
		SEntries[j].name = SEntries[j].name.substr(0, SEntries[j].name.find('.')+4);
	}
	for(int i=0; i<SEntries.size(); ++i)
	{
		Cimage nimg;
		nimg.bitmap = getSprite(i);
		nimg.imName = SEntries[i].name;
		nimg.groupNumber = SEntries[i].group;
		ourImages.push_back(nimg);
	}
	delete FDef;
	FDef = NULL;
}

void CDefHandler::openFromMemory(unsigned char *table, int size, std::string name)
{
	int i,j, totalInBlock;
	char Buffer[13];
	defName=name;
	FDef = new unsigned char[size]; // allocate memory 
	for (int i=0;i<size;i++)
		FDef[i]=table[i];
	int andame = size;
	i = 0;
	DEFType = readNormalNr(i,4); i+=4;
	fullWidth = readNormalNr(i,4); i+=4;
	fullHeight = readNormalNr(i,4); i+=4;
	i=0xc;
	totalBlocks = readNormalNr(i,4); i+=4;

	i=0x10;
	for (int it=0;it<256;it++)
	{
		palette[it].R = FDef[i++];
		palette[it].G = FDef[i++];
		palette[it].B = FDef[i++];
		palette[it].F = 0;
	}
	i=0x310;
	totalEntries=0;
	for (int z=0; z<totalBlocks; z++)
	{
		int unknown1 = readNormalNr(i,4); i+=4;
		totalInBlock = readNormalNr(i,4); i+=4;
		for (j=SEntries.size(); j<totalEntries+totalInBlock; j++)
			SEntries.push_back(SEntry());
		int unknown2 = readNormalNr(i,4); i+=4;
		int unknown3 = readNormalNr(i,4); i+=4;
		for (j=0; j<totalInBlock; j++)
		{
			for (int k=0;k<13;k++) Buffer[k]=FDef[i+k]; 
			i+=13;
			SEntries[totalEntries+j].name=Buffer;
		}
		for (j=0; j<totalInBlock; j++)
		{ 
			SEntries[totalEntries+j].offset = readNormalNr(i,4);
			int unknown4 = readNormalNr(i,4); i+=4;
		}
		//totalEntries+=totalInBlock;
		for(int hh=0; hh<totalInBlock; ++hh)
		{
			SEntries[totalEntries].group = z;
			++totalEntries;
		}
	}
	for(j=0; j<SEntries.size(); ++j)
	{
		SEntries[j].name = SEntries[j].name.substr(0, SEntries[j].name.find('.')+4);
	}
	for(int i=0; i<SEntries.size(); ++i)
	{
		Cimage nimg;
		nimg.bitmap = getSprite(i);
		nimg.imName = SEntries[i].name;
		nimg.groupNumber = SEntries[i].group;
		ourImages.push_back(nimg);
	}
	delete FDef;
	FDef = NULL;
}

unsigned char * CDefHandler::writeNormalNr (int nr, int bytCon)
{
	//int tralalalatoniedziala = 2*9+100-4*bytCon;
	//unsigned char * ret = new unsigned char[bytCon];
	unsigned char * ret = NULL;
	for(int jj=0; jj<100; ++jj)
	{
		ret = (unsigned char*)calloc(1, bytCon);
		if(ret!=NULL)
			break;
	}
	long long amp = pow((float)256,bytCon-1);
	for (int i=bytCon-1; i>=0;i--)
	{
		int test2 = nr/(amp);
		ret[i]=test2;
		nr -= (nr/(amp))*amp;
		amp/=256;
	}
	return ret;
}
void CDefHandler::expand(unsigned char N,unsigned char & BL, unsigned char & BR)
{
	BL = (N & 0xE0) >> 5;
	BR = N & 0x1F;
}
int CDefHandler::readNormalNr (int pos, int bytCon, unsigned char * str, bool cyclic)
{
	int ret=0;
	int amp=1;
	if (str)
	{
		for (int i=0; i<bytCon; i++)
		{
			ret+=str[pos+i]*amp;
			amp*=256;
		}
	}
	else 
	{
		for (int i=0; i<bytCon; i++)
		{
			ret+=FDef[pos+i]*amp;
			amp*=256;
		}
	}
	if(cyclic && bytCon<4 && ret>=amp/2)
	{
		ret = ret-amp;
	}
	return ret;
}
void CDefHandler::print (std::ostream & stream, int nr, int bytcon)
{
	unsigned char * temp = writeNormalNr(nr,bytcon);
	for (int i=0;i<bytcon;i++)
		stream << char(temp[i]);
	free(temp);
}

SDL_Surface * CDefHandler::getSprite (int SIndex)
{
	SDL_Surface * ret;

	long BaseOffset, 
		SpriteWidth, SpriteHeight, //format sprite'a
		LeftMargin, RightMargin, TopMargin,BottomMargin,
		i, add, FullHeight,FullWidth,
		TotalRowLength, // dlugosc przeczytanego segmentu
		NextSpriteOffset, RowAdd;
	std::ifstream Fdef;
	unsigned char SegmentType, SegmentLength, BL, BR;
	unsigned char * TempDef; //memory

	std::string FTemp;
	

	i=BaseOffset=SEntries[SIndex].offset;
	int prSize=readNormalNr(i,4,FDef);i+=4;
	int defType2 = readNormalNr(i,4,FDef);i+=4;
	FullWidth = readNormalNr(i,4,FDef);i+=4;
	FullHeight = readNormalNr(i,4,FDef);i+=4;
	SpriteWidth = readNormalNr(i,4,FDef);i+=4;
	SpriteHeight = readNormalNr(i,4,FDef);i+=4;
	LeftMargin = readNormalNr(i,4,FDef);i+=4;
	TopMargin = readNormalNr(i,4,FDef);i+=4;
	RightMargin = FullWidth - SpriteWidth - LeftMargin;
	BottomMargin = FullHeight - SpriteHeight - TopMargin;

	BMPHeader tb;
	tb.x = FullWidth;
	tb.y = FullHeight;
	tb.dataSize2 = tb.dataSize1 = tb.x*tb.y;
	tb.fullSize = tb.dataSize1+436;
	tb._h3=tb.fullSize-36;
	
	//add = (int)(4*(((float)1) - ((int)(((int)((float)FullWidth/(float)4))-((float)FullWidth/(float)4)))));
	add = 4 - FullWidth%4;
	/*if (add==4)
		add=0;*/ //moved to defcompression dependent block

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    int rmask = 0xff000000;
    int gmask = 0x00ff0000;
    int bmask = 0x0000ff00;
    int amask = 0x000000ff;
#else
    int rmask = 0x000000ff;
    int gmask = 0x0000ff00;
    int bmask = 0x00ff0000;
    int amask = 0xff000000;
#endif
		
	ret = SDL_CreateRGBSurface(SDL_SWSURFACE, FullWidth, FullHeight, 8, 0, 0, 0, 0);
	//int tempee2 = readNormalNr(0,4,((unsigned char *)tempee.c_str()));

	int BaseOffsetor = BaseOffset = i;

	for(int i=0; i<256; ++i)
	{
		SDL_Color pr;
		pr.r = palette[i].R;
		pr.g = palette[i].G;
		pr.b = palette[i].B;
		pr.unused = palette[i].F;
		(*(ret->format->palette->colors+i))=pr;
	}

	for (int i=0;i<800;i++)
		fbuffer[i]=0;

	if (defType2==0)
	{
		if (add==4)
		add=0;
		if (TopMargin>0)
		{
			for (int i=0;i<TopMargin;i++)
			{
				for (int j=0;j<FullWidth+add;j++)
					FTemp+=fbuffer[j];
			}
		}
		for (int i=0;i<SpriteHeight;i++)
		{
			if (LeftMargin>0)
			{
				for (int j=0;j<LeftMargin;j++)
					FTemp+=fbuffer[j];
			}
			for (int j=0; j<SpriteWidth;j++)
				FTemp+=FDef[BaseOffset++];
			if (RightMargin>0)
			{
				for (int j=0;j<add;j++)
					FTemp+=fbuffer[j];
			}
		}
		if (BottomMargin>0)
		{
			for (int i=0;i<BottomMargin;i++)
			{
				for (int j=0;j<FullWidth+add;j++)
					FTemp+=fbuffer[j];
			}
		}

	}
	if (defType2==1)
	{
		if (add==4)
			add=0; ////////was 3
		if (TopMargin>0)
		{
			for (int i=0;i<TopMargin;i++)
			{
				for (int j=0;j<FullWidth+add;j++)
					FTemp+=fbuffer[j];
			}
		}
		RLEntries = new int[SpriteHeight];
		for (int i=0;i<SpriteHeight;i++)
		{
			RLEntries[i]=readNormalNr(BaseOffset,4,FDef);BaseOffset+=4;
		}
		for (int i=0;i<SpriteHeight;i++)
		{
			BaseOffset=BaseOffsetor+RLEntries[i];
			if (LeftMargin>0)
			{
				for (int j=0;j<LeftMargin;j++)
					FTemp+=fbuffer[j];
			}
			TotalRowLength=0;
			do
			{
				SegmentType=FDef[BaseOffset++];
				SegmentLength=FDef[BaseOffset++];
				if (SegmentType==0xFF)
				{
					for (int k=0;k<=SegmentLength;k++)
					{
						FTemp+=FDef[BaseOffset+k];
						if ((TotalRowLength+k+1)>=SpriteWidth)
							break;
					}
					BaseOffset+=SegmentLength+1;////
					TotalRowLength+=SegmentLength+1;
				}
				else
				{
					for (int k=0;k<SegmentLength+1;k++)
					{
						FTemp+=SegmentType;//
						//FTemp+='\0';
					}
					TotalRowLength+=SegmentLength+1;
				}
			}while(TotalRowLength<SpriteWidth);
			RowAdd=SpriteWidth-TotalRowLength;
			if (RightMargin>0)
			{
				for (int j=0;j<RightMargin;j++)
					FTemp+=fbuffer[j];
			}
			if (add>0)
			{
				for (int j=0;j<add+RowAdd;j++)
					FTemp+=fbuffer[j];
			}
		}
		delete RLEntries;
		RLEntries = NULL;
		if (BottomMargin>0)
		{
			for (int i=0;i<BottomMargin;i++)
			{
				for (int j=0;j<FullWidth+add;j++)
					FTemp+=fbuffer[j];
			}
		}
	}
	if (defType2==2)
	{
		if (add==4)
			add=0;
		if (TopMargin>0)
		{
			for (int i=0;i<TopMargin;i++)
			{
				for (int j=0;j<FullWidth+add;j++)
					FTemp+=fbuffer[j];
			}
		}
		RWEntries = new unsigned int[SpriteHeight];
		for (int i=0;i<SpriteHeight;i++)
		{
			BaseOffset=BaseOffsetor+i*2*(SpriteWidth/32);
			RWEntries[i] = readNormalNr(BaseOffset,2,FDef);
		}
		BaseOffset = BaseOffsetor+RWEntries[0];
		for (int i=0;i<SpriteHeight;i++)
		{
			//BaseOffset = BaseOffsetor+RWEntries[i];
			if (LeftMargin>0)
			{
				for (int j=0;j<LeftMargin;j++)
					FTemp+=fbuffer[j];
			}
			TotalRowLength=0;
			do
			{
				SegmentType=FDef[BaseOffset++];
				unsigned char code = SegmentType / 32;
				unsigned char value = (SegmentType & 31) + 1;
				if(code==7)
				{
					for(int h=0; h<value; ++h)
					{
						FTemp+=FDef[BaseOffset++];
					}
				}
				else
				{
					for(int h=0; h<value; ++h)
					{
						FTemp+=code;
					}
				}
				TotalRowLength+=value;
			} while(TotalRowLength<SpriteWidth);
			if (RightMargin>0)
			{
				for (int j=0;j<RightMargin;j++)
					FTemp+=fbuffer[j];
			}
			if (add>0)
			{
				for (int j=0;j<add+RowAdd;j++)
					FTemp+=fbuffer[j];
			}
		}
		delete RWEntries;
		RWEntries = NULL;
		if (BottomMargin>0)
		{
			for (int i=0;i<BottomMargin;i++)
			{
				for (int j=0;j<FullWidth+add;j++)
					FTemp+=fbuffer[j];
			}
		}
	}
	if (defType2==3)
	{
		if (add==4)
			add=0;
		if (TopMargin>0)
		{
			for (int i=0;i<TopMargin;i++)
			{
				for (int j=0;j<FullWidth+add;j++)
					FTemp+=fbuffer[j];
			}
		}
		RWEntries = new unsigned int[SpriteHeight];
		for (int i=0;i<SpriteHeight;i++)
		{
			BaseOffset=BaseOffsetor+i*2*(SpriteWidth/32);
			RWEntries[i] = readNormalNr(BaseOffset,2,FDef);
		}
		for (int i=0;i<SpriteHeight;i++)
		{
			BaseOffset = BaseOffsetor+RWEntries[i];
			if (LeftMargin>0)
			{
				for (int j=0;j<LeftMargin;j++)
					FTemp+=fbuffer[j];
			}
			TotalRowLength=0;
			do
			{
				SegmentType=FDef[BaseOffset++];
				unsigned char code = SegmentType / 32;
				unsigned char value = (SegmentType & 31) + 1;
				if(code==7)
				{
					for(int h=0; h<value; ++h)
					{
						FTemp+=FDef[BaseOffset++];
					}
				}
				else
				{
					for(int h=0; h<value; ++h)
					{
						FTemp+=code;
					}
				}
				TotalRowLength+=value;
			}while(TotalRowLength<SpriteWidth);
			if (RightMargin>0)
			{
				for (int j=0;j<RightMargin;j++)
					FTemp+=fbuffer[j];
			}
			if (add>0)
			{
				for (int j=0;j<add+RowAdd;j++)
					FTemp+=fbuffer[j];
			}
		}
		delete RWEntries;
		RWEntries=NULL;
		if (BottomMargin>0)
		{
			for (int i=0;i<BottomMargin;i++)
			{
				for (int j=0;j<FullWidth+add;j++)
					FTemp+=fbuffer[j];
			}
		}
	}
	for (int i=0; i<FullHeight; ++i)
	{
		for (int j=0;j<FullWidth+add;j++)
		{
			*((char*)ret->pixels + ret->format->BytesPerPixel * (i*(fullWidth+add) + j)) = FTemp[i*(FullWidth+add)+j];
		}
	}
	SDL_Color ttcol = ret->format->palette->colors[0];
	Uint32 keycol = SDL_MapRGBA(ret->format, ttcol.r, ttcol.b, ttcol.g, ttcol.unused);
	SDL_SetColorKey(ret, SDL_SRCCOLORKEY, keycol);
	return ret;
};

CDefEssential * CDefHandler::essentialize()
{
	CDefEssential * ret = new CDefEssential;
	ret->ourImages = ourImages;
	notFreeImgs = true;
	return ret;
}
