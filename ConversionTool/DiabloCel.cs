using System;
using System.IO;
using System.Collections.Generic;

namespace ConversionTool
{
	class DiabloCel
	{
		public static bool indexedCelArrayHack = false;

		public enum D1CEL_FRAME_TYPE
		{
			REGULAR,      // == LEVEL_TYPE_1
			REGULAR_DATASKIP,
			LEVEL_TYPE_0, // 0x400 full opaque
			LEVEL_TYPE_2, // 0x220 left transparency
			LEVEL_TYPE_3, // 0x220 right transparency
			LEVEL_TYPE_4, // 0x320 left transparency
			LEVEL_TYPE_5  // 0x320 right transparency
		};

		public enum D1CEL_TYPE
		{
			NONE,
			V1_REGULAR,
			V1_COMPILATION,
			V1_LEVEL,
			V2_MONO_GROUP,
			V2_MULTIPLE_GROUPS
		};

		protected List<Tuple<UInt16, UInt16>> groupFrameIndices = new List<Tuple<ushort, ushort>>();
		protected List<Tuple<UInt32, UInt32>> frameOffsets = new List<Tuple<uint, uint>>();
		protected List<DiabloCelBase> frames = new List<DiabloCelBase>();

		protected D1CEL_TYPE type = D1CEL_TYPE.NONE;
		protected uint groupCount = 0;
		protected int frameCount = 0;

		public int NumFrames
		{
			get
			{
				return frames.Count;
			}
		}

		public DiabloCelBase GetFrame(int index)
		{
			return frames[index];
		}

		protected DiabloCel()
		{

		}

		public DiabloCel(string fileName, int width = 0, int height = 0, int[] widthTable = null, int[] heightTable = null, bool forceFrameHeaderSkip = false)
		{
			byte[] buffer = File.ReadAllBytes(fileName);
			Parse(new BinaryReader(new MemoryStream(buffer)), width, height, widthTable, heightTable, forceFrameHeaderSkip);
		}

		public static readonly bool[] D1CEL_LEVEL_FRAME_TYPE_2 = { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, true, false, false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, false, false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };

		public static readonly ushort[] D1CEL_LEVEL_FRAME_TYPE_2_ZEROED_BYTES = { 0, 1, 8, 9, 24, 25, 48, 49, 80, 81, 120, 121, 168, 169, 224, 225, 288, 289, 348, 349, 400, 401, 444, 445, 480, 481, 508, 509, 528, 529, 540, 541 };

		public static readonly bool[] D1CEL_LEVEL_FRAME_TYPE_3 = { true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, true, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, false, false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, false, false, false, false, false, false, false, false, false, false, true, true, true, true, true, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };

		public static readonly ushort[] D1CEL_LEVEL_FRAME_TYPE_3_ZEROED_BYTES = { 2, 3, 14, 15, 34, 35, 62, 63, 98, 99, 142, 143, 194, 195, 254, 255, 318, 319, 374, 375, 422, 423, 462, 463, 494, 495, 518, 519, 534, 535, 542, 543 };

		public static readonly bool[] D1CEL_LEVEL_FRAME_TYPE_4 = { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, true, false, false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true };

		public static readonly ushort[] D1CEL_LEVEL_FRAME_TYPE_4_ZEROED_BYTES = { 0, 1, 8, 9, 24, 25, 48, 49, 80, 81, 120, 121, 168, 169, 224, 225 };

		public static readonly bool[] D1CEL_LEVEL_FRAME_TYPE_5 = { true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, true, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, false, false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true };

		public static readonly ushort[] D1CEL_LEVEL_FRAME_TYPE_5_ZEROED_BYTES = { 2, 3, 14, 15, 34, 35, 62, 63, 98, 99, 142, 143, 194, 195, 254, 255 };

		D1CEL_FRAME_TYPE getLevelFrame220Type(byte[] rawFrameData )
		{
			D1CEL_FRAME_TYPE type = D1CEL_FRAME_TYPE.REGULAR;
			byte readByte = 0;

			for (int i = 0; i < 32; i++)
			{
				readByte = rawFrameData[D1CEL_LEVEL_FRAME_TYPE_2_ZEROED_BYTES[i]];
				if (readByte != 0)
					break;
				if (i == 31)
					type = D1CEL_FRAME_TYPE.LEVEL_TYPE_2;
			}

			for (int i = 0; i < 32; i++)
			{
				readByte = rawFrameData[D1CEL_LEVEL_FRAME_TYPE_3_ZEROED_BYTES[i]];
				if (readByte != 0)
					break;
				if (i == 31)
					type = D1CEL_FRAME_TYPE.LEVEL_TYPE_3;
			}

			return type;
		}

