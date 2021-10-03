using System;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.PixelFormats;

namespace ConversionTool
{
	class DiabloPNG
	{
		public static void SavePNG(string filename, byte[] pixels, int width, int height)
		{
			for (int i = 0; i < width * height; i++)
			{
				byte red = pixels[(i * 4) + 0];
				pixels[(i * 4) + 0] = pixels[(i * 4) + 2];
				pixels[(i * 4) + 2] = red;
			}

			Image image = Image.LoadPixelData<Byte4>(pixels, width, height);
			image.SaveAsPng(filename);
		}

		public static void SavePNG(string filename, byte[] data, byte[] palette, int width, int height, byte transr, byte transg, byte transb, bool bgr)
		{
			byte[] buffer = new byte[width * height * 4];

			for(int i = 0; i < width * height; i++)
			{
				if (palette[(data[i] * 3) + 0] == transr && palette[(data[i] * 3) + 1] == transg && palette[(data[i] * 3) + 2] == transb)
				{
					buffer[(i * 4) + 0] = 0;
					buffer[(i * 4) + 1] = 0;
					buffer[(i * 4) + 2] = 0;
					buffer[(i * 4) + 3] = 0;
					continue;
				}

				if(bgr)
				{
					buffer[(i * 4) + 0] = palette[(data[i] * 3) + 2];
					buffer[(i * 4) + 1] = palette[(data[i] * 3) + 1];
					buffer[(i * 4) + 2] = palette[(data[i] * 3) + 0];
				}
				else
				{
					buffer[(i * 4) + 0] = palette[(data[i] * 3) + 0];
					buffer[(i * 4) + 1] = palette[(data[i] * 3) + 1];
					buffer[(i * 4) + 2] = palette[(data[i] * 3) + 2];
				}
				
				buffer[(i * 4) + 3] = 255;
			}

			Image image = Image.LoadPixelData<Byte4>(buffer, width, height);
			image.SaveAsPng(filename);
		}
	}
}
