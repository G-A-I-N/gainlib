// Copyright 2014
//   erkki.salonen@tpnet.fi
//   ville.kankainen@gmail.com
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

