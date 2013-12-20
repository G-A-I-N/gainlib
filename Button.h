/*
 * Button.h
 *
 *  Created on: 12 Dec 2013
 *      Author: w
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <vector>
#include "Rect.h"

namespace Gain {

typedef enum _ButtonState {
	ButtonDown=0,
	ButtonUp
} ButtonState;

typedef unsigned int ButtonIndex;

class Button: public Gain::Rect, Gain::TouchInterface {
private:
	typedef Gain::Rect super;
public:
	Button(float x, float y, float width, float height);
	virtual ~Button();

	virtual TouchState TouchDown(TouchPoint* aPoint);
	virtual TouchState TouchMove(TouchPoint* aPoint);
	virtual TouchState TouchUp(TouchPoint* aPoint);

	virtual void render() const;
	virtual void updateG(float time, float deltaTime) ;

	virtual bool setupGraphics();
	virtual bool initVariables();
	virtual void enableAttributes() const;
	virtual void disableAttributes() const;

	void addButtonState(Gain::Rect* aRect,  ButtonState aState, ButtonIndex aIndex = 0);
protected:
	int pActiveIndex;
	int pPointerId;
	ButtonState pActiveState;

	glm::mat4 invert_anim;

	LOCK pButtonStateLock;
	std::vector< std::vector<Gain::Rect*> > pButtonFaces;
};

} /* namespace Gain */
#endif /* BUTTON_H_ */
