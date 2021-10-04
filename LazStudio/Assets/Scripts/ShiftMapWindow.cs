using System;
using System.IO;
using UnityEngine;
using UnityEditor;

public class ShiftMapWindow : EditorWindow
{
	string shiftWidthTxt = "0";
	string shiftHeightTxt = "0";

	public static void ShowModal()
	{
		ShiftMapWindow window = new ShiftMapWindow();
		window.maxSize = new Vector2(300, 130);
		window.minSize = window.maxSize;
		window.ShowModalUtility();
	}

	void OnGUI()
	{
		GUILayout.BeginArea(new Rect(10, 10, 300, 130));
			GUILayout.BeginHorizontal();
				shiftWidthTxt = GUILayout.TextField(shiftWidthTxt);
				shiftHeightTxt = GUILayout.TextField(shiftHeightTxt);
			GUILayout.EndHorizontal();

			if(GUILayout.Button("Shift map"))
			{
				int shiftWidth = int.Parse(shiftWidthTxt);
				int shiftHeight = int.Parse(shiftHeightTxt);

				if(shiftWidth != 0)
				{
					Debug.LogError("Width shift not supported!");
					return;
				} 

				string sourceFileName = EditorUtility.OpenFilePanel("Open Dungeon File", LevelEditorWindow.path, "duntext");
				string destFileName = null;
				if (sourceFileName != null && sourceFileName.Length > 0)
				{
					DataTable dataTable = new DataTable(sourceFileName);
					destFileName = EditorUtility.SaveFilePanel("Save Dungeon File", LevelEditorWindow.path, "newdungeon", "duntext");

					if (dataTable != null && destFileName != null && destFileName.Length > 0)
					{

						int mapWidth = dataTable.GetTokenInt("x", dataTable.NumRows - 1);
						int mapHeight = dataTable.GetTokenInt("y", dataTable.NumRows - 1);

						using (StreamWriter writer = File.CreateText(destFileName))
						{
							writer.WriteLine("x,y,tilid");

							mapWidth += shiftWidth;
							mapHeight += shiftHeight;

							int tile = 0;

							for (int y = 0; y < mapHeight  + 1; y++)
							{
								for (int x = 0; x < mapWidth + 1; x++)
								{
									if (y < shiftHeight)
									{
										writer.WriteLine(x + "," + y + "," + -1);
									}
									else
									{
										int texture = dataTable.GetTokenInt("tilid", tile);
										writer.WriteLine(x + "," + y + "," + texture);
										tile++;
									}
								}
							}
						}

					Close();
					}
				}				
			}

			if (GUILayout.Button("Cancel"))
			{
				Close();
			}
		GUILayout.EndArea();
	}
}
