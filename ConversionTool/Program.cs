using System;
using System.IO;

namespace ConversionTool
{
    class Program
    {

		public static bool exportingTileset = false;
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

		static void ExportLevels(string path)
		{
			string BlizzDatapath = "mpq_data/" + path;

			// Dun files
			{
                string[] files = System.IO.Directory.GetFiles(BlizzDatapath, "*.dun");

                foreach (string f in files)
                    SaveDun(f);
            }

			// cel files
			{
				string[] minfiles = System.IO.Directory.GetFiles(BlizzDatapath, "*.min");
				string[] tilfiles = System.IO.Directory.GetFiles(BlizzDatapath, "*.til");
				string[] files = System.IO.Directory.GetFiles(BlizzDatapath, "*.cel");
				string[] pal = System.IO.Directory.GetFiles(BlizzDatapath, "*.pal");
				string[] solFiles = System.IO.Directory.GetFiles(BlizzDatapath, "*.sol");
				string[] ampfiles = System.IO.Directory.GetFiles(BlizzDatapath, "*.amp");

				DiabloAMP ampFile = null;

				if(ampfiles.Length > 0)
					ampFile = new DiabloAMP(ampfiles[0]);

				foreach (string p in pal)
				{
					if (p.Contains("palg"))
						continue;

					ExportTileset.SetColorPalette(p);					
					ExportTileset.Export(files[0], minfiles[0], tilfiles[0], solFiles[0], ampFile);

					if (files.Length > 1)
					{
						if (files[1].Contains("town"))
							ConvertSingleImage(files[1].Remove(0, new string("mpq_data/").Length), 64, null, null, false, false, "special", "special");
						else
							ConvertSingleImage(files[1].Remove(0, new string("mpq_data/").Length), 64, null, null, false, true, "special", "special");
					}

					break;
				}
			}
		}

