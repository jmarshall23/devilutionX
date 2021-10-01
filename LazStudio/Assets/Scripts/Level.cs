using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System.IO;

public class Level : MonoBehaviour
{
	public static Level currentLevel = null;
	public static DataTable table;
	public static Tileset tileset;
	private List<GameObject> cells = new List<GameObject>();
	private List<int> cellTileIds = new List<int>();
	private List<Vector2> cellScreenXY = new List<Vector2>();

	const int tileWidthHalf = 64;
	const int tileHeightHalf = 32;

	private int previousCell = -1;

	public static int currentSelectedTileForPainting = -1;

	public void SaveLevel()
	{
		using (StreamWriter writer = File.CreateText(table.fileName))
		{
			writer.WriteLine("x,y,tilid");

			for (int i = 0; i < table.NumRows; i++)
			{
				int x = table.GetTokenInt("x", i);
				int y = table.GetTokenInt("y", i);
				int tilid = cellTileIds[i];
				writer.WriteLine(x + "," + y + "," + tilid);
			}
		}
	}

	public void UpdateHighlightTile(int cell)
	{
		if (previousCell == cell)
			return;

		if(previousCell != -1)
		{
			cells[previousCell].GetComponent<MeshRenderer>().material.SetColor("_Color", Color.white);
		}

		cells[cell].GetComponent<MeshRenderer>().material.SetColor("_Color", Color.blue);

		previousCell = cell;
	}

	public void UpdatePaintCell(int cell)
	{
		if (currentSelectedTileForPainting == -1)
			return;

		cells[cell].GetComponent<MeshRenderer>().material.mainTexture = tileset.tiles[currentSelectedTileForPainting];

		MeshFilter mf = cells[cell].GetComponent<MeshFilter>();

		float width = 0;
		float height = 0;
		float prevWidth = 0;
		float prevHeight = 0;

		if (cellTileIds[cell] == -1)
		{
			prevWidth = tileset.transTile.width;
			prevHeight = tileset.transTile.height;
		}
		else
		{
			prevWidth = tileset.tiles[cellTileIds[cell]].width;
			prevHeight = tileset.tiles[cellTileIds[cell]].height;
		}

		if (currentSelectedTileForPainting == -1)
		{
			width = tileset.transTile.width;
			height = tileset.transTile.height;
		}
		else
		{
			width = tileset.tiles[currentSelectedTileForPainting].width;
			height = tileset.tiles[currentSelectedTileForPainting].height;
		}

		mf.mesh = CreatePlane((int)height, (int)width);
		mf.sharedMesh = mf.mesh;

		transform.rotation = Quaternion.Euler(0, 0, 0);
		cells[cell].transform.position = new Vector3(cellScreenXY[cell].x + height, cellScreenXY[cell].y, 0);
		transform.rotation = Quaternion.Euler(0, -180, 90);
		cellTileIds[cell] = currentSelectedTileForPainting;
	}

	Vector2 IsometricToScreen(float isoX, float isoY, float tileWidthHalf, float tileHeightHalf)
	{
		tileWidthHalf = tileWidthHalf;
		tileHeightHalf = tileHeightHalf ;

		float screenX = (isoX - isoY) * tileWidthHalf;
		float screenY = (isoX + isoY) * tileHeightHalf;

		return new Vector2(screenX, screenY);
	}

	private Mesh CreatePlane(int width, int height)
	{
		Mesh m = new Mesh();
		m.name = "Scripted_Plane_New_Mesh";
		m.vertices = new Vector3[] { new Vector3(-width, -height, 0), new Vector3(width, -height, 0), new Vector3(width, height, 0), new Vector3(-width, height, 0) };
		m.uv = new Vector2[] { new Vector2(0, 0), new Vector2(0, 1), new Vector2(1, 1), new Vector2(1, 0) };
		m.triangles = new int[] { 0, 1, 2, 0, 2, 3 };
		m.RecalculateNormals();
		m.RecalculateBounds();
		return m;
	}

	public void Init(string filename, Tileset tileset)
	{
		currentLevel = this;
		table = new DataTable(filename); 
		Level.tileset = tileset;

		int mapwidth = table.GetTokenInt("x", table.NumRows - 1);
		int mapheight = table.GetTokenInt("y", table.NumRows - 1);

		for (int i = 0; i < table.NumRows; i++)
		{
			int x = table.GetTokenInt("x", i);
			int y = table.GetTokenInt("y", i);
			int tilid = table.GetTokenInt("tilid", i);

			float width = 0;
			float height = 0;

			if (tilid == -1)
			{
				width = tileset.transTile.width;
				height = tileset.transTile.height;
			}
			else { 
				width = tileset.tiles[tilid].width;
				height = tileset.tiles[tilid].height;
			}

			GameObject plane = new GameObject("" + i, typeof(MeshFilter), typeof(MeshRenderer));
			MeshFilter mf = plane.GetComponent<MeshFilter>();

			mf.mesh = CreatePlane((int)height, (int)width);
			mf.sharedMesh = mf.mesh;
			plane.transform.SetParent(this.gameObject.transform);

			Material material = null;
			if (tilid == -1)
			{
				material = new Material(Shader.Find("Unlit/UnlitAlpha"));
				material.mainTexture = (Texture)tileset.transTile;
				plane.GetComponent<Renderer>().material = material;
				cellTileIds.Add(-1);
			}
			else
			{

				material = new Material(Shader.Find("Unlit/UnlitAlpha"));
				material.mainTexture = (Texture)tileset.tiles[tilid];
				plane.GetComponent<Renderer>().material = material;
				cellTileIds.Add(tilid);
			}

			float depthOffset = ((((float)y / (float)mapwidth)) * 1.1f) + ((float)x / (float)mapheight);
			if (tilid == -1)
			{
				depthOffset = 0;
			}
		//	material.SetVector("_TileInfo", new Vector4(depthOffset, 0, 0, 0));

			Vector2 ScreenXY = IsometricToScreen(mapwidth - y, x, 64, 128);

			plane.transform.position = new Vector3(ScreenXY.x + height, ScreenXY.y, depthOffset);
			//plane.transform.rotation = Quaternion.Euler(90, 0, -180);
			//plane.transform.localScale = new Vector3(1, 1, 1);

			cellScreenXY.Add(ScreenXY);
			cells.Add(plane);
		}

		transform.rotation = Quaternion.Euler(0, -180, 90);
	}

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
