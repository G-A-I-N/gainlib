/*
 * Bitmap.h
 *
 *  Created on: Feb 2, 2013
 *      Author: villekankainen
 */

#ifndef BITMAP_H_
#define BITMAP_H_

#include "Rect.h"

namespace Gain {

class Bitmap: public Gain::Rect
{
private:
	typedef Gain::Rect super;

public:
	Bitmap(float x, float y, float width, float height, const char* vertex = NULL, const char* fragment = NULL);
	Bitmap(int x, int y, int width, int height, const char* vertex = NULL, const char* fragment = NULL);
	virtual ~Bitmap();

	//Base
	virtual void updateG(float time, float timeDelta);

	//return about of bytes in one bitmap slot
	int getBitmap(int* width, int* height, uint8_t** bitmap);
protected:

	virtual bool initVariables();
	virtual void enableAttributes();
	virtual void disableAttributes();

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
