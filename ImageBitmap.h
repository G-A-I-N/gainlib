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

#ifndef __IMAGEBITMAP_H__
#define __IMAGEBITMAP_H__

#include <string>
#include <map>
#include <functional>
#include "Bitmap.h"

namespace Gain {

class ImageBitmapCacheData
{
public:
	ImageBitmapCacheData(){};
	virtual ~ImageBitmapCacheData(){}

	uint8_t* bitmap;
	int width;
	int height;
	int bitsPerPixel;
	std::string literal;
};

class ImageBitmapCache
{
public:
	ImageBitmapCache(){};
	virtual ~ImageBitmapCache(){};

	ImageBitmapCacheData loadBitmap(std::string filename);
	GLuint openglId(ImageBitmapCacheData* data);
//protected:
	std::map<std::string, ImageBitmapCacheData> pBitmapMap;
	std::map<std::string, unsigned int> pOpenglCache;
};


class ImageBitmap: public Gain::Bitmap
{
private:
	typedef Gain::Bitmap super;

public:
	ImageBitmap(int x, int y, int width, int height, const char* aPngFile);
	ImageBitmap(float x, float y, float width, float height, const char* aPngFile);
	ImageBitmap(const char* aPngFile);
	virtual ~ImageBitmap();

	void readFile(const char* aPngFile);

	//Base
	virtual void updateG(float time, float timeDelta);

protected:

	std::string pPngLiteral;

	virtual bool initVariables();
	virtual void enableAttributes() const;
	virtual void disableAttributes() const;

};

} /* namespace Gain */
#endif /* __IMAGEBITMAP_H__ */
