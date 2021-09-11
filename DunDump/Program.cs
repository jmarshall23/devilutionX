using System;
using System.IO;
using StormLibSharp;

namespace DunDump
{
    class Program
    {
        static void SaveDun(string filename)
        {
            string dunFile = filename;
            string outFile = Path.GetDirectoryName(dunFile) + "/" + Path.GetFileNameWithoutExtension(dunFile) + ".duntext";

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
            string outFile = Path.GetDirectoryName(tilFile) + "/" + Path.GetFileNameWithoutExtension(tilFile) + ".soltext";

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
			string outFile = Path.GetDirectoryName(tilFile) + "/" + Path.GetFileNameWithoutExtension(tilFile) + ".tiltext";

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

		static void ExtractAllFilesFromFolder(string path, MpqArchive archive, string[] textFileContent)
		{
			foreach(string s in textFileContent)
			{
				string newPath = s.ToLower();
				if(s.Contains(newPath))
				{
					Directory.CreateDirectory("../build/" + Path.GetDirectoryName(s));
					Console.WriteLine("Extracting " + newPath);
					archive.ExtractFile(s, "../build/" + newPath);
				}
			}
		}

        static void Main(string[] args)
        {
			if(args.Length > 1)
			{
				if(args[0] == "-cook")
				{
					CookTileset.CookTiles(args[1]);
					return;
				}
			}

			if (args[0] == "-extract")
			{
				MpqArchive diabdat = new MpqArchive("../build/diabdat.mpq", FileAccess.Read);
				string[] textFileContent = File.ReadAllLines("diablo.txt");
				ExtractAllFilesFromFolder("level\\", diabdat, textFileContent);

				return;
			}

			// Dun files
			{
                string[] files = System.IO.Directory.GetFiles(args[0], "*.dun");

                foreach (string f in files)
                    SaveDun(f);
            }

            // sol files
            {
                string[] files = System.IO.Directory.GetFiles(args[0], "*.sol");

                foreach (string f in files)
                    SaveSol(f);
            }

			// til files
			{
				string[] files = System.IO.Directory.GetFiles(args[0], "*.til");

				foreach (string f in files)
					SaveTil(f);
			}

			// cel files
			{
				string[] minfiles = System.IO.Directory.GetFiles(args[0], "*.min");
				string[] tilfiles = System.IO.Directory.GetFiles(args[0], "*.til");
				string[] files = System.IO.Directory.GetFiles(args[0], "*.cel");

				ExportTileset.Export(files[0], minfiles[0], tilfiles[0]);
			}
		}
    }
}
