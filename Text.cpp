/*
 * Text.cpp
 *
 *  Created on: Feb 2, 2013
 *      Author: villekankainen
 */

#include "Text.h"


namespace Gain {

static FT_Library gFt = NULL;

static const char gVertexShader[] =
		"attribute vec2 coord2d;\n"
		"attribute vec2 texcoord;"
		"varying vec2 f_texcoord;"
		"uniform mat4 anim;"

		"void main() {\n"
		"  gl_Position = anim * vec4(coord2d.x, -coord2d.y, 0.0, 1.0);"
		"  f_texcoord = texcoord;"
		"}\n";

static const char gFragmentShader[] =
		"precision mediump float;\n"
		"varying vec2 f_texcoord;"
		"uniform sampler2D mytexture;"
		"uniform vec4 color;"

		"void main(void) {"
		"  vec2 flipped_texcoord = vec2(f_texcoord.x, f_texcoord.y);"
		//		"  gl_FragColor = texture2D(mytexture, flipped_texcoord);"
		//		"  gl_FragColor[0] = color.a;"
		"  gl_FragColor = vec4(1,1,1,texture2D(mytexture, flipped_texcoord).a)*color;"
		"}"
		;


Text::Text(float x,float y, float pixelSize, const char* text) :
		super(x,y,0.f,0.f, gVertexShader, gFragmentShader), pFace(0)
{
	pTextBuffer = new char[256];
	pBitmap = NULL;
	pPixelSize = GetCore()->screen_width*pixelSize;

	if(!pFace)
	{
		if(!gFt) {
			if(FT_Init_FreeType(&gFt)) {
				fprintf(stderr, "Could not init freetype library\n");
				return;
			}
		}

#ifdef IOS
        const char* font = get_asset_filepath( "Fresca-Regular.ttf" );
#else
        const char* font = "Roboto-Regular.ttf";
#endif

		if(FT_New_Face(gFt, font, 0, &pFace)) {

			fprintf(stderr, "Could not open font\n");
			return;
		}

		FT_Set_Pixel_Sizes(pFace, 0, pPixelSize);

		if(FT_Load_Char(pFace, 'A', FT_LOAD_RENDER)) {
			fprintf(stderr, "Could not load character 'A'\n");
			return;
		}
	}
	setText(text);
}

Text::Text(int x,int y, int pixelSize, const char* text) :
		super(x,y,0,0, gVertexShader, gFragmentShader)
{
	pTextBuffer = new char[256];
	pBitmap = NULL;
	pPixelSize = pixelSize;

	if(!gFt) {
		if(FT_Init_FreeType(&gFt)) {

			fprintf(stderr, "Could not init freetype library\n");
			return;
		}
	}

#ifdef IOS
    const char* font = get_asset_filepath( "Fresca-Regular.ttf" );
#else
    const char* font = "Roboto-Regular.ttf";
#endif
    
    if(FT_New_Face(gFt, font, 0, &pFace)) {
		fprintf(stderr, "Could not open font\n");
		return;
	}
	FT_Set_Pixel_Sizes(pFace, 0, pPixelSize);

	if(FT_Load_Char(pFace, 'A', FT_LOAD_RENDER)) {
		fprintf(stderr, "Could not load character 'A'\n");
		return;
	}

	setText(text);
}

Text::~Text() {
	// TODO Auto-generated destructor stub
}

void Text::setText(const char* text)
{
	int length = strlen(text);
	if(length > 255) length = 255;

	memset(pTextBuffer, 0x00, 256);
	memcpy(pTextBuffer, text, length);

	const char* p;

	int width=0,height=0;
	int x=0,y=0;

	for(p = pTextBuffer; *p; p++) {
		if(FT_Load_Char(pFace, *p, FT_LOAD_RENDER))
			continue;

		int temp_height = y+pFace->glyph->bitmap.rows;
		if(temp_height > height)
		{
			height = temp_height;
		}

		int temp_width = x+pFace->glyph->bitmap.width;
		if(temp_width > width)
		{
			width=temp_width;
		}

		x += (pFace->glyph->advance.x >> 6);
		y += (pFace->glyph->advance.y >> 6);
	}


	uint8_t* oldBitmap = pBitmap;
	pBitmap = (uint8_t*)malloc(width*pPixelSize);
	if(oldBitmap)
	{
		free(oldBitmap);
	}


	memset(pBitmap,0,width*pPixelSize);

	pBitmapWidth = width;
	pBitmapHeight = pPixelSize;

	setSizeN(
			2.f*((float)pBitmapWidth)/GetCore()->screen_width,
			2.f*((float)pBitmapHeight)/GetCore()->screen_width
	);

	x=0;
	y=0;

	for(p = pTextBuffer; *p; p++)
	{
		if(FT_Load_Char(pFace, *p, FT_LOAD_RENDER))
			continue;

		FT_GlyphSlot g = pFace->glyph;
		int z = height - g->bitmap_top ;

		for(int w=0;w<g->bitmap.rows;w++)
		{
			memcpy(&pBitmap[(y+w+z)*width + x],&g->bitmap.buffer[w*g->bitmap.width],g->bitmap.width);
		}
		x+=g->advance.x>>6;
		y+=g->advance.y>>6;
	}
	updateBitmap = true;
}

char *Text::getText() {
	return pTextBuffer;
}

bool Text::initVariables()
{
	if(!super::initVariables()) {
		return false;
	}

	const char* attribute_name;
	const char* uniform_name;

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
	        GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			GL_CLAMP_TO_EDGE);

	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, /*GL10.GL_REPLACE*/ GL_MODULATE);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//alpha
	pBitmapSlotSize = 1;

	static uint8_t tempbuf = 0x80;
	glTexImage2D(GL_TEXTURE_2D, // target
			0,  // level, 0 = base, no minimap,
			GL_ALPHA, // internalformat
			1,  // width
			1,  // height
			0,  // border, always 0 in OpenGL ES
			GL_ALPHA,  // format
			GL_UNSIGNED_BYTE, // type
			&tempbuf);

	GLfloat square_texcoords[] = {
			// front
			0.0, 0.0,
			1.0, 0.0,
			1.0, 1.0,
			0.0, 1.0,
	};

    GL_EXT_FUNC glGenBuffers(1, &vbo_square_texcoords);
	checkGlError("glGenBuffers");

    GL_EXT_FUNC glBindBuffer(GL_ARRAY_BUFFER, vbo_square_texcoords);
	checkGlError("glBindBuffer");

    GL_EXT_FUNC glBufferData(GL_ARRAY_BUFFER, sizeof(square_texcoords), square_texcoords, GL_STATIC_DRAW);
	checkGlError("glBufferData");

	return true;
}



