using System;
using System.IO;
using UnityEngine;

public static class TGALoader
{

	public static Texture2D LoadTGA(string fileName)
	{
		using (var imageFile = File.OpenRead(fileName))
		{
			return LoadTGA(imageFile);
		}
	}

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

	public static Texture2D LoadTGA(Stream TGAStream)
	{

		using (BinaryReader r = new BinaryReader(TGAStream))
		{
			// Skip some header info we don't care about.
			// Even if we did care, we have to move the stream seek point to the beginning,
			// as the previous method in the workflow left it at the end.
			r.BaseStream.Seek(12, SeekOrigin.Begin);

			short width = r.ReadInt16();
			short height = r.ReadInt16();
			int bitDepth = r.ReadByte();

			// Skip a byte of header information we don't care about.
			r.BaseStream.Seek(1, SeekOrigin.Current);

			Texture2D tex = new Texture2D(width, height);
			Color32[] pulledColors = new Color32[width * height];

			if (bitDepth == 32)
			{
				for (int i = 0; i < width * height; i++)
				{
					byte red = r.ReadByte();
					byte green = r.ReadByte();
					byte blue = r.ReadByte();
					byte alpha = r.ReadByte();

					pulledColors[width * height - 1 - i] = new Color32(blue, green, red, alpha);
				}
			}
			else if (bitDepth == 24)
			{
				for (int i = 0; i < width * height; i++)
				{
					byte red = r.ReadByte();
					byte green = r.ReadByte();
					byte blue = r.ReadByte();

					pulledColors[width * height - 1 - i] = new Color32(blue, green, red, 1);
				}
			}
			else
			{
				throw new Exception("TGA texture had non 32/24 bit depth.");
			}

			tex.SetPixels32(FastFlipHorizontalBuffer(pulledColors, width, height));
			tex.Apply();
			return tex;

		}
	}
}
