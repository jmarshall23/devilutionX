Shader "Unlit/UnlitAlpha"
{
    Properties
    {
		_Color("Color", Color) = (1,1,1,1)
        _MainTex ("Texture", 2D) = "white" {}
		_TileInfo("_TileInfo", Vector) = (0, 0, 0, 0)
    }
    SubShader
    {
        Tags { "RenderType"="Opaque" }
        LOD 100
		Blend SrcAlpha OneMinusSrcAlpha

        Pass
        {
            CGPROGRAM
			

            #pragma vertex vert
            #pragma fragment frag
            // make fog work
            #pragma multi_compile_fog

            #include "UnityCG.cginc"

            struct appdata
            {
                float4 vertex : POSITION;
                float2 uv : TEXCOORD0;
            };

            struct v2f
            {
                float2 uv : TEXCOORD0;
                UNITY_FOG_COORDS(1)
                float4 vertex : SV_POSITION;
            };

            uniform sampler2D _MainTex;
            uniform float4 _MainTex_ST;
			uniform float4 _Color;
			uniform float4 _TileInfo;

            v2f vert (appdata v)
            {
                v2f o;
				float4 inputVertex = v.vertex;
                o.vertex = UnityObjectToClipPos(inputVertex);
                o.uv = TRANSFORM_TEX(v.uv, _MainTex);
                UNITY_TRANSFER_FOG(o,o.vertex);

                return o;
            }

			struct fragmentOut {
				half4 color : COLOR;
			};

			fragmentOut frag (v2f i) : SV_Target
            {
                // sample the texture
                fixed4 col = tex2D(_MainTex, i.uv);
				if (col.a == 0)
					discard;

				fragmentOut fOut;

				fOut.color = col * float4(_Color.x, _Color.y, _Color.z, 1.0);
				return fOut;
            }
            ENDCG
        }
    }
}
