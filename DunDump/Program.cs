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

                    for(int i = 0; i < numEntries; i++)
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

		enum TileType
		{
			Square,
			TransparentSquare,
			LeftTriangle,
			RightTriangle,
			LeftTrapezoid,
			RightTrapezoid,
		}

        static void SaveMin(string filename)
        {
            string tilFile = filename;
            string outFile = Path.GetDirectoryName(tilFile) + "/" + Path.GetFileNameWithoutExtension(tilFile) + ".mintext";

            Console.WriteLine("Opening min file {0}", tilFile);

            using (StreamWriter writer = File.CreateText(outFile))
            {
                using (BinaryReader reader = new BinaryReader(File.Open(tilFile, FileMode.Open)))
                {
                    writer.WriteLine("index,celid");

                    long numEntries = reader.BaseStream.Length / 2;					

					for (int i = 0; i < numEntries; i++)
                    {
                        ushort val = reader.ReadUInt16();

						ushort type = (ushort)((val & 0x7000) >> 12);
						TileType tileType = (TileType)type;
						ushort frameTable = (ushort)(val & 0xFFF);

						writer.WriteLine(i + "," + frameTable);
                    }

                    if (reader.BaseStream.Position != reader.BaseStream.Length)
                        throw new Exception("invalid min parse");
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
            // Dun files
            {
                string[] files = System.IO.Directory.GetFiles(args[0], "*.dun");

                foreach (string f in files)
                    SaveDun(f);
            }

            // Til files
            {
                string[] files = System.IO.Directory.GetFiles(args[0], "*.til");

                foreach (string f in files)
                    SaveTil(f);
            }

            // min files
            {
                string[] files = System.IO.Directory.GetFiles(args[0], "*.min");

                foreach (string f in files)
                    SaveMin(f);
            }

            // sol files
            {
                string[] files = System.IO.Directory.GetFiles(args[0], "*.sol");

                foreach (string f in files)
                    SaveSol(f);
            }

			// cel files
			{
				string[] files = System.IO.Directory.GetFiles(args[0], "*.cel");

				ExportTileset.Export(files[0]);
			}
		}
    }
}
