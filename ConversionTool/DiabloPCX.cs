using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConversionTool
{
	public static class DiabloPCX
	{
		private class pcx_t
		{
			public byte manufacturer;
			public byte version;
			public byte encoding;
			public byte bits_per_pixel;
			public ushort xmin, ymin, xmax, ymax;
			public ushort hres, vres;
			public byte[] palette = new byte[48];
			public byte reserved;
			public byte color_planes;
			public ushort bytes_per_line;
			public ushort palette_type;
			public byte[] filler = new byte[58];

			public pcx_t(BinaryReader reader)
			{
				manufacturer = reader.ReadByte();
				version = reader.ReadByte();
				encoding = reader.ReadByte();
				bits_per_pixel = reader.ReadByte();
				xmin = reader.ReadUInt16();
				ymin = reader.ReadUInt16();
				xmax = reader.ReadUInt16();
				ymax = reader.ReadUInt16();
				hres = reader.ReadUInt16();
				vres = reader.ReadUInt16();
				palette = reader.ReadBytes(48);
				reserved = reader.ReadByte();
				color_planes = reader.ReadByte();
				bytes_per_line = reader.ReadUInt16();
				palette_type = reader.ReadUInt16();
				filler = reader.ReadBytes(58);
			}
		};

		/*
		==============
		LoadPCX
		==============
		*/
		private static void LoadPCX( string filename, out byte[] pic, out byte[] palette, out int width, out int height)
		{
			BinaryReader reader = new BinaryReader(new MemoryStream(File.ReadAllBytes(filename)));
			pcx_t pcx = new pcx_t(reader);
			byte[] raw = reader.ReadBytes((int)(reader.BaseStream.Length - reader.BaseStream.Position));
			int x, y;
			int len;
			int dataByte, runLength;
			int xmax, ymax;

			
  			xmax = (pcx.xmax);
			ymax = (pcx.ymax);

			if (pcx.manufacturer != 0x0a
				|| pcx.version != 5
				|| pcx.encoding != 1
				|| pcx.bits_per_pixel != 8)
			{
				throw new Exception("Invalid PCX file!");
			}

			pic = new byte[(ymax + 1) * (xmax + 1)];

			BinaryReader reader2 = new BinaryReader(new MemoryStream(raw));

			width = xmax + 1;
			height = ymax + 1;

			int offset = 0;
			for (y = 0; y <= ymax; y++)
			{
				for (x = 0; x <= xmax;)
				{
					dataByte = reader2.ReadByte();

					if ((dataByte & 0xC0) == 0xC0)
					{
						runLength = dataByte & 0x3F;
						dataByte = reader2.ReadByte();
					}
					else
						runLength = 1;

					while (runLength-- > 0)
						pic[offset + x++] = (byte)dataByte;
				}
				offset += xmax + 1;
			}

			palette = reader2.ReadBytes(768);
		}

		public static byte[] LoadPCX32(string filename, out int width, out int height)
		{
			byte[] pcxBuffer;
			byte[] palette;

			LoadPCX(filename, out pcxBuffer, out palette, out width, out height);

			ExportTileset.currentColorPalette = new byte[768];

			for (int i = 0; i < 256; i++)
			{
				ExportTileset.currentColorPalette[(i * 3) + 0] = palette[(i * 3) + 1];
				ExportTileset.currentColorPalette[(i * 3) + 1] = palette[(i * 3) + 2];
				ExportTileset.currentColorPalette[(i * 3) + 2] = palette[(i * 3) + 0];
			}

			return pcxBuffer;
		}

		public static void WriteConvertedPCX2TGA(string filename, byte[] data, int width, int height)
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

			for (i = 0; i < width * height; i++)
			{
				if (ExportTileset.currentColorPalette[(data[i] * 3) + 0] == 0 && ExportTileset.currentColorPalette[(data[i] * 3) + 1] == 255 && ExportTileset.currentColorPalette[(data[i] * 3) + 2] == 0)
				{
					buffer[imgStart + (i * 4) + 0] = 0;
					buffer[imgStart + (i * 4) + 1] = 0;
					buffer[imgStart + (i * 4) + 2] = 0;
					buffer[imgStart + (i * 4) + 3] = 0;
					continue;
				}

				buffer[imgStart + (i * 4) + 0] = ExportTileset.currentColorPalette[(data[i] * 3) + 2];
				buffer[imgStart + (i * 4) + 1] = ExportTileset.currentColorPalette[(data[i] * 3) + 1];
				buffer[imgStart + (i * 4) + 2] = ExportTileset.currentColorPalette[(data[i] * 3) + 0];
				buffer[imgStart + (i * 4) + 3] = 255;
			}

			File.WriteAllBytes(filename, buffer);
		}
	}
}
