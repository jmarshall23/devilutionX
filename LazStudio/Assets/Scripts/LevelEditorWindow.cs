using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public class LevelEditorWindow : EditorWindow
{
	Tileset[] tilesets = null;
	int tileSetSelected = 0;
	float tileSliderPosition = 0;
	GameObject level = null;
	Level lvl = null;

	public static Tileset currentTileset = null;

    [MenuItem("LazStudio/Level Editor")]
    static void ShowLevelEditor()
    {
        LevelEditorWindow window = (LevelEditorWindow)GetWindow(typeof(LevelEditorWindow));
        window.Show();
    }

	// Start is called before the first frame update
	void OnGUI()
	{
		GUILayout.BeginArea(new Rect(10, 10, 300, 130));
			GUILayout.Label("Diablo Game Folder(edit in Config.cs)");
			GUILayout.Label(Config.GamePath);
			if(tilesets == null)
			{
				if(GUILayout.Button("Load Tilesets"))
				{
					tilesets = Tileset.LoadGameTilesets();
				}
			}
			else
			{
				List<string> levelList = new List<string>();

				foreach(Tileset tileset in tilesets)
				{
					if(tileset.tileSetFilename == "towndata")
					{
						levelList.Add("town");
					}
					else
					{
						levelList.Add(tileset.tileSetFilename);
					}					
				}

				tileSetSelected = GUILayout.SelectionGrid(tileSetSelected, levelList.ToArray(), levelList.Count);
				tilesets[tileSetSelected].LoadTiles();
				currentTileset = tilesets[tileSetSelected];

				GUILayout.BeginHorizontal();
					if(GUILayout.Button("New Map"))
					{
						NewMapWindow.ShowModal();
					}

					if(GUILayout.Button("Shift Map"))
					{
						ShiftMapWindow.ShowModal();
					}
				GUILayout.EndHorizontal();
				if(GUILayout.Button("Load Map"))
				{
					string fileName = EditorUtility.OpenFilePanel("Open Dungeon File", Config.GamePath, "duntext");
					if(fileName != null && fileName.Length > 0)
					{
						GameObject lastGameObject = GameObject.Find("__dungeon__");
						if (lastGameObject != null)
							GameObject.DestroyImmediate(lastGameObject);

						level = new GameObject("__dungeon__", typeof(LevelEditor));
						lvl = level.AddComponent<Level>();
						lvl.Init(fileName, tilesets[tileSetSelected]);

						Level.currentLevel = lvl;

						var scene = SceneView.lastActiveSceneView;

						SceneView sv = SceneView.sceneViews[0] as SceneView;
						sv.in2DMode = true;

						scene.pivot = new Vector3(3087.2f, 115.0f, -745.1f);

						Selection.activeObject = level; 
					}
				}
				if (lvl != null)
				{
					if(GUILayout.Button("Save Map"))
					{
						lvl.SaveLevel();
					}
				}
			}
			
		GUILayout.EndArea();


		if(tilesets != null && tilesets.Length > 0)
		{
			tileSliderPosition = GUI.VerticalSlider(new Rect(10, 140, 40, 700), tileSliderPosition, 0, tilesets[tileSetSelected].tiles.Count - 1);

			GUILayout.BeginArea(new Rect(20, 140, 300, 700));
			
				GUILayout.BeginVertical("Tiles");
					for(int i = 0; i < 4; i++)
					{
						if (i + tileSliderPosition >= tilesets[tileSetSelected].tiles.Count)
							break;

						GUILayout.Label("Tile " + (i + (int)tileSliderPosition));

						if(GUILayout.Button(tilesets[tileSetSelected].tiles[i + (int)tileSliderPosition]))
						{
							Level.currentSelectedTileForPainting = i + (int)tileSliderPosition; 
						}
					}
				GUILayout.EndVertical();
			GUILayout.EndArea();
		}
		
	}

    // Update is called once per frame
    void Update()
    {
        
    }
}
