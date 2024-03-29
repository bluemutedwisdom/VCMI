#ifndef __SDL_EXTENSIONS_H__
#define __SDL_EXTENSIONS_H__
#include "../global.h"
#include "SDL_video.h"
#include "SDL_ttf.h"
#include <string>
#include <vector>
#include <sstream>
#include "FontBase.h"

/*
 * SDL_Extensions.h, part of VCMI engine
 *
 * Authors: listed in file AUTHORS in main folder
 *
 * License: GNU General Public License v2.0 or later
 * Full text of license available in license.txt file, in main folder
 *
 */


//A macro to force inlining some of our functions. Compiler (at least MSVC) is not so smart here-> without that displaying is MUCH slower
#ifdef _MSC_VER
	#define STRONG_INLINE __forceinline
#elif __GNUC__
	#define STRONG_INLINE __attribute__((always_inline))
#else
	#define STRONG_INLINE inline
#endif

struct Rect;

extern SDL_Surface * screen, *screen2, *screenBuf;
extern SDL_Color tytulowy, tlo, zwykly ;
void blitAt(SDL_Surface * src, int x, int y, SDL_Surface * dst=screen);
void blitAt(SDL_Surface * src, const SDL_Rect & pos, SDL_Surface * dst=screen);
void updateRect (SDL_Rect * rect, SDL_Surface * scr = screen);
bool isItIn(const SDL_Rect * rect, int x, int y);

template<typename IntType>
std::string makeNumberShort(IntType number) //the output is a string containing at most 5 characters [4 if positive] (eg. intead 10000 it gives 10k)
{
	int initialLength;
	bool negative = (number < 0);
	std::ostringstream ost, rets;
	ost<<number;
	initialLength = ost.str().size();

	if(negative)
	{
		if(initialLength <= 4)
			return ost.str();
	}
	else
	{
		if(initialLength <= 5)
			return ost.str();
	}

	//make the number short
	char symbol[] = {'G', 'M', 'k'};

	if(negative) number = (-number); //absolute value

	for(int divisor = 1000000000, it = 0; divisor > 1; divisor /= 1000, ++it)
	{
		if(number >= divisor)
		{
			if(negative) rets <<'-';
			rets << (number / divisor) << symbol[it];
			return rets.str();
		}
	}

	throw std::string("We shouldn't be here - makeNumberShort");
}

typedef void (*TColorPutter)(Uint8 *&ptr, const Uint8 & R, const Uint8 & G, const Uint8 & B);
typedef void (*TColorPutterAlpha)(Uint8 *&ptr, const Uint8 & R, const Uint8 & G, const Uint8 & B, const Uint8 & A);


inline SDL_Rect genRect(const int & hh, const int & ww, const int & xx, const int & yy)
{
	SDL_Rect ret;
	ret.h=hh;
	ret.w=ww;
	ret.x=xx;
	ret.y=yy;
	return ret;
}

//TODO: inlining will work only if functions are defined in each compilation unit (like placing them in headers)
//however here PutColor defined in SDL_Extensions.cpp, but used in CAnimation.cpp and CCreatureAnimation.cpp
template<int bpp, int incrementPtr>
struct ColorPutter
{
	static STRONG_INLINE void PutColor(Uint8 *&ptr, const Uint8 & R, const Uint8 & G, const Uint8 & B);
	static STRONG_INLINE void PutColor(Uint8 *&ptr, const Uint8 & R, const Uint8 & G, const Uint8 & B, const Uint8 & A);
	static STRONG_INLINE void PutColorAlphaSwitch(Uint8 *&ptr, const Uint8 & R, const Uint8 & G, const Uint8 & B, const Uint8 & A);
	static STRONG_INLINE void PutColor(Uint8 *&ptr, const SDL_Color & Color);
	static STRONG_INLINE void PutColorAlpha(Uint8 *&ptr, const SDL_Color & Color);
	static STRONG_INLINE void PutColorRow(Uint8 *&ptr, const SDL_Color & Color, size_t count);
};

