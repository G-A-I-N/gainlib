// Copyright 2014
//   erkki.salonen@tpnet.fi
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

	/**
	 * Set pixel color and alpha values to specified coordinate.
	 **
	 * \param x 		x-coordinate of pixel.
	 * \param y 		y-coordinate of pixel.
	 * \param alpha		alpha value of pixel.
	 * \param blue		blue component (RGB) of pixel.
	 * \param green 	green component of pixel.
	 * \param red		red component of pixel.
	 */

	void setPixel(int x, int y, char alpha,char blue,char green,char red);

	/**
	 * Set pixel color and alpha values to specified coordinate.
	 **
	 * \param x 		x-coordinate of pixel.
	 * \param y 		y-coordinate of pixel.
	 * \param abgr		32-bit combined value of pixel where alpha resides in most significant byte.
	 */

	void setPixel(int x, int y, unsigned int abgr);

	/**
	 * Get pixel color and alpha values from specified coordinate.
	 **
	 * \param x 		x-coordinate of pixel.
	 * \param y 		y-coorindate of pixel.
	 * \param *alpha	alpha value of checked pixel.
	 * \param *blue		blue component (RGB) of checked pixel.
	 * \param *green 	green component of checked pixel.
	 * \param *red		red component of checked pixel.
	 */

	void getPixel(int x, int y, char *alpha,char *blue,char *green,char *red);

	/**
	 * Get pixel color and alpha values from specified coordinate.
	 **
	 * \param x 		x-coordinate of pixel.
	 * \param y 		y-coorindate of pixel.
	 * \param *abgr		32-bit combined value of pixel where alpha resides in most significant byte.
	 */

	void getPixel(int x, int y, unsigned int *abgr);

	/**
	 * Canvas render function. This is called by Gain framework.
	 **
	 * \param sec elapsed time since start.
	 * \param deltaSec elapsed time last update.
	 *
	 */

	virtual void updateG(float sec,float deltaSec);

	/**
	 * Clear Canvas
	 **
	 */

	virtual void clear();
protected:

	virtual bool initVariables();
	virtual void enableAttributes() const;
	virtual void disableAttributes() const;

    unsigned int pMaxPixelBufferSize;
};

} /* namespace Gain */
#endif /* PIXELCANVAS_H_ */
