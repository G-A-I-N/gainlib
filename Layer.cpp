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

#include "Layer.h"

namespace Gain {

Layer::Layer() :
	Gain::Base()
{
}

Layer::~Layer() = default;

void Layer::addRenderClient(Gain::Base* aBase)
{
    std::lock_guard<std::mutex> guard(renderClientsLock);
    addClientsFifo.push(aBase);
}
void Layer::removeRenderClient(Gain::Base* aBase)
{
    std::lock_guard<std::mutex> guard(renderClientsLock);
    removeClientsFifo.push(aBase);
}

void Layer::removeAllRenderClients()
{
    // Queue every current client for removal. updateG drains the FIFO and
    // deletes each base (matching removeRenderClient's lifecycle), so this
    // method is consistent with single-removal semantics.
    std::lock_guard<std::mutex> guard(renderClientsLock);
    for (auto* base : renderClients) {
        removeClientsFifo.push(base);
    }
}

void Layer::renderPre() const
{}
void Layer::renderPost() const
{}

void Layer::render() const
{
    renderPre();
    std::set<Gain::Base*, Gain::BaseCompare>::iterator it;
	for (it=renderClients.begin(); it!=renderClients.end(); ++it)
	{
		Base* child = *it;
		if (child->getState() == INITIALIZED) {
			child->render();
		}
	}
    renderPost();
}

void Layer::updateG(float time, float deltaTime)
{

	int startFlags = flags;

	if(flags & (FLAG_DIRTY_PIVOT | FLAG_DIRTY_ROTATION | FLAG_DIRTY_TRANSLATION))
	{
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(pPositionX,pPositionY, 0.0));
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), pAngle, glm::vec3(0.0, 0.0, 1.0)) ;
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.f, CORE->ratio, 1.f));
		glm::mat4 reverse_scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.f, CORE->reversed_ratio, 1.f));
		glm::mat4 pivot = glm::translate(glm::mat4(1.0f), glm::vec3(pPivot[POS_X],pPivot[POS_Y], 0.0));

		pAnim =  translate * scale * pivot * rotate * glm::inverse(pivot) * reverse_scale;
		flags &= 0xffffffff^(FLAG_DIRTY_PIVOT | FLAG_DIRTY_ROTATION | FLAG_DIRTY_TRANSLATION);
		flags |= FLAG_DIRTY_INVERSE | FLAG_DIRTY_ANIM;
	}

	super::updateG(time, deltaTime);

    {
        std::lock_guard<std::mutex> guard(renderClientsLock);
        while (!removeClientsFifo.empty())
        {
            Gain::Base* base = removeClientsFifo.front();
            removeClientsFifo.pop();
            auto it = renderClients.find(base);
            if (it != renderClients.end())
            {
                renderClients.erase(it);
                if (!(base->flags & FLAG_DIRTY_ZORDER))
                {
                    delete base;
                }
            }
        }
        while (!addClientsFifo.empty())
        {
            Gain::Base* base = addClientsFifo.front();
            addClientsFifo.pop();
            renderClients.insert(base);
            base->flags &= 0xffffffff ^ FLAG_DIRTY_ZORDER;
        }
    }

    std::set<Gain::Base*, Gain::BaseCompare>::iterator it;
	for (it=renderClients.begin(); it!=renderClients.end(); ++it)
	{
		Base* child = *it;
		if (child->getState() == NOT_INITIALIZED) {
			child->setupGraphics();
			child->setGlobalAnim(anim);
			child->setGlobalColor(color);
			flags |= child->flags & FLAG_FEATURE_TOUCH_INTERFACE;
		}

		if (startFlags & (FLAG_DIRTY_ROTATION | FLAG_DIRTY_PIVOT | FLAG_DIRTY_TRANSLATION))
		{
			child->setGlobalAnim(anim);
		}
		if (startFlags & FLAG_DIRTY_COLOR)
		{
			child->setGlobalColor(color);
		}
		if (child->flags & FLAG_DIRTY_ZORDER)
		{
			//readd to correct the ZOrder;
			removeRenderClient(child);
			addRenderClient(child);
		}
		child->updateG(time, deltaTime);
	}
}

bool Layer::setupGraphics()
{
	setReady();
	return true;
}
void Layer::invalidate()
{
	super::invalidate();   // reset this layer (pProgram=0, pState=NOT_INITIALIZED)
	std::lock_guard<std::mutex> guard(renderClientsLock);
	for (Gain::Base* child : renderClients) {
		child->invalidate();   // a child Layer recurses on its own lock
	}
}
bool Layer::initVariables()
{
	return true;
}
void Layer::enableAttributes() const
{}
void Layer::disableAttributes() const
{}

TouchState Layer::offerTouch(TouchPoint* aTouchPoint, TouchType aType) {
	TouchState touchState = TOUCH_NOT_CONSUMED;

	std::set<Gain::Base*, Gain::BaseCompare>::reverse_iterator it;
	for (it=renderClients.rbegin(); it!=renderClients.rend(); ++it)
	{
		Base* child = *it;
		if(child->flags & FLAG_FEATURE_TOUCH_INTERFACE)
		{
			touchState = child->offerTouch(aTouchPoint,aType);
			if(touchState == TOUCH_CONSUMED)
			{
				break;
			}
		}
	}
	return touchState;
}



} /* namespace Gain */