		D1CEL_FRAME_TYPE getLevelFrame320Type(byte[] rawFrameData )
		{
			D1CEL_FRAME_TYPE type = D1CEL_FRAME_TYPE.REGULAR;
			byte readByte = 0;

			for (int i = 0; i < 16; i++)
			{
				readByte = rawFrameData[D1CEL_LEVEL_FRAME_TYPE_4_ZEROED_BYTES[i]];
				if (readByte != 0)
					break;
				if (i == 15)
					type = D1CEL_FRAME_TYPE.LEVEL_TYPE_4;
			}

			for (int i = 0; i < 16; i++)
			{
				readByte = rawFrameData[D1CEL_LEVEL_FRAME_TYPE_5_ZEROED_BYTES[i]];
				if (readByte != 0)
					break;
				if (i == 15)
					type = D1CEL_FRAME_TYPE.LEVEL_TYPE_5;
			}

			return type;
		}

		D1CEL_FRAME_TYPE getLevelFrame400Type(byte[] rawFrameData )
		{
			byte readByte = 0;
			uint globalPixelCount = 0;
			int pixelCount = 0;

			// Going through the frame data to find pixel groups
			for (int o = 0; o < rawFrameData.Length; o++)
			{
				readByte = rawFrameData[o];

				// Transparent pixels group
				if (readByte > 0x80)
				{
					pixelCount += (256 - readByte);
					globalPixelCount += (uint)pixelCount;
					if (pixelCount > 32)
						return D1CEL_FRAME_TYPE.LEVEL_TYPE_0;
					pixelCount = 0;
				}
				else if (readByte == 0x80)
				{
					pixelCount += 0x80;
					if (pixelCount > 32)
						return D1CEL_FRAME_TYPE.LEVEL_TYPE_0;
				}
				// Palette indices pixel group
				else if (readByte == 0x7F)
				{
					pixelCount += 0x7F;
					if (pixelCount > 32 || o + 0x7F >= rawFrameData.Length)
						return D1CEL_FRAME_TYPE.LEVEL_TYPE_0;
					o += 0x7F;
				}
				else
				{
					pixelCount += readByte;
					globalPixelCount += (uint)pixelCount;
					if (pixelCount > 32 || o + readByte >= rawFrameData.Length)
						return D1CEL_FRAME_TYPE.LEVEL_TYPE_0;
					pixelCount = 0;
					o += readByte;
				}
			}

			if (globalPixelCount == 32 * 32)
				return D1CEL_FRAME_TYPE.REGULAR;
			else
				return D1CEL_FRAME_TYPE.LEVEL_TYPE_0;
		}


