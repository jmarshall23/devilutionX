using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public class LevelEditorWindow : EditorWindow
{
	Tileset[] tilesets = null;
	int tileSetSelected = 0;
	float tileSliderPosition = 0;
	DataTable level = null;

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
			GUILayout.Label("Diablo Base Folder");
			string path = GUILayout.TextField("C:\\Projects\\Diablo\\build\\base");
			if(tilesets == null)
			{
				if(GUILayout.Button("Load Tilesets"))
				{
					tilesets = Tileset.LoadTilesetsFromFolder(path + "\\levels\\");
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
				GUILayout.Button("New Map");
				if(GUILayout.Button("Load Map"))
				{
					string fileName = EditorUtility.OpenFilePanel("Open Dungeon File", path, "duntext");
					if(fileName != null)
					{
						level = new DataTable(fileName);
					}
				}
				GUILayout.Button("Save Map");
				tilesets[tileSetSelected].LoadTiles();
			}
			
		GUILayout.EndArea();


		if(tilesets.Length > 0)
		{
			tileSliderPosition = GUI.VerticalSlider(new Rect(10, 140, 40, 700), tileSliderPosition, 0, tilesets[tileSetSelected].tiles.Count - 1);

			GUILayout.BeginArea(new Rect(20, 140, 300, 700));
			
				GUILayout.BeginVertical("Tiles");
					for(int i = 0; i < 4; i++)
					{
						if (i + tileSliderPosition >= tilesets[tileSetSelected].tiles.Count)
							break;

						GUILayout.Button(tilesets[tileSetSelected].tiles[i + (int)tileSliderPosition]);
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
