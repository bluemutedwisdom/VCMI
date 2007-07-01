#include "stdafx.h"
#include "CDefHandler.h"
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
	for (int z=0;z<totalBlocks;z++)
	{
		i+=4;
		totalInBlock = readNormalNr(i,4); i+=4;
		for (j=SEntries.size();j<totalEntries+totalInBlock;j++)
			SEntries.push_back(SEntry());
		i+=8;
		for (j=0;j<totalInBlock;j++)
		{
			for (int k=0;k<13;k++) Buffer[k]=FDef[i+k]; 
			i+=13;
			SEntries[totalEntries+j].name=Buffer;
		}
		for (j=0;j<totalInBlock;j++)
		{ 
			SEntries[totalEntries+j].offset = readNormalNr(i,4);
			i+=4;
		}
		totalEntries+=totalInBlock;
	}
	for (j=0;j<totalEntries;j++)
	{
		for (int k=0; k<totalEntries-1;k++)
		{
			if (SEntries[k].offset > SEntries[k+1].offset)
			{
				int temp=SEntries[k].offset;
				SEntries[k].offset = SEntries[k+1].offset;
				SEntries[k+1].offset = temp;
				std::string temp2 = SEntries[k].name;
				SEntries[k].name = SEntries[k+1].name;
				SEntries[k+1].name = temp;
			}
		}
	}
}
unsigned char * CDefHandler::writeNormalNr (int nr, int bytCon)
{
	long long amp = pow((float)256,bytCon-1);
	unsigned char * ret = new unsigned char[bytCon];
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
}

void CDefHandler::getSprite (long SIndex) //procedure GetSprite(SIndex: LongInt; var SOut: PStream);
{
	//std::ostringstream BMP;
	//std::ofstream BMP("testtt.bmp");
	std::ofstream BMP;
	BMP.open("testtt.bmp", std::ios::binary);

	long BaseOffset, 
		SpriteWidth, SpriteHeight, //format sprite'a
		LeftMargin, RightMargin, TopMargin,BottomMargin, //Отступы от края полного изображения
		i, add, FullHeight,FullWidth,
		TotalRowLength, // dlugosc przeczytanego segmentu
		NextSpriteOffset, RowAdd;
	std::ifstream Fdef;
	unsigned char SegmentType, SegmentLength, BL, BR;
	unsigned char * TempDef; //memory

	std::string FTemp;
	

	BaseOffset=SEntries[SIndex].offset;//Запоминаем начальное смещение спрайта
	i=BaseOffset+4;
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
	
	add = (int)(4*(((float)1) - ((int)(((int)((float)FullWidth/(float)4))-((float)FullWidth/(float)4)))));
	/*if (add==4)
		add=0;*/ //moved to defcompression dependent block
	BMP << "BM";
		

	//int tempee2 = readNormalNr(0,4,((unsigned char *)tempee.c_str()));
	print(BMP,tb.fullSize,4);
	BMP << '\0' << '\0' << '\0' << '\0';
	print(BMP,0x436,4);
	print(BMP,0x28,4);
	print(BMP,tb.x,4); //w
	print(BMP,tb.y,4); //h
	print(BMP,1,2); //layers
	print(BMP,8,2); 
	print(BMP,0,4);
	print(BMP,0,4);
	print(BMP,tb.dataSize1,4);
	print(BMP,tb.dataSize2,4); 
	print(BMP,0,4);
	print(BMP,0,4); 
	int BaseOffsetor= BaseOffset = i;

	for (int i=0;i<256;i++)
	{
		print(BMP,palette[i].B,1);
		print(BMP,palette[i].G,1);
		print(BMP,palette[i].R,1);
		print(BMP,palette[i].F,1);
	}

	for (int i=0;i<800;i++)
		fbuffer[i]=0;

	BMP << std::flush;
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
			add=2;
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
					TotalRowLength+=SegmentLength+1;
				}
				else
				{
					for (int k=0;k<SegmentLength+1;k++)
					{
						//FTemp+=FDef[BaseOffset+k];//
						FTemp+='\0';
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
				expand(SegmentType,BL,BR);
				if (BL==0) //transparent
				{
					for (int k=0;k<100;k++)
						fbuffer[k]='\0';
					for (int k=0;k<BR+1;k++)
					{
						FTemp+=fbuffer[k];
					}
					TotalRowLength=TotalRowLength+BR+1;
				}
				if (BL==1) //shadow
				{
					for (int k=0;k<100;k++)
						fbuffer[k]='\1';
					for (int k=0;k<BR+1;k++)
					{
						FTemp+=fbuffer[k];
					}
					TotalRowLength=TotalRowLength+BR+1;
				}
				if (BL==2) //shadow
				{
					for (int k=0;k<100;k++)
						fbuffer[k]='\2';
					for (int k=0;k<BR+1;k++)
					{
						FTemp+=fbuffer[k];
					}
					TotalRowLength=TotalRowLength+BR+1;
				}
				if (BL==3) //shadow
				{
					for (int k=0;k<100;k++)
						fbuffer[k]='\3';
					for (int k=0;k<BR+1;k++)
					{
						FTemp+=fbuffer[k];
					}
					TotalRowLength=TotalRowLength+BR+1;
				}
				if (BL==4) //shadow
				{
					for (int k=0;k<100;k++)
						fbuffer[k]='\4';
					for (int k=0;k<BR+1;k++)
					{
						FTemp+=fbuffer[k];
					}
					TotalRowLength=TotalRowLength+BR+1;
				}
				if (BL==5) //team colour?
				{
					for (int k=0;k<100;k++)
						fbuffer[k]='\5';
					for (int k=0;k<BR+1;k++)
					{
						FTemp+=fbuffer[k];
					}
					TotalRowLength=TotalRowLength+BR+1;
				}
				if (BL==7) //opaque
				{
					for (int k=0;k<BR+1;k++)
					{
						FTemp+=FDef[BaseOffset++];
					}
					TotalRowLength=TotalRowLength+BR+1;
				}
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
		if (BottomMargin>0)
		{
			for (int i=0;i<BottomMargin;i++)
			{
				for (int j=0;j<FullWidth+add;j++)
					FTemp+=fbuffer[j];
			}
		}
	}
	for (int i=1;i<=FullHeight;i++)
	{
		int at = (FullHeight-i)*(FullWidth+add);
		for (int j=0;j<FullWidth+add;j++)
		{
			BMP << FTemp[at+j];
		}
	}
//  for i:=1 to FullHeight do
//  begin
//    FTemp.Seek((FullHeight-i)*(FullWidth+Add),spBegin);
//    Stream2Stream(SOut,FTemp,FullWidth+Add);
//  end;
//  SOut.Seek(0,spBegin);
	BMP.close();
};