		static void ConvertSingleImage(string filename, int width, int[] widthTable, int[] heightTable, bool forceAtlas = false, bool forceFrameHeaderSkip = false, string newOutputFolder = "", string fileNameOverride = "")
		{
			DiabloCel cel = null;

			bool isCl2 = filename.ToLower().Contains(".cl2");

			if (isCl2)
			{
				cel = new DiabloCL2("mpq_data/" + filename);

				if (cel.NumFrames == 0)
				{
					Console.WriteLine("Warning invalid cl2 file!");
					return;
				}
			}
			else
			{
				cel = new DiabloCel("mpq_data/" + filename, width, 0, widthTable, heightTable, forceFrameHeaderSkip);
			}

			string outputPath = "base\\" + ExportTileset.FixExportPath(filename);

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

					if(fileNameOverride != "")
					{
						ExportTileset.WriteTGA(outputPath + "/" + fileNameOverride + i + ".tga", fixedBuffer, frame.Width, frame.Height, true);
					}
					else
					{
						ExportTileset.WriteTGA(outputPath + "/" + fileNameWithoutExtension + "_" + i + ".tga", fixedBuffer, frame.Width, frame.Height, true);
					}
				}
			}
			else
			{
				DiabloCelBase baseFrame = cel.GetFrame(0);

				// These have to be atlased or they create too many files and it would be slow to load off disk
				int destWidth = (cel.NumFrames * baseFrame.Width);
				byte[] tempBuffer = new byte[destWidth * baseFrame.Height];

				for (int i = 0,d = cel.NumFrames - 1; i < cel.NumFrames; i++, d--)
				{
					DiabloCelBase frame = cel.GetFrame((short)d);

					byte[] fixedBuffer = ExportTileset.FastFlipHorizontalBuffer(frame.Pixels, frame.Width, frame.Height);
					ExportTileset.BlitImage2(fixedBuffer, 0, 0, frame.Width, tempBuffer, (i * baseFrame.Width), 0, destWidth, baseFrame.Height, frame.Width, frame.Height, true);
				}

				ExportTileset.WriteTGA(outputPath + "/" + fileNameWithoutExtension + ".tga", tempBuffer, destWidth, baseFrame.Height, true);

				string description = "" + cel.NumFrames;
				File.WriteAllText(outputPath + "/" + fileNameWithoutExtension + ".txt", description);
			}
		}

		static void CopySoundFiles()
		{
			{
				string[] soundFiles = System.IO.Directory.GetFiles("mpq_data\\sfx\\", "*.wav", SearchOption.AllDirectories);
				foreach (string file in soundFiles)
				{
					string newPath = ExportTileset.FixExportPath(file);

					Directory.CreateDirectory(newPath);

					File.Copy(file, newPath + "\\" + Path.GetFileName(file), true);
				}
			}

			{
				string[] soundFiles = System.IO.Directory.GetFiles("mpq_data\\monsters\\", "*.wav", SearchOption.AllDirectories);
				foreach (string file in soundFiles)
				{
					string newPath = ExportTileset.FixExportPath(file);

					Directory.CreateDirectory(newPath);

					File.Copy(file, newPath + "\\" + Path.GetFileName(file), true);
				}
			}

			{
				string[] soundFiles = System.IO.Directory.GetFiles("mpq_data\\music\\", "*.wav", SearchOption.AllDirectories);
				foreach (string file in soundFiles)
				{
					string newPath = ExportTileset.FixExportPath(file);

					Directory.CreateDirectory(newPath);

					File.Copy(file, newPath + "\\" + Path.GetFileName(file), true);
				}
			}
		}

		static void CopyUIBin()
		{
			string[] soundFiles = System.IO.Directory.GetFiles("mpq_data\\ui_art\\", "*.bin", SearchOption.AllDirectories);
			foreach (string file in soundFiles)
			{
				string newPath = ExportTileset.FixExportPath(file);

				Directory.CreateDirectory(newPath);

				File.Copy(file, newPath + "\\" + Path.GetFileName(file), true);
			}
		}

		static void CopyMovies()
		{
			string[] movieFiles = System.IO.Directory.GetFiles("mpq_data\\gendata\\", "*.smk", SearchOption.AllDirectories);
			foreach (string file in movieFiles)
			{
				Directory.CreateDirectory("base\\movies");

				File.Copy(file, "base\\movies\\" + Path.GetFileName(file), true);
			}
		}

		static void Main(string[] args)
        {
			Console.WriteLine("Copying movie files");
			CopyMovies();

			Console.WriteLine("Copying UI bin files");
			CopyUIBin();

			Console.WriteLine("Copy sound files...");
			CopySoundFiles();

			Console.WriteLine("Exporting UI...");
			{
				string[] uiPCXFiles = System.IO.Directory.GetFiles("mpq_data\\ui_art\\", "*.pcx", SearchOption.AllDirectories);
				foreach (string file in uiPCXFiles)
				{
					if (file == "mpq_data\\ui_art\\listbox.pcx")
						continue;

					if (file == "mpq_data\\ui_art\\list_gry.pcx")
						continue;

					string f = file.Remove(0, new string("mpq_data/").Length);
					Console.WriteLine("Processing:" + f);

					int width = 0;
					int height = 0;

					byte[] data = DiabloPCX.LoadPCX32(file, out width, out height);

					Directory.CreateDirectory("base\\" + Path.GetDirectoryName(f));
					string filename = "base\\" + f;
					filename = Path.ChangeExtension(filename, ".tga");
					DiabloPCX.WriteConvertedPCX2TGA(filename, data, width, height);
				}
			}

			ExportTileset.SetColorPalette("mpq_data\\Levels\\TownData\\Town.pal");
			ExportTileset.DumpPaletteToTextFile("mpq_data\\Levels\\TownData\\Town.pal");

			Console.WriteLine("Exporting store graphics...");
			foreach (ConversionTables.ConvEntry entry in ConversionTables.store)
			{
				ConvertSingleImage(entry.path, entry.width, null, null);
			}

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
				string[] playerCl2Files = System.IO.Directory.GetFiles("mpq_data\\objects\\", "*.cel", SearchOption.AllDirectories);
				foreach (string file in playerCl2Files)
				{
					string f = file.Remove(0, new string("mpq_data/").Length);
					Console.WriteLine("Processing:" + f);
					ConvertSingleImage(f, 0, null, null, true);
				}
			}

			Console.WriteLine("Exporting Missiles...");
			{
				string[] missileFiles = System.IO.Directory.GetFiles("mpq_data\\missiles\\", "*.cl2", SearchOption.AllDirectories);
				foreach (string file in missileFiles)
				{
					string f = file.Remove(0, new string("mpq_data/").Length);
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
				string[] playerCl2Files = System.IO.Directory.GetFiles("mpq_data\\monsters\\", "*.cl2", SearchOption.AllDirectories);
				foreach (string file in playerCl2Files)
				{
					string f = file.Remove(0, new string("mpq_data/").Length);
					Console.WriteLine("Processing:" + f);
					ConvertSingleImage(f, 0, null, null);
				}
			}

			Console.WriteLine("Exporting Player Gfx...");
			{
				string[] playerCl2Files = System.IO.Directory.GetFiles("mpq_data\\plrgfx\\", "*.cl2", SearchOption.AllDirectories);
				foreach(string file in playerCl2Files)
				{
					string f = file.Remove(0, new string("mpq_data/").Length);
					Console.WriteLine("Processing:" + f);
					ConvertSingleImage(f, 0, null, null);
				}
			}

			Console.WriteLine("Exporting GenData Images...");
			foreach (ConversionTables.ConvEntry entry in ConversionTables.GenDataImages)
			{
				string palette = Path.ChangeExtension(entry.path, ".pal");
				ExportTileset.SetColorPalette("mpq_data\\" + palette);
				ConvertSingleImage(entry.path, entry.width, null, null);
			}

			ExportTileset.SetColorPalette("mpq_data\\Levels\\TownData\\Town.pal");

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
			Program.exportingTileset = true;
			ExportTileset.SetColorPalette("mpq_data\\Levels\\TownData\\Town.pal");
			ExportLevels("levels/towndata");
			ExportLevels("levels/l1data");
			ExportLevels("levels/l2data");
			ExportLevels("levels/l3data");
			ExportLevels("levels/l4data");
			ExportLevels("nlevels/l5data");
			ExportLevels("nlevels/l6data");
		}
    }
}