		private void Parse(BinaryReader reader, int width, int height, int[] widthTable, int[] heightTable, bool forceFrameHeaderSkip)
		{
			uint firstDword = 0;
			uint fileSizeDword = 0;
			uint lastCelOffset = 0;
			uint lastCelFrameCount = 0;
			uint lastCelSize = 0;
			uint celOffset = 0;
			uint celFrameCount = 0;
			uint celFrameStartOffset = 0;
			uint celFrameEndOffset = 0;
			uint celFrameSize = 0;
			uint level0x400FrameCount = 0;
			uint level0x320FrameCount = 0;
			uint level0x220FrameCount = 0;

			firstDword = reader.ReadUInt32();

			if (reader.BaseStream.Length < (4 + firstDword * 4 + 4))
				throw new Exception("Invalid cel file length");

			reader.BaseStream.Seek(firstDword * 4 + 4, SeekOrigin.Begin);

			fileSizeDword = reader.ReadUInt32();

			// If the dword is not equal to the file size then
			// check if it's a CEL compilation
			if (reader.BaseStream.Length != fileSizeDword)
			{
				// Read offset of the last CEL of the CEL compilation
				reader.BaseStream.Seek(firstDword - 4, SeekOrigin.Begin);
				lastCelOffset = reader.ReadUInt32();

				// Go to last CEL of the CEL compilation
				if (reader.BaseStream.Length < (lastCelOffset + 8))
					throw new Exception("Invalid cel file length");

				reader.BaseStream.Seek(lastCelOffset, SeekOrigin.Begin);

				// Read last CEL header
				lastCelFrameCount = reader.ReadUInt32();

				// Read the last CEL size
				if (reader.BaseStream.Length < (lastCelOffset + 4 + lastCelFrameCount * 4 + 4))
					throw new Exception("Invalid cel file length");

				reader.BaseStream.Seek(lastCelOffset + 4 + lastCelFrameCount * 4, SeekOrigin.Begin);
				lastCelSize = reader.ReadUInt32();

				// If the last CEL size plus the last CEL offset is equal to
				// the file size then it's a CEL compilation
				if (reader.BaseStream.Length  == (lastCelOffset + lastCelSize))
				{
					type = D1CEL_TYPE.V1_COMPILATION;
					groupCount = firstDword / 4;
				}
				else
					throw new Exception("Invalid cel file length");
			}
			else
			{
				type = D1CEL_TYPE.V1_REGULAR;
				groupCount = 1;
			}

			groupFrameIndices.Clear();
			frameOffsets.Clear();
			if (type == D1CEL_TYPE.V1_COMPILATION)
			{
				// Going through all CELs
				for (uint i = 0; i * 4 < firstDword; i++)
				{
					reader.BaseStream.Seek(i * 4, SeekOrigin.Begin);
					celOffset = reader.ReadUInt32();

					reader.BaseStream.Seek(celOffset, SeekOrigin.Begin);
					celFrameCount = reader.ReadUInt32();

					groupFrameIndices.Add(new Tuple<UInt16, UInt16>((UInt16)frameOffsets.Count, (UInt16)(frameOffsets.Count + celFrameCount - 1)));

					// Going through all frames of the CEL
					for (uint j = 1; j <= celFrameCount; j++)
					{
						celFrameStartOffset = 0;
						celFrameEndOffset = 0;

						reader.BaseStream.Seek(celOffset + j * 4, SeekOrigin.Begin);
						celFrameStartOffset = reader.ReadUInt32();
						celFrameEndOffset = reader.ReadUInt32();

						frameOffsets.Add(new Tuple<UInt32, UInt32>(celOffset + celFrameStartOffset, celOffset + celFrameEndOffset));						
					}
				}
			}
			else
			{
				// Going through all frames of the CEL
				for (uint i = 1; i <= firstDword; i++)
				{
					celFrameStartOffset = 0;
					celFrameEndOffset = 0;

					reader.BaseStream.Seek(i * 4, SeekOrigin.Begin);
					celFrameStartOffset = reader.ReadUInt32();
					celFrameEndOffset = reader.ReadUInt32();

					frameOffsets.Add(new Tuple<UInt32, UInt32>(celFrameStartOffset, celFrameEndOffset));
					
					// Level CEL Check
					celFrameSize = celFrameEndOffset - celFrameStartOffset;
					if (celFrameSize == 0x400)
						level0x400FrameCount++;
					if (celFrameSize == 0x320)
						level0x320FrameCount++;
					if (celFrameSize == 0x220)
						level0x220FrameCount++;
				}

				// If there is more than 50 frames of 0x400, 0x320 and 0x220 size, then
				// it's a level frame.
				if (level0x400FrameCount + level0x320FrameCount + level0x220FrameCount > 512)
					type = D1CEL_TYPE.V1_LEVEL;
			}

			if (frameOffsets.Count != 0)
				frameCount = frameOffsets.Count;
			else
				throw new Exception("Empty frame offsets!");

			// BUILDING {CEL FRAMES}
			frames.Clear();
			for (int i = 0; i < frameOffsets.Count; i++)
			{
				celFrameSize = frameOffsets[i].Item2 - frameOffsets[i].Item1;
				reader.BaseStream.Seek(frameOffsets[i].Item1, SeekOrigin.Begin);
				byte[] celFrameRawData = reader.ReadBytes((int)celFrameSize);// fileBuffer.read(celFrameSize);

				// If it's not a level CEL
				if (type != D1CEL_TYPE.V1_LEVEL)
				{
					if (widthTable != null)
					{
						if(!indexedCelArrayHack)
						{
							frames.Add(new DiabloCelBase(celFrameRawData, widthTable[i], heightTable[i], D1CEL_FRAME_TYPE.REGULAR_DATASKIP));
						}
						else
						{
							frames.Add(new DiabloCelBase(celFrameRawData, widthTable[i], heightTable[i], D1CEL_FRAME_TYPE.REGULAR));
						}
					}
					else
					{
						if(forceFrameHeaderSkip)
						{
							frames.Add(new DiabloCelBase(celFrameRawData, width, height, D1CEL_FRAME_TYPE.REGULAR_DATASKIP));
						}
						else
						{
							frames.Add(new DiabloCelBase(celFrameRawData, width, height, D1CEL_FRAME_TYPE.REGULAR));
						}
					}
				}
				// If it's a level CEL
				else
				{
					D1CEL_FRAME_TYPE celFrameType = D1CEL_FRAME_TYPE.REGULAR;

					switch (celFrameSize)
					{
						case 0x400:
							celFrameType = getLevelFrame400Type(celFrameRawData);
							break;
						case 0x320:
							celFrameType = getLevelFrame320Type(celFrameRawData);
							break;
						case 0x220:
							celFrameType = getLevelFrame220Type(celFrameRawData);
							break;
						default:
							break;
					}

					frames.Add(new DiabloCelBase(celFrameRawData, 32, 32, celFrameType));
				}
			}
		}
	}
}
