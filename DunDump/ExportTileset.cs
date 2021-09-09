using System;
using System.IO;

namespace DunDump
{
	public static class ExportTileset
	{
		public static void BlitImage(byte[] source, int sourceX, int sourceY, int sourceWidth, byte[] dest, int destX, int destY, int destWidth, int destHeight, int width, int height)
		{
			destY = destHeight - destY - height;

			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					int _x = x * 1;
					int _y = y * 1;
					int destPos = (destWidth * (_y + (destY * 1))) + (_x + (destX * 1));
					int sourcePos = (sourceWidth * (_y + (sourceY * 1))) + (_x + (sourceX * 1));

					if (source[sourcePos + 0] == 255)
						continue;

					dest[destPos + 0] = source[sourcePos + 0];
					//dest[destPos + 1] = source[sourcePos + 1];
					//dest[destPos + 2] = source[sourcePos + 2];
					//dest[destPos + 3] = source[sourcePos + 3];
				}
			}
		}


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


		public static void Export(string filename, string minfile, string tilPath)
		{
			DiabloCel cel = new DiabloCel(filename);

			string fname = Path.GetFileNameWithoutExtension(filename);

			string tilePath = Path.GetDirectoryName(filename) + "/tiles/";
			Directory.CreateDirectory(tilePath);

			//for(int i = 0; i < cel.NumFrames; i++)
			//{
			//	DiabloCelBase frame = cel.GetFrame(i);
			//
			//	WriteTGA(tilePath + "tile" + i + ".tga", frame.Pixels, frame.Width, frame.Height, true);
			//}

			D1Min min = new D1Min(minfile, cel);
			D1Til til = new D1Til(tilPath, min);

			for(int i = 0; i < til.getTileCount(); i++)
			{				
				byte[] buffer = til.getTileImage((ushort)i);

				WriteTGA(tilePath + "tile" + i + ".tga", buffer, til.getTilePixelWidth(), til.getTilePixelHeight(), true);
			}
		}
	}
}
