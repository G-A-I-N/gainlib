/*
 * Bitmap.h
 *
 *  Created on: Feb 2, 2013
 *      Author: villekankainen
 */

#ifndef PNGBITMAP_H_
#define PNGBITMAP_H_

#include <string.h>
#include "Bitmap.h"

namespace Gain {

class PngBitmap: public Gain::Bitmap
{
private:
	typedef Gain::Bitmap super;

public:
	PngBitmap(int x, int y, int width, int height, const char* aPngFile);
	PngBitmap(const char* aPngFile);
	virtual ~PngBitmap();

	void readFile(const char* aPngFile);

	//Base
	virtual void updateG(float time, float timeDelta);

protected:

	virtual bool initVariables();
	virtual void enableAttributes();
	virtual void disableAttributes();

};

} /* namespace Gain */
#endif /* PNGBITMAP_H_ */
