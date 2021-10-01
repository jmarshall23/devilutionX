using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;
using System.IO;

public class NewMapWindow : EditorWindow
{
	int mapWidthSelect = 0;
	int mapHeightSelect = 0;

	public static void ShowModal()
	{
		NewMapWindow window = new NewMapWindow();
		window.maxSize = new Vector2(300, 130);
		window.minSize = window.maxSize;
		window.ShowModalUtility();
	}

	void OnGUI()
	{
		string[] dimensions = new string[] { "4", "6", "8", "12", "16", "24" };

		GUILayout.BeginArea(new Rect(10, 10, 300, 130));
			GUILayout.BeginHorizontal();
				mapWidthSelect = EditorGUILayout.Popup(mapWidthSelect, dimensions);
				mapHeightSelect = EditorGUILayout.Popup(mapHeightSelect, dimensions);
			GUILayout.EndHorizontal();

			GUILayout.BeginHorizontal();
				if(GUILayout.Button("Create Map"))
				{
					string fileName = EditorUtility.SaveFilePanel("New Dungeon File", LevelEditorWindow.path, "newdungeon", "duntext");
					if(fileName.Length != null && fileName.Length > 0)
					{
						using (StreamWriter writer = File.CreateText(fileName))
						{
							writer.WriteLine("x,y,tilid");

							int mapWidth = int.Parse(dimensions[mapWidthSelect]);
							int mapHeight = int.Parse(dimensions[mapHeightSelect]);

							for(int y = 0; y < mapHeight; y++)
							{
								for(int x = 0; x < mapWidth; x++)
								{
									writer.WriteLine(x + "," + y + "," + -1);
								}
							}							
						}

						Close();
					}
				}
				if(GUILayout.Button("Cancel"))
				{
					Close();
				}
			GUILayout.EndHorizontal();
		GUILayout.EndArea();
	}
}
