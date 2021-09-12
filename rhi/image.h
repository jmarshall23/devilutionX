// image.h
//

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

	private:
		std::string name;
		std::vector<ImageFrame_t> frames;
	};
}
