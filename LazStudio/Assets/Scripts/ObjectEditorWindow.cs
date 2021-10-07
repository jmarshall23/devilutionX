using System;
using System.IO;
using UnityEngine;
using UnityEditor;

public class ObjectEditorWindow : EditorWindow
{
	// Vector3(-0.0470066965,0,0.407809913)
	// Rotation: 0 -21.989 0 

	Camera camera = null;

	[MenuItem("LazStudio/Object Editor")]
	static void ShowTownerEditor()
	{
		ObjectEditorWindow window = (ObjectEditorWindow)GetWindow(typeof(ObjectEditorWindow));
		window.Show();
	}

	void OnGUI()
	{
		GUILayout.BeginArea(new Rect(10, 10, 300, 130));
			if(GUILayout.Button("Init Scene"))
			{
				if(camera != null)
					GameObject.DestroyImmediate(camera.gameObject);

				SceneView sv = SceneView.sceneViews[0] as SceneView;
				sv.in2DMode = false;

				GameObject cameraObject = new GameObject("Camera", typeof(Camera));
				camera = cameraObject.GetComponent<Camera>();
				camera.transform.position = new Vector3(-0.589999974f, 13.0100002f, 10.29f);
				camera.transform.rotation = new Quaternion(0f, -0.923879564f, 0.382683426f, 0f);
				camera.orthographic = true;
				camera.orthographicSize = 3;
				//camera.backgroundColor = Color.white;
				camera.clearFlags = CameraClearFlags.Nothing;
				camera.targetTexture = RenderTexture.GetTemporary(128, 128, 32);
			}

			if(camera != null)
			{
				if(GUILayout.Button("Save Single Frame"))
				{
					Texture2D texture = DiabloEditorUtility.RTImage(camera);

					byte[] bytes = texture.EncodeToPNG();

					string fileName = EditorUtility.SaveFilePanel("Render Object to PNG", Config.GamePath, "newobject", "png");
					if(fileName != null && fileName.Length > 0)
					{
						File.WriteAllBytes(fileName, bytes);
					}
				}
			}
			
		GUILayout.EndArea();
	}
}
