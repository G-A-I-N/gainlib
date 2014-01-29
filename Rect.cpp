/*
 * Rect.cpp
 *
 *  Created on: Feb 2, 2013
 *      Author: villekankainen
 */

#include <algorithm>
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
	setPlacement(TOP_LEFT);
}

Rect::Rect(float x, float y, float width, float height, const char* vertexShader, const char* fragmentShader)
{
	privateConstruct(vertexShader, fragmentShader);
	setN(x,y,width,height);
}
Rect::Rect(const char* vertexShader, const char* fragmentShader)
{
	privateConstruct(vertexShader, fragmentShader);
	setN(0,0,0,0);
}

void Rect::privateConstruct(const char* aVertexShader, const char* aFragmentShader)
{
	pVertexShader = aVertexShader?aVertexShader:gVertexShader;
	pFragmentShader = aFragmentShader?aFragmentShader:gFragmentShader;
	program = 0;
    // set default to white
    setColor(1.0f,1.0f,1.0f,1.f);
	pAngle=0.f;
	setPlacement(MID_CENTER);
}

Rect::~Rect()
{
	/* empty */
}


Rect* Rect::setColor(GLfloat aColor[4])
{
	memcpy(color,aColor, sizeof(color));
	return this;
}

Rect* Rect::setColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	GLfloat color[4] = {red, green, blue, alpha};
	setColor(color);
	return this;
}

Rect* Rect::setAlpha(GLfloat alpha)
{
	color[COLOR_APLHA] = alpha;
	return this;
}
Rect* Rect::setRotation(GLfloat aAngle)
{
	pAngle = aAngle;
	return this;
}

float Rect::getRotation()
{
	return pAngle;
}

//void Rect::mapToGraphics() {
//	Core* core = CORE;
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


Rect* Rect::setX(int aX)
{
	setXN(-1.f + 2.f*((float)aX)/CORE->screen_width);
	return this;
}

Rect* Rect::setY(int aY)
{
	 setYN(-1.f*CORE->reversed_ratio + 2.f*((float)aY)/CORE->screen_width);
	 return this;
}

Rect* Rect::setXN(float x)
{
	pPositionX = x;
	return this;
}

Rect* Rect::setYN(float y)
{
	pPositionY = -y*CORE->ratio;
	return this;
}

float Rect::getXN()
{
	return pPositionX;
}

float Rect::getYN()
{
	return -pPositionY*CORE->reversed_ratio;
}

float Rect::getWidthN()
{
	return pHalfWidth*2;
}

float Rect::getHeightN()
{
	return pHalfHeight*2;
}

Rect* Rect::setWidth(int aWidth)
{
	setWidthN(2.f*((float)aWidth)/CORE->screen_width);
	return this;
}
Rect* Rect::setHeight(int aHeight)
{
	setHeightN(2.f*((float)aHeight)/CORE->screen_width);
	return this;
}

Rect* Rect::set(int aX, int aY, int aWidth, int aHeight)
{
	setX(aX);
	setY(aY);
	setWidth(aWidth);
	setHeight(aHeight);

	return this;
}

Rect* Rect::setN(float aX, float aY, float aWidth, float aHeight)
{
	setXN(aX);
	setYN(aY);
	setWidthN(aWidth);
	setHeightN(aHeight);
	return this;
}


Rect* Rect::setWidthN(float width)
{
	pHalfWidth=width*0.5f;
	square_vertices[0] = -pHalfWidth;
	square_vertices[2] = pHalfWidth;
	square_vertices[4] = pHalfWidth;
	square_vertices[6] = -pHalfWidth;

	return this;
}
Rect* Rect::setHeightN(float height)
{
	pHalfHeight=height*0.5f;
	square_vertices[1] = -pHalfHeight;
	square_vertices[3] = -pHalfHeight;
	square_vertices[5] = pHalfHeight;
	square_vertices[7] = pHalfHeight;
	return this;
}


Rect* Rect::setSizeN(float width, float height)
{
	setWidthN(width);
	setHeightN(height);
	return this;
}
Rect* Rect::setPositionN(float x, float y, Placement aPlacement)
{
	setXN(x);
	setYN(y);
	setPlacement(aPlacement);
	return this;
}

Rect* Rect::setPlacement(Placement aPlacement)
{
	pPlacement = aPlacement;
	return this;
}

Rect* Rect::setCenterN(float x, float y)
{
	setXN(x);
	setYN(y);
	setPlacement(MID_CENTER);
	return this;
}

Rect* Rect::setCornersN(float tl_x, float tl_y, float tr_x, float tr_y,
		float bl_x, float bl_y, float br_x, float br_y)
{
	square_vertices[0] = tl_x;
	square_vertices[1] = tl_y;
	square_vertices[2] = tr_x;
	square_vertices[3] = tr_y;
	square_vertices[4] = br_x;
	square_vertices[5] = br_y;
	square_vertices[6] = bl_x;
	square_vertices[7] = bl_y;

	pPositionX = 0;
	pPositionY = 0;
	return this;
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
    setReady();

	return true;
}

