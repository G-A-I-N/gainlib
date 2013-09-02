/*
 * Text.h
 *
 *  Created on: Feb 2, 2013
 *      Author: villekankainen
 */

#ifndef TEXT_H_
#define TEXT_H_

#include "Bitmap.h"


#include <ft2build.h>
#include FT_FREETYPE_H

namespace Gain {

class Text :
    public Gain::Bitmap
{
private:
	typedef Gain::Bitmap super;


public:
	Text(float x, float y, float pixelSize, const char* text);
	Text(int x, int y, int pixelSize, const char* text);
	virtual ~Text();

	void setText(const char* text);
	//Base
	virtual void updateG(float time, float timeDelta);

protected:

	virtual bool initVariables();
	virtual void enableAttributes();
	virtual void disableAttributes();

	char* pTextBuffer;
	int pPixelSize;
	FT_Library pFt;
	FT_Face pFace;
};

} /* namespace Gain */
#endif /* TEXT_H_ */
