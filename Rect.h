/*
 * Bitmap.h
 *
 *  Created on: Feb 2, 2013
 *      Author: villekankainen
 */

#ifndef RECT_H_
#define RECT_H_

#include "Base.h"

namespace Gain {

class Rect: public Gain::Base {
public:
	Rect(int x, int y, int width, int height, const char* vertexShader=NULL, const char* fragmentShader=NULL);
	Rect(float x, float y, float width, float height, const char* vertexShader=NULL, const char* fragmentShader=NULL);
private:
	void privateConstruct(const char* vertexShader, const char* fragmentShader);
public:
	virtual ~Rect();

	void set(int x, int y, int width, int height);

	void setN(float x, float y, float width, float height);
	void setSizeN(float width, float height);
	void setCenterN(float x, float y);
	void setCornersN(
			float tl_x, float tl_y,
			float tr_x, float tr_y,
			float bl_x, float bl_y,
			float br_x, float br_y
			);

	void setColor(GLfloat color[4]);
	void setColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

	void setRotation(GLfloat angle);

	//Base
	virtual bool setupGraphics();
	virtual void render();
	virtual void updateG(float time, float deltaTime);
	//virtual void mapToGraphics();

protected:
	virtual bool initVariables();
	virtual void enableAttributes();
	virtual void disableAttributes();

//	float pXcenter,pYcenter,pWidth,pHeight;

	GLfloat pAngle;

	GLint attribute_coord2d;
	GLuint vbo_square_vertices;
	GLuint ibo_square_elements;

	glm::mat4 anim;

	GLint uniform_anim;
	GLint uniform_color;

	GLfloat square_vertices[8];
	GLfloat translate[2];

	GLfloat TODO_fix_this_add_memoryleakage;
	GLfloat color[4];

	glm::mat4 translate_mat;
	glm::mat4 translate_parent;
	glm::mat4 scale;
	glm::mat4 rotate;

	const char* pVertexShader;
	const char* pFragmentShader;
};

} /* namespace Gain */
#endif /* RECT_H_ */
