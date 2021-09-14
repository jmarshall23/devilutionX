using System;
using System.IO;

namespace ConversionTool
{
    class Program
    {
        static void SaveDun(string filename)
        {
            string dunFile = filename;
            string outFile = ExportTileset.FixExportPath(filename) + "/" + Path.GetFileNameWithoutExtension(dunFile) + ".duntext";

            Console.WriteLine("Opening dungeon file {0}", dunFile);

            using (StreamWriter writer = File.CreateText(outFile))
            {
                using (BinaryReader reader = new BinaryReader(File.Open(dunFile, FileMode.Open)))
                {
                    int width = reader.ReadInt16();
                    int height = reader.ReadInt16();

                    writer.WriteLine("x,y,tilid");
                    //writer.WriteLine("Width," + width + ",0");
                    //writer.WriteLine("Height," + height + ",0");

                    Console.WriteLine("Width {0} Height {1}", width, height);

                    ushort[] tileLayer = new ushort[reader.BaseStream.Length - 4];
                    for (int i = 0; i < (reader.BaseStream.Length - 4) / 2; i++)
                        tileLayer[i] = reader.ReadUInt16();

                    for (int j = 0; j < height; j++)
                    {
                        for (int i = 0; i < width; i++)
                        {
                            int tileId = (tileLayer[j * width + i]) - 1;

                            writer.WriteLine(i + "," + j + "," + tileId);
                        }
                    }

                    if (reader.BaseStream.Position != reader.BaseStream.Length)
                        throw new Exception("invalid dun parse");
                }
            }
        }

        static void SaveSol(string filename)
        {
            string tilFile = filename;
            string outFile = ExportTileset.FixExportPath(filename) + "/" + Path.GetFileNameWithoutExtension(tilFile) + ".soltext";

            Console.WriteLine("Opening sol file {0}", tilFile);

            using (StreamWriter writer = File.CreateText(outFile))
            {
                using (BinaryReader reader = new BinaryReader(File.Open(tilFile, FileMode.Open)))
                {
                    writer.WriteLine("index,val");

                    long numEntries = reader.BaseStream.Length;

                    for (int i = 0; i < numEntries; i++)
                    {
                        byte val = reader.ReadByte();

                        writer.WriteLine(i + "," + val);
                    }

                    if (reader.BaseStream.Position != reader.BaseStream.Length)
                        throw new Exception("invalid sol parse");
                }
            }
        }

		static void SaveTil(string filename)
		{
			string tilFile = filename;
			string outFile = ExportTileset.FixExportPath(filename)  + "/megatiles.tiltext";

			Console.WriteLine("Opening til file {0}", tilFile);

			using (StreamWriter writer = File.CreateText(outFile))
			{
				using (BinaryReader reader = new BinaryReader(File.Open(tilFile, FileMode.Open)))
				{
					writer.WriteLine("index,top,right,left,bottom");

					long numEntries = reader.BaseStream.Length / (2 * 4);

					for (int i = 0; i < numEntries; i++)
					{
						ushort top = reader.ReadUInt16();
						ushort right = reader.ReadUInt16();
						ushort left = reader.ReadUInt16();
						ushort bottom = reader.ReadUInt16();

						writer.WriteLine(i + "," + top + "," + right + "," + left + "," + bottom);
					}

					if (reader.BaseStream.Position != reader.BaseStream.Length)
						throw new Exception("invalid til parse");
				}
			}
		}

		static void ExportLevels(string path)
		{
			string BlizzDatapath = "BlizzData/" + path;

			// Dun files
			{
                string[] files = System.IO.Directory.GetFiles(BlizzDatapath, "*.dun");

                foreach (string f in files)
                    SaveDun(f);
            }

            // sol files
            {
                string[] files = System.IO.Directory.GetFiles(BlizzDatapath, "*.sol");

                foreach (string f in files)
                    SaveSol(f);
            }

			// til files
			{
				string[] files = System.IO.Directory.GetFiles(BlizzDatapath, "*.til");

				foreach (string f in files)
					SaveTil(f);
			}

			// cel files
			{
				string[] minfiles = System.IO.Directory.GetFiles(BlizzDatapath, "*.min");
				string[] tilfiles = System.IO.Directory.GetFiles(BlizzDatapath, "*.til");
				string[] files = System.IO.Directory.GetFiles(BlizzDatapath, "*.cel");

				ExportTileset.Export(files[0], minfiles[0], tilfiles[0]);
			}
		}

