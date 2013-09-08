/*
 * Layer.h
 *
 *  Created on: 7 Sep 2013
 *      Author: w
 */

#ifndef LAYER_H_
#define LAYER_H_

#include <gain.h>

namespace Gain {

class Layer :public Gain::Base {
private:
	typedef Gain::Base super;
public:
	Layer();
	virtual ~Layer();

	virtual void render();
	virtual void updateG(float time, float deltaTime) ;

	virtual bool setupGraphics();
	virtual bool initVariables();
	virtual void enableAttributes();
	virtual void disableAttributes();

	void addRenderClient(Gain::Base* aBase);
	void removeRenderClient(Gain::Base* aBase);
protected:
	std::vector<BaseContainer*> renderClients;

    LOCK renderClientsLock;

};

} /* namespace Gain */
#endif /* LAYER_H_ */
