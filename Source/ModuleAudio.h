#ifndef __Module_Audio_H__
#define __Module_Audio_H__

#include "Module.h"
#include "External/SDL_mixer\include\SDL_mixer.h"
#include <map>

#define DEFAULT_MUSIC_FADE_TIME 2.0f

enum SFX
{
	PLAYER_JUMP,
	POWER_UP_COLLECT,
	CHECK_POINT,
	PLAYER_JUMP_BLOCK,
	ENEMY_DIE,
	ENEMY_HIT,
	GROUND,
	SELECT,
	BLIP_SELECT,
	BUTTONDOWN,
	BUTTONFEEDBACK,
	MOUSEDOWN
};

struct FxInfo
{
	uint id;
	int repeat;
};

class ModuleAudio : public Module
{
public:

	ModuleAudio(bool start_enabled = true);
	~ModuleAudio();

	bool Init(pugi::xml_node&);

	UpdateStatus PreUpdate() override;

	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int id, int repeat = 0);

	void SetMusicVolume(int vol);

	void SetSFXVolume(int vol);

private:

	void PlayFrameFx(unsigned int id, int repeat = 0);

public:
	float musicVol = 0;

	float fxVol = 0;

private:

	Mix_Music*			music = nullptr;

	List<Mix_Chunk*>	fx;

	List<std::string> audioPaths;

	std::map<uint, int> frameAudios;
};

#endif // __Module_Audio_H__