template <int incrementPtr>
struct ColorPutter<2, incrementPtr>
{
	static STRONG_INLINE void PutColor(Uint8 *&ptr, const Uint8 & R, const Uint8 & G, const Uint8 & B);
	static STRONG_INLINE void PutColor(Uint8 *&ptr, const Uint8 & R, const Uint8 & G, const Uint8 & B, const Uint8 & A);
	static STRONG_INLINE void PutColorAlphaSwitch(Uint8 *&ptr, const Uint8 & R, const Uint8 & G, const Uint8 & B, const Uint8 & A);
	static STRONG_INLINE void PutColor(Uint8 *&ptr, const SDL_Color & Color);
	static STRONG_INLINE void PutColorAlpha(Uint8 *&ptr, const SDL_Color & Color);
	static STRONG_INLINE void PutColorRow(Uint8 *&ptr, const SDL_Color & Color, size_t count);
};

typedef void (*BlitterWithRotationVal)(SDL_Surface *src,SDL_Rect srcRect, SDL_Surface * dst, SDL_Rect dstRect, ui8 rotation);

namespace CSDL_Ext
{
	void blitSurface(SDL_Surface * src, SDL_Rect * srcRect, SDL_Surface * dst, SDL_Rect * dstRect);
	void fillRect(SDL_Surface *dst, SDL_Rect *dstrect, Uint32 color);
	extern SDL_Surface * std32bppSurface;

	void SDL_PutPixelWithoutRefresh(SDL_Surface *ekran, const int & x, const int & y, const Uint8 & R, const Uint8 & G, const Uint8 & B, Uint8 A = 255);
	void SDL_PutPixelWithoutRefreshIfInSurf(SDL_Surface *ekran, const int & x, const int & y, const Uint8 & R, const Uint8 & G, const Uint8 & B, Uint8 A = 255);

	SDL_Surface * rotate01(SDL_Surface * toRot); //vertical flip
	SDL_Surface * hFlip(SDL_Surface * toRot); //horizontal flip
	SDL_Surface * rotate02(SDL_Surface * toRot); //rotate 90 degrees left
	SDL_Surface * rotate03(SDL_Surface * toRot); //rotate 180 degrees
	SDL_Cursor * SurfaceToCursor(SDL_Surface *image, int hx, int hy); //creates cursor from bitmap
	Uint32 SDL_GetPixel(SDL_Surface *surface, const int & x, const int & y, bool colorByte = false);
	SDL_Color SDL_GetPixelColor(SDL_Surface *surface, int x, int y); //returns color of pixel at given position
	void alphaTransform(SDL_Surface * src); //adds transparency and shadows (partial handling only; see examples of using for details)
	bool isTransparent(SDL_Surface * srf, int x, int y); //checks if surface is transparent at given position


	Uint8 *getPxPtr(const SDL_Surface * const &srf, const int & x, const int & y);
	const TColorPutter getPutterFor(SDL_Surface  * const &dest, int incrementing); //incrementing: -1, 0, 1
	const TColorPutterAlpha getPutterAlphaFor(SDL_Surface  * const &dest, int incrementing); //incrementing: -1, 0, 1
	BlitterWithRotationVal getBlitterWithRotation(SDL_Surface *dest);
	BlitterWithRotationVal getBlitterWithRotationAndAlpha(SDL_Surface *dest);

	template<int bpp> void blitWithRotateClip(SDL_Surface *src,SDL_Rect * srcRect, SDL_Surface * dst, SDL_Rect * dstRect, ui8 rotation);//srcRect is not used, works with 8bpp sources and 24bpp dests preserving clip_rect
	template<int bpp> void blitWithRotateClipVal(SDL_Surface *src,SDL_Rect srcRect, SDL_Surface * dst, SDL_Rect dstRect, ui8 rotation);//srcRect is not used, works with 8bpp sources and 24bpp dests preserving clip_rect

	template<int bpp> void blitWithRotateClipWithAlpha(SDL_Surface *src,SDL_Rect * srcRect, SDL_Surface * dst, SDL_Rect * dstRect, ui8 rotation);//srcRect is not used, works with 8bpp sources and 24bpp dests preserving clip_rect
	template<int bpp> void blitWithRotateClipValWithAlpha(SDL_Surface *src,SDL_Rect srcRect, SDL_Surface * dst, SDL_Rect dstRect, ui8 rotation);//srcRect is not used, works with 8bpp sources and 24bpp dests preserving clip_rect

