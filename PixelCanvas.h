/*
 * Bitmap.h
 *
 *  Created on: Feb 2, 2013
 *      Author: villekankainen
 */

#ifndef PIXELCANVAS_H_
#define PIXELCANVAS_H_

#include "Bitmap.h"

namespace Gain {

class PixelCanvas:
    public Gain::Bitmap
{
private:
	typedef Gain::Bitmap super;

public:
	PixelCanvas(int x, int y, int width, int height);
	PixelCanvas(int width, int height);
	virtual ~PixelCanvas();

	virtual void updateG(float,float);

	void setPixel(int x, int y, char alpha,char blue,char green,char red);
	void setPixel(int x, int y, unsigned int abgr);

	void getPixel(int x, int y, char *alpha,char *blue,char *green,char *red);
	void getPixel(int x, int y, unsigned int *abgr);
protected:

	virtual bool initVariables();
	virtual void enableAttributes() const;
	virtual void disableAttributes() const;

    unsigned int pMaxPixelBufferSize;
};

} /* namespace Gain */
#endif /* PIXELCANVAS_H_ */
