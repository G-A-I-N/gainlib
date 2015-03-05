// Copyright 2015
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

#ifndef GAIN_SLIDER_H_
#define GAIN_SLIDER_H_

#include "Rect.h"

namespace Gain {
class Slider: public Gain::Rect, public Gain::TouchInterface {
private:
	typedef Gain::Rect super;
public:
	Slider(float x, float y, float width, float height);
	Slider();
	virtual ~Slider();

	virtual TouchState TouchDown(TouchPoint* aPoint);
	virtual TouchState TouchMove(TouchPoint* aPoint);
	virtual TouchState TouchUp(TouchPoint* aPoint);

	virtual void render() const;
	virtual void updateG(float time, float deltaTime) ;

	virtual bool setupGraphics();
	virtual bool initVariables();
	virtual void enableAttributes() const;
	virtual void disableAttributes() const;


	virtual float getValue();
	virtual void setMin(float min);
	virtual void setMax(float max);
protected:
	int pPointerId;

	float pCurrentPosition;

	float pMin, pMax;
	glm::mat4 invert_anim;
	Gain::Rect* pSlider;
	Gain::Rect* pSliderBackground;
};

} /* namespace Gain */
#endif /* GAIN_SLIDER_H_ */
