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
		tileSetFilename = Path.GetFileName(path);

		transTile = TGALoader.LoadTGA(path + "../../../../mod/levels/trans.tga");
	}


	public void LoadTiles()
	{
		if (tiles.Count != 0)
			return;

		string[] files = Directory.GetFiles(tileSetPath, "*.tga");
		Array.Sort(files, new AlphanumComparatorFast());

		int currentFile = 0;
		foreach (string file in files)
		{
			EditorUtility.DisplayProgressBar("Loading tileset " + tileSetFilename, file, currentFile / files.Length);

			Texture2D tex = TGALoader.LoadTGA(new MemoryStream(File.ReadAllBytes(file)));

			tiles.Add(tex);

			currentFile++;
		}
		EditorUtility.ClearProgressBar();
	}

	public static Tileset[] LoadTilesetsFromFolder(string folder)
	{
		string[] tileFolders = Directory.GetDirectories(folder);
		Tileset[] tilesets = new Tileset[tileFolders.Length];

		int index = 0;
		foreach(string tileFolder in tileFolders)
		{
			tilesets[index++] = new Tileset(tileFolder);
		}

		return tilesets;
	}

	public readonly string tileSetPath;
	public readonly string tileSetFilename;

	public Texture2D transTile;
	//public Material transMaterial;
	public List<Texture2D> tiles = new List<Texture2D>();
	//public List<Material> materials = new List<Material>();
}
