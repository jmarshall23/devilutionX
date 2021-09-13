using System;
using System.IO;
using System.Collections.Generic;
using System.Security.Cryptography;
using System.Text;

namespace ConversionTool
{
	public static class ExportTileset
	{
		public static string FixExportPath(string path)
		{
			string dpath = Path.GetDirectoryName(path);
			dpath = dpath.Replace("BlizzData", "Build");
			Directory.CreateDirectory(dpath);
			return dpath;
		}
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

		public static void BlitImage2(byte[] source, int sourceX, int sourceY, int sourceWidth, byte[] dest, int destX, int destY, int destWidth, int destHeight, int width, int height)
		{
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

		public static bool ImageSegmentHasData(byte[] source, int sourceX, int sourceY, int sourceWidth, int width, int height)
		{
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					int _x = x * 1;
					int _y = y * 1;
					int sourcePos = (sourceWidth * (_y + (sourceY * 1))) + (_x + (sourceX * 1));

					if (source[sourcePos + 0] != 255)
						return true;
				}
			}

			return false;
		}


		public static void WriteTGA(string filename, byte[] data, int width, int height, bool flipVertical)
		{
			byte[] buffer;
			int i, d;
			int bufferSize = width * height + 18;
			int imgStart = 18;

			buffer = new byte[bufferSize];			
			buffer[2] = 3; // uncompressed type
			buffer[12] = (byte)(width & 255);
			buffer[13] = (byte)(width >> 8);
			buffer[14] = (byte)(height & 255);
			buffer[15] = (byte)(height >> 8);
			buffer[16] = 8; // pixel size
			buffer[17] = (byte)(1 << 5); // flip bit, for normal top to bottom raster order

			if (!flipVertical)
			{
				for (i = 0; i < width * height; i++)
				{
					buffer[imgStart + i] = data[i];
				}
			}
			else
			{
				for (i = width * height - 1, d = 0; i >= 0; i--, d++)
				{
					buffer[imgStart + i] = data[d];
				}
			}
			

			File.WriteAllBytes(filename, buffer);			
		}

		private static int FindStartY(byte[] buffer, int width, int height)
		{
			int startY = 0;

			while(startY < height)
			{
				if (ImageSegmentHasData(buffer, 0, startY, width, width, 32))
					return startY;

				startY += 32;
			}

			//throw new Exception("FindStartY: Image is empty!");

			return 0;
		}

		public static byte[] FastFlipBuffer(byte[] data)
		{
			byte[] buffer = new byte[data.Length];
			for (int i = 0; i < data.Length; i++)
			{
				buffer[i] = data[data.Length - 1 - i]; 
			}

			return buffer;
		}

		public static byte[] FastFlipHorizontalBuffer(byte[] data, int width, int height)
		{
			byte[] buffer = new byte[data.Length];
			for(int y = 0; y < height; y++)
			{
				for(int x = 0; x < width; x++)
				{
					buffer[y * width + x] = data[y * width + (width - x - 1)];
				}
			}

			return buffer;
		}

		public static bool ExportFixedTarga(string filename, byte[] buffer, int width, int height)
		{
			int startY = FindStartY(buffer, width, height);

			int newHeight = height - startY;
			byte[] copyBuffer = new byte[width * newHeight];

			for (int d = 0; d < copyBuffer.Length; d++)
				copyBuffer[d] = 255;

			BlitImage2(buffer, 0, startY, width, copyBuffer, 0, 0, width, newHeight, width, newHeight);

			WriteTGA(filename, copyBuffer, width, newHeight, false);

			return true;
		}
		public static string GetHashSHA1(this byte[] data)
		{
			using (SHA1Managed sha1 = new SHA1Managed())
			{
				var hash = sha1.ComputeHash(data);
				return Convert.ToBase64String(hash);
			}
		}

		public static void Export(string filename, string minfile, string tilPath)
		{
			DiabloCel cel = new DiabloCel(filename);

			string fname = Path.GetFileNameWithoutExtension(filename);

			string tilePath = ExportTileset.FixExportPath(filename) + "/tiles/";
			Directory.CreateDirectory(tilePath);

			//for(int i = 0; i < cel.NumFrames; i++)
			//{
			//	DiabloCelBase frame = cel.GetFrame(i);
			//
			//	WriteTGA(tilePath + "tile" + i + ".tga", frame.Pixels, frame.Width, frame.Height, true);
			//}

			D1Min min = new D1Min(minfile, cel);
			D1Til til = new D1Til(tilPath, min);

			byte[] tempQuadBuffer = new byte[64 * 32];

			int tileIndex = 0;

			List<string> hashes = new List<string>();
			for(int i = 0; i < til.getTileCount(); i++)
			{
				// Export mod ready tiles
				//byte[] buffer = FastFlipBuffer(til.getTileImage((ushort)i));
				//
				//ExportFixedTarga(tilePath + "tile" + i + ".tga", buffer, til.getTilePixelWidth(), til.getTilePixelHeight());

				// Stop gap export for til ready rendering without min.
				D1Til.ImageTemp[] tempImages = til.getTileImagesTemp((ushort)i);

				foreach(D1Til.ImageTemp temp in tempImages)
				{
					byte[] buffer = FastFlipBuffer(temp.data);

					string hash = GetHashSHA1(buffer);

					if(hashes.Contains(hash))
						continue;

					if (ExportFixedTarga(tilePath + "tile_" + tileIndex + ".tga", buffer, temp.width, temp.height))
					{
						tileIndex++;
					}

					hashes.Add(hash);
				}
			}
		}
	}
}
