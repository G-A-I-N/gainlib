/*
 * Bitmap.cpp
 *
 *  Created on: Feb 2, 2013
 *      Author: villekankainen
 */


#include <png.h>
#include "PngBitmap.h"


namespace Gain {

PngBitmap::PngBitmap(int x,int y, int width, int height, const char* aPngFile) :
Bitmap(x,y,width,height)
{
	readFile(aPngFile);

	initVariables();
	// TODO Auto-generated constructor stub

}

PngBitmap::~PngBitmap() {
	// TODO Auto-generated destructor stub
}

void PngBitmap::readFile(const char* aFileName)
{
    unsigned char header[8];    // 8 is the maximum size that can be checked

    int x, y;

    png_byte color_type;
    png_byte bit_depth;

    png_structp png_ptr;
    png_infop info_ptr;
    int number_of_passes;
    png_bytep * row_pointers;

    /* open file and test for it being a png */
    FILE *fp = fopen(aFileName, "rb");
    if (!fp)
            printf("[read_png_file] File %s could not be opened for reading", aFileName);
    fread(header, 1, 8, fp);

    if (png_sig_cmp(header, 0, 8))
            printf("[read_png_file] File %s is not recognized as a PNG file", aFileName);


    /* initialize stuff */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr)
            printf("[read_png_file] png_create_read_struct failed");

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
            printf("[read_png_file] png_create_info_struct failed");

    if (setjmp(
    		png_jmpbuf( png_ptr ) )
        )
            printf("[read_png_file] Error during init_io");

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);

    png_read_info(png_ptr, info_ptr);

    pBitmapWidth = png_get_image_width(png_ptr, info_ptr);
    pBitmapHeight = png_get_image_height(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    number_of_passes = png_set_interlace_handling(png_ptr);
    png_read_update_info(png_ptr, info_ptr);


    /* read file */
    if (setjmp(png_jmpbuf(png_ptr)))
            printf("[read_png_file] Error during read_image");

    int row_bytes = png_get_rowbytes(png_ptr,info_ptr);
    pBitmapSlotSize = row_bytes*8/pBitmapWidth;
    pBitmap = (png_byte*) malloc(pBitmapHeight*row_bytes);

    row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * pBitmapHeight);
    for (y=0; y<pBitmapHeight; y++)
            row_pointers[y] = (png_byte*) &pBitmap[y*row_bytes];

    png_read_image(png_ptr, row_pointers);

    //free(row_pointers);

    fclose(fp);
}
bool PngBitmap::initVariables() {
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

	glTexImage2D(GL_TEXTURE_2D, // target
			0,  // level, 0 = base, no minimap,
			GL_RGBA, // internalformat
			pBitmapWidth,  // width
			pBitmapHeight,  // height
			0,  // border, always 0 in OpenGL ES
			GL_RGBA,  // format
			GL_UNSIGNED_BYTE, // type
			pBitmap);

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




void PngBitmap::updateG(float time, float timeDelta)
{
	super::updateG( time,  timeDelta);
}


void PngBitmap::enableAttributes() {
	super::enableAttributes();

    GL_EXT_FUNC glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);
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

void PngBitmap::disableAttributes() {
    GL_EXT_FUNC glDisableVertexAttribArray(attribute_texcoord);
	super::disableAttributes();
}



} /* namespace Gain */
