using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DunDump
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
				throw new Exception("Invalid width!");
				// Checking the presence of the {CEL FRAME HEADER}
				//if (rawData[0] == 0x0A && rawData[1] == 0x00)
				//{
				//	frameDataStartOffset += 0x0A;
				//	// If header is present, try to compute frame width from frame header
				//	this.width = this.computeWidthFromHeader(rawData);
				//}
				//
				//// If width could not be calculated with frame header,
				//// attempt to calculate it from the frame data (by identifying pixel groups line wraps)
				//if (this.width == 0)
				//{
				//	this.width = this.computeWidthFromData(rawData);
				//}
				//
				//// if CEL width was not found, return false
				//if (this.width == 0)
				//{
				//	return false;
				//}
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
					throw new Exception("invlaid height!");
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
