using System;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public class Tileset 
{
	public Tileset(string path)
	{
		tileSetPath = path + "\\tiles\\";
		modTileSetPath = tileSetPath.Replace("base", "mod");

		tileSetFilename = Path.GetFileName(path);

		transTile = TGALoader.LoadPNG(path + "../../../../mod/levels/trans.png");
	}


	public void LoadTiles()
	{
		if (tiles.Count != 0)
			return;

		List<string> files = new List<string>();
		while(true)
		{
			string baseFilePath = tileSetPath + "tile" + files.Count + ".png";
			string modFilePath = modTileSetPath + "tile" + files.Count + ".png";

			if(File.Exists(modFilePath))
			{
				files.Add(modFilePath);
				continue;
			}

			if(File.Exists(baseFilePath))
			{
				files.Add(baseFilePath);
				continue;
			}

			break;
		}

		int currentFile = 0;
		foreach (string file in files)
		{
			EditorUtility.DisplayProgressBar("Loading tileset " + tileSetFilename, file, currentFile / files.Count);

			Texture2D tex = TGALoader.LoadPNG(file);

			tiles.Add(tex);

			currentFile++;
		}
		EditorUtility.ClearProgressBar();
	}

	public static Tileset[] LoadGameTilesets()
	{
		string[] tileFolders = Directory.GetDirectories(Config.GamePath + "\\base\\levels\\");
		Tileset[] tilesets = new Tileset[tileFolders.Length];

		int index = 0;
		foreach(string tileFolder in tileFolders)
		{
			tilesets[index++] = new Tileset(tileFolder);
		}

		return tilesets;
	}

	public readonly string tileSetPath;
	public readonly string modTileSetPath;
	public readonly string tileSetFilename;

	public Texture2D transTile;
	//public Material transMaterial;
	public List<Texture2D> tiles = new List<Texture2D>();
	//public List<Material> materials = new List<Material>();
}
