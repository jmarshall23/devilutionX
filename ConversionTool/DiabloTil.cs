using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace ConversionTool
{
	public enum D1TIL_TYPE
	{
		REGULAR_HEIGHT,
		EXTENDED_HEIGHT
	}

	class D1Til
	{
		public D1Til()
		{
			this.type = D1TIL_TYPE.REGULAR_HEIGHT;
			//this.file = new QFile();
			this.min = null;
			this.tileCount = 0;
			this.tileWidth = 0;
			this.tileHeight = 0;
			this.tilePixelWidth = 0;
			this.tilePixelHeight = 0;
		}

		public D1Til(string path, D1Min m)
		{
			this.type = D1TIL_TYPE.REGULAR_HEIGHT;
			//this.file = new QFile();
			this.min = m;
			this.tileCount = 0;
			this.tileWidth = 0;
			this.tileHeight = 0;
			this.tilePixelWidth = 0;
			this.tilePixelHeight = 0;			
			this.load(path);
			setMin(m);
		}

		public void Dispose()
		{
		}

		private bool load(string tilFilePath)
		{
			ushort readWord = (0);

			file = new BinaryReader(new MemoryStream(File.ReadAllBytes(tilFilePath)));

			this.tileCount = (ushort)(this.file.BaseStream.Length / 8);

			this.subtileIndices.Clear();
			for (int i = 0; i < this.tileCount; i++)
			{
				List<ushort> subtileIndicesList = new List<ushort>();

				subtileIndicesList.Clear();
				for (int j = 0; j < 4; j++)
				{
					readWord = file.ReadUInt16();
					subtileIndicesList.Add(readWord);
				}
				this.subtileIndices.Add(subtileIndicesList);
			}

			return true;
		}

		public struct ImageTemp
		{
			public byte[] data;
			public int width;
			public int height;
		}

		ImageTemp LoadSubImageTemp(int tileIndex)
		{
			ImageTemp temp = new ImageTemp();
			temp.data = min.getSubtileImage((ushort)tileIndex, ref temp.width, ref temp.height);
			return temp;
		}

		private bool IsFloor(int sol)
		{
			return (sol & 1) == 0;
		}

		public byte[] getTileImage(ushort tileIndex, int sol1, int sol2, int sol3, int sol4, bool isTown)
		{
			if (this.min == null || tileIndex >= this.subtileIndices.Count)
			{
				throw new Exception("Broken");
			}

			ImageTemp[] tempImages = new ImageTemp[4];

			for (int i = 0; i < 4; i++)
				tempImages[i] = LoadSubImageTemp(subtileIndices[tileIndex][i]);


			int width = this.tilePixelWidth;
			int height = this.tilePixelHeight;

			byte[] tile = new byte[width * height];
			for (int i = 0; i < width * height; i++)
				tile[i] = 255;

			bool isFloor = IsFloor(sol1) && IsFloor(sol2) && IsFloor(sol3) && IsFloor(sol4);
			bool forceDisableFloorRemovalIdea = true;

			if (isFloor || isTown || forceDisableFloorRemovalIdea)
			{
				ExportTileset.BlitImage(tempImages[0].data, 0, 0, tempImages[0].width, tile, 32, 0, width, height, tempImages[0].width, tempImages[0].height); 
				ExportTileset.BlitImage(tempImages[1].data, 0, 0, tempImages[1].width, tile, 64, 16, width, height, tempImages[1].width, tempImages[1].height); 
				ExportTileset.BlitImage(tempImages[2].data, 0, 0, tempImages[2].width, tile, 0, 16, width, height, tempImages[2].width, tempImages[2].height); 
				ExportTileset.BlitImage(tempImages[3].data, 0, 0, tempImages[3].width, tile, 32, 32, width, height, tempImages[3].width, tempImages[3].height); 
			}
			else
			{
				if (!IsFloor(sol1))
					ExportTileset.BlitImage(tempImages[0].data, 0, 0, tempImages[0].width, tile, 32, 0, width, height, tempImages[0].width, tempImages[0].height);

				if (!IsFloor(sol2))
					ExportTileset.BlitImage(tempImages[1].data, 0, 0, tempImages[1].width, tile, 64, 16, width, height, tempImages[1].width, tempImages[1].height);

				if (!IsFloor(sol3))
					ExportTileset.BlitImage(tempImages[2].data, 0, 0, tempImages[2].width, tile, 0, 16, width, height, tempImages[2].width, tempImages[2].height);

				if(!IsFloor(sol4))
					ExportTileset.BlitImage(tempImages[3].data, 0, 0, tempImages[3].width, tile, 32, 32, width, height, tempImages[3].width, tempImages[3].height);
			}
			

			return tile;
		}

		public void GetSubTileInfo(int tileIndex, int subTileIndex, int startY, out int x, out int y, out int width, out int height)
		{
			if (this.min == null || tileIndex >= this.subtileIndices.Count)
			{
				throw new Exception("Broken");
			}

			ImageTemp tempImage = LoadSubImageTemp(subtileIndices[tileIndex][subTileIndex]);

			x = 0;
			y = 0;
			width = 0;
			height = 0;

			switch (subTileIndex)
			{
				case 0:
					x = 32;
					y = 0;
					break;

				case 1:
					x = 64;
					y = 16;
					break;

				case 2:
					x = 0;
					y = 16;
					break;

				case 3:
					x = 32;
					y = 32;
					break;

				default:
					throw new Exception("Invalid sub index");
			}
			width = tempImage.width;
			height = tempImage.height - startY;
		}
		
		public ImageTemp[] getTileImagesTemp(ushort tileIndex)
		{
			if (this.min == null || tileIndex >= this.subtileIndices.Count)
			{
				throw new Exception("Broken");
			}

			ImageTemp[] tempImages = new ImageTemp[4];

			for (int i = 0; i < 4; i++)
				tempImages[i] = LoadSubImageTemp(subtileIndices[tileIndex][i]);

			return tempImages;
		}

		private D1TIL_TYPE getType()
		{
			return this.type;
		}



		private D1Min getMin()
		{
			return this.min;
		}

		private void setMin(D1Min m)
		{
			this.min = m;

			this.tileWidth = (ushort)(this.min.getSubtileWidth() * 2);
			this.tileHeight = (ushort)(this.min.getSubtileHeight() + 1);

			
			this.tilePixelWidth = (ushort)(this.tileWidth * 32);
			
			this.tilePixelHeight = (ushort)(this.tileHeight * 32);
		}

		public ushort getTileCount()
		{
			return (this.tileCount);
		}

		private ushort getTileWidth()
		{
			return (this.tileWidth);
		}

		private ushort getTileHeight()
		{
			return (this.tileHeight);
		}

		public ushort getTilePixelWidth()
		{
			return (this.tilePixelWidth);
		}

		public ushort getTilePixelHeight()
		{
			return (this.tilePixelHeight);
		}

		private List<ushort> getSubtileIndices(ushort tileIndex)
		{
			if (tileIndex < this.tileCount)
			{
				return this.subtileIndices[tileIndex];
			}
			else
			{
				return new List<ushort>();
			}
		}

		private D1TIL_TYPE type;
		private BinaryReader file;
		private D1Min min;
		private ushort tileCount = (0);
		private ushort tileWidth = (0);
		private ushort tileHeight = (0);
		private ushort tilePixelWidth = (0);
		private ushort tilePixelHeight = (0);
		public List<List<ushort>> subtileIndices = new List<List<ushort>>();
	}



}
