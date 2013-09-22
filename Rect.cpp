/*
 * Rect.cpp
 *
 *  Created on: Feb 2, 2013
 *      Author: villekankainen
 */

#include "Rect.h"

namespace Gain {

static const char gVertexShader[] =
        "attribute vec2 coord2d;\n"
		"uniform mat4 anim;"
		"void main() {\n"
		"  gl_Position = anim * vec4(coord2d.x, -coord2d.y, 0.0, 1.0);"
		"}\n";

static const char gFragmentShader[] =
        "precision mediump float;\n"
		"uniform vec4 color;"

		"void main(void) {"
		"  gl_FragColor = color;"
		"}"
		;



Rect::Rect(int x, int y, int width, int height, const char* vertexShader, const char* fragmentShader)
{
	privateConstruct(vertexShader, fragmentShader);
	set(x,y,width,height);
}

Rect::Rect(float x, float y, float width, float height, const char* vertexShader, const char* fragmentShader)
{
	privateConstruct(vertexShader, fragmentShader);
	setN(x,y,width,height);
}


void Rect::privateConstruct(const char* aVertexShader, const char* aFragmentShader)
{
	pVertexShader = aVertexShader?aVertexShader:gVertexShader;
	pFragmentShader = aFragmentShader?aFragmentShader:gFragmentShader;
	program = 0;
    // set default to white
    setColor(1.0f,1.0f,1.0f,1.f);
	pAngle=0.f;
}

Rect::~Rect()
{
	/* empty */
}

void Rect::set(int aX, int aY, int aWidth, int aHeight)
{
	setN(
		 -1.f + 2.f*(float)aX/GetCore()->screen_width,
		 -1.f/GetCore()->ratio + 2.f*(float)aY/GetCore()->screen_width,
		 2.f*(float)aWidth/GetCore()->screen_width,
		 2.f*(float)aHeight/GetCore()->screen_width
		 );
}

void Rect::setColor(GLfloat aColor[4])
{
	memcpy(color,aColor, sizeof(color));
}

void Rect::setColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	GLfloat color[4] = {red, green, blue, alpha};
	setColor(color);
}

void Rect::setRotation(GLfloat aAngle)
{
	pAngle = aAngle;
}

//void Rect::mapToGraphics() {
//	Core* core = GetCore();
//
//	float half_width  = pWidth / core->screen_width;
//	float half_height = pHeight / core->screen_width;
//
//
//	//center the rectangle within the GL coordinates bound to be [-1,1] in vertical, and [reversed_screen_ratio,-reversed_screen_ratio] in horizontal
//
//	square_vertices[0] = -half_width;
//	square_vertices[1] = -half_height;
//
//	square_vertices[2] =  half_width;
//	square_vertices[3] = -half_height;
//
//	square_vertices[4] =  half_width;
//	square_vertices[5] =  half_height;
//
//	square_vertices[6] = -half_width;
//	square_vertices[7] =  half_height;
//
//	translate[0] = 2.f*(pX-core->screen_width/2.f)/core->screen_width + half_width;
//
//	translate[1] = -2.f*((pY+pHeight/2.f)-core->screen_height/2.f)/core->screen_height;
//
//}

/*
 * params x,y center of rect
 * params width, height
 */

void Rect::setN(float x, float y, float width, float height)
{
	setSizeN(width,height);
	setCenterN(x + width*0.5f, y + height*0.5f);
}

void Rect::setSizeN(float width, float height)
{
	float half_width=width*0.5f;
	float half_height=height*0.5f;

	square_vertices[0] = -half_width;
	square_vertices[1] = -half_height;
	square_vertices[2] = half_width;
	square_vertices[3] = -half_height;
	square_vertices[4] = half_width;
	square_vertices[5] = half_height;
	square_vertices[6] = -half_width;
	square_vertices[7] = half_height;
}

void Rect::setCenterN(float x, float y)
{
	translate[0] = x;
	translate[1] = -y*GetCore()->ratio;
}

