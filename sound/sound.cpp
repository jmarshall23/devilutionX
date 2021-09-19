/**
 * @file sound.cpp
 *
 * Implementation of functions setting up the audio pipeline.
 */
#include "sound.h"

#include <cstdint>
#include <list>
#include <memory>
#include <mutex>

#include <SDL.h>

#include "../source/init.h"
#include "../source/options.h"
#include "../source/storm/storm.h"
#include "../source/storm/storm_sdl_rw.h"
#include "../source/utils/log.hpp"
#include "../source/utils/math.h"
#include "../source/utils/sdl_mutex.h"
#include "../source/utils/stdcompat/algorithm.hpp"
#include "../source/utils/stdcompat/optional.hpp"
#include "../source/utils/stdcompat/shared_ptr_array.hpp"
#include "../source/utils/stubs.h"
#include "../source/datatable.h"

#include <al/al.h>
#include <al/alc.h>

namespace devilution {

bool gbSndInited;
/** The active background music track id. */
_music_id sgnMusicTrack = NUM_MUSIC;

bool gbMusicOn = true;
/** Specifies whether sound effects are enabled. */
bool gbSoundOn = true;

ALCdevice *openALDevice;
ALCcontext *openALContext;

#define MAX_PLAY_SOURCES 64
ALuint sourcePool[MAX_PLAY_SOURCES];

ALuint musicSource;

namespace {
	std::unique_ptr<TSnd> currentMusic;

int CapVolume(int volume)
{
	return clamp(volume, VOLUME_MIN, VOLUME_MAX);
}

} // namespace

void ClearDuplicateSounds()
{

}

void snd_play_snd(TSnd *pSnd, int lVolume, int lPan)
{
	if (pSnd == nullptr || !gbSoundOn) {
		return;
	}

	uint32_t tc = SDL_GetTicks();
	if (tc - pSnd->start_tc < 80) {
		return;
	}

	pSnd->Play(lVolume, sgOptions.Audio.nSoundVolume, lPan);
	pSnd->start_tc = tc;
}

std::unique_ptr<TSnd> sound_file_load(const char *path, bool stream)
{
	auto snd = std::make_unique<TSnd>();
	snd->start_tc = SDL_GetTicks() - 80 - 1;

	HANDLE file;
	if (!SFileOpenFile(path, &file)) {
		ErrDlg("SFileOpenFile failed", path, __FILE__, __LINE__);
	}
	size_t dwBytes = SFileGetFileSize(file);
	auto waveFile = MakeArraySharedPtr<std::uint8_t>(dwBytes);
	SFileReadFileThreadSafe(file, waveFile.get(), dwBytes);
	//int error = snd->DSB.SetChunk(waveFile, dwBytes);
	snd.get()->waveinfo = GetWavinfo((char *)path, waveFile.get(), dwBytes);

	ALsizei size = size = snd.get()->waveinfo.samples * snd.get()->waveinfo.width;;
	ALenum format = snd.get()->waveinfo.width == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO8;

	TSnd* sndPtr = snd.get();;

	alGenBuffers(1, &sndPtr->alHandle);
	alBufferData(sndPtr->alHandle, format, waveFile.get() + sndPtr->waveinfo.dataofs, size, sndPtr->waveinfo.rate);
	SFileCloseFileThreadSafe(file);

	return snd;
}

bool TSnd::isPlaying(int index) {

	for (int i = 0; i < MAX_PLAY_SOURCES; i++)
	{
		ALint state;
		alGetSourcei(sourcePool[i], AL_SOURCE_STATE, &state);
		if (state == AL_PLAYING)
		{
			ALint sourceHandle;
			alGetSourcei(sourcePool[i], AL_BUFFER, &sourceHandle);

			if (alHandle == sourceHandle)
				return true;
		}
	}

	return false;
}

void TSnd::Play(int volume, int currentSystemVolume, int pan) {
	for (int i = 0; i < MAX_PLAY_SOURCES; i++)
	{
		ALint state;
		alGetSourcei(sourcePool[i], AL_SOURCE_STATE, &state);
		if (state != AL_PLAYING)
		{
			alSourcei(sourcePool[i], AL_BUFFER, alHandle);
			alSourcePlay(sourcePool[i]);
			return;
		}
	}
}

void TSnd::PlayMusic(void) {
	alSourcei(musicSource, AL_BUFFER, alHandle);
	alSourcePlay(musicSource);
}

void TSnd::StopMusic(void) {
	alSourceStop(musicSource);
}

void TSnd::Stop(void) {
	for (int i = 0; i < MAX_PLAY_SOURCES; i++)
	{
		ALint state;
		alGetSourcei(sourcePool[i], AL_SOURCE_STATE, &state);
		if (state == AL_PLAYING)
		{
			ALint sourceHandle;
			alGetSourcei(sourcePool[i], AL_BUFFER, &sourceHandle);

			if (alHandle == sourceHandle)
				alSourceStop(sourcePool[i]);
		}
	}
}

TSnd::~TSnd()
{
	alDeleteBuffers(1, &alHandle);
}

void snd_init()
{
	sgOptions.Audio.nSoundVolume = CapVolume(sgOptions.Audio.nSoundVolume);
	gbSoundOn = sgOptions.Audio.nSoundVolume > VOLUME_MIN;
	sgbSaveSoundOn = gbSoundOn;

	sgOptions.Audio.nMusicVolume = CapVolume(sgOptions.Audio.nMusicVolume);
	gbMusicOn = sgOptions.Audio.nMusicVolume > VOLUME_MIN;

	openALDevice = alcOpenDevice(nullptr);
	if (!openALDevice)
		devilution::app_fatal("Failed to open OpenAL device!");

	openALContext = alcCreateContext(openALDevice, nullptr);
	if (openALContext == nullptr)
		devilution::app_fatal("Failed to create OpenAL context!");

	if (!alcMakeContextCurrent(openALContext))
		devilution::app_fatal("Failed to switch to OpenAL context!");

	alGenSources(MAX_PLAY_SOURCES, &sourcePool[0]);
	alGenSources(1, &musicSource);

	gbSndInited = true;
}

void snd_think()
{

}

void snd_deinit()
{
	gbSndInited = false;
}

void music_stop()
{
	if (currentMusic)
		currentMusic->StopMusic();

	currentMusic = nullptr;
}

void music_start(uint8_t nTrack)
{
	music_stop();
	if (gbMusicOn) {
		const char* trackPath = musicTable->GetValue("filename", nTrack);
		currentMusic = sound_file_load(trackPath, false);
		currentMusic->PlayMusic();
	}

#if 0
	bool success;
	const char *trackPath;

	assert(nTrack < NUM_MUSIC);
	music_stop();
	if (gbMusicOn) {		
		trackPath = musicTable->GetValue("filename", nTrack);

		HANDLE handle;
		success = SFileOpenFile(trackPath, &handle);
		if (!success) {
			handle = nullptr;
		} else {
			LoadMusic(handle);
			if (!music->open()) {
				LogError(LogCategory::Audio, "Aulib::Stream::open (from music_start): {}", SDL_GetError());
				CleanupMusic();
				return;
			}

			music->setVolume(VolumeLogToLinear(sgOptions.Audio.nMusicVolume, VOLUME_MIN, VOLUME_MAX));
			if (!music->play(/*iterations=*/0)) {
				LogError(LogCategory::Audio, "Aulib::Stream::play (from music_start): {}", SDL_GetError());
				CleanupMusic();
				return;
			}

			sgnMusicTrack = (_music_id)nTrack;
		}
	}
#endif
}

void sound_disable_music(bool disable)
{
	if (disable) {
		music_stop();
	} else if (sgnMusicTrack != NUM_MUSIC) {
		music_start(sgnMusicTrack);
	}
}

int sound_get_or_set_music_volume(int volume)
{
	if (volume == 1)
		return sgOptions.Audio.nMusicVolume;

	sgOptions.Audio.nMusicVolume = volume;

	//if (music)
	//	music->setVolume(VolumeLogToLinear(sgOptions.Audio.nMusicVolume, VOLUME_MIN, VOLUME_MAX));

	return sgOptions.Audio.nMusicVolume;
}

int sound_get_or_set_sound_volume(int volume)
{
	if (volume == 1)
		return sgOptions.Audio.nSoundVolume;

	sgOptions.Audio.nSoundVolume = volume;

	return sgOptions.Audio.nSoundVolume;
}

void music_mute()
{
	//if (music)
	//	music->setVolume(VolumeLogToLinear(VOLUME_MIN, VOLUME_MIN, VOLUME_MAX));
}

void music_unmute()
{
	//if (music)
	//	music->setVolume(VolumeLogToLinear(sgOptions.Audio.nMusicVolume, VOLUME_MIN, VOLUME_MAX));
}

} // namespace devilution
