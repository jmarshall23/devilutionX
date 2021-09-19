// wave.cpp
//

#include "sound.h"

namespace devilution
{
	void app_fatal(const char* pszFmt, ...);
}

unsigned char* data_p;
unsigned char* iff_end;
unsigned char* last_chunk;
unsigned char* iff_data;
int iff_chunk_len;

short
GetLittleShort(void)
{
	short val = 0;

	val = *data_p;
	val = val + (*(data_p + 1) << 8);
	data_p += 2;
	return val;
}

int
GetLittleLong(void)
{
	int val = 0;

	val = *data_p;
	val = val + (*(data_p + 1) << 8);
	val = val + (*(data_p + 2) << 16);
	val = val + (*(data_p + 3) << 24);
	data_p += 4;
	return val;
}

void
FindNextChunk(char* name)
{
	while (1)
	{
		data_p = last_chunk;
		data_p += 4;

		if (data_p >= iff_end)
		{
			data_p = NULL;
			return;
		}

		iff_chunk_len = GetLittleLong();

		if (iff_chunk_len < 0)
		{
			data_p = NULL;
			return;
		}

		data_p -= 8;
		last_chunk = data_p + 8 + ((iff_chunk_len + 1) & ~1);

		if (!strncmp((const char*)data_p, name, 4))
		{
			return;
		}
	}
}

void
FindChunk(char* name)
{
	last_chunk = iff_data;
	FindNextChunk(name);
}

wavinfo_t GetWavinfo(char* name, unsigned char* wav, int wavlength)
{
	wavinfo_t info;
	int format;
	int samples;

	memset(&info, 0, sizeof(info));

	if (!wav)
	{
		return info;
	}

	iff_data = wav;
	iff_end = wav + wavlength;

	/* find "RIFF" chunk */
	FindChunk("RIFF");

	if (!(data_p && !strncmp((const char*)data_p + 8, "WAVE", 4)))
	{
		devilution::app_fatal("Missing RIFF/WAVE chunks\n");
		return info;
	}

	/* get "fmt " chunk */
	iff_data = data_p + 12;

	FindChunk("fmt ");

	if (!data_p)
	{
		devilution::app_fatal("Missing fmt chunk\n");
		return info;
	}

	data_p += 8;
	format = GetLittleShort();

	if (format != 1)
	{
		devilution::app_fatal("Microsoft PCM format only\n");
		return info;
	}

	info.channels = GetLittleShort();
	info.rate = GetLittleLong();
	data_p += 4 + 2;
	info.width = GetLittleShort() / 8;

	/* get cue chunk */
	FindChunk("cue ");

	if (data_p)
	{
		data_p += 32;
		info.loopstart = GetLittleLong();

		/* if the next chunk is a LIST chunk,
		   look for a cue length marker */
		FindNextChunk("LIST");

		if (data_p)
		{
			if (((data_p - wav) + 32 <= wavlength) &&
				!strncmp((const char*)data_p + 28, "mark", 4))
			{
				int i;

				/* this is not a proper parse,
				   but it works with cooledit... */
				data_p += 24;
				i = GetLittleLong(); /* samples in loop */
				info.samples = info.loopstart + i;
			}
		}
	}
	else
	{
		info.loopstart = -1;
	}

	/* find data chunk */
	FindChunk("data");

	if (!data_p)
	{
		devilution::app_fatal("Missing data chunk\n");
		return info;
	}

	data_p += 4;
	samples = GetLittleLong() / info.width;

	if (info.samples)
	{
		if (samples < info.samples)
		{
			devilution::app_fatal( "Sound %s has a bad loop length");
		}
	}
	else
	{
		info.samples = samples;
	}

	info.dataofs = (intptr_t)(data_p - wav);

	return info;
}
