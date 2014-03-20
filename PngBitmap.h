// Copyright 2014
//   ville.kankainen@gmail.com
//   ville.kankainen@kangain.com
// 
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
