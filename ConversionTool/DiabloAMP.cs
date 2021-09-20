using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConversionTool
{
	class DiabloAMP
	{
		public enum Types
		{
			None = 0,
			Diamond,
			Vertical,
			Horizontal,
			Cross,
			FenceVertical,
			FenceHorizontal,
			Corner,
			CaveHorizontalCross,
			CaveVerticalCross,
			CaveHorizontal,
			CaveVertical,
			CaveCross,
		};

		public static void ConvertAutomapFile(string filename)
		{
			byte[] tileTypes = File.ReadAllBytes(filename);
			string exportFilename = ExportTileset.FixExportPath(filename) + "/automap.amptxt";

			using (StreamWriter writer = File.CreateText(exportFilename))
			{
				writer.WriteLine("index,type,flags");

				for(int i = 0; i < tileTypes.Length / 2; i++)
				{
					Types t = (Types)tileTypes[(i * 2) + 0];
					byte flags = tileTypes[(i * 2) + 1];

					writer.WriteLine("" + i + "," + t.ToString() + "," + flags);
				}
			}
		}
	}
}
