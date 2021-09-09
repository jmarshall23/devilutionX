using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
namespace DunDump
{
	public enum D1MIN_TYPE
	{
		REGULAR_HEIGHT,
		EXTENDED_HEIGHT
	}

	class D1Min 
	{
		public D1Min()
		{
			this.type = D1MIN_TYPE.REGULAR_HEIGHT;
			//this.file = new QFile();
			this.cel = null;
			this.subtileWidth = 2;
			this.subtileHeight = 5;
			this.subtileCount = 0;
		}

		public D1Min(string path, DiabloCel c)
		{
			this.type = D1MIN_TYPE.REGULAR_HEIGHT;
			//this.file = new QFile();
			this.cel = c;
			this.subtileWidth = 2;
			this.subtileHeight = 5;
			this.subtileCount = 0;
			this.load(path);
		}

		public void Dispose()
		{
			//if (this.file.isOpen())
			//{
			//	this.file.close();
			//}
		}

		private bool load(string minFilePath)
		{
			int subtileNumberOfCelFrames = 0;
			ushort readWord = new ushort();

			file = new BinaryReader(new MemoryStream(File.ReadAllBytes(minFilePath)));

			if (file.BaseStream.Length < 64)
			{
				return false;
			}

			// Determine MIN type by checking if ground CEL frames
			// of the first 2 sub-tiles are empty or not
			for (int i = 0; i < 2; i++)
			{
				file.BaseStream.Seek(18 + i * 20, SeekOrigin.Begin);
				readWord = file.ReadUInt16();

				if (readWord == 0x0000)
				{
					this.type = D1MIN_TYPE.EXTENDED_HEIGHT;
					break;
				}
			}

			// File size checks
			if (this.type == D1MIN_TYPE.REGULAR_HEIGHT)
			{
				if (this.file.BaseStream.Length % 20 != 0)
				{
					return false;
				}
				this.subtileHeight = 5;
				this.subtileCount = (ushort)(this.file.BaseStream.Length / 20);
			}
			if (this.type == D1MIN_TYPE.EXTENDED_HEIGHT)
			{
				if (this.file.BaseStream.Length % 32 != 0)
				{
					return false;
				}
				this.subtileHeight = 8;
				this.subtileCount = (ushort)(this.file.BaseStream.Length / 32);
			}

			// Read sub-tile data
			//C++ TO C# CONVERTER TODO TASK: The following line was determined to be a copy assignment (rather than a reference assignment) - this should be verified and a 'CopyFrom' method should be created:
			//ORIGINAL LINE: subtileNumberOfCelFrames = this->subtileWidth * this->subtileHeight;
			subtileNumberOfCelFrames = this.subtileWidth * this.subtileHeight;
			this.file.BaseStream.Seek(0, SeekOrigin.Begin);
			this.celFrameIndices.Clear();
			this.celFrameTypes.Clear();
			for (int i = 0; i < this.subtileCount; i++)
			{
				List<ushort> celFrameIndicesList = new List<ushort>();
				List<byte> celFrameTypesList = new List<byte>();

				celFrameIndicesList.Clear();
				celFrameTypesList.Clear();
				for (int j = 0; j < subtileNumberOfCelFrames; j++)
				{
					readWord = file.ReadUInt16();
					celFrameIndicesList.Add((ushort)(readWord & 0x0FFF));					
					celFrameTypesList.Add((byte)((readWord & 0xF000) >> 12));
				}
				this.celFrameIndices.Add(celFrameIndicesList);
				this.celFrameTypes.Add(celFrameTypesList);
			}

			return true;
		}


		public byte[] getSubtileImage(ushort subtileIndex, ref int width, ref int height)
		{
			ushort celFrameIndex = 0;
			ushort dx = 0;
			ushort dy = 0;
			byte[] subtile;

			if (this.cel == null || subtileIndex >= this.celFrameIndices.Count)
			{
				throw new Exception("Things are broken");
			}

			//List<ushort> test = this->celFrameIndices.at( subtileIndex );
			width = this.subtileWidth * 32;
			height = this.subtileHeight * 32;

			subtile = new byte[width * height];
			for (int i = 0; i < width * height; i++)
				subtile[i] = 255;

			

			for (int i = 0; i < this.subtileWidth * this.subtileHeight; i++)
			{
				celFrameIndex = this.celFrameIndices[subtileIndex][i];

				if (celFrameIndex > 0)
				{
					DiabloCelBase frame = cel.GetFrame(celFrameIndex - 1);
					// 
					ExportTileset.BlitImage(frame.Pixels, 0, 0, frame.Width, subtile, dx, dy, width, height, frame.Width, frame.Height);
				}

				if (dx == 32)
				{
					dy += 32;
					dx = 0;
				}
				else
				{
					dx = 32;
				}
			}

			return subtile;
		}

		private D1MIN_TYPE getType()
		{
			return this.type;
		}

		private DiabloCel getCel()
		{
			return this.cel;
		}

		private void setCel(DiabloCel c)
		{
			this.cel = c;
		}

		public ushort getSubtileWidth()
		{
			return subtileWidth;
		}

		public ushort getSubtileHeight()
		{
			return subtileHeight;
		}

		public ushort getSubtileCount()
		{
			return subtileCount;
		}

		private List<ushort> getCelFrameIndices(ushort subTileIndex)
		{
			if (subTileIndex < this.subtileCount)
			{
				return this.celFrameIndices[subTileIndex];
			}
			else
			{
				throw new Exception("Invalid frame index");
			}
		}

		private D1MIN_TYPE type;
		private BinaryReader file;
		private DiabloCel cel;
		private byte subtileWidth = new byte();
		private byte subtileHeight = new byte();
		private ushort subtileCount = new ushort();
		private List<List<ushort>> celFrameIndices = new List<List<ushort>>();
		private List<List<byte>> celFrameTypes = new List<List<byte>>();
	}

	//C++ TO C# CONVERTER WARNING: The original C++ declaration of the following method implementation was not found:
	//ORIGINAL LINE: D1Min::D1Min() : type(D1MIN_TYPE::REGULAR_HEIGHT), file(new QFile()), cel(NULL), subtileWidth(2), subtileHeight(5), subtileCount(0)



}
