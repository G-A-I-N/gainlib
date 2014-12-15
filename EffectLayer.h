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

#ifndef EFFECTLAYER_H_
#define EFFECTLAYER_H_

#include "Layer.h"
#include "FboBitmap.h"

namespace Gain {

class EffectLayer : public Gain::Layer {
private:
	typedef Gain::Layer super;
public:
	EffectLayer();
	virtual ~EffectLayer();

protected:
	virtual void updateG(float sec, float deltaSec);

private:
	virtual void renderPre() const;
	virtual void renderPost() const;
	virtual bool setupGraphics();


	virtual bool initVariables();
	virtual void enableAttributes() const;
	virtual void disableAttributes() const;
private:
	FboBitmap* pFbo[2];
	int pCurrent;

};

} //namespace Gain

#endif /* EFFECTLAYER_H_ */
