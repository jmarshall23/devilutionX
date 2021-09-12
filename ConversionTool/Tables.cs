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
			}

			public int width;
			public string path;
		}

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
