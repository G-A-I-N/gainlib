// Copyright 2014
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
	flags = 0x0;
    // set default to white
    setColor(1.0f,1.0f,1.0f,1.f);
	pAngle=0.f;
	setPlacement(MID_CENTER);
}

Rect::~Rect()
{
	/* empty */
}



float Rect::getRotation()
{
	return pAngle;
}


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

float Rect::getWidthN()
{
	return pWidth;
}

float Rect::getHeightN()
{
	return pHeight;
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
	pWidth=width;
	if(pPlacement) {
		setPlacement(pPlacement);
	} else {
		flags |= FLAG_DIRTY_PIVOT;
	}
	return this;
}

Rect* Rect::setHeightN(float height)
{
	pHeight=height;
	if(pPlacement) {
		setPlacement(pPlacement);
	} else {
		flags |= FLAG_DIRTY_PIVOT;
	}
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
	float newX, newY;

	if(pPlacement & PLACEMENT_RIGHT )
		newX = pWidth;
	if(pPlacement & PLACEMENT_CENTER )
		newX = pWidth*0.5f;
	if(pPlacement & PLACEMENT_LEFT )
		newX = 0.f;

	if(pPlacement & PLACEMENT_BOTTOM)
		newY = pHeight;
	if(pPlacement & PLACEMENT_MIDDLE)
		newY = pHeight*0.5f;
	if(pPlacement & PLACEMENT_TOP)
		newY= 0.f;

	setPivot(newX, newY);

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
	pSquareVertices[SV_TL_X] = tl_x;
	pSquareVertices[SV_TL_Y] = tl_y;
	pSquareVertices[SV_TR_X] = tr_x;
	pSquareVertices[SV_TR_Y] = tr_y;
	pSquareVertices[SV_BR_X] = br_x;
	pSquareVertices[SV_BR_Y] = br_y;
	pSquareVertices[SV_BL_X] = bl_x;
	pSquareVertices[SV_BL_Y] = bl_y;

	flags |= FLAG_DIRTY_VERTICES;

	pPositionX = 0;
	pPositionY = 0;
	pAngle = 0;
	pPlacement = PLACEMENT_NONE;
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
    GL_EXT_FUNC glBufferData(GL_ARRAY_BUFFER, sizeof(pSquareVertices), pSquareVertices, GL_STATIC_DRAW);


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



	if(flags & FLAG_DIRTY_PIVOT)
	{
		// calculate pivot, pos 0, width 1, pivot 0.5 -> tl_x = -0.5, tr_x = 0.5
		pSquareVertices[SV_BL_X] =
		pSquareVertices[SV_TL_X] = -pPivot[POS_X];

		pSquareVertices[SV_TR_Y] =
		pSquareVertices[SV_TL_Y] = -pPivot[POS_Y];

		pSquareVertices[SV_BR_X] =
		pSquareVertices[SV_TR_X] = pWidth - pPivot[POS_X];

		pSquareVertices[SV_BL_Y] =
		pSquareVertices[SV_BR_Y] = pHeight - pPivot[POS_Y];

		flags ^= FLAG_DIRTY_PIVOT;
		flags |= FLAG_DIRTY_VERTICES;
	}
	if(flags & (FLAG_DIRTY_VERTICES | FLAG_DIRTY_ROTATION | FLAG_DIRTY_TRANSLATION))
	{
		glm::mat4 translate_mat = glm::translate(glm::mat4(1.0f), glm::vec3(pPositionX,pPositionY, 0.0));
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.f,CORE->ratio, 1.0));
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), pAngle, glm::vec3(0.0, 0.0, 1.0)) ;

		pAnim = translate_mat * scale * rotate;
		flags &= 0xffffffff^(FLAG_DIRTY_VERTICES | FLAG_DIRTY_ROTATION | FLAG_DIRTY_TRANSLATION);
		flags |= FLAG_DIRTY_INVERSE;
	}

    super::updateG(sec,deltaSec);
}

void Rect::enableAttributes() const
{

    GL_EXT_FUNC glEnableVertexAttribArray(attribute_coord2d);

    GL_EXT_FUNC glBindBuffer(GL_ARRAY_BUFFER, vbo_square_vertices);
    GL_EXT_FUNC glBufferData(GL_ARRAY_BUFFER, sizeof(pSquareVertices), pSquareVertices, GL_STATIC_DRAW);

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

Rect* Rect::toSizeN(float aTargetWidth, float aTargetHeight, float sec)
{
	AnimationContainerRect* anim = new AnimationContainerRect();
	anim->targetWidth = aTargetWidth;
	anim->targetHeight = aTargetHeight;
	anim->startWidth = getWidthN();
	anim->startHeight = getHeightN();
	anim->type = ANIM_RECT_SIZE;

	anim->time = sec;

	pAnimationList.insert(anim);
	return this;
}

void Rect::updateAnimationPart(float currentPosition, AnimationContainer* anim) {
    if(ANIM_RECT_SIZE == anim->type)
    {
    	AnimationContainerRect* animRect = (AnimationContainerRect*)anim;
		if(animRect->startWidth != animRect->targetWidth){
			setWidthN(animRect->startWidth + (animRect->targetWidth - animRect->startWidth)*currentPosition);
		}
		if(animRect->startHeight != animRect->targetHeight){
			setHeightN(animRect->startHeight + (animRect->targetHeight - animRect->startHeight)*currentPosition);
		}
		return;
    }
    super::updateAnimationPart(currentPosition, anim);
}


bool Rect::isWithin(float Xn, float Yn)
{
	glm::vec4 translated_pos = getTranslatedPos(Xn, Yn);
	if(translated_pos.x <= pSquareVertices[SV_TR_X] &&
	   translated_pos.x >= pSquareVertices[SV_TL_X] &&
	   translated_pos.y <= pSquareVertices[SV_BL_Y] &&
	   translated_pos.y >= pSquareVertices[SV_TL_Y]
	   	  	   )
	{
//		LOGI("- %02.3f <= %02.3f <= %02.3f ... %02.3f <= %02.3f <= %02.3f",
//				pSquareVertices[SV_TL_X], translated_pos.x, pSquareVertices[SV_TR_X],
//				pSquareVertices[SV_TL_Y], translated_pos.y, pSquareVertices[SV_BL_Y]);
		return true;
	}
// 	LOGI("E %02.3f <= %02.3f <= %02.3f ... %02.3f <= %02.3f <= %02.3f",
//			pSquareVertices[SV_TL_X], translated_pos.x, pSquareVertices[SV_TR_X],
//			pSquareVertices[SV_TL_Y], translated_pos.y, pSquareVertices[SV_BL_Y]);
	return false;
}

} /* namespace Gain */
