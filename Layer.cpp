/*
 * Layer.cpp
 *
 *  Created on: 7 Sep 2013
 *      Author: w
 */

#include "Layer.h"

namespace Gain {

Layer::Layer() :
	Gain::Base()
{
    LOCK_INIT(renderClientsLock);
}

Layer::~Layer() {
	// TODO Auto-generated destructor stub
}

void Layer::addRenderClient(Gain::Base* aBase)
{
    LOCK_ACQUIRE(renderClientsLock);
	;
	addClientsFifo.push(aBase);
	LOCK_RELEASE(renderClientsLock);
}
void Layer::removeRenderClient(Gain::Base* aBase)
{
    LOCK_ACQUIRE(renderClientsLock);
	;
	removeClientsFifo.push(aBase);
   	LOCK_RELEASE(renderClientsLock);
}

void Layer::removeAllRenderClients()
{
    LOCK_ACQUIRE(renderClientsLock);
	;
	//renderClients.clear();
    LOCK_RELEASE(renderClientsLock);
}

void Layer::renderPre() const
{}
void Layer::renderPost() const
{}

void Layer::render() const
{
    renderPre();
    std::set<Base*>::iterator it;
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
    LOCK_ACQUIRE(renderClientsLock);
    while(addClientsFifo.empty()==false)
    {

    	Gain::Base* base = addClientsFifo.front();
    	addClientsFifo.pop();
    	renderClients.insert(base);
    }
    while(removeClientsFifo.empty()==false)
    {
    	Gain::Base* base = removeClientsFifo.front();
    	removeClientsFifo.pop();
        std::set<Base*>::iterator it =
        		renderClients.find(base);
        if(it != renderClients.end())
        {
        	renderClients.erase(it);
        }
	}
    LOCK_RELEASE(renderClientsLock);

    std::set<Base*>::iterator it;
	for (it=renderClients.begin(); it!=renderClients.end(); ++it)
	{
		Base* child = *it;
		if (child->getState() == NOT_INITIALIZED) {
			child->setupGraphics();
		}
		child->updateG(time, deltaTime);
	}
}

bool Layer::setupGraphics()
{
	setReady();
	return true;
}
bool Layer::initVariables()
{
	return true;
}
void Layer::enableAttributes() const
{
}
void Layer::disableAttributes() const
{
}

} /* namespace Gain */
