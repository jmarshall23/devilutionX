using System;
using System.IO;

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
