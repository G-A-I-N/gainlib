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
    std::multiset<Gain::Base*, Gain::BaseCompare>::iterator it;
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
    while( !addClientsFifo.empty() )
    {

    	Gain::Base* base = addClientsFifo.front();
    	addClientsFifo.pop();
    	renderClients.insert(base);
    }
    while( !removeClientsFifo.empty() )
    {
    	Gain::Base* base = removeClientsFifo.front();
    	removeClientsFifo.pop();
        std::multiset<Gain::Base*, Gain::BaseCompare>::iterator it =
                renderClients.begin();
        while(it != renderClients.end())
        {
            std::multiset<Gain::Base*, Gain::BaseCompare>::iterator it_temp = it;
            ++it;
            if( base == *it_temp) {
                renderClients.erase(it_temp);
            }
        }
	}
    LOCK_RELEASE(renderClientsLock);

    std::multiset<Gain::Base*, Gain::BaseCompare>::iterator it;
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
