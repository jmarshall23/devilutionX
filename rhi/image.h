// image.h
//

#pragma once

#include <vector>
#include <string>

namespace devilution {
	struct ImageFrame_t {
		ImageFrame_t()
		{
			buffer = nullptr;
			width = 0;
			height = 0;
		}

		void Free()
		{
			if (buffer)
				delete buffer;
		}

		int width;
		int height;
		byte* buffer;
	};

	class StormImage {
	public:
		static StormImage* LoadImageSequence(const char* path, bool isTiles, bool isAtlas);
		static StormImage* AllocateSytemImage(const char* path, int width, int height);

		int NumFrames()
		{
			return frames.size() + 1;
		}

		const ImageFrame_t& GetFrame(int index)
		{
			return frames[index - 1];
		}

		void Blit(StormImage* image, int x, int y, int sourcex, int sourcey, int sourceFrame, int destFrame, bool allowTrans, int customHeight = -1);
		void Draw(const Surface& out, int x, int y, int sourcex, int sourcey, int sourceFrame, bool allowTrans, bool allowflip, byte *remapTable = nullptr);

		void ClipRenderNoLighting(const Surface& out, int sx, int sy, int frame);
		void ClipRenderOutline(const Surface& out, int color, int sx, int sy, int frame);
	private:
		void CreateImagesFromAtlas(ImageFrame_t& atlasImage, int numFrames);

		std::string name;
		std::vector<ImageFrame_t> frames;
	};
}
