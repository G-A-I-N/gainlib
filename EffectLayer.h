/*
 * EffectLayer.h
 *
 *  Created on: 11 Nov 2013
 *      Author: w
 */

#ifndef PIECELAYER_H_
#define PIECELAYER_H_

#include "Layer.h"
#include "FboBitmap.h"

class EffectLayer : public Gain::Layer {
private:
	typedef Gain::Layer super;
public:
	EffectLayer();
	virtual ~EffectLayer();

protected:
	virtual void updateG(float sec, float deltaSec);

private:
	virtual void renderPre() const;
	virtual void renderPost() const;
	virtual bool setupGraphics();


	virtual bool initVariables();
	virtual void enableAttributes() const;
	virtual void disableAttributes() const;
private:
	FboBitmap* pFbo[2];
	int pCurrent;

};

#endif /* PIECELAYER_H_ */
