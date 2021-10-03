
extern "C"
{
	#include <png.h>


	static void png_Error(png_structp pngPtr, png_const_charp msg)
	{
		//	common->FatalError("%s", msg);
	}

	static void png_Warning(png_structp pngPtr, png_const_charp msg)
	{
	//	common->Warning("%s", msg);
	}

	static void	png_ReadData(png_structp pngPtr, png_bytep data, png_size_t length)
	{
		memcpy(data, (unsigned char*)pngPtr->io_ptr, length);

		pngPtr->io_ptr = ((unsigned char*)pngPtr->io_ptr) + length;
	}

}

/*
=============
R_LoadPNG
=============
*/
void R_LoadPNG(unsigned char* fbuffer, int fileSize, unsigned char** pic, int* width, int* height)
{
	*pic = nullptr;

	// create png_struct with the custom error handlers
	png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, (png_voidp)NULL, png_Error, png_Warning);
	if (!pngPtr)
	{
		//common->Error("LoadPNG( %s ): png_create_read_struct failed", filename);
		return;
	}

	// allocate the memory for image information
	png_infop infoPtr = png_create_info_struct(pngPtr);
	if (!infoPtr)
	{
		//common->Error("LoadPNG( %s ): png_create_info_struct failed", filename);
		return;
	}

	png_set_read_fn(pngPtr, fbuffer, png_ReadData);

	png_set_sig_bytes(pngPtr, 0);

	png_read_info(pngPtr, infoPtr);

	png_uint_32 pngWidth, pngHeight;
	int bitDepth, colorType, interlaceType;
	png_get_IHDR(pngPtr, infoPtr, &pngWidth, &pngHeight, &bitDepth, &colorType, &interlaceType, NULL, NULL);

	// 16 bit -> 8 bit
	png_set_strip_16(pngPtr);

	// 1, 2, 4 bit -> 8 bit
	if (bitDepth < 8)
	{
		png_set_packing(pngPtr);
	}

#if 1
	if (colorType & PNG_COLOR_MASK_PALETTE)
	{
		png_set_expand(pngPtr);
	}

	if (!(colorType & PNG_COLOR_MASK_COLOR))
	{
		png_set_gray_to_rgb(pngPtr);
	}

#else
	if (colorType == PNG_COLOR_TYPE_PALETTE)
	{
		png_set_palette_to_rgb(pngPtr);
	}

	if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8)
	{
		png_set_expand_gray_1_2_4_to_8(pngPtr);
	}
#endif

	// set paletted or RGB images with transparency to full alpha so we get RGBA
	if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS))
	{
		png_set_tRNS_to_alpha(pngPtr);
	}

	// make sure every pixel has an alpha value
	if (!(colorType & PNG_COLOR_MASK_ALPHA))
	{
		png_set_filler(pngPtr, 255, PNG_FILLER_AFTER);
	}

	png_read_update_info(pngPtr, infoPtr);

	unsigned char* out = new unsigned char[pngWidth * pngHeight * 4];

	*pic = out;
	*width = pngWidth;
	*height = pngHeight;

	png_uint_32 rowBytes = png_get_rowbytes(pngPtr, infoPtr);

	png_bytep* rowPointers = new png_bytep[pngHeight];// (png_bytep*)R_StaticAlloc(sizeof(png_bytep) * pngHeight);
	for (png_uint_32 row = 0; row < pngHeight; row++)
	{
		rowPointers[row] = (png_bytep)(out + (row * pngWidth * 4));
	}

	png_read_image(pngPtr, rowPointers);

	png_read_end(pngPtr, infoPtr);

	png_destroy_read_struct(&pngPtr, &infoPtr, NULL);

	delete rowPointers;
}

