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
	renderClients.push_back(container);
	LOCK_RELEASE(renderClientsLock);
}
void Layer::removeRenderClient(Gain::Base* aBase)
{
    LOCK_ACQUIRE(renderClientsLock);
	for (size_t n = 0; n < renderClients.size(); n++)
		if (aBase == renderClients[n]->base) {
			renderClients.erase(renderClients.begin() + n);
			goto end;
		}

	end :
    	LOCK_RELEASE(renderClientsLock);
}

void Layer::render()
{
    LOCK_ACQUIRE(renderClientsLock);
	for (size_t n = 0; n < renderClients.size(); n++) {
				BaseContainer* child = renderClients[n];
				if (child->state == INITIALIZED) {
					child->base->render();
				}
			}
    LOCK_RELEASE(renderClientsLock);
}

void Layer::updateG(float time, float deltaTime)
{
    LOCK_ACQUIRE(renderClientsLock);
	for (size_t n = 0; n < renderClients.size(); n++) {
		BaseContainer* child = renderClients[n];
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
