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
	BaseContainer* container = new BaseContainer();
	container->base = aBase;
	container->state = NOT_INITIALIZED;
    LOCK_ACQUIRE(renderClientsLock);
	LOGSCOPE;
	addClientsFifo.push(container);
	LOCK_RELEASE(renderClientsLock);
}
void Layer::removeRenderClient(Gain::Base* aBase)
{
	BaseContainer* container = new BaseContainer();
	container->base = aBase;
	container->state = NOT_INITIALIZED;
    LOCK_ACQUIRE(renderClientsLock);
	LOGSCOPE;
	removeClientsFifo.push(container);
   	LOCK_RELEASE(renderClientsLock);
}

void Layer::removeAllRenderClients()
{
    LOCK_ACQUIRE(renderClientsLock);
	LOGSCOPE;
	//renderClients.clear();
    LOCK_RELEASE(renderClientsLock);
}

void Layer::render()
{
    LOCK_ACQUIRE(renderClientsLock);
    while(addClientsFifo.size())
    {

    	Gain::Base* base = addClientsFifo.front()->base;
    	addClientsFifo.pop();
    	BaseContainer* container = new BaseContainer();
    	container->base = base;
    	container->state = NOT_INITIALIZED;
    	renderClients.insert(std::pair<Base*,BaseContainer*>(base,container));
    }
    while(removeClientsFifo.size())
    {
    	Gain::Base* base = removeClientsFifo.front()->base;
    	removeClientsFifo.pop();
        std::map<Base*,BaseContainer*>::iterator it =
        		renderClients.find(base);
        if(it != renderClients.end())
        {
        	renderClients.erase(it);
        }
	}
    LOCK_RELEASE(renderClientsLock);

    LOGSCOPE;
    std::map<Base*,BaseContainer*>::iterator it;
	for (it=renderClients.begin(); it!=renderClients.end(); ++it)
	{
		BaseContainer* child = it->second;
		if (child->state != INITIALIZED) {
			child->base->setupGraphics();
		}
		child->base->render();
	}
}

void Layer::updateG(float time, float deltaTime)
{
    LOCK_ACQUIRE(renderClientsLock);
	LOGSCOPE;
    std::map<Base*,BaseContainer*>::iterator it;
	for (it=renderClients.begin(); it!=renderClients.end(); ++it)
	{
		BaseContainer* child = it->second;
		if (child->state == NOT_INITIALIZED) {
			child->base->setupGraphics();
			child->state = INITIALIZED;
		}
		child->base->updateG(time, deltaTime);
	}
    LOCK_RELEASE(renderClientsLock);
}

bool Layer::setupGraphics()
{
	return true;
}
bool Layer::initVariables()
{
	return true;
}
void Layer::enableAttributes()
{

}
void Layer::disableAttributes()
{

}

} /* namespace Gain */
