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
	Button();
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

	virtual Button* setIndex(ButtonIndex aIndex);
	virtual Button* nextIndex() ;
	virtual ButtonIndex getIndex() ;
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
