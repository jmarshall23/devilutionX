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
		static StormImage* LoadImageSequence(const char* path, bool isTiles);

		int NumFrames()
		{
			return frames.size() + 1;
		}

		const ImageFrame_t& GetFrame(int index)
		{
			return frames[index - 1];
		}

		void ClipRenderNoLighting(const Surface& out, int sx, int sy, int frame);
	private:
		std::string name;
		std::vector<ImageFrame_t> frames;
	};
}
