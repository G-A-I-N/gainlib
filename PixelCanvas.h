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

	/**
	 * Create PixelCanvas to specified location on screen.
	 *
	 *
	 * \param x x-coordinate of canvas start corner.
	 * \param y y-coordinate of canvas start corner.
	 * \param width the width of canvas.
	 * \param height the height of canvas.
	 */

	PixelCanvas(int x, int y, int width, int height);

	/**
	 * Create PixelCanvas with certain width and height
	 **
	 * \param width the width of canvas.
	 * \param height the height of canvas.
	 */

	PixelCanvas(int width, int height);

	virtual ~PixelCanvas();

	void setPixel(int x, int y, char alpha,char blue,char green,char red);
	void setPixel(int x, int y, unsigned int abgr);

	void getPixel(int x, int y, char *alpha,char *blue,char *green,char *red);
	void getPixel(int x, int y, unsigned int *abgr);

	/**
	 * Canvas render function. This is called by Gain framework.
	 **
	 * \param sec elapsed time since start.
	 * \param deltaSec elapsed time last update.
	 */

	virtual void updateG(float sec,float deltaSec);
protected:

	virtual bool initVariables();
	virtual void enableAttributes() const;
	virtual void disableAttributes() const;

    unsigned int pMaxPixelBufferSize;
};

} /* namespace Gain */
#endif /* PIXELCANVAS_H_ */
