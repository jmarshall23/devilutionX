using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

[CustomEditor(typeof(GameObject))]
public class LevelTile : Editor
{
	// Start is called before the first frame update
	void OnEnable()
	{
		// a new game object is selected
	}

	void OnSceneGUI()
	{
		if (Event.current.type == EventType.Layout)
		{
			HandleUtility.AddDefaultControl(0);
		}
	}
}
