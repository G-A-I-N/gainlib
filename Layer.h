// Copyright 2014
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

#ifndef LAYER_H_
#define LAYER_H_

#include <queue>
#include <set>
#include <Base.h>

namespace Gain {

class Layer :public Gain::Base {
private:
	typedef Gain::Base super;
public:
	Layer();
	virtual ~Layer();

	virtual void renderPre() const;
	virtual void renderPost() const;
	virtual void render() const;
	virtual void updateG(float time, float deltaTime) ;

	virtual bool setupGraphics();
	virtual bool initVariables();
	virtual void enableAttributes() const;
	virtual void disableAttributes() const;

	void addRenderClient(Gain::Base* aBase);
	void removeRenderClient(Gain::Base* aBase);
	void removeAllRenderClients();
protected:
	std::set<Gain::Base*> renderClients;
	std::queue<Gain::Base*> addClientsFifo;
	std::queue<Gain::Base*> removeClientsFifo;

    LOCK renderClientsLock;

};

} /* namespace Gain */
#endif /* LAYER_H_ */
