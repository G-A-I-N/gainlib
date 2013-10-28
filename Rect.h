/*
 * Bitmap.h
 *
 *  Created on: Feb 2, 2013
 *      Author: villekankainen
 */

#ifndef RECT_H_
#define RECT_H_

#include <queue>
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

class AnimationContainer
{
public:
	AnimationContainer()
	:elapsedTime(0),time(0),startX(0), startY(0), targetX(0),targetY(0)
	{}

	float elapsedTime, time;

	float startX,startY;
	float targetX, targetY;

};

class Rect: public Gain::Base {
public:
	Rect(int x, int y, int width, int height, const char* vertexShader=NULL, const char* fragmentShader=NULL);
	Rect(float x, float y, float width, float height, const char* vertexShader=NULL, const char* fragmentShader=NULL);
	Rect(const char* vertexShader=NULL, const char* fragmentShader=NULL);
private:
	void privateConstruct(const char* vertexShader, const char* fragmentShader);
public:
	virtual ~Rect();

	void set(int x, int y, int width, int height);
	void setN(float x, float y, float width, float height);

	void setX(int x);
	void setY(int y);

	void setXN(float x);
	void setYN(float y);

	float getXN();
	float getYN();

	void setWidth(int width);
	void setHeight(int height);
	void setWidthN(float width);
	void setHeightN(float height);

	Rect* setSizeN(float width, float height);
	Rect* setPositionN(float x,float y,Placement placement);

	void setPlacement(Placement aPlacement);



	void setCenterN(float x, float y);
	void setCornersN(
			float tl_x, float tl_y,
			float tr_x, float tr_y,
			float bl_x, float bl_y,
			float br_x, float br_y
			);

	void setColor(GLfloat color[4]);
	Rect* setColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

	void setRotation(GLfloat angle);

	//Base
	virtual bool setupGraphics();
	virtual void render();
	virtual void updateG(float time, float deltaTime);

	//virtual void mapToGraphics();

	virtual void addAnimationListener(Base* aListener);
	virtual void moveToN(float targetX, float targetY, float sec);

protected:
	virtual bool initVariables();
	virtual void enableAttributes();
	virtual void disableAttributes();

	virtual void updateAnimation(float sec, float deltaSec);




//	float pXcenter,pYcenter,pWidth,pHeight;

	float pHalfWidth,pHalfHeight;
	float pPositionX, pPositionY;

	Placement pPlacement;

	GLfloat pAngle;

	GLint attribute_coord2d;
	GLuint vbo_square_vertices;
	GLuint ibo_square_elements;

	glm::mat4 anim;

	GLint uniform_anim;
	GLint uniform_color;

	GLfloat square_vertices[8];
	GLfloat trunslate[2];

	GLfloat TODO_fix_this_add_memoryleakage;
	GLfloat color[4];

	glm::mat4 translate_mat;
	glm::mat4 translate_parent;
	glm::mat4 scale;
	glm::mat4 rotate;

	const char* pVertexShader;
	const char* pFragmentShader;

	std::queue<AnimationContainer*> pAnimationList;
	std::map<Base*,Base*> pAnimationListener;
};

} /* namespace Gain */
#endif /* RECT_H_ */
