using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Level : MonoBehaviour
{
	public DataTable table;
	public Tileset tileset;
	private List<GameObject> cells = new List<GameObject>();

	const int tileWidthHalf = 64;
	const int tileHeightHalf = 32;

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
		table = new DataTable(filename); 
		this.tileset = tileset;

		int mapwidth = table.GetTokenInt("x", table.NumRows - 1);

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

			GameObject plane = new GameObject("Plane " + y, typeof(MeshFilter), typeof(MeshRenderer));
			MeshFilter mf = plane.GetComponent<MeshFilter>();

			mf.mesh = CreatePlane((int)height, (int)width);
			mf.sharedMesh = mf.mesh;
			plane.transform.SetParent(this.gameObject.transform);

			if (tilid == -1)
			{
				plane.GetComponent<Renderer>().material = tileset.transMaterial;
			}
			else
			{
				plane.GetComponent<Renderer>().material = tileset.materials[tilid];
			}

			

			Vector2 ScreenXY = IsometricToScreen(mapwidth - y, x, 64, 128);

			plane.transform.position = new Vector3(ScreenXY.x + height, ScreenXY.y, 0);
			//plane.transform.rotation = Quaternion.Euler(90, 0, -180);
			plane.transform.localScale = new Vector3(1, 1, 1);


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
