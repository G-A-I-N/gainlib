/*
 * EffectLayer.cpp
 *
 *  Created on: 11 Nov 2013
 *      Author: w
 */

#include "EffectLayer.h"
#include <gain.h>
#include <Core.h>

using namespace Gain;

Rect* clear;

EffectLayer::EffectLayer()
:pCurrent(0)
{
	pFbo[0] = new FboBitmap(0,0,
			CORE->screen_width, CORE->screen_height);
	pFbo[1] = new FboBitmap(0,0,
			CORE->screen_width, CORE->screen_height);

	clear = new Rect(0,0,(int)CORE->screen_width, (int)CORE->screen_height);
	clear->setColor(0,0,0,0.1);
}

EffectLayer::~EffectLayer() {
}

bool EffectLayer::initVariables()
{
	return true;
}

void EffectLayer::enableAttributes() const
{
}

void EffectLayer::disableAttributes() const
{
}


void EffectLayer::renderPre() const
{
	pFbo[(pCurrent)&1]->SetAsActiveFrameBuffer();
	glClearColor(0,0,0,0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//clear->render();

    pFbo[(pCurrent+1)&1]->setColor(0.5,0.5,0.5,0.6);
    pFbo[(pCurrent+1)&1]->render();
	glBlendFunc(GL_ONE,GL_ONE);
}

void EffectLayer::renderPost() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    pFbo[(pCurrent)&1]->setColor(1,1,1,1);
    pFbo[(pCurrent)&1]->render();
}

bool EffectLayer::setupGraphics()
{
	super::setupGraphics();
	pFbo[0]->setupGraphics();
	pFbo[1]->setupGraphics();
	clear->setupGraphics();

	pFbo[(pCurrent)&1]->SetAsActiveFrameBuffer();
	glClearColor(0,0,0,0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    pFbo[(pCurrent+1)&1]->SetAsActiveFrameBuffer();
	glClearColor(0,0,0,0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    return true;
}

void EffectLayer::updateG(float sec, float deltaSec)
{
	super::updateG(sec, deltaSec);
	pFbo[0]->updateG(sec, deltaSec);
	pFbo[1]->updateG(sec, deltaSec);
	clear->updateG(sec,deltaSec);
	++pCurrent;


}

