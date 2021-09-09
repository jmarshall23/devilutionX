using System;
using System.IO;

namespace DunDump
{
	public static class ExportTileset
	{
		private static void WriteTGA(string filename, byte[] data, int width, int height, bool flipVertical)
		{
			byte[] buffer;
			int i;
			int bufferSize = width * height * 4 + 18;
			int imgStart = 18;

			buffer = new byte[bufferSize];			
			buffer[2] = 2; // uncompressed type
			buffer[12] = (byte)(width & 255);
			buffer[13] = (byte)(width >> 8);
			buffer[14] = (byte)(height & 255);
			buffer[15] = (byte)(height >> 8);
			buffer[16] = 32; // pixel size
			if (!flipVertical)
			{
				buffer[17] = (byte)(1 << 5); // flip bit, for normal top to bottom raster order
			}

			// swap rgb to bgr
			for (i = 0; i < width * height; i++)
			{
				buffer[imgStart + (i * 4) + 0] = data[i]; // blue
				buffer[imgStart + (i * 4) + 1] = data[i]; // blue
				buffer[imgStart + (i * 4) + 2] = data[i]; // blue
				buffer[imgStart + (i * 4) + 3] = 255;
			}

			File.WriteAllBytes(filename, buffer);			
		}


		public static void Export(string filename)
		{
			DiabloCel cel = new DiabloCel(filename);

			string fname = Path.GetFileNameWithoutExtension(filename);

			string tilePath = Path.GetDirectoryName(filename) + "/tiles/";
			Directory.CreateDirectory(tilePath);

			for(int i = 0; i < cel.NumFrames; i++)
			{
				DiabloCelBase frame = cel.GetFrame(i);

				WriteTGA(tilePath + "tile" + i + ".tga", frame.Pixels, frame.Width, frame.Height, false);
			}
		}
	}
}
