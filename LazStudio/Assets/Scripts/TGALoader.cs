using System;
using System.IO;
using UnityEngine;

public static class TGALoader
{
	private static Color32[] FastFlipHorizontalBuffer(Color32[] data, int width, int height)
	{
		Color32[] buffer = new Color32[data.Length];
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				buffer[y * width + x] = data[y * width + (width - x - 1)];
			}
		}

		return buffer;
	}

	public static Texture2D LoadPNG(string filename)
	{
		Texture2D tex = new Texture2D(2, 2);
		tex.LoadImage(File.ReadAllBytes(filename));
		tex.filterMode = FilterMode.Point;
		return tex;
	}
}
