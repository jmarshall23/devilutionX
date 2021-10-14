using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace ConversionTool
{
	public class DiabloAMP
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

		public struct Entry
		{
			public Types type;
			public byte flags;
		}

		public List<Entry> entries = new List<Entry>();

		public DiabloAMP(string filename)
		{
			byte[] tileTypes = File.ReadAllBytes(filename);

			for (int i = 0; i < tileTypes.Length / 2; i++)
			{
				Types t = (Types)tileTypes[(i * 2) + 0];
				byte flags = tileTypes[(i * 2) + 1];

				Entry entry = new Entry();
				entry.type = t;
				entry.flags = flags;

				entries.Add(entry);
			}
		}
	}
}
