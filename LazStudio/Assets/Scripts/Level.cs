using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Level : MonoBehaviour
{
	public DataTable table;
	public Tileset tileset;
	private List<GameObject> cells = new List<GameObject>();

	const int tileWidthHalf = 8;
	const int tileHeightHalf = 4;

	Vector2 IsometricToScreen(float isoX, float isoY)
	{
		float screenX = (isoX - isoY) * tileWidthHalf;
		float screenY = (isoX + isoY) * tileHeightHalf;

		return new Vector2(screenX, screenY);
	}

	public void Init(string filename, Tileset tileset)
	{
		table = new DataTable(filename); 
		this.tileset = tileset; 

		for(int i = 0; i < table.NumRows; i++)
		{
			int x = table.GetTokenInt("x", i);
			int y = table.GetTokenInt("y", i);
			int tilid = table.GetTokenInt("tilid", i);

			if (tilid == -1)
				continue;

			Vector2 ScreenXY = IsometricToScreen(y, x);

			GameObject plane = GameObject.CreatePrimitive(PrimitiveType.Plane);
			plane.transform.SetParent(this.gameObject.transform);
			plane.GetComponent<Renderer>().material = tileset.materials[tilid];

			plane.transform.position = new Vector3(ScreenXY.x, ScreenXY.y, 0);
			plane.transform.rotation = Quaternion.Euler(90, 0, -180);

			if (tileset.tiles[tilid].width == 128 && tileset.tiles[tilid].height == 64)
				plane.transform.localScale = new Vector3(1, 1, 0.5f);

			cells.Add(plane);
		}
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
