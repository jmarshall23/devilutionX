using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConversionTool
{
	class DiabloCelBase
	{
		private const int transparentIndex = 255;

		public DiabloCelBase(byte[] pixels, int width, int height, DiabloCel.D1CEL_FRAME_TYPE type)
		{			
			_width = width;
			_height = height;
			_type = type;

			load(pixels);
		}

		int computeWidthFromHeader(byte[] rawFrameData )
		{
			BinaryReader file = new BinaryReader(new MemoryStream(rawFrameData));

			// Reading the frame header

			int[] celFrameHeader = new int[5];
			int[] celFrameWidth = new int[] { 0, 0, 0, 0 };
			int pixelCount = 0;
			int  readByte = 0;

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

					if (readByte > 0x7F)
					{
						pixelCount += (256 - readByte);
					}
					else
					{
						pixelCount += readByte;
						j += readByte;
					}
				}

				celFrameWidth[i] = pixelCount / 32;
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

		private void load(byte[] rawData)
		{
			uint frameDataStartOffset = 0;
			byte readByte = 0;
			byte secondReadByte = 0;

			List<byte> pixelLine = new List<byte>();

			if (rawData.Length == 0)
			{
				throw new Exception("Invalid raw data!");
			}

			// If the frame size wasnt provided then it needs to be calculated
			if (_width == 0)
			{
				// Checking the presence of the {CEL FRAME HEADER}
				if (rawData[0] == 0x0A && rawData[1] == 0x00)
				{
					frameDataStartOffset += 0x0A;
					// If header is present, try to compute frame width from frame header
					_width = computeWidthFromHeader(rawData);
				}
				
				// if CEL width was not found, return false
				if (_width == 0)
				{
					throw new Exception("Invalid width!");
				}
			}
			else if(_type == DiabloCel.D1CEL_FRAME_TYPE.REGULAR_DATASKIP)
			{
				frameDataStartOffset += 0x0A;
				_type = DiabloCel.D1CEL_FRAME_TYPE.REGULAR;
			}

			// READ {CEL FRAME DATA}
			// if it is a CEL level frame
			if (_type != DiabloCel.D1CEL_FRAME_TYPE.REGULAR)
			{
				// 0x400 frame
				if (_type == DiabloCel.D1CEL_FRAME_TYPE.LEVEL_TYPE_0)
				{
					for (int i = 0; i < 32; i++)
					{
						for (int j = 0; j < 32; j++)
						{
							pixelLine.Add(rawData[i * 32 + j]);
						}
						_pixels.AddRange(pixelLine);
						pixelLine.Clear();
					}
				}
				// 0x220 or 0x320 frame
				else
				{
					bool[] dataPattern = null;
					ushort[] dataPatternZeroedBytes = null;

					switch (_type)
					{
						case DiabloCel.D1CEL_FRAME_TYPE.LEVEL_TYPE_2:
							dataPattern = DiabloCel.D1CEL_LEVEL_FRAME_TYPE_2;
							dataPatternZeroedBytes = DiabloCel.D1CEL_LEVEL_FRAME_TYPE_2_ZEROED_BYTES;
							break;
						case DiabloCel.D1CEL_FRAME_TYPE.LEVEL_TYPE_3:
							dataPattern = DiabloCel.D1CEL_LEVEL_FRAME_TYPE_3;
							dataPatternZeroedBytes = DiabloCel.D1CEL_LEVEL_FRAME_TYPE_3_ZEROED_BYTES;
							break;
						case DiabloCel.D1CEL_FRAME_TYPE.LEVEL_TYPE_4:
							dataPattern = DiabloCel.D1CEL_LEVEL_FRAME_TYPE_4;
							dataPatternZeroedBytes = DiabloCel.D1CEL_LEVEL_FRAME_TYPE_4_ZEROED_BYTES;
							break;
						case DiabloCel.D1CEL_FRAME_TYPE.LEVEL_TYPE_5:
							dataPattern = DiabloCel.D1CEL_LEVEL_FRAME_TYPE_5;
							dataPatternZeroedBytes = DiabloCel.D1CEL_LEVEL_FRAME_TYPE_5_ZEROED_BYTES;
							break;
						default:
							throw new Exception("Invalid type!");
					}

					// Going through the data structure
					int offset = 0;
					int zeroedBytesIndex = 0;
					pixelLine.Clear();
					for (int i = 0; i < 512; i++)
					{
						// if dataPattern[i] is true, then read and add 2 pixels to the line
						if (dataPattern[i])
						{
							readByte = rawData[offset];
							secondReadByte = rawData[offset + 1];

							if (zeroedBytesIndex < dataPatternZeroedBytes.Length && readByte == 0x00 && secondReadByte == 0x00 && offset == dataPatternZeroedBytes[zeroedBytesIndex]) // jmarshall: added bounds check here. 
							{
								// Skip the 0x00 0x00 bytes
								offset += 2;
								// and read the next 2 bytes
								readByte = rawData[offset];
								secondReadByte = rawData[offset + 1];

								// move forward in the zeroed bytes structure
								zeroedBytesIndex += 2;
							}

							pixelLine.Add(readByte);
							pixelLine.Add(secondReadByte);

							offset += 2;
						}
						// else add 2 transparent pixels to the line
						else
						{
							pixelLine.Add(transparentIndex);
							pixelLine.Add(transparentIndex);
						}

						// If it is the end of the pixel line, add the lien to the frame
						if (pixelLine.Count == 32)
						{
							_pixels.AddRange(pixelLine);
							pixelLine.Clear();
						}
					}
				}
			}
			// if it's a regular CEL frame
			else
			{
				pixelLine.Clear();
				for (uint o = frameDataStartOffset; o < rawData.Length; o++)
				{
					readByte = rawData[o];

					// Transparent pixels group
					if (readByte > 0x7F)
					{
						// A pixel line can't exceed the image width
						if ((pixelLine.Count + (256 - readByte)) > _width)
						{
							throw new Exception("Invalid width!");
						}

						for (int i = 0; i < (256 - readByte); i++)
						{
							pixelLine.Add(transparentIndex);
						}
					}
					// Palette indices group
					else
					{
						// A pixel line can't exceed the image width
						if ((pixelLine.Count + readByte) > _width)
						{
							throw new Exception("Invalid width!");
						}

						for (int i = 0; i < readByte; i++)
						{
							o++;
							pixelLine.Add(rawData[o]); // (D1CelPixel(false, rawData[o]));
						}
					}

					if (pixelLine.Count == _width)
					{
						_pixels.AddRange(pixelLine);
						pixelLine.Clear();
					}
				}

				if (_height == 0)
				{
					_height = _pixels.Count / _width;
				}
			}
		}


		public int Width
		{
			get
			{
				return _width;
			}
		}

		public int Height
		{
			get
			{
				return _height;
			}
		}

		public byte[] Pixels
		{
			get
			{
				return _pixels.ToArray();
			}
		}

		public DiabloCel.D1CEL_FRAME_TYPE Type
		{
			get
			{
				return _type;
			}
		}

		int _width;
		int _height;
		List<byte> _pixels = new List<byte>();
		DiabloCel.D1CEL_FRAME_TYPE _type;
	}
}
