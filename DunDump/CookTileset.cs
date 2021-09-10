using System;
using System.IO;
using System.Collections.Generic;

namespace DunDump
{
	public class CookTileset
	{
		class Image_t
		{
			public int width;
			public int height;
			public byte[] buffer;
		}

		class TilEntry_t
		{
			public int tile1;
			public int tile2;
			public int tile3;
			public int tile4;
		}

		private static Image_t LoadTilesetImage(string path)
		{
			Image_t image = new Image_t();
			byte[] buffer = File.ReadAllBytes(path);
			BinaryReader reader = new BinaryReader(new MemoryStream(buffer));

			reader.BaseStream.Position = 12;
			image.width = reader.ReadInt16();
			image.height = reader.ReadInt16();
			reader.BaseStream.Position = 18;

			byte[] data = reader.ReadBytes((int)(reader.BaseStream.Length - 18));
			image.buffer = new byte[image.width * image.height];

			for (int i = 0; i < image.width * image.height; i++)
			{
				image.buffer[i] = data[(i * 4) + 0];
			}

			return image;
		}

		private static void GenerateMinTiles(string path, Image_t image, ref int tileIndex, ref List<int> min)
		{
			int subtileWidth = image.width / 32;
			int subtileHeight = image.height / 32;
			int dx = 0;
			int dy = 0;			

			for (int i = 0; i < subtileWidth * subtileHeight; i++)
			{
				if(ExportTileset.ImageSegmentHasData(image.buffer, dx, dy, image.width, 32, 32))
				{
					byte[] tempBuffer = new byte[32 * 32];
					for (int d = 0; d < 32 * 32; d++)
						tempBuffer[d] = 255;

					ExportTileset.BlitImage2(image.buffer, dx, dy, image.width, tempBuffer, 0, 0, 32, 32, 32, 32);

					ExportTileset.WriteTGA(path + "/cooked/" + "tile" + tileIndex + ".tga", tempBuffer, 32, 32, true);

					min.Add(tileIndex++);
				}
				else
				{
					min.Add(0);
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
		}

		static void SaveTil(List<TilEntry_t> til, string path)
		{
			string outFile = path + "/megatiles.tiltext";

			Console.WriteLine("Saving til file {0}", outFile);

			using (StreamWriter writer = File.CreateText(outFile))
			{
				writer.WriteLine("index,top,right,left,bottom");

				for (int i = 0; i < til.Count; i++)
				{
					writer.WriteLine(i + "," + til[i].tile1 + "," + til[i].tile2 + "," + til[i].tile3 + "," + til[i].tile4);
				}
			}
		}

		static void SaveMin(List<int> min, string path)
		{
			string outFile = path + "/lvlpieces.mintext";

			Console.WriteLine("Saving min file {0}", outFile);

			using (StreamWriter writer = File.CreateText(outFile))
			{
				writer.WriteLine("index,celid");

				for (int i = 0; i < min.Count; i++)
				{
					writer.WriteLine(i + "," + min[i]);
				}
			}
		}

		public static void CookTiles(string path)
		{
			string[] files = System.IO.Directory.GetFiles(path + "/tiles/", "*.tga");
			List<TilEntry_t> til = new List<TilEntry_t>();
			List<int> min = new List<int>();

			Directory.CreateDirectory(path + "/cooked/");

			int index = 0;
			int tileIndex = 0;

			foreach(string f in files)
			{
				Image_t image = LoadTilesetImage(f);
				TilEntry_t tilEntry = new TilEntry_t();

				const int minSegementWidth = 64;
				const int minSegmentHeight = 256;

				Image_t[] tempImages = new Image_t[4];

				for(int i = 0; i < 4; i++)
				{
					tempImages[i] = new Image_t();

					tempImages[i].width = minSegementWidth;
					tempImages[i].height = minSegmentHeight;
					tempImages[i].buffer = new byte[tempImages[i].width * tempImages[i].height];

					for (int d = 0; d < tempImages[i].width * tempImages[i].height; d++)
						tempImages[i].buffer[d] = 255;
				}

				ExportTileset.BlitImage(image.buffer, 32, 0, image.width, tempImages[0].buffer, 0, 0, tempImages[0].width, tempImages[0].height, minSegementWidth, minSegmentHeight);
				//ExportTileset.WriteTGA(path + "/test.tga", tempImages[0].buffer, tempImages[0].width, tempImages[0].height, false);
				ExportTileset.BlitImage(image.buffer, 64, 16, image.width, tempImages[1].buffer, 0, 0, tempImages[1].width, tempImages[1].height, minSegementWidth, minSegmentHeight);
				ExportTileset.BlitImage(image.buffer, 0, 16, image.width, tempImages[2].buffer, 0, 0, tempImages[2].width, tempImages[2].height, minSegementWidth, minSegmentHeight);
				ExportTileset.BlitImage(image.buffer, 32, 32, image.width, tempImages[3].buffer, 0, 0, tempImages[3].width, tempImages[3].height, minSegementWidth, minSegmentHeight);

				for(int i = 0; i < 4; i++)
				{
					GenerateMinTiles(path, tempImages[i], ref tileIndex, ref min);
				}

				tilEntry.tile1 = (index * 4) + 1;
				tilEntry.tile2 = (index * 4) + 2;
				tilEntry.tile3 = (index * 4) + 3;
				tilEntry.tile4 = (index * 4) + 4;

				til.Add(tilEntry);
				index++;
			}

			SaveTil(til, path);
			SaveMin(min, path);
		}
	}
}
