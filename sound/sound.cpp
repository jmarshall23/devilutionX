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

	if (pSnd->isPlaying()) {
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

	alGenBuffers(1, &snd.get()->alHandle);
	alBufferData(snd.get()->alHandle, format, waveFile.get() + snd.get()->waveinfo.dataofs, size, snd.get()->waveinfo.rate);
	alGenSources(1, &snd.get()->alSource);

	alSourcei(snd.get()->alSource, AL_BUFFER, snd.get()->alHandle);

	SFileCloseFileThreadSafe(file);

	return snd;
}

bool TSnd::isPlaying() {
	ALint state;
	alGetSourcei(alSource, AL_SOURCE_STATE, &state);
	return state == AL_PLAYING;
}

void TSnd::Play(int volume, int currentSystemVolume, int pan) {
	alSourcePlay(alSource);
}

void TSnd::Stop(void) {
	alSourceStop(alSource);
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
		currentMusic->Stop();

	currentMusic = nullptr;
}

void music_start(uint8_t nTrack)
{
	music_stop();
	if (gbMusicOn) {
		const char* trackPath = musicTable->GetValue("filename", nTrack);
		currentMusic = sound_file_load(trackPath, false);
		currentMusic->Play(sgOptions.Audio.nMusicVolume, sgOptions.Audio.nMusicVolume, 0);
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
