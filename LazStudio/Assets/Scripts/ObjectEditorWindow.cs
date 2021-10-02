using System;
using System.IO;
using UnityEngine;
using UnityEditor;

public class ObjectEditorWindow : EditorWindow
{
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
				camera.transform.position = new Vector3(-0.519999981f, 11.79f, 11.5100002f);
				camera.transform.rotation = new Quaternion(-0.0192171894f, 0.940588295f, -0.338903844f, -0.00828242023f);
				camera.orthographic = true;
				camera.orthographicSize = 3;
				camera.backgroundColor = Color.white;
				camera.clearFlags = CameraClearFlags.SolidColor;
				camera.targetTexture = RenderTexture.GetTemporary(128, 128, 32);
			}

			if(camera != null)
			{
				if(GUILayout.Button("Save Single Frame"))
				{
					Texture2D texture = DiabloEditorUtility.RTImage(camera);

					byte[] bytes = texture.EncodeToTGA();

					string fileName = EditorUtility.SaveFilePanel("Render Object to TGA", LevelEditorWindow.path, "newobject", "tga");
					if(fileName != null && fileName.Length > 0)
					{
						File.WriteAllBytes(fileName, bytes);
					}
				}
			}
			
		GUILayout.EndArea();
	}
}
