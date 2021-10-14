using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace ConversionTool
{
	class DiabloCL2Frame : DiabloCelBase
	{
		public DiabloCL2Frame(byte[] pixels, int width, int height)
		{
			_width = width;
			_height = height;

			load(pixels);
		}

		protected override void load(byte[] rawData)
		{
			int frameDataStartOffset = 0;
			byte readByte = 0;
			List<byte> pixelLine = new List<byte>();

			// Checking the presence of the {CL2 FRAME HEADER}
			if (rawData[0] == 0x0A && rawData[1] == 0x00)
			{
				frameDataStartOffset += 0x0A;
				// If header is present, try to compute frame width from frame header
				_width = computeWidthFromHeader(rawData);
			}
			else
				throw new Exception("Invalid header");

			if (_width == 0)
				throw new Exception("Invalid width!");

			// READ {CL2 FRAME DATA}

			pixelLine.Clear();
			for (int o = frameDataStartOffset; o < rawData.Length; o++)
			{
				readByte = rawData[o];

				// Transparent pixels
				if (readByte > 0x00 && readByte < 0x80)
				{
					for (int i = 0; i < readByte; i++)
					{
						// Add transparent pixel
						pixelLine.Add(255);

						if (pixelLine.Count == _width)
						{
							_pixels.AddRange(pixelLine);
							pixelLine.Clear();
						}
					}
				}
				// Repeat palette index
				else if (readByte >= 0x80 && readByte < 0xBF)
				{
					// Go to the palette index offset
					o++;

					for (int i = 0; i < (0xBF - readByte); i++)
					{
						// Add opaque pixel
						pixelLine.Add(rawData[o]);

						if (pixelLine.Count == _width)
						{
							_pixels.AddRange(pixelLine);
							pixelLine.Clear();
						}
					}
				}
				// Palette indices
				else if (readByte >= 0xBF)
				{
					for (int i = 0; i < (256 - readByte); i++)
					{
						// Go to the next palette index offset
						o++;
						// Add opaque pixel
						pixelLine.Add(rawData[o]);

						if (pixelLine.Count == _width)
						{
							_pixels.AddRange(pixelLine);
							pixelLine.Clear();
						}
					}
				}
				else if (readByte == 0x00)
				{
					Console.WriteLine("0x00 found");
				}

			}

			if (_height == 0)
				_height = _pixels.Count / _width;
		}

		protected override int computeWidthFromHeader(byte[] rawFrameData )
		{
			BinaryReader file = new BinaryReader(new MemoryStream(rawFrameData));

			ushort[] celFrameHeader = new ushort[5];
			ushort[] celFrameWidth = new ushort[4] { 0, 0, 0, 0 };
			ushort pixelCount = 0;
			byte readByte = 0;

			// Read the {CEL FRAME HEADER}
			for (int i = 0; i < 5; i++)
				celFrameHeader[i] = file.ReadUInt16();

			// Read the five 32 pixel-lines block to calculate the image width
			for (int i = 0; i < 4; i++)
			{
				if (celFrameHeader[i + 1] == 0)
					break;

				for (int j = celFrameHeader[i]; j < celFrameHeader[i + 1]; j++)
				{
					readByte = rawFrameData[j];

					if (readByte > 0x00 && readByte < 0x80)
					{
						pixelCount += readByte;
					}
					else if (readByte >= 0x80 && readByte < 0xBF)
					{
						pixelCount += (ushort)(0xBF - readByte);
						j++;
					}
					else if (readByte >= 0xBF)
					{
						pixelCount += (ushort)(256 - readByte);
						j += (256 - readByte);
					}
				}

				celFrameWidth[i] = (ushort)(pixelCount / 32);
				pixelCount = 0;
			}

			// The calculated width has to be the identical for each 32 pixel-line block
			// If it's not the case, 0 is returned
			for (int i = 0; i < 3; i++)
			{
				if (celFrameWidth[i + 1] != 0 && celFrameWidth[i] != celFrameWidth[i + 1])
					return 0;
			}

			return celFrameWidth[0];
		}
	}
}
