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
		unsigned int glHandle;
	};

	class StormImage {
	public:
		static StormImage* LoadImageSequence(const char* path, bool isTiles, bool isAtlas, int instanceStride = -1, const char *specialName = nullptr);
		static StormImage* AllocateSytemImage(const char* path, int width, int height);

		int NumFrames() const
		{
			return frames.size() + 1;
		}

		const ImageFrame_t& GetFrame(int index)
		{
			if (index == 0)
				return frames[0];

			return frames[index - 1];
		}

		const StormImage *GetFrameInstance(int index) const
		{
			return &imageInstances[index];
		}

		const StormImage* operator[](int index)
		{
			return &imageInstances[index];
		}

		void Draw(const Surface& out, int x, int y, int sourcex, int sourcey, int sourceFrame, bool allowTrans, bool allowflip, byte *remapTable = nullptr);

		void ClipRenderWithLighting(const Surface& out, int sx, int sy, int frame, int light) const;
		void ClipRenderWithLighting(const Surface& out, int sx, int sy, int frame) const;
		void ClipRenderNoLighting(const Surface& out, int sx, int sy, int frame, int startx = 0, int starty = 0) const;
		void ClipRenderUI(const Surface& out, int sx, int sy, int frame, int startx = 0, int starty = 0) const;
		void ClipRenderOutline(const Surface& out, int r, int g, int b,int sx, int sy, int frame) const;

		void Draw2D(int frame, int x, int y, int width, int height, int sourcex, int sourcey, int uvwidth, int uvheight);

		void ClipRenderWithLightingTrans(const Surface& out, int sx, int sy, int frame, int alpha) const;

		int Width() const {
			return frames[0].width;
		}

		int Height() const {
			return frames[0].height;
		}
	private:
		void CreateImagesFromAtlas(ImageFrame_t& atlasImage, int numFrames);

		std::string name;
		std::vector<ImageFrame_t> frames;

		std::vector<StormImage> imageInstances;
	};
}
