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

#ifndef BITMAP_H_
#define BITMAP_H_

#include "Rect.h"

namespace Gain {

class Bitmap: public Rect
{
private:
	typedef Gain::Rect super;

public:
	Bitmap(float x, float y, float width, float height, const char* vertex = NULL, const char* fragment = NULL);
	Bitmap(int x, int y, int width, int height, const char* vertex = NULL, const char* fragment = NULL);
	Bitmap(const char* vertex = NULL, const char* fragment = NULL);
	virtual ~Bitmap();

	//Base
	virtual void updateG(float time, float timeDelta);

	//return about of bytes in one bitmap slot
	int getBitmap(int* width, int* height, uint8_t** bitmap);
protected:

	virtual bool initVariables();
	virtual void enableAttributes() const;
	virtual void disableAttributes() const;

	uint8_t* pBitmap;
	int pBitmapWidth;
	int pBitmapHeight;
	//size of the slot in bytes
	int pBitmapSlotSize;


	GLint attribute_texcoord;
	GLuint vbo_square_texcoords;


	GLuint texture_id;
	GLint uniform_mytexture;

	bool updateBitmap;
};

} /* namespace Gain */
#endif /* BITMAP_H_ */
