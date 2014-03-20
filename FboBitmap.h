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

#ifndef FBOBITMAP_H_
#define FBOBITMAP_H_

#include "Bitmap.h"

class FboBitmap: public Gain::Bitmap
{
private:
	typedef Gain::Bitmap super;

public:
	FboBitmap(int x, int y, int width, int height);
	virtual ~FboBitmap();
	void SetAsActiveFrameBuffer() const;
	virtual bool initVariables();

protected:
	virtual void enableAttributes() const;
	virtual void disableAttributes() const;

protected:
	GLuint fbo, rbo_depth;
	int virgin;

};

#endif /* FBOBITMAP_H_ */
