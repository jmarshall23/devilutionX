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
			dpath = dpath.Replace("mpq_data", "base");
			//	Directory.CreateDirectory(dpath);
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

		public static void BlitImage2(byte[] source, int sourceX, int sourceY, int sourceWidth, byte[] dest, int destX, int destY, int destWidth, int destHeight, int width, int height, bool allowTrans = false)
		{
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					int _x = x * 1;
					int _y = y * 1;
					int destPos = (destWidth * (_y + (destY * 1))) + (_x + (destX * 1));
					int sourcePos = (sourceWidth * (_y + (sourceY * 1))) + (_x + (sourceX * 1));

					if (source[sourcePos + 0] == 255 && !allowTrans)
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

		public static byte[] currentColorPalette;
		public static string palette_name;

		public static void DumpPaletteToTextFile(string filename)
		{
			string outputPath = Path.ChangeExtension(filename, ".txt");

			string lines = "// For debug purposes only\n";
			for (int i = 0; i < 256; i++)
			{
				lines += "" + i + " " + currentColorPalette[(i * 3) + 0] + " " + currentColorPalette[(i * 3) + 1] + " " + currentColorPalette[(i * 3) + 2] + "\n";
			}

			File.WriteAllText(outputPath, lines);
		}

		public static void SetColorPalette(string filename)
		{
			palette_name = Path.GetFileNameWithoutExtension(filename);
			currentColorPalette = File.ReadAllBytes(filename);
		}

		public static void WriteTGA(string filename, byte[] data, int width, int height, bool flipVertical)
		{
			byte[] buffer;
			int i, d;
			int bufferSize = (width * height * 4) + 18;
			int imgStart = 18;

			buffer = new byte[bufferSize];
			buffer[2] = 2; // uncompressed type
			buffer[12] = (byte)(width & 255);
			buffer[13] = (byte)(width >> 8);
			buffer[14] = (byte)(height & 255);
			buffer[15] = (byte)(height >> 8);
			buffer[16] = 32; // pixel size
			buffer[17] = (byte)(1 << 5); // flip bit, for normal top to bottom raster order

			if (!flipVertical)
			{
				for (i = 0; i < width * height; i++)
				{
					if (data[i] == 255)
					{
						buffer[imgStart + (i * 4) + 0] = 255;
						buffer[imgStart + (i * 4) + 1] = 255;
						buffer[imgStart + (i * 4) + 2] = 255;
						buffer[imgStart + (i * 4) + 3] = 0;
						continue;
					}
					buffer[imgStart + (i * 4) + 0] = currentColorPalette[(data[i] * 3) + 2];
					buffer[imgStart + (i * 4) + 1] = currentColorPalette[(data[i] * 3) + 1];
					buffer[imgStart + (i * 4) + 2] = currentColorPalette[(data[i] * 3) + 0];

					if (buffer[imgStart + (i * 4) + 0] == 0 && buffer[imgStart + (i * 4) + 1] == 0 && buffer[imgStart + (i * 4) + 2] == 0 && !Program.exportingTileset)
						buffer[imgStart + (i * 4) + 3] = 128;
					else
						buffer[imgStart + (i * 4) + 3] = 255;
				}
			}
			else
			{
				for (i = width * height - 1, d = 0; i >= 0; i--, d++)
				{
					if (data[d] == 255)
					{
						buffer[imgStart + (i * 4) + 0] = 255;
						buffer[imgStart + (i * 4) + 1] = 255;
						buffer[imgStart + (i * 4) + 2] = 255;
						buffer[imgStart + (i * 4) + 3] = 0;
						continue;
					}
					buffer[imgStart + (i * 4) + 0] = currentColorPalette[(data[d] * 3) + 2];
					buffer[imgStart + (i * 4) + 1] = currentColorPalette[(data[d] * 3) + 1];
					buffer[imgStart + (i * 4) + 2] = currentColorPalette[(data[d] * 3) + 0];

					if (buffer[imgStart + (i * 4) + 0] == 0 && buffer[imgStart + (i * 4) + 1] == 0 && buffer[imgStart + (i * 4) + 2] == 0 && !Program.exportingTileset)
						buffer[imgStart + (i * 4) + 3] = 128;
					else
						buffer[imgStart + (i * 4) + 3] = 255;
				}
			}


			File.WriteAllBytes(filename, buffer);
		}

		public static int FindStartY(byte[] buffer, int width, int height)
		{
			int startY = 0;

			while (startY < height)
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
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
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

		public static void Export(string filename, string minfile, string tilPath, string solPath)
		{
			DiabloCel cel = new DiabloCel(filename);

			byte[] solData = File.ReadAllBytes(solPath);

			string fname = Path.GetFileNameWithoutExtension(filename);

			string tilePath = ExportTileset.FixExportPath(filename) + "/tiles/";
			Directory.CreateDirectory(tilePath);

			D1Min min = new D1Min(minfile, cel);
			D1Til til = new D1Til(tilPath, min);

			for (int i = 0; i < til.getTileCount(); i++)
			{
				// Export mod ready tiles
				byte[] buffer = ExportTileset.FastFlipHorizontalBuffer(FastFlipBuffer(til.getTileImage((ushort)i)), til.getTilePixelWidth(), til.getTilePixelHeight());

				int startY = FindStartY(buffer, til.getTilePixelWidth(), til.getTilePixelHeight());

				using (StreamWriter writer = File.CreateText(tilePath + "tile" + i + ".tileinfo"))
				{
					writer.WriteLine("index,x,y,width,height,solflag");
					for (int d = 0; d < 4; d++)
					{
						int x, y, width, height;
						til.GetSubTileInfo(i, d, startY, out x, out y, out width, out height);

						writer.WriteLine("" + d + "," + x + "," + y + "," + width + "," + height + "," + solData[til.subtileIndices[i][d]]);
					}
				}

				ExportFixedTarga(tilePath + "tile" + i + "_" + palette_name + ".tga", buffer, til.getTilePixelWidth(), til.getTilePixelHeight());
			}
		}
	}
}
