using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DunDump
{
	class DiabloCelBase
	{
		public DiabloCelBase(byte[] pixels, int width, int height, DiabloCel.D1CEL_FRAME_TYPE type)
		{
			_pixels = pixels;
			_width = width;
			_height = height;
			_type = type;
		}

		public int Width
		{
			get
			{
				return _width;
			}
		}

		public int Height
		{
			get
			{
				return _height;
			}
		}

		public byte[] Pixels
		{
			get
			{
				return _pixels;
			}
		}

		public DiabloCel.D1CEL_FRAME_TYPE Type
		{
			get
			{
				return _type;
			}
		}

		int _width;
		int _height;
		byte[] _pixels;
		DiabloCel.D1CEL_FRAME_TYPE _type;
	}
}
