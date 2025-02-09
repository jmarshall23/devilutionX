using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

[CustomEditor(typeof(GameObject))]
public class LevelEditor : Editor
{
	GameObject pickedObject = null;

	// Start is called before the first frame update
	void OnEnable()
	{
		// a new game object is selected
	}

	void OnSceneGUI()
	{
		HandleUtility.AddDefaultControl(0);

		if (Event.current.type == EventType.MouseMove)
			pickedObject = HandleUtility.PickGameObject(Event.current.mousePosition, true);

		if (pickedObject == null)
			return;

		int index = int.Parse(pickedObject.name);

		int x = Level.table.GetTokenInt("x", index);
		int y = Level.table.GetTokenInt("y", index);
		int tileID = Level.table.GetTokenInt("tilid", index);


		Handles.BeginGUI();
		GUILayout.BeginArea(new Rect(20, 60, 150, 150));

			GUILayout.Label("Index: " + index);
			GUILayout.Label("World X: " + x);
			GUILayout.Label("World Y: " + y);
			GUILayout.Label("Tile ID: " + tileID);

		GUILayout.EndArea();
		Handles.EndGUI();


		if(Event.current.type == EventType.MouseDown && Event.current.button == 0)
		{
			Level.currentLevel.UpdatePaintCell(index); 
		}

		Level.currentLevel.UpdateHighlightTile(index);
	}
}
