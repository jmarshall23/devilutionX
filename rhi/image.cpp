// image.cpp
//

#include "../source/diablo.h"
#include "../source/storm/storm.h"
#include "../source/engine/load_file.hpp"
#include "image.h"
#include "../source/engine/render/common_impl.h"

namespace devilution
{

	std::vector<StormImage*> globalImageList;
	/*
	=======================
	StormImage::ClipRenderNoLighting
	=======================
	*/
	void StormImage::ClipRenderNoLighting(const Surface& out, int sx, int sy, int frame)
	{
		const ImageFrame_t& image = frames[frame - 1];

		sy -= image.height;

		for (int y = 0; y < image.height; y++)
		{
			for (int x = 0; x < image.width; x++)
			{
				int screenX = x + sx;
				int screenY = y + sy;

				if(screenX < 0)
					continue;

				if (screenY < 0)
					continue;

				if (screenX >= out.w())
					continue;

				if (screenY >= out.h())
					continue;

				int sourcePos = (image.width * (image.height - y - 1)) + (image.width - x - 1);

				if(image.buffer[sourcePos] == (byte)255)
					continue;

				std::uint8_t* dst = out.at(screenX, screenY);

				*dst = (uint8_t)image.buffer[sourcePos];
			}
		}
	}

	/*
	=======================
	StormImage::LoadImageSequence
	=======================
	*/
	StormImage* StormImage::LoadImageSequence(const char* path, bool isTiles)
	{
		// Check to see if the image is already loaded.
		for (int i = 0; i < globalImageList.size(); i++)
		{
			if (globalImageList[i]->name == path)
				return globalImageList[i];
		}

		StormImage* image = new StormImage();
		image->name = path;

		while (true) {
			char framePath[512];
			ImageFrame_t frame;

			if (isTiles)
				sprintf(framePath, "%s\\tiles\\tile%d.tga", path, image->frames.size());
			else
				sprintf(framePath, "%s%d.tga", path, image->frames.size());


			HANDLE file;
			if (!SFileOpenFile(framePath, &file)) {
				break;
			}

			SFileCloseFile(file);

			std::unique_ptr<byte[]> data = LoadFileInMem(framePath);

			frame.width = *(short*)((byte*)&data.get()[12]);
			frame.height = *(short*)((byte*)&data.get()[14]);
			frame.buffer = new byte[frame.width * frame.height];

			for (int i = 0, d = (frame.width * frame.height) - 1; i < frame.width * frame.height; i++, d--) {
				frame.buffer[d] = data[18 + (i * 4) + 0];
			}

			image->frames.push_back(frame);
		}

		globalImageList.push_back(image);
		return image;
	}
}
