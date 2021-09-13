using System;

namespace ConversionTool
{
	public static class ConversionTables
	{
		public struct ConvEntry
		{
			public ConvEntry(string path, int width)
			{
				this.path = path;
				this.width = width;
				widthTable = null;
				heightTable = null;
			}

			public ConvEntry(string path, int[] widthTable, int[] heightTable)
			{
				this.path = path;
				this.width = -1;
				this.widthTable = widthTable;
				this.heightTable = heightTable;
			}

			public int width;
			public string path;

			public int[] widthTable;
			public int[] heightTable;
		}

		/** Maps from objcurs.cel frame number to frame width. */
		private static readonly int[] InvItemWidth1 = new int[] {
			// clang-format off
			// Cursors
			33, 32, 32, 32, 32, 32, 32, 32, 32, 32, 23,
			// Items
			1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28,
			1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28,
			1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28,
			1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28,
			1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28,
			1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28,
			1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28,
			1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28,
			2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28,
			2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28,
			2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28,
			2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28,
			2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28,
			2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28,
			2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28,
			2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28,
			2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28,
		};

		/** Maps from objcurs.cel frame number to frame height. */
		private static readonly int[] InvItemHeight1 = new int[] {
			// clang-format off
			// Cursors
			29, 32, 32, 32, 32, 32, 32, 32, 32, 32, 35,
			// Items
			1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28,
			1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28,
			1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28,
			1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28,
			1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28, 1 * 28,
			2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28,
			3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28,
			3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28,
			2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28,
			2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28, 2 * 28,
			3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28,
			3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28,
			3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28,
			3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28,
			3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28,
			3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28,
			3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28, 3 * 28,
		};

		public static readonly ConvEntry[] CtrlPan =
		{
			new ConvEntry("CtrlPan\\Golddrop.cel", 261),
			new ConvEntry("CtrlPan\\P8Bulbs.CEL", 88),
			new ConvEntry("CtrlPan\\P8But2.CEL", 33),
			new ConvEntry("CtrlPan\\Panel8.CEL", 640),
			new ConvEntry("CtrlPan\\Panel8bu.CEL", 71),
			new ConvEntry("CtrlPan\\SmalText.CEL", 13),
			new ConvEntry("CtrlPan\\SpelIcon.CEL", 56),
			new ConvEntry("CtrlPan\\TalkButt.CEL", 61),
			new ConvEntry("CtrlPan\\TalkPanl.CEL", 640)
		};

		public static readonly ConvEntry[] DataInv =
		{
			new ConvEntry("Data\\inv\\inv.cel", 320),
			new ConvEntry("Data\\inv\\inv_rog.cel", 320),
			new ConvEntry("Data\\inv\\inv_sor.cel", 320),
			new ConvEntry("Data\\inv\\objcurs.cel", InvItemWidth1, InvItemHeight1)
		};

		public static readonly ConvEntry[] GenDataImages =
		{
			new ConvEntry("Gendata\\Cutstart.cel", 640),
			new ConvEntry("Gendata\\Cuttt.cel", 640),
			new ConvEntry("Gendata\\Cutl1d.cel", 640),
			new ConvEntry("Gendata\\Cut2.cel", 640),
			new ConvEntry("Gendata\\Cut3.cel", 640),
			new ConvEntry("Gendata\\Cut4.cel", 640),
			//new ConvEntry("Nlevels\\Cutl5.cel", 640),
			//new ConvEntry("Nlevels\\Cutl6.cel", 640),
			new ConvEntry("Gendata\\Cutportl.cel", 640),
			new ConvEntry("Gendata\\Cutportr.cel", 640),
			new ConvEntry("Gendata\\Cutgate.cel", 640)
		};

		public static readonly string[] Towners = {
            "Towners\\Smith\\SmithN.CEL",
            "Towners\\Healer\\Healer.CEL",
            "Towners\\Butch\\Deadguy.CEL",
            "Towners\\TwnF\\TwnFN.CEL",
            "Towners\\Strytell\\Strytell.CEL",
            "Towners\\Drunk\\TwnDrunk.CEL",
            "Towners\\TownWmn1\\Witch.CEL",
            "Towners\\TownWmn1\\WmnN.CEL",
            "Towners\\TownBoy\\PegKid1.CEL",
            "Towners\\Animals\\Cow.CEL"
        };
    }
}
