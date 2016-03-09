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

#include "Text.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>

namespace Gain {

stbtt_fontinfo *gFontInfo = NULL;


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
		super(x,y,0.f,0.f, gVertexShader, gFragmentShader)
{
	internalInit(pixelSize);
	setText(text);

}

Text::Text(int x,int y, int pixelSize, const char* text) :
		super(x,y,0,0, gVertexShader, gFragmentShader)
{
	internalInit(pixelSize);
	setText(text);
}

Text::~Text() {
	// TODO Auto-generated destructor stub
}

void Text::internalInit(int pixelSize)
{
	pBitmap = NULL;
	pPixelSize = pixelSize;

	if(gFontInfo)
		return;

#ifdef IOS
    const char* font = get_asset_filepath( "Roboto-Regular.ttf" );
#else
    const char* font = "Roboto-Regular.ttf";
#endif

	/* load font file */
	long size;
	long readed_size;
	unsigned char* fontBuffer;

	FILE* fontFile = fopen(font, "rb");
	fseek(fontFile, 0, SEEK_END);
	size = ftell(fontFile); /* how long is the file ? */
	fseek(fontFile, 0, SEEK_SET); /* reset */

	fontBuffer = (unsigned char*)malloc(size);
	gFontInfo = (stbtt_fontinfo*)calloc(1,sizeof(stbtt_fontinfo));

	readed_size = fread(fontBuffer, 1, size, fontFile);
	fclose(fontFile);

	if(readed_size == size)
	/* prepare font */
	if (!stbtt_InitFont(gFontInfo, fontBuffer, 0))
	{
		LOGI("failed to create font\n");
		if(gFontInfo) {
			free(gFontInfo);
			gFontInfo = 0;
		}

		if(fontBuffer) {
			free(fontBuffer);
			fontBuffer = 0;
		}

	}

}

void Text::setText(const char* text)
{
	int length = strlen(text);
	if(length > 255) length = 255;

	memset(pTextBuffer, 0x00, 256);
	memcpy(pTextBuffer, text, length);

	const char* p;

	int width=0,height=0;

    /* calculate font scaling */
    float scale = stbtt_ScaleForPixelHeight(gFontInfo, pPixelSize);

    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(gFontInfo, &ascent, &descent, &lineGap);

    ascent *= scale;
    descent *= scale;

    // calculate bitmap size
    for (p = pTextBuffer; *p; p++)
    {
        /* get bounding box for character (may be offset to account for chars that dip above or below the line */
        int c_x1, c_y1, c_x2, c_y2;
        stbtt_GetCodepointBitmapBox(gFontInfo, p[0], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

        /* compute y (different characters have different heights */
        int y_temp = ascent + c_y1;

        if(y_temp > height)
        {
        	height = y_temp;
        }

        /* how wide is this character */
        int ax;
        stbtt_GetCodepointHMetrics(gFontInfo, p[0], &ax, 0);
        width += ax * scale;

        /* add kerning */
        int kern;
        kern = stbtt_GetCodepointKernAdvance(gFontInfo, p[0], p[1]);
        width += kern * scale;
    }

	uint8_t* oldBitmap = pBitmap;
	pBitmap = (uint8_t*)malloc(width*height);
	if(oldBitmap)
	{
		free(oldBitmap);
	}


	memset(pBitmap,0,width*height);

	pBitmapWidth = width;
	pBitmapHeight = height;

	setSizeN(
			2.f*((float)pBitmapWidth)/GetCore()->screen_width,
			2.f*((float)pBitmapHeight)/GetCore()->screen_width
	);

	int x=0,y=0;

	// render text to buffer
	for (p = pTextBuffer; *p; p++)
    {
        /* get bounding box for character (may be offset to account for chars that dip above or below the line */
        int c_x1, c_y1, c_x2, c_y2;
        stbtt_GetCodepointBitmapBox(gFontInfo, p[0], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

        /* compute y (different characters have different heights */
        y = ascent + c_y1;

        /* render character (stride and offset is important here) */
        int byteOffset = x + (y  * width);
        stbtt_MakeCodepointBitmap(gFontInfo, pBitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, width, scale, scale, p[0]);

        /* how wide is this character */
        int ax;
        stbtt_GetCodepointHMetrics(gFontInfo, p[0], &ax, 0);
        x += ax * scale;

        /* add kerning */
        int kern;
        kern = stbtt_GetCodepointKernAdvance(gFontInfo, p[0], p[1]);
        x += kern * scale;
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