void Rect::updateG(float sec, float deltaSec)
{
	updateAnimation(sec,deltaSec);
//	if(updateTranslationMat)
	{
		float translateX = pPositionX;
		float translateY = pPositionY;

		if(pPlacement & (PLACEMENT_LEFT | PLACEMENT_RIGHT))
		{
			translateX += pPlacement & PLACEMENT_RIGHT ? -pHalfWidth : pHalfWidth;
		}
		if(pPlacement & (PLACEMENT_TOP | PLACEMENT_BOTTOM))
		{
			translateY += (pPlacement & PLACEMENT_TOP ? -pHalfHeight : pHalfHeight)*CORE->ratio;
		}

		translate_mat = glm::translate(glm::mat4(1.0f), glm::vec3(translateX,translateY, 0.0));
	}
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.f,CORE->ratio, 1.0));
	rotate = glm::rotate(glm::mat4(1.0f), pAngle, glm::vec3(0.0, 0.0, 1.0)) ;

	anim = translate_mat * scale * rotate;

    Base::updateG(sec,deltaSec);
}

void Rect::enableAttributes() const
{

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

void Rect::disableAttributes() const
{
    GL_EXT_FUNC glDisableVertexAttribArray(attribute_coord2d);
	checkGlError("glDisableVertexAttribArray");
}

void Rect::render() const
{
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

Rect* Rect::toPositionN(float aTargetX, float aTargetY, float sec)
{

	AnimationContainer* anim = new AnimationContainer();
	anim->targetX = aTargetX;
	anim->targetY = aTargetY;
	anim->startX = getXN();
	anim->startY = getYN();
	anim->type = ANIM_MOVE;

	anim->time = sec;

	pAnimationList.insert(anim);
	return this;
}


Rect* Rect::toAlphaN(float aTargetAplha, float sec)
{

	AnimationContainer* anim = new AnimationContainer();
	memcpy(anim->startColor,color,sizeof(color));
	memcpy(anim->targetColor,color,sizeof(color));
	anim->targetColor[COLOR_APLHA] = aTargetAplha;
	anim->type = ANIM_FADE;

	anim->time = sec;

	pAnimationList.insert(anim);
	return this;
}

Rect* Rect::toColorN(float aRed, float aGreen, float aBlue, float aTargetAplha, float sec)
{
	AnimationContainer* anim = new AnimationContainer();
	memcpy(anim->startColor,color,sizeof(color));
	anim->targetColor[COLOR_APLHA] = aTargetAplha;
	anim->targetColor[COLOR_RED] = aRed;
	anim->targetColor[COLOR_GREEN] = aGreen;
	anim->targetColor[COLOR_BLUE] = aBlue;
	anim->type = ANIM_COLOR;

	anim->time = sec;

	pAnimationList.insert(anim);
	return this;
}


Rect* Rect::toSizeN(float aTargetWidth, float aTargetHeight, float sec)
{
	AnimationContainer* anim = new AnimationContainer();
	anim->targetWidth = aTargetWidth;
	anim->targetHeight = aTargetHeight;
	anim->startWidth = getWidthN();
	anim->startHeight = getHeightN();
	anim->type = ANIM_SIZE;

	anim->time = sec;

	pAnimationList.insert(anim);
	return this;
}


void Rect::updateAnimation(float sec, float deltaSec)
{
	std::set<AnimationContainer*>::iterator it = pAnimationList.begin();
	while(it!=pAnimationList.end())
	{
		AnimationContainer* anim = *it;
	    anim->elapsedTime += deltaSec;

	    float currentPosition = std::min(1.f, anim->elapsedTime / anim->time);

	    if(ANIM_MOVE == anim->type)
	    {
			if(anim->startX != anim->targetX){
				setXN(anim->startX + (anim->targetX - anim->startX)*currentPosition);
			}
			if(anim->startY != anim->targetY){
				setYN(anim->startY + (anim->targetY - anim->startY)*currentPosition);
			}
	    }
	    if(ANIM_COLOR == anim->type)
	    {
	    	for(unsigned int color_i=COLOR_RED;color_i<=COLOR_APLHA;++color_i)
	    	{
	    		color[color_i] = anim->startColor[color_i] + (anim->targetColor[color_i] - anim->startColor[color_i])*currentPosition;
	    	}
	    }
	    if(ANIM_FADE == anim->type)
	    {
	    	ColorIndex color_i=COLOR_APLHA;
	    	color[color_i] = anim->startColor[color_i] + (anim->targetColor[color_i] - anim->startColor[color_i])*currentPosition;
	    }
	    if(ANIM_SIZE == anim->type)
	    {

			if(anim->startWidth != anim->targetWidth){
				setWidthN(anim->startWidth + (anim->targetWidth - anim->startWidth)*currentPosition);
			}
			if(anim->startHeight != anim->targetHeight){
				setHeightN(anim->startHeight + (anim->targetHeight - anim->startHeight)*currentPosition);
			}
	    }

	    std::set<AnimationContainer*>::iterator delete_it = it;
	    it++;

	    if(currentPosition == 1.f)
	    {
		    triggerEvent(EVENT_ANIMATION_FINISHED);
		    pAnimationList.erase(delete_it);
	    	delete anim;
	    	anim=0;
	    }
	}
}

bool Rect::isWithin(float Xn, float Yn)
{
	float yInGl = -Yn*CORE->ratio;
	//TODO: currently works only on centered rects
	return (Xn < (pPositionX+pHalfWidth))  &&
		   (Xn > (pPositionX-pHalfWidth))  &&
		   (yInGl < (pPositionY+pHalfHeight)) &&
		   (yInGl > (pPositionY-pHalfHeight));
}

} /* namespace Gain */