	template<int bpp> void blitWithRotate1(const SDL_Surface *src, const SDL_Rect * srcRect, SDL_Surface * dst, const SDL_Rect * dstRect);//srcRect is not used, works with 8bpp sources and 24bpp dests
	template<int bpp> void blitWithRotate2(const SDL_Surface *src, const SDL_Rect * srcRect, SDL_Surface * dst, const SDL_Rect * dstRect);//srcRect is not used, works with 8bpp sources and 24bpp dests
	template<int bpp> void blitWithRotate3(const SDL_Surface *src, const SDL_Rect * srcRect, SDL_Surface * dst, const SDL_Rect * dstRect);//srcRect is not used, works with 8bpp sources and 24bpp dests
	template<int bpp> void blitWithRotate1WithAlpha(const SDL_Surface *src, const SDL_Rect * srcRect, SDL_Surface * dst, const SDL_Rect * dstRect);//srcRect is not used, works with 8bpp sources and 24bpp dests
	template<int bpp> void blitWithRotate2WithAlpha(const SDL_Surface *src, const SDL_Rect * srcRect, SDL_Surface * dst, const SDL_Rect * dstRect);//srcRect is not used, works with 8bpp sources and 24bpp dests
	template<int bpp> void blitWithRotate3WithAlpha(const SDL_Surface *src, const SDL_Rect * srcRect, SDL_Surface * dst, const SDL_Rect * dstRect);//srcRect is not used, works with 8bpp sources and 24bpp dests

	template<int bpp>
	int blit8bppAlphaTo24bppT(const SDL_Surface * src, const SDL_Rect * srcRect, SDL_Surface * dst, SDL_Rect * dstRect); //blits 8 bpp surface with alpha channel to 24 bpp surface
	int blit8bppAlphaTo24bpp(const SDL_Surface * src, const SDL_Rect * srcRect, SDL_Surface * dst, SDL_Rect * dstRect); //blits 8 bpp surface with alpha channel to 24 bpp surface
	Uint32 colorToUint32(const SDL_Color * color); //little endian only

	void printAtWB(const std::string & text, int x, int y, EFonts font, int charpr, SDL_Color kolor=zwykly, SDL_Surface * dst=screen);
	void printAt(const std::string & text, int x, int y, EFonts font, SDL_Color kolor=zwykly, SDL_Surface * dst=screen);
	void printTo(const std::string & text, int x, int y, EFonts font, SDL_Color kolor=zwykly, SDL_Surface * dst=screen);
	void printAtMiddle(const std::string & text, int x, int y, EFonts font, SDL_Color kolor=zwykly, SDL_Surface * dst=screen);
	void printAtMiddleWB(const std::string & text, int x, int y, EFonts font, int charpr, SDL_Color kolor=tytulowy, SDL_Surface * dst=screen);

	void update(SDL_Surface * what = screen); //updates whole surface (default - main screen)
	void drawBorder(SDL_Surface * sur, int x, int y, int w, int h, const int3 &color);
	void drawBorder(SDL_Surface * sur, const SDL_Rect &r, const int3 &color);
	void drawDashedBorder(SDL_Surface * sur, const Rect &r, const int3 &color);
	void setPlayerColor(SDL_Surface * sur, unsigned char player); //sets correct color of flags; -1 for neutral
	std::string processStr(std::string str, std::vector<std::string> & tor); //replaces %s in string
	SDL_Surface * newSurface(int w, int h, SDL_Surface * mod=screen); //creates new surface, with flags/format same as in surface given
	SDL_Surface * copySurface(SDL_Surface * mod); //returns copy of given surface
	void VflipSurf(SDL_Surface * surf); //fluipis given surface by vertical axis
	void applyEffect(SDL_Surface * surf, const SDL_Rect * rect, int mode); //mode: 0 - sepia, 1 - grayscale
};


#endif // __SDL_EXTENSIONS_H__
