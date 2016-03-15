// Copyright 2014
//   erkki.salonen@tpnet.fi
//   ville.kankainen@gmail.com
//   ville.kankainen@kangain.com
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

#ifndef RECT_H_
#define RECT_H_

#include <set>
#include <map>

#include "Base.h"


//Square vertices positions
typedef enum _SV_POS
{
	SV_TL_X=0,
	SV_TL_Y,
	SV_TR_X,
	SV_TR_Y,
	SV_BR_X,
	SV_BR_Y,
	SV_BL_X,
	SV_BL_Y,
	SV_SIZE
}SV_POS;

namespace Gain {

typedef enum _Placement
{
	PLACEMENT_NONE    = 0x00,
	PLACEMENT_LEFT    = 0x02,
	PLACEMENT_RIGHT   = 0x01,
	PLACEMENT_CENTER  = 0x04,
	PLACEMENT_TOP     = 0x20,
	PLACEMENT_BOTTOM  = 0x10,
	PLACEMENT_MIDDLE  = 0x40,

	TOP_LEFT      = PLACEMENT_TOP | PLACEMENT_LEFT ,
	TOP_CENTER    = PLACEMENT_TOP | PLACEMENT_CENTER,
	TOP_RIGHT     = PLACEMENT_TOP | PLACEMENT_RIGHT,

	MID_LEFT      = PLACEMENT_MIDDLE | PLACEMENT_LEFT,
	MID_CENTER    = PLACEMENT_MIDDLE | PLACEMENT_CENTER,
	MID_RIGHT     = PLACEMENT_MIDDLE | PLACEMENT_RIGHT,

	BOT_LEFT      = PLACEMENT_BOTTOM | PLACEMENT_LEFT,
	BOT_CENTER    = PLACEMENT_BOTTOM | PLACEMENT_CENTER,
	BOT_RIGHT     = PLACEMENT_BOTTOM | PLACEMENT_RIGHT

} Placement;

typedef struct _AnimationContainerRect : public AnimationContainer
{
	float startWidth,startHeight;
	float targetWidth, targetHeight;
} AnimationContainerRect;

class Rect: public Gain::Base {
private:
	typedef Gain::Base super;
public:

	/**
	 * Create rectangle item.
	 *
	 *
	 *
	 * \param x x-coordinate of rect handle. Handle is set using setPlacement.
	 * \param x y-coordinate of rect handle.
	 * \widht Rect width in pixels.
	 * \heigh Rect height in pixels.
	 * \vertexShader Pointer to possible vertexShader. If null, default is used.
	 * \fragmentShader Pointer to possible fragmentShader. If null, default is used.
	 */

	Rect(int x, int y, int width, int height, const char* vertexShader=NULL, const char* fragmentShader=NULL);
	Rect(float x, float y, float width, float height, const char* vertexShader=NULL, const char* fragmentShader=NULL);
	Rect(const char* vertexShader=NULL, const char* fragmentShader=NULL);
private:
	void privateConstruct(const char* vertexShader, const char* fragmentShader);
public:
	virtual ~Rect();

	Rect* set(int x, int y, int width, int height);
	virtual Rect* setN(float x, float y, float width, float height);

	Rect* setX(int x);
	Rect* setY(int y);

	float getWidthN();
	float getHeightN();

	/*
	 * Returns current angle in degrees
	 *
	 *\return angle in degrees
	 */
	float getRotation();


	Rect* setWidth(int width);
	Rect* setHeight(int height);
	Rect* setWidthN(float width);
	Rect* setHeightN(float height);

	Rect* setSizeN(float width, float height);
	virtual Rect* setPositionN(float x,float y,Placement placement);

	virtual Rect* setPlacement(Placement aPlacement);



	Rect* setCenterN(float x, float y);
	Rect* setCornersN(
			float tl_x, float tl_y,
			float tr_x, float tr_y,
			float bl_x, float bl_y,
			float br_x, float br_y
			);




	//Base
	virtual bool setupGraphics();
	virtual void render() const;
	virtual void updateG(float time, float deltaTime);

	/**
	 * Animates to target width and height in given time
	 *
	 * Uses Rect's animation framework to animate current size to given size in given time.
	 * Is parallel to all other animation that are running.
	 *
	 * \param aTargetWidth to animate to
	 * \param aTargetHeight to animate to
	 * \param sec time to spend fading
	 * \return pointer to this object
	 */
	virtual Rect* toSizeN(float aTargetWidth, float aTargetHeight, float sec);

	/**
	 * Checks if rect overlaps given coordinate. If yes, returns true.
	 *
	 * API function for checking object overlaps specified position.
	 *
	 *
	 * \param nX as normalized x-coordinate.
	 * \param nY as normalized y-coordinate.
	 */
	virtual bool isWithin(float nX, float nY);

protected:

	virtual bool initVariables();
	virtual void enableAttributes() const;
	virtual void disableAttributes() const;

	virtual void updateAnimationPart(float currentPosition, AnimationContainer* anim);
public:

	GLfloat pSquareVertices[SV_SIZE];

protected:
	float pWidth,pHeight;

	GLint attribute_coord2d;
	GLuint vbo_square_vertices;
	GLuint ibo_square_elements;

	GLint uniform_anim;
	GLint uniform_color;

	GLfloat trunslate[2];

	GLfloat TODO_fix_this_add_memoryleakage;

	const char* pVertexShader;
	const char* pFragmentShader;

	Placement pPlacement;
};

} /* namespace Gain */
#endif /* RECT_H_ */
