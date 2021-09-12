using System;
using System.IO;

namespace ConversionTool
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
			string outFile = Path.GetDirectoryName(tilFile) + "/megatiles.tiltext";

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
			// Dun files
			{
                string[] files = System.IO.Directory.GetFiles(path, "*.dun");

                foreach (string f in files)
                    SaveDun(f);
            }

            // sol files
            {
                string[] files = System.IO.Directory.GetFiles(path, "*.sol");

                foreach (string f in files)
                    SaveSol(f);
            }

			// til files
			{
				string[] files = System.IO.Directory.GetFiles(path, "*.til");

				foreach (string f in files)
					SaveTil(f);
			}

			// cel files
			{
				string[] minfiles = System.IO.Directory.GetFiles(path, "*.min");
				string[] tilfiles = System.IO.Directory.GetFiles(path, "*.til");
				string[] files = System.IO.Directory.GetFiles(path, "*.cel");

				ExportTileset.Export(files[0], minfiles[0], tilfiles[0]);
			}
		}

        static void Main(string[] args)
        {
			Console.WriteLine("Converting Level Data...");
			ExportLevels("build/levels/towndata");
			ExportLevels("build/levels/l1data");
			ExportLevels("build/levels/l2data");
			ExportLevels("build/levels/l3data");
			ExportLevels("build/levels/l4data");
		}
    }
}