void Text::updateG(float time, float timeDelta)
{
	super::updateG( time,  timeDelta);

	if(updateBitmap)
	{
		updateBitmap = false;
		GL_EXT_FUNC glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
						GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
				GL_CLAMP_TO_EDGE);

		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, /*GL10.GL_REPLACE*/ GL_MODULATE);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexImage2D(GL_TEXTURE_2D, // target
				0,  // level, 0 = base, no minimap,
				GL_ALPHA, // internalformat
				pBitmapWidth,  // width
				pBitmapHeight,  // height
				0,  // border, always 0 in OpenGL ES
				GL_ALPHA,  // format
				GL_UNSIGNED_BYTE, // type
				pBitmap);

		GL_EXT_FUNC glUniform1i(uniform_mytexture, 0);

		GL_EXT_FUNC glEnableVertexAttribArray(attribute_texcoord);
		GL_EXT_FUNC glBindBuffer(GL_ARRAY_BUFFER, vbo_square_texcoords);
		GL_EXT_FUNC glVertexAttribPointer(
				attribute_texcoord, // attribute
				2,                  // number of elements per vertex, here (x,y)
				GL_FLOAT,           // the type of each element
				GL_FALSE,           // take our values as-is
				0,                  // no extra data between each position
				0                   // offset of first element
		);
	}
}


void Text::enableAttributes() const
{
	super::enableAttributes();
	GL_EXT_FUNC glEnableVertexAttribArray(attribute_texcoord);
}

void Text::disableAttributes() const
{
    GL_EXT_FUNC glDisableVertexAttribArray(attribute_texcoord);
	super::disableAttributes();
}



} /* namespace Gain */
