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
				skipHeader = false;
			}

			public ConvEntry(string path, int width, bool skipheader)
			{
				this.path = path;
				this.width = width;
				widthTable = null;
				heightTable = null;
				this.skipHeader = skipheader;
			}

			public ConvEntry(string path, int[] widthTable, int[] heightTable)
			{
				this.path = path;
				this.width = -1;
				this.widthTable = widthTable;
				this.heightTable = heightTable;
				skipHeader = false;
			}

			public int width;
			public string path;

			public int[] widthTable;
			public int[] heightTable;

			public bool skipHeader;
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

		private static readonly int[] CharButsWidth = { 95, 41, 41, 41, 41, 41, 41,41,41,41 };
		private static readonly int[] CharButsHeight = { 22, 22, 22, 22, 22, 22, 22, 22, 22,22,22 };

		public static readonly ConvEntry[] store =
		{
			new ConvEntry("Data\\TextBox.CEL", 591),
			new ConvEntry("Data\\TextBox2.CEL", 271),
			new ConvEntry("Data\\TextSlid.CEL", 12),
		};

		public static readonly ConvEntry[] fonts =
		{
			new ConvEntry("Data\\MedTextS.CEL", 22),
			new ConvEntry("Data\\BigTGold.CEL", 46),
			new ConvEntry("Data\\PentSpn2.CEL", 12)
		};

		public static readonly ConvEntry[] InGameMenuAssets =
		{
			new ConvEntry("Data\\Diabsmal.CEL", 296),
			new ConvEntry("Data\\PentSpin.CEL", 48),
			new ConvEntry("Data\\option.CEL", 27),
			new ConvEntry("Data\\optbar.CEL", 287)
		};

		public static readonly ConvEntry[] MiscControlUI =
		{
			new ConvEntry("Data\\Char.cel", 320),
			new ConvEntry("Data\\CharBut.cel", CharButsWidth, CharButsHeight),
			new ConvEntry("Items\\DurIcons.cel", 32),
			new ConvEntry("Data\\SpellBk.cel", 320),
			new ConvEntry("Data\\SpellBkB.cel", 76),
			new ConvEntry("Data\\Quest.cel", 320),
			new ConvEntry("CtrlPan\\golddrop.cel", 261)
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

		public static readonly ConvEntry[] ItemPaths =
		{
			new ConvEntry("items\\Armor2.cel",96),
			new ConvEntry("items\\Axe.cel",96),
			new ConvEntry("items\\FBttle.cel",96),
			new ConvEntry("items\\Bow.cel",96),
			new ConvEntry("items\\GoldFlip.cel",96),
			new ConvEntry("items\\Helmut.cel",96),
			new ConvEntry("items\\Mace.cel",96),
			new ConvEntry("items\\Shield.cel",96),
			new ConvEntry("items\\SwrdFlip.cel",96),
			new ConvEntry("items\\Rock.cel",96),
			new ConvEntry("items\\Cleaver.cel",96),
			new ConvEntry("items\\Staff.cel",96),
			new ConvEntry("items\\Ring.cel",96),
			new ConvEntry("items\\CrownF.cel",96),
			new ConvEntry("items\\LArmor.cel",96),
			new ConvEntry("items\\WShield.cel",96),
			new ConvEntry("items\\Scroll.cel",96),
			new ConvEntry("items\\FPlateAr.cel",96),
			new ConvEntry("items\\FBook.cel",96),
			new ConvEntry("items\\Food.cel",96),
			new ConvEntry("items\\FBttleBB.cel",96),
			new ConvEntry("items\\FBttleDY.cel",96),
			new ConvEntry("items\\FBttleOR.cel",96),
			new ConvEntry("items\\FBttleBR.cel",96),
			new ConvEntry("items\\FBttleBL.cel",96),
			new ConvEntry("items\\FBttleBY.cel",96),
			new ConvEntry("items\\FBttleWH.cel",96),
			new ConvEntry("items\\FBttleDB.cel",96),
			new ConvEntry("items\\FEar.cel",96),
			new ConvEntry("items\\FBrain.cel",96),
			new ConvEntry("items\\FMush.cel",96),
			new ConvEntry("items\\Innsign.cel",96),
			new ConvEntry("items\\Bldstn.cel",96),
			new ConvEntry("items\\Fanvil.cel",96),
			new ConvEntry("items\\FLazStaf.cel",96)
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
            "Towners\\Animals\\Cow.CEL",
			"towners\\priest\\priest8.cel"
		};
    }
}
