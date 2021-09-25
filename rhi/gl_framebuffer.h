// gl_framebuffer.h
//

/*
================================================================================================
	Render Texture
================================================================================================
*/
namespace devilution
{
	/*
	================================================
	StormRenderTexture holds both the color and depth images that are made
	resident on the video hardware.
	================================================
	*/
	class StormRenderTexture {
	public:
		StormRenderTexture(StormImage* colorImage, StormImage* depthImage);
		~StormRenderTexture();

		int			GetWidth() const { return (colorImages.size() > 0) ? colorImages[0]->Width() : depthImage->Width(); }
		int			GetHeight() const { return (colorImages.size() > 0) ? colorImages[0]->Height() : depthImage->Height(); }

		StormImage* GetColorImage(int idx) const { return colorImages[idx]; }
		StormImage* GetDepthImage() const { return depthImage; }

		int						GetNumColorImages() const { return colorImages.size(); }

		void					Resize(int width, int height);

		void					MakeCurrent(void);
		static void				BindNull(void);

		unsigned int			GetDeviceHandle(void) { return deviceHandle; }

		void					AddRenderImage(StormImage* image);
		void					InitRenderTexture(void);
	private:

		std::vector<StormImage*>	colorImages;
		StormImage* depthImage;
		unsigned int				deviceHandle;
	};
}
