// image.cpp
//

#include "../source/diablo.h"
#include "../source/storm/storm.h"
#include "../source/engine/load_file.hpp"
#include "image.h"
#include "../source/engine/render/common_impl.h"
#include "gl_render.h"
#include "../source/datatable.h"

void R_LoadPNG(unsigned char* fbuffer, int fileSize, unsigned char** pic, int* width, int* height);

namespace devilution
{
	void ParseAutomapData(const char* fileName);
	int DRLG_GetCeilingTile(void);

	const char* pal_name = nullptr;

	byte* FastFlipHorizontalBuffer(byte *data, int width, int height)
	{
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				data[y * width + x] = data[y * width + (width - x - 1)];
			}
		}

		return data;
	}

	/*
	==============
	R_CopyImage
	==============
	*/
	void R_CopyImage(byte* source, int sourceX, int sourceY, int sourceWidth, byte* dest, int destX, int destY, int destWidth, int destHeight, int width, int height, bool allowTrans, bool allowFlip, byte *remapTable)
	{
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				int _x = x * 1;
				int destPos = (destWidth * (y + destY)) + (_x + destX); // Game drew bottom up.
				int sourcePos = (sourceWidth * (y + sourceY)) + (_x + sourceX);

				if (allowFlip)
				{
					int flipY = (height - y - 1);
					destPos = (destWidth * (flipY + destY)) + (_x + destX);
					sourcePos = (sourceWidth * (y + sourceY)) + (width - (_x + sourceX) - 1);
				}

				if (source[sourcePos] == (byte)255 && !allowTrans)
				{
					continue;
				}

				if (destPos >= destWidth * destHeight)
					continue;

				if (remapTable)
				{
					dest[destPos] = remapTable[(int)source[sourcePos]];
				}
				else
				{
					dest[destPos] = source[sourcePos];
				}
			}
		}
	}

	std::vector<StormImage*> globalImageList;

	/*
	=======================
	StormImage::ClipRenderNoLighting
	=======================
	*/
	void StormImage::ClipRenderUI(const Surface& out, int sx, int sy, int frame, int startx, int starty) const
	{
		GL_ToggleLighting(false);

		const ImageFrame_t& image = frames[frame - 1];

		float start_u = (float)startx / image.width;
		float start_v = (float)starty / image.height;

		GL_RenderImage(image.glHandle, sx, sy, image.width, image.height, startx, starty, start_u, start_v );
	}

	/*
	=======================
	StormImage::ClipRenderNoLighting
	=======================
	*/
	void StormImage::ClipRenderNoLighting(const Surface& out, int sx, int sy, int frame, int startx, int starty) const
	{
		GL_ToggleLighting(false);
		const ImageFrame_t& image = frames[frame - 1];

		sy -= image.height;

		GL_RenderImage(image.glHandle, sx, sy, image.width, image.height);
	}

	/*
	=======================
	StormImage::ClipRenderNoLighting
	=======================
	*/
	void StormImage::ClipRenderWithLighting2(const Surface& out, int sx, int sy, int frame, int light) const
	{
		GL_ToggleLighting(true);
		const ImageFrame_t& image = frames[frame - 1];

		uint8_t *lightTable = GetLightTable(light);

		sy -= image.height;

		//GL_SetColor(lightScale, lightScale, lightScale);		
		GL_RenderImage(image.glHandle, sx, sy, image.width, image.height);
		GL_SetColor(255, 255, 255);
	}
	/*
	=======================
	StormImage::ClipRenderWithLightingTrans
	=======================
	*/
	void StormImage::ClipRenderWithLightingTrans(const Surface& out, int sx, int sy, int frame, int alpha) const {
		GL_SetAlpha(alpha);
		ClipRenderWithLighting(out, sx, sy, frame);
		GL_SetAlpha(255);
	}

	/*
	=======================
	StormImage::ClipRenderNoLighting
	=======================
	*/
	void StormImage::ClipRenderWithLighting(const Surface& out, int sx, int sy, int frame, bool upbright) const
	{
		if (LightTableIndex == 0) {
			ClipRenderNoLighting(out, sx, sy, frame);
			return;
		}

		GL_ToggleLighting(true);

		const ImageFrame_t& image = frames[frame - 1];

		//uint8_t* lightTable = &LightTables[LightTableIndex * 256];

		sy -= image.height;

		if (upbright)
		{
			GL_SetColor(0, 255, 0);
		}
		else if (image.megaTileId == DRLG_GetCeilingTile())
		{
			GL_SetColor(220, 0, 0); // 50%
		}
		else if (IsFloorTile(frame))
		{
			GL_SetColor(0, 0, 0); // 100%
		}
		else
		{
			GL_SetColor(239, 0, 0); // 90%
		}

		//
		GL_RenderImage(image.glHandle, sx, sy, image.width, image.height);
		GL_SetColor(255, 255, 255);
	}

	/*
	=======================
	StormImage::ClipRenderNoLighting
	=======================
	*/
	void StormImage::ClipRenderOutline(const Surface& out, int r, int g, int b, int sx, int sy, int frame) const
	{
		GL_ToggleLighting(false);

		const ImageFrame_t& image = frames[frame - 1];
		sy -= image.height;

		if(r >= 0 && g >= 0 && b >= 0)
			GL_SetColor(r, g, b);

		GL_SetSolidColorMode(true);
		GL_RenderImage(image.glHandle, sx - 1, sy - 1, image.width, image.height);
		GL_RenderImage(image.glHandle, sx + 1, sy + 1, image.width, image.height);
		GL_SetSolidColorMode(false);
		GL_SetColor(255, 255, 255);
	}

	void StormImage::Draw2D(int frame, int x, int y, int width, int height, int sourcex, int sourcey, int uvwidth, int uvheight)
	{
		const ImageFrame_t& image = frames[frame - 1];
		GL_ToggleLighting(false);

		GL_RenderImageScaledUV(image.glHandle, x, y, width, height, sourcex, sourcey, uvwidth, uvheight, image.width, image.height);
	}

	/*
	=======================
	StormImage::Draw
	=======================
	*/
	void StormImage::Draw(const Surface& out, int x, int y, int sourcex, int sourcey, int sourceFrame, bool allowtrans, bool allowflip, byte *remapTable)
	{
		sourceFrame = sourceFrame - 1;
		R_CopyImage(frames[sourceFrame].buffer, sourcex, sourcey, frames[sourceFrame].width, (byte *)out.at(0, 0), x, y, out.w(), out.h(), frames[sourceFrame].width - sourcex, frames[sourceFrame].height - sourcey, allowtrans, allowflip, remapTable);
	}

	/*
	=======================
	StormImage::LoadImageSequence
	=======================
	*/
	StormImage* StormImage::AllocateSytemImage(const char* path, int width, int height)
	{
		// Check to see if the image is already loaded.
		for (int i = 0; i < globalImageList.size(); i++)
		{
			if (globalImageList[i]->name == path)
				return globalImageList[i];
		}

		StormImage* image = new StormImage();
		image->name = path;

		ImageFrame_t frame;
		frame.buffer = new byte[width * height];
		memset(frame.buffer, 255, width * height);
		frame.width = width;
		frame.height = height;

		image->frames.push_back(frame);
		globalImageList.push_back(image);

		return image;
	}

	/*
	=======================
	StormImage::CreateImagesFromAtlas
	=======================
	*/
	void StormImage::CreateImagesFromAtlas(ImageFrame_t& atlasImage, int numFrames) {
		int subImageWidth = atlasImage.width / numFrames;

		for (int i = 0; i < numFrames; i++)
		{
			ImageFrame_t subImage;

			subImage.width = subImageWidth;
			subImage.height = atlasImage.height - 1;

			subImage.buffer = new byte[subImage.width * subImage.height * 4];

			int frameOffset = subImageWidth * i;

			for (int y = 0; y < subImage.height; y++)
			{
				for (int x = 0; x < subImageWidth; x++)
				{
					int sourcePos = (y * atlasImage.width) + (x + frameOffset);
					int destPos = (y * subImage.width) + x;

					if (sourcePos < 0)
						sourcePos = 0;

					for(int d = 0; d < 4; d++)
						subImage.buffer[(destPos * 4) + d] = atlasImage.buffer[(sourcePos * 4) + d];
				}
			}

			//R_CopyImage(atlasImage.buffer, i * subImageWidth, 0, atlasImage.width, subImage.buffer, 0, 0, subImage.width, subImage.height, subImage.width, subImage.height, true, false, nullptr);

			frames.push_back(subImage);
		}
	}
	
	/*
	=======================
	StormImage::LoadImageSequence
	=======================
	*/
	StormImage* StormImage::LoadImageSequence(const char* path, bool isTiles, bool isAtlas, int instanceStride, const char* specialName)
	{
		// Check to see if the image is already loaded.
		for (int i = 0; i < globalImageList.size(); i++)
		{
			if (globalImageList[i]->name == path)
				return globalImageList[i];
		}

		StormImage* image = new StormImage();
		image->name = path;

		if (!isAtlas && !isTiles && strstr(path, ".png"))
		{
			char framePath[512];
			ImageFrame_t frame;

			size_t fsize;
			std::unique_ptr<byte[]> data = LoadFileInMem(path, &fsize);


			R_LoadPNG((unsigned char *)data.get(), fsize, (unsigned char**)&frame.buffer, &frame.width, &frame.height);

			image->frames.push_back(frame);
		}
		else if (isAtlas)
		{
			char atlasPath[512];
			char descripPath[512];

			sprintf(atlasPath, "%s.png", path);
			sprintf(descripPath, "%s.txt", path);

			size_t fsize;
			std::unique_ptr<byte[]> descripData = LoadFileInMem(descripPath, &fsize);

			int numFrames = atoi((const char *)descripData.get());

			std::unique_ptr<byte[]> data = LoadFileInMem(atlasPath);

			ImageFrame_t atlasImage;
			R_LoadPNG((unsigned char*)data.get(), fsize, (unsigned char**)&atlasImage.buffer, &atlasImage.width, &atlasImage.height);

			image->CreateImagesFromAtlas(atlasImage, numFrames);

			delete atlasImage.buffer;
		}
		else if (isTiles)
		{
			if (specialName) {
				image->name += specialName;
			}

			//if (pal_name) {
			//	image->name += pal_name;
			//}

			int numTileFiles = 0;
			bool hasAutomapData = false;

			while (true) {
				// Check to see if this tile exists. 
				char framePath[512];
				ImageFrame_t frame;

				if (specialName != nullptr)
				{
					sprintf(framePath, "%s\\special%d.png", path, numTileFiles);
				}
				else
				{
					sprintf(framePath, "%s\\tiles\\tile%d.png", path, numTileFiles);
				}

				HANDLE file;
				if (!SFileOpenFile(framePath, &file)) {
					break;
				}

				SFileCloseFile(file);

				// Load in the automap data
				if (specialName == nullptr)
				{
					HANDLE file;
					char automapPath[512];
					sprintf(automapPath, "%s\\tiles\\tile%d.automap", path, numTileFiles);

					if (numTileFiles == 0)
					{
						if (SFileOpenFile(automapPath, &file)) {
							SFileCloseFile(file);
							hasAutomapData = true;
						}
					}

					if (hasAutomapData)
					{
						ParseAutomapData(automapPath);
					}
				}

				// Load in the map data.
				size_t fsize;
				std::unique_ptr<byte[]> data = LoadFileInMem(framePath, &fsize);
				R_LoadPNG((unsigned char*)data.get(), fsize, (unsigned char**)&frame.buffer, &frame.width, &frame.height);

				if (specialName != nullptr)
				{
					image->frames.push_back(frame);
					numTileFiles++;
				}
				else
				{
					char tileInfoPath[512];
					sprintf(framePath, "%s\\tiles\\tile%d.tileinfo", path, numTileFiles);

					DataTable* table = new DataTable(framePath);

					MegaTile megaTile;
					for (int d = 0; d < 4; d++)
					{
						int overridePos = table->GetInt("tilid", d);
						int testMicro = -1;
						if (overridePos != -1)
						{
							switch (d)
							{
							case 0:
								testMicro = megaTile.micro1 = overridePos;
								break;
							case 1:
								testMicro = megaTile.micro2 = overridePos;
								break;
							case 2:
								testMicro = megaTile.micro3 = overridePos;
								break;
							case 3:
								testMicro = megaTile.micro4 = overridePos;
								break;
							default:
								devilution::app_fatal("Invalid micro");
								break;

							}
						}
						else
						{
							switch (d)
							{
							case 0:
								testMicro = megaTile.micro1 = image->frames.size();
								break;
							case 1:
								testMicro = megaTile.micro2 = image->frames.size();
								break;
							case 2:
								testMicro = megaTile.micro3 = image->frames.size();
								break;
							case 3:
								testMicro = megaTile.micro4 = image->frames.size();
								break;
							default:
								devilution::app_fatal("Invalid micro");
								break;

							}
						}

						bool floorHack = false;
						if (testMicro < image->frames.size() && overridePos != -1)
						{
							int solflag = table->GetInt("solflag", d);

							// stupid hack for the floors.
							if (solflag == 0 && frame.width == 128 && frame.height == 64 && image->frames[testMicro].height > 64)
							{
								floorHack = true;
							}
							else if (solflag == 15 && frame.width == 128 && frame.height == 128)
							{
								floorHack = true;
							}
							else
							{
								continue;
							}
						}

						ImageFrame_t subImage;

						subImage.width = table->GetInt("width", d);
						subImage.height = table->GetInt("height", d);
						subImage.buffer = new byte[subImage.width * subImage.height * 4];
						subImage.megaTileId = numTileFiles;

						int subImageX = table->GetInt("x", d);
						int subImageY = table->GetInt("y", d);

						for (int y = 0; y < subImage.height; y++)
						{
							for (int x = 0; x < subImage.width; x++)
							{
								int sourcePos = ((y + subImageY) * frame.width) + (x + subImageX);
								int destPos = (y * subImage.width) + x;

								if (sourcePos < 0)
									sourcePos = 0;

								for (int d = 0; d < 4; d++)
									subImage.buffer[(destPos * 4) + d] = frame.buffer[(sourcePos * 4) + d];
							}
						}

						if (!floorHack)
						{
							image->solData.push_back(table->GetInt("solflag", d));
							image->frames.push_back(subImage);
						}
						else
						{
							delete image->frames[testMicro].buffer;
							image->frames[testMicro] = subImage;
						}
						
					}

					image->megaTiles.push_back(megaTile);
					numTileFiles++;


					delete frame.buffer;
					delete table;
				}
			}
		}
		else
		{
			while (true) {
				char framePath[512];
				ImageFrame_t frame;

				sprintf(framePath, "%s_%d.png", path, image->frames.size());

				HANDLE file;
				if (!SFileOpenFile(framePath, &file)) {
					break;
				}

				SFileCloseFile(file);

				size_t fsize;
				std::unique_ptr<byte[]> data = LoadFileInMem(framePath, &fsize);

				R_LoadPNG((unsigned char*)data.get(), fsize, (unsigned char**)&frame.buffer, &frame.width, &frame.height);

				image->frames.push_back(frame);
			}
		}


		// Create the hardware image.
		for (int i = 0; i < image->frames.size(); i++)
		{
			image->frames[i].glHandle = GL_CreateTexture2D((::byte *)image->frames[i].buffer, image->frames[i].width, image->frames[i].height, 32);

			delete image->frames[i].buffer;
			image->frames[i].buffer = nullptr;
		}

		// Create image instances.
		if (instanceStride == -1)
		{
			for (int i = 0; i < image->frames.size(); i++)
			{
				StormImage instanceImage;

				instanceImage.name = image->name + "_instance";
				instanceImage.frames.push_back(image->frames[i]);
				image->imageInstances.push_back(instanceImage);
			}
		}
		else
		{
			int numImagesPerStride = image->frames.size() / instanceStride;
			int imageId = 0;
			for (int i = 0; i < instanceStride; i++)
			{
				StormImage instanceImage;
				instanceImage.name = image->name + "_instance";
				for (int d = 0; d < numImagesPerStride; d++)
				{
					instanceImage.frames.push_back(image->frames[imageId++]);
				}
				image->imageInstances.push_back(instanceImage);
				
			}
		}

		globalImageList.push_back(image);
		return image;
	}
}
