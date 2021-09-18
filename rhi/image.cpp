// image.cpp
//

#include "../source/diablo.h"
#include "../source/storm/storm.h"
#include "../source/engine/load_file.hpp"
#include "image.h"
#include "../source/engine/render/common_impl.h"
#include "gl_render.h"

namespace devilution
{
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
		const ImageFrame_t& image = frames[frame - 1];

		sy -= image.height;

		GL_RenderImage(image.glHandle, sx, sy, image.width, image.height);
	}

	/*
	=======================
	StormImage::ClipRenderNoLighting
	=======================
	*/
	void StormImage::ClipRenderWithLighting(const Surface& out, int sx, int sy, int frame, int light) const
	{
		const ImageFrame_t& image = frames[frame - 1];

		uint8_t *lightTable = GetLightTable(light);

		sy -= image.height;

		GL_RenderImage(image.glHandle, sx, sy, image.width, image.height);
	}

	/*
	=======================
	StormImage::ClipRenderNoLighting
	=======================
	*/
	void StormImage::ClipRenderWithLighting(const Surface& out, int sx, int sy, int frame) const
	{
		if (LightTableIndex == 0) {
			ClipRenderNoLighting(out, sx, sy, frame);
			return;
		}

		const ImageFrame_t& image = frames[frame - 1];

		uint8_t* lightTable = &LightTables[LightTableIndex * 256];

		sy -= image.height;

		GL_RenderImage(image.glHandle, sx, sy, image.width, image.height);
	}

	/*
	=======================
	StormImage::ClipRenderNoLighting
	=======================
	*/
	void StormImage::ClipRenderOutline(const Surface& out, int color, int sx, int sy, int frame) const
	{
		return;

		const ImageFrame_t& image = frames[frame - 1];

		sy -= image.height;

		sx -= 1;
		sy -= 1;

		for (int y = 0; y < image.height; y++)
		{
			for (int x = 0; x < image.width; x++)
			{
				int screenX = x + sx;
				int screenY = y + sy;

				if (screenX < 0)
					continue;

				if (screenY < 0)
					continue;

				if (screenX >= out.w())
					continue;

				if (screenY >= out.h())
					continue;

				int sourcePos = (image.width * (image.height - y - 1)) + (image.width - x - 1);

				if (image.buffer[sourcePos] == (byte)255 || image.buffer[sourcePos] == (byte)0)
					continue;

				std::uint8_t* dst = out.at(screenX, screenY);

				*dst = (uint8_t)color;
			}
		}

		sx += 2;
		sy += 2;

		for (int y = 0; y < image.height; y++)
		{
			for (int x = 0; x < image.width; x++)
			{
				int screenX = x + sx;
				int screenY = y + sy;

				if (screenX < 0)
					continue;

				if (screenY < 0)
					continue;

				if (screenX >= out.w())
					continue;

				if (screenY >= out.h())
					continue;

				int sourcePos = (image.width * (image.height - y - 1)) + (image.width - x - 1);

				if (image.buffer[sourcePos] == (byte)255 || image.buffer[sourcePos] == (byte)0)
					continue;

				std::uint8_t* dst = out.at(screenX, screenY);

				*dst = (uint8_t)color;
			}
		}
	}

	void StormImage::Draw2D(int frame, int x, int y, int width, int height, int sourcex, int sourcey, int uvwidth, int uvheight)
	{
		const ImageFrame_t& image = frames[frame - 1];

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

			int frameOffset = subImageWidth * (numFrames - i - 1);

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

		if (!isAtlas && !isTiles && strstr(path, ".tga"))
		{
			char framePath[512];
			ImageFrame_t frame;

			std::unique_ptr<byte[]> data = LoadFileInMem(path);

			frame.width = *(short*)((byte*)&data.get()[12]);
			frame.height = *(short*)((byte*)&data.get()[14]);
			frame.buffer = new byte[frame.width * frame.height * 4];

			memcpy(frame.buffer, &data[18], frame.width * frame.height * 4);

			image->frames.push_back(frame);
		}
		else if (isAtlas)
		{
			char atlasPath[512];
			char descripPath[512];

			sprintf(atlasPath, "%s.tga", path);
			sprintf(descripPath, "%s.txt", path);

			std::unique_ptr<byte[]> descripData = LoadFileInMem(descripPath);

			int numFrames = atoi((const char *)descripData.get());

			std::unique_ptr<byte[]> data = LoadFileInMem(atlasPath);

			ImageFrame_t atlasImage;
			atlasImage.width = *(short*)((byte*)&data.get()[12]);
			atlasImage.height = *(short*)((byte*)&data.get()[14]);
			atlasImage.buffer = new byte[atlasImage.width * atlasImage.height * 4];


			byte* sourceData = data.get();
			memcpy(atlasImage.buffer, &data[18], atlasImage.width * atlasImage.height * 4);

			image->CreateImagesFromAtlas(atlasImage, numFrames);

			delete atlasImage.buffer;
		}
		else
		{
			while (true) {
				char framePath[512];
				ImageFrame_t frame;

				if (isTiles)
				{
					if (specialName != nullptr)
					{
						sprintf(framePath, "%s\\%s_%d.tga", path, specialName, image->frames.size());
					}
					else
					{
						sprintf(framePath, "%s\\tiles_%s\\tile_%d.tga", path, pal_name, image->frames.size());
					}
				}
				else
				{
					sprintf(framePath, "%s_%d.tga", path, image->frames.size());
				}


				HANDLE file;
				if (!SFileOpenFile(framePath, &file)) {
					break;
				}

				SFileCloseFile(file);

				std::unique_ptr<byte[]> data = LoadFileInMem(framePath);

				frame.width = *(short*)((byte*)&data.get()[12]);
				frame.height = *(short*)((byte*)&data.get()[14]);
				frame.buffer = new byte[frame.width * frame.height * 4];

				memcpy(frame.buffer, &data[18], frame.width * frame.height * 4);

				image->frames.push_back(frame);
			}
		}

		static byte* temp = new byte[4096 * 8192 * 4];

		// Create the hardware image.
		for (int i = 0; i < image->frames.size(); i++)
		{

			for (int d = 0; d < image->frames[i].width * image->frames[i].height; d++)
			{
				temp[(d * 4) + 0] = image->frames[i].buffer[(d * 4) + 2];
				temp[(d * 4) + 1] = image->frames[i].buffer[(d * 4) + 1];
				temp[(d * 4) + 2] = image->frames[i].buffer[(d * 4) + 0];
				temp[(d * 4) + 3] = image->frames[i].buffer[(d * 4) + 3];
			}

			image->frames[i].glHandle = GL_CreateTexture2D((::byte *)temp, image->frames[i].width, image->frames[i].height, 32);
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
