/*
 * FboBitmap.h
 *
 *  Created on: 11 Nov 2013
 *      Author: w
 */

#ifndef FBOBITMAP_H_
#define FBOBITMAP_H_

#include "Bitmap.h"

class FboBitmap: public Gain::Bitmap
{
private:
	typedef Gain::Bitmap super;

public:
	FboBitmap(int x, int y, int width, int height);
	virtual ~FboBitmap();
	void SetAsActiveFrameBuffer() const;
	virtual bool initVariables();

protected:
	virtual void enableAttributes() const;
	virtual void disableAttributes() const;

protected:
	GLuint fbo, rbo_depth;
	int virgin;

};

#endif /* FBOBITMAP_H_ */
