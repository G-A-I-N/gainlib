/*
 * Bitmap.h
 *
 *  Created on: Feb 2, 2013
 *      Author: villekankainen
 */

#ifndef PNGBITMAP_H_
#define PNGBITMAP_H_

#include <string>
#include <map>
#include <functional>
#include "Bitmap.h"

namespace Gain {

class BitmapCacheData
{
public:
	BitmapCacheData(){};
	virtual ~BitmapCacheData(){}

	uint8_t* bitmap;
	int width;
	int height;
	int bitsPerPixel;
};

class PngBitmapCache
{
public:
	PngBitmapCache(){};
	virtual ~PngBitmapCache(){};

	BitmapCacheData loadBitmap(std::string filename);
	GLuint openglId(BitmapCacheData* data);
//protected:
	std::map<std::string, BitmapCacheData> pBitmapMap;
	std::map<unsigned long, unsigned int> pOpenglCache;
};


class PngBitmap: public Gain::Bitmap
{
private:
	typedef Gain::Bitmap super;

public:
	PngBitmap(int x, int y, int width, int height, const char* aPngFile);
	PngBitmap(float x, float y, float width, float height, const char* aPngFile);
	PngBitmap(const char* aPngFile);
	virtual ~PngBitmap();

	void readFile(const char* aPngFile);

	//Base
	virtual void updateG(float time, float timeDelta);

protected:

	virtual bool initVariables();
	virtual void enableAttributes() const;
	virtual void disableAttributes() const;

};

} /* namespace Gain */
#endif /* PNGBITMAP_H_ */
