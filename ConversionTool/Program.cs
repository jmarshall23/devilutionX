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

			Directory.CreateDirectory(Path.GetDirectoryName(outFile));

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

				string[] pal = System.IO.Directory.GetFiles(BlizzDatapath, "*.pal");

				foreach(string p in pal)
				{
					ExportTileset.SetColorPalette(p);					
					ExportTileset.Export(files[0], minfiles[0], tilfiles[0]);
				}
				

				if (files.Length > 1)
				{
					if (files[1].Contains("town"))
						ConvertSingleImage(files[1].Remove(0, new string("BlizzData/").Length), 64, null, null, false, false, "special");
					else
						ConvertSingleImage(files[1].Remove(0, new string("BlizzData/").Length), 64, null, null, false, true, "special");
				}
			}
		}

		static void ConvertSingleImage(string filename, int width, int[] widthTable, int[] heightTable, bool forceAtlas = false, bool forceFrameHeaderSkip = false, string newOutputFolder = "")
		{
			DiabloCel cel = null;

			bool isCl2 = filename.ToLower().Contains(".cl2");

			if (isCl2)
			{
				cel = new DiabloCL2("BlizzData/" + filename);

				if (cel.NumFrames == 0)
				{
					Console.WriteLine("Warning invalid cl2 file!");
					return;
				}
			}
			else
			{
				cel = new DiabloCel("BlizzData/" + filename, width, 0, widthTable, heightTable, forceFrameHeaderSkip);
			}

			string outputPath = "Build\\" + ExportTileset.FixExportPath(filename);

			if(newOutputFolder != "")
			{
				outputPath += "\\" + newOutputFolder;
			}

			Directory.CreateDirectory(outputPath);

			string fileNameWithoutExtension = Path.GetFileNameWithoutExtension(filename);


			if (!isCl2 && !forceAtlas)
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

				string description = "" + cel.NumFrames;
				File.WriteAllText(outputPath + "/" + fileNameWithoutExtension + ".txt", description);
			}
		}

		static void Main(string[] args)
        {
			Console.WriteLine("Exporting UI...");
			{
				string[] uiPCXFiles = System.IO.Directory.GetFiles("BlizzData\\ui_art\\", "*.pcx", SearchOption.AllDirectories);
				foreach (string file in uiPCXFiles)
				{
					if (file == "BlizzData\\ui_art\\listbox.pcx")
						continue;

					if (file == "BlizzData\\ui_art\\list_gry.pcx")
						continue;

					string f = file.Remove(0, new string("BlizzData/").Length);
					Console.WriteLine("Processing:" + f);

					int width = 0;
					int height = 0;

					byte[] data = DiabloPCX.LoadPCX32(file, out width, out height);

					Directory.CreateDirectory("Build\\" + Path.GetDirectoryName(f));
					string filename = "Build\\" + f;
					filename = Path.ChangeExtension(filename, ".tga");
					DiabloPCX.WriteConvertedPCX2TGA(filename, data, width, height);
				}
			}

			ExportTileset.SetColorPalette("BlizzData\\Levels\\TownData\\Town.pal");

			Console.WriteLine("Exporting Fonts...");
			foreach (ConversionTables.ConvEntry entry in ConversionTables.fonts)
			{
				ConvertSingleImage(entry.path, entry.width, null, null);
			}

			Console.WriteLine("Exporting In Game Menu Items...");
			foreach (ConversionTables.ConvEntry entry in ConversionTables.InGameMenuAssets)
			{
				ConvertSingleImage(entry.path, entry.width, null, null);
			}

			Console.WriteLine("Exporting misc control items...");
			DiabloCel.indexedCelArrayHack = true;
			foreach (ConversionTables.ConvEntry entry in ConversionTables.MiscControlUI)
			{
				ConvertSingleImage(entry.path, entry.width, entry.widthTable, entry.heightTable, false, entry.skipHeader);
			}
			DiabloCel.indexedCelArrayHack = false;
			Console.WriteLine("Exporting Objects...");
			{
				string[] playerCl2Files = System.IO.Directory.GetFiles("BlizzData\\objects\\", "*.cel", SearchOption.AllDirectories);
				foreach (string file in playerCl2Files)
				{
					string f = file.Remove(0, new string("BlizzData/").Length);
					Console.WriteLine("Processing:" + f);
					ConvertSingleImage(f, 0, null, null, true);
				}
			}

			Console.WriteLine("Exporting Missiles...");
			{
				string[] missileFiles = System.IO.Directory.GetFiles("BlizzData\\missiles\\", "*.cl2", SearchOption.AllDirectories);
				foreach (string file in missileFiles)
				{
					string f = file.Remove(0, new string("BlizzData/").Length);
					Console.WriteLine("Processing:" + f);
					ConvertSingleImage(f, 0, null, null);
				}
			}

			Console.WriteLine("Exporting Items...");
			foreach (ConversionTables.ConvEntry entry in ConversionTables.ItemPaths)
			{
				ConvertSingleImage(entry.path, entry.width, null, null, true, true);
			}

			Console.WriteLine("Exporting Monsters...");
			{
				string[] playerCl2Files = System.IO.Directory.GetFiles("BlizzData\\monsters\\", "*.cl2", SearchOption.AllDirectories);
				foreach (string file in playerCl2Files)
				{
					string f = file.Remove(0, new string("BlizzData/").Length);
					Console.WriteLine("Processing:" + f);
					ConvertSingleImage(f, 0, null, null);
				}
			}

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
				string palette = Path.ChangeExtension(entry.path, ".pal");
				ExportTileset.SetColorPalette("BlizzData\\" + palette);
				ConvertSingleImage(entry.path, entry.width, null, null);
			}

			ExportTileset.SetColorPalette("BlizzData\\Levels\\TownData\\Town.pal");

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
			ExportTileset.SetColorPalette("BlizzData\\Levels\\TownData\\Town.pal");
			ExportLevels("levels/towndata");
			ExportLevels("levels/l1data");
			ExportLevels("levels/l2data");
			ExportLevels("levels/l3data");
			ExportLevels("levels/l4data");
		}
    }
}