		static void ConvertSingleImage(string filename, int width, int[] widthTable, int[] heightTable)
		{
			DiabloCel cel = null;

			bool isCl2 = filename.ToLower().Contains(".cl2");

			if (isCl2)
			{
				cel = new DiabloCL2("BlizzData/" + filename);
			}
			else
			{
				cel = new DiabloCel("BlizzData/" + filename, width, 0, widthTable, heightTable);
			}

			string outputPath = "Build\\" + ExportTileset.FixExportPath(filename);

			Directory.CreateDirectory(outputPath);

			string fileNameWithoutExtension = Path.GetFileNameWithoutExtension(filename);


			if (!isCl2)
			{
				for (int i = 0; i < cel.NumFrames; i++)
				{
					DiabloCelBase frame = cel.GetFrame((short)i);

					byte[] fixedBuffer = ExportTileset.FastFlipHorizontalBuffer(frame.Pixels, frame.Width, frame.Height);

					ExportTileset.WriteTGA(outputPath + "/" + fileNameWithoutExtension + "_" + i + ".tga", fixedBuffer, frame.Width, frame.Height, true);
				}
			}
			else
			{
				DiabloCelBase baseFrame = cel.GetFrame(0);

				// These have to be atlased or they create too many files and it would be slow to load off disk
				int destWidth = (cel.NumFrames * baseFrame.Width);
				byte[] tempBuffer = new byte[destWidth * baseFrame.Height];

				for (int i = 0; i < cel.NumFrames; i++)
				{
					DiabloCelBase frame = cel.GetFrame((short)i);

					byte[] fixedBuffer = ExportTileset.FastFlipHorizontalBuffer(frame.Pixels, frame.Width, frame.Height);
					ExportTileset.BlitImage2(fixedBuffer, 0, 0, frame.Width, tempBuffer, (i * baseFrame.Width), 0, destWidth, baseFrame.Height, frame.Width, frame.Height, true);
				}

				ExportTileset.WriteTGA(outputPath + "/" + fileNameWithoutExtension + ".tga", tempBuffer, destWidth, baseFrame.Height, true);
			}
		}

		static void Main(string[] args)
        {
			Console.WriteLine("Exporting Player Gfx...");
			{
				string[] playerCl2Files = System.IO.Directory.GetFiles("BlizzData\\plrgfx\\", "*.cl2", SearchOption.AllDirectories);
				foreach(string file in playerCl2Files)
				{
					string f = file.Remove(0, new string("BlizzData/").Length);
					Console.WriteLine("Processing:" + f);
					ConvertSingleImage(f, 0, null, null);
				}
			}

			Console.WriteLine("Exporting GenData Images...");
			foreach (ConversionTables.ConvEntry entry in ConversionTables.GenDataImages)
			{
				ConvertSingleImage(entry.path, entry.width, null, null);
			}

			Console.WriteLine("Exporting Data Inv...");
			foreach (ConversionTables.ConvEntry entry in ConversionTables.DataInv)
			{
				ConvertSingleImage(entry.path, entry.width, entry.widthTable, entry.heightTable);
			}

			Console.WriteLine("Exporting Control Panel...");
			foreach(ConversionTables.ConvEntry entry in ConversionTables.CtrlPan)
			{
				ConvertSingleImage(entry.path, entry.width, null, null);
			}

			Console.WriteLine("Converting Towners...");
            foreach(string path in ConversionTables.Towners)
            {
				ConvertSingleImage(path, 0, null, null);
            }

			Console.WriteLine("Converting Level Data...");
			ExportLevels("levels/towndata");
			ExportLevels("levels/l1data");
			ExportLevels("levels/l2data");
			ExportLevels("levels/l3data");
			ExportLevels("levels/l4data");
		}
    }
}
