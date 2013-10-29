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
	LOGSCOPE;
	addClientsFifo.push(aBase);
	LOCK_RELEASE(renderClientsLock);
}
void Layer::removeRenderClient(Gain::Base* aBase)
{
    LOCK_ACQUIRE(renderClientsLock);
	LOGSCOPE;
	removeClientsFifo.push(aBase);
   	LOCK_RELEASE(renderClientsLock);
}

void Layer::removeAllRenderClients()
{
    LOCK_ACQUIRE(renderClientsLock);
	LOGSCOPE;
	//renderClients.clear();
    LOCK_RELEASE(renderClientsLock);
}

void Layer::render() const
{
    LOGSCOPE;
    std::set<Base*>::iterator it;
	for (it=renderClients.begin(); it!=renderClients.end(); ++it)
	{
		Base* child = *it;
		if (child->getState() == INITIALIZED) {
			child->render();
		}
	}
}

void Layer::updateG(float time, float deltaTime)
{
    LOCK_ACQUIRE(renderClientsLock);
	LOGSCOPE;
    while(addClientsFifo.size())
    {

    	Gain::Base* base = addClientsFifo.front();
    	addClientsFifo.pop();
    	renderClients.insert(base);
    }
    while(removeClientsFifo.size())
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
