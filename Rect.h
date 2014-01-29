/*
 * Bitmap.h
 *
 *  Created on: Feb 2, 2013
 *      Author: villekankainen
 */

#ifndef RECT_H_
#define RECT_H_

#include <set>
#include <map>

#include "Base.h"


namespace Gain {

typedef enum _Placement
{
	PLACEMENT_LEFT   = 0x02,
	PLACEMENT_RIGHT  = 0x01,
	PLACEMENT_TOP    = 0x20,
	PLACEMENT_BOTTOM = 0x10,

	TOP_LEFT      = 0x22,
	TOP_CENTER    = 0x20,
	TOP_RIGHT     = 0x21,

	MID_LEFT      = 0x02,
	MID_CENTER    = 0x00,
	MID_RIGHT     = 0x01,

	BOT_LEFT      = 0x12,
	BOT_CENTER    = 0x10,
	BOT_RIGHT     = 0x11

} Placement;

typedef enum _AnimationType
{
	ANIM_NONE=0,
	ANIM_MOVE,
	ANIM_COLOR,
	ANIM_FADE,
	ANIM_SIZE
} AnimationType;

typedef enum _colorIndex {
	COLOR_RED=0,
	COLOR_GREEN,
	COLOR_BLUE,
	COLOR_APLHA
} ColorIndex;

typedef struct _AnimationContainer
{
public:


	float elapsedTime, time;

	float startX,startY;
	float targetX, targetY;

	float startWidth,startHeight;
	float targetWidth, targetHeight;

	float startColor[4];
	float targetColor[4];
	//Animation
	AnimationType type;
} AnimationContainer;

class Rect: public Gain::Base {
public:
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

	virtual Rect* setXN(float x);
	virtual Rect* setYN(float y);

	float getXN();
	float getYN();

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
	virtual Rect* setWidthN(float width);
	virtual Rect* setHeightN(float height);

	Rect* setSizeN(float width, float height);
	Rect* setPositionN(float x,float y,Placement placement);

	virtual Rect* setPlacement(Placement aPlacement);



	Rect* setCenterN(float x, float y);
	Rect* setCornersN(
			float tl_x, float tl_y,
			float tr_x, float tr_y,
			float bl_x, float bl_y,
			float br_x, float br_y
			);


	Rect* setColor(GLfloat color[4]);
	Rect* setColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	Rect* setAlpha(GLfloat alpha);

	Rect* setRotation(GLfloat angle);

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
protected:
	virtual bool initVariables();
	virtual void enableAttributes() const;
	virtual void disableAttributes() const;

	virtual void updateAnimation(float sec, float deltaSec);




//	float pXcenter,pYcenter,pWidth,pHeight;

	float pHalfWidth,pHalfHeight;
	float pPositionX, pPositionY;

	Placement pPlacement;

	GLfloat pAngle;

	GLint attribute_coord2d;
	GLuint vbo_square_vertices;
	GLuint ibo_square_elements;

public:
	glm::mat4 anim;
	GLfloat square_vertices[8];
	GLfloat color[4];
protected:

	GLint uniform_anim;
	GLint uniform_color;

	GLfloat trunslate[2];

	GLfloat TODO_fix_this_add_memoryleakage;


	glm::mat4 translate_mat;
	glm::mat4 translate_parent;
	glm::mat4 scale;
	glm::mat4 rotate;

	const char* pVertexShader;
	const char* pFragmentShader;

	std::set<AnimationContainer*> pAnimationList;
};

} /* namespace Gain */
#endif /* RECT_H_ */
