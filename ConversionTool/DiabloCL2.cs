using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConversionTool
{
	class DiabloCL2 : DiabloCel
	{
		public DiabloCL2(string fileName)
		{
			byte[] buffer = File.ReadAllBytes(fileName);
			Load(new BinaryReader(new MemoryStream(buffer)));
		}

		private bool Load(BinaryReader file)
		{
			uint firstDword = 0;
			uint fileSizeDword = 0;
			uint lastCl2GroupHeaderOffset = 0;
			uint lastCl2GroupFrameCount = 0;

			uint cl2GroupOffset = 0;
			uint cl2GroupFrameCount = 0;
			uint cl2FrameStartOffset = 0;
			uint cl2FrameEndOffset = 0;

			uint cl2FrameSize = 0;

			byte[] cl2FrameRawData;

			// CL2 HEADER CHECKS

			// Read first DWORD
			firstDword = file.ReadUInt32();

			// Trying to find file size in CL2 header
			if (file.BaseStream.Length < (firstDword * 4 + 4 + 4))
				return false;

			file.BaseStream.Seek(firstDword * 4 + 4, SeekOrigin.Begin);
			fileSizeDword = file.ReadUInt32();

			// If the dword is not equal to the file size then
			// check if it's a CL2 with multiple groups
			if (file.BaseStream.Length != fileSizeDword)
			{
				// Read offset of the last CL2 group header
				file.BaseStream.Seek(firstDword - 4, SeekOrigin.Begin);
				lastCl2GroupHeaderOffset = file.ReadUInt32();

				// Read the number of frames of the last CL2 group
				if (file.BaseStream.Length < lastCl2GroupHeaderOffset)
					return false;

				file.BaseStream.Seek(lastCl2GroupHeaderOffset, SeekOrigin.Begin);
				lastCl2GroupFrameCount = file.ReadUInt32();

				// Read the last frame offset corresponding to the file size
				if (file.BaseStream.Length
					< lastCl2GroupHeaderOffset + lastCl2GroupFrameCount * 4 + 4 + 4)
					return false;

				file.BaseStream.Seek(lastCl2GroupHeaderOffset + lastCl2GroupFrameCount * 4 + 4, SeekOrigin.Begin);
				fileSizeDword = file.ReadUInt32();
				// The offset is from the beginning of the last group header
				// so we need to add the offset of the lasr group header
				// to have an offset from the beginning of the file
				fileSizeDword += lastCl2GroupHeaderOffset;

				if (file.BaseStream.Length == fileSizeDword)
				{
					type = D1CEL_TYPE.V2_MULTIPLE_GROUPS;
					groupCount = firstDword / 4;
				}
				else
					return false;
			}
			else
			{
				type = D1CEL_TYPE.V2_MONO_GROUP;
				groupCount = 1;
			}

			// CL2 FRAMES OFFSETS CALCULATION

			frameOffsets.Clear();
			if (type == D1CEL_TYPE.V2_MULTIPLE_GROUPS)
			{
				// Going through all groups
				for (int i = 0; i * 4 < firstDword; i++)
				{
					file.BaseStream.Seek(i * 4, SeekOrigin.Begin);
					cl2GroupOffset = file.ReadUInt32();

					file.BaseStream.Seek(cl2GroupOffset, SeekOrigin.Begin);
					cl2GroupFrameCount = file.ReadUInt32();

					groupFrameIndices.Add(new Tuple<UInt16, UInt16>((UInt16)frameOffsets.Count, (UInt16)(frameOffsets.Count + cl2GroupFrameCount - 1)));

					// Going through all frames of the group
					for (int j = 1; j <= cl2GroupFrameCount; j++)
					{
						cl2FrameStartOffset = 0;
						cl2FrameEndOffset = 0;

						file.BaseStream.Seek(cl2GroupOffset + j * 4, SeekOrigin.Begin);
						cl2FrameStartOffset = file.ReadUInt32();
						cl2FrameEndOffset = file.ReadUInt32();

						frameOffsets.Add(new Tuple<UInt32, UInt32>(cl2GroupOffset + cl2FrameStartOffset, cl2GroupOffset + cl2FrameEndOffset));
					}
				}
			}
			else
			{
				// Going through all frames of the only group
				for (int i = 1; i <= firstDword; i++)
				{
					cl2FrameStartOffset = 0;
					cl2FrameEndOffset = 0;

					file.BaseStream.Seek(i * 4, SeekOrigin.Begin);
					cl2FrameStartOffset = file.ReadUInt32();
					cl2FrameEndOffset = file.ReadUInt32();

					frameOffsets.Add(new Tuple<UInt32, UInt32>(cl2FrameStartOffset, cl2FrameEndOffset));

				}
			}

			frameCount = frameOffsets.Count;

			if (frameCount == 0)
				throw new Exception("FrameCount == 0");

			// BUILDING {CL2 FRAMES}

			frames.Clear();
			for (int i = 0; i < frameOffsets.Count; i++)
			{
				cl2FrameSize = frameOffsets[i].Item2 - frameOffsets[i].Item1;
				file.BaseStream.Seek(frameOffsets[i].Item1, SeekOrigin.Begin);
				cl2FrameRawData = file.ReadBytes((int)cl2FrameSize);

				frames.Add(new DiabloCL2Frame(cl2FrameRawData, 0, 0));
			}

			return true;
		}
	}
}
