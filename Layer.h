/*
 * Layer.h
 *
 *  Created on: 7 Sep 2013
 *      Author: w
 */

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