void Rect::setCornersN(float tl_x, float tl_y, float tr_x, float tr_y,
		float bl_x, float bl_y, float br_x, float br_y)
{
	//LOGSCOPE;
	square_vertices[0] = tl_x;
	square_vertices[1] = tl_y;
	square_vertices[2] = tr_x;
	square_vertices[3] = tr_y;
	square_vertices[4] = br_x;
	square_vertices[5] = br_y;
	square_vertices[6] = bl_x;
	square_vertices[7] = bl_y;

	translate[0] = 0;
	translate[1] = 0;
}


bool Rect::initVariables()
{
	const char* attribute_name;
	const char* uniform_name;

	attribute_name = "coord2d";
    attribute_coord2d = GL_EXT_FUNC glGetAttribLocation(program, attribute_name);
	if (attribute_coord2d == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return 0;
	}

	uniform_name = "anim";
    uniform_anim = GL_EXT_FUNC glGetUniformLocation(program, uniform_name);
	if (uniform_anim == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
		return 0;
	}

	uniform_name = "color";
    uniform_color = GL_EXT_FUNC glGetUniformLocation(program, uniform_name);
	if (uniform_color == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
		return 0;
	}

    GL_EXT_FUNC glGenBuffers(1, &vbo_square_vertices);
    GL_EXT_FUNC glBindBuffer(GL_ARRAY_BUFFER, vbo_square_vertices);
    GL_EXT_FUNC glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertices), square_vertices, GL_STATIC_DRAW);


	GLushort square_elements[] = {
			// front
			0, 1, 2,
			2, 3, 0,
	};
    GL_EXT_FUNC glGenBuffers(1, &ibo_square_elements);
    GL_EXT_FUNC glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_square_elements);
    GL_EXT_FUNC glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(square_elements), square_elements, GL_STATIC_DRAW);
	checkGlError("glBufferData");
	return true;
}

bool Rect::setupGraphics() {

	if (!program) {
		program = createProgram(pVertexShader,pFragmentShader);

		if (!program) {
			LOGE("Could not create program.");
			return false;
		}
	}
	initVariables();

	return true;
}

void Rect::updateG(float time, float timeDelta)
{
	translate_mat = glm::translate(glm::mat4(1.0f), glm::vec3(translate[0],translate[1], 0.0));
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.f,GetCore()->ratio, 1.0));
	rotate = glm::rotate(glm::mat4(1.0f), pAngle, glm::vec3(0.0, 0.0, 1.0)) ;

	anim = translate_mat * scale * rotate;

    Base::updateG(time,timeDelta);
}

void Rect::enableAttributes() {

    GL_EXT_FUNC glEnableVertexAttribArray(attribute_coord2d);

    GL_EXT_FUNC glBindBuffer(GL_ARRAY_BUFFER, vbo_square_vertices);
    GL_EXT_FUNC glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertices), square_vertices, GL_STATIC_DRAW);

    GL_EXT_FUNC glVertexAttribPointer(
			attribute_coord2d,   // attribute
			2,                   // number of elements per vertex, here (x,y)
			GL_FLOAT,            // the type of each element
			GL_FALSE,            // take our values as-is
			0,  // next coord3d appears every 6 floats
			0                    // offset of first element
	);

    GL_EXT_FUNC glUniformMatrix4fv(uniform_anim, 1, GL_FALSE, glm::value_ptr(anim));
    GL_EXT_FUNC glUniform4fv(uniform_color, 1, color);

}

void Rect::disableAttributes() {
    GL_EXT_FUNC glDisableVertexAttribArray(attribute_coord2d);
	checkGlError("glDisableVertexAttribArray");
}

void Rect::render() {
    GL_EXT_FUNC glUseProgram(program);
	checkGlError("glUseProgram");

	enableAttributes();
    GL_EXT_FUNC glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_square_elements);
    int size;
    GL_EXT_FUNC glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

	checkGlError("glDrawElements");
	disableAttributes();


}


} /* namespace Gain */
