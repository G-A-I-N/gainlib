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

typedef enum _AnimationType
{
	ANIM_NONE=0,
	ANIM_MOVE,
	ANIM_COLOR,
	ANIM_FADE,
	ANIM_SIZE
} AnimationType;



typedef struct _AnimationContainer
{
public:


	float elapsedTime, time;

	float startX,startY;
	float targetX, targetY;

	float startWidth,startHeight;
	float targetWidth, targetHeight;

	float startColor[COLOR_SIZE];
	float targetColor[COLOR_SIZE];
	//Animation
	AnimationType type;
} AnimationContainer;

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
	 * Moves target to given coordinates in given time
	 *
	 * Uses Rect's animation framework to move item to specified position in given time.
	 * Is parallel to all other animation that are running.
	 *
	 * Coordinates are given in normalized coordinates.
	 *
	 * \param targetX of target x-coordinate in normalized form.
	 * \param targetY of target y-coordinate in normalized form.
	 * \param sec time (seconds) to spend transition from current pos to new position.
	 */
	virtual Rect* toPositionN(float targetX, float targetY, float sec);

	/**
	 * Fades to target alpha in given time
	 *
	 * Uses Rect's animation framework to fade the alpha value to given level in given time.
	 * Is parallel to all other animation that are running.
	 *
	 * \param aTargetAlpha to fade to
	 * \param sec time (seconds) to spend fading
	 * \return pointer to this object
	 */
	virtual Rect* toAlphaN(float aTargetAplha, float sec);

	/**
	 * Fades to target color in given time
	 *
	 * Uses Rect's animation framework to fade the color value to given level in given time.
	 * Is parallel to all other animation that are running.
	 *
	 * \param r red value to fade to
	 * \param g green value to fade to
	 * \param b blue value to fade to
	 * \param aTargetAlpha to fade to
	 * \param sec time to spend fading
	 * \return pointer to this object
	 */
	virtual Rect* toColorN(float r, float g, float b, float aTargetAplha, float sec);

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


	virtual void cancelAllAnimations();


protected:

	virtual bool initVariables();
	virtual void enableAttributes() const;
	virtual void disableAttributes() const;

	virtual void updateAnimation(float sec, float deltaSec);



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

	std::set<AnimationContainer*> pAnimationList;

	Placement pPlacement;
};

} /* namespace Gain */
#endif /* RECT_H_ */
