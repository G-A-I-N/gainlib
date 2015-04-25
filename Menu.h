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

#ifndef MENU_H_
#define MENU_H_

#include <Layer.h>
#include <TouchInterface.h>
#include <Rect.h>

namespace Gain {

class Menu : public Gain::Layer {

public:
	Menu();
	virtual ~Menu();

	//Touch interface
	virtual TouchState TouchDown(TouchPoint* point);
	virtual TouchState TouchMove(TouchPoint* point);
	virtual TouchState TouchUp(TouchPoint* point);

	virtual void update(float sec, float deltaSec);

	virtual void onEvent(Base*, EventType);
protected:
	int pPointerId,pPointerIdMenu;
	Rect* pMenuBg;
	float pXCorrection;
	bool pMenuVisible;
};

} /* namespace Gain */

#endif /* MENU_H_ */
