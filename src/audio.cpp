#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "audio.h"
#include <iostream>

struct AudioManager::Impl {
    ma_engine engine;
    ma_sound music;
    bool musicLoaded;
    bool initialized;
};

AudioManager::AudioManager() 
    : pImpl(new Impl())
    , musicVolume(0.5f)
    , soundVolume(1.0f)
{
    pImpl->musicLoaded = false;
    pImpl->initialized = false;
}

AudioManager::~AudioManager() {
    shutdown();
    delete pImpl;
}

bool AudioManager::init() {
    ma_result result = ma_engine_init(NULL, &pImpl->engine);
    if (result != MA_SUCCESS) {
        std::cerr << "Failed to initialize audio engine" << std::endl;
        return false;
    }
    pImpl->initialized = true;
    std::cout << "Audio initialized" << std::endl;
    return true;
}

void AudioManager::shutdown() {
    if (pImpl->musicLoaded) {
        ma_sound_uninit(&pImpl->music);
        pImpl->musicLoaded = false;
    }
    if (pImpl->initialized) {
        ma_engine_uninit(&pImpl->engine);
        pImpl->initialized = false;
    }
}

void AudioManager::playSound(const std::string& filepath) {
    if (!pImpl->initialized) return;
    ma_engine_play_sound(&pImpl->engine, filepath.c_str(), NULL);
}

void AudioManager::playMusic(const std::string& filepath) {
    if (!pImpl->initialized) return;
    
    if (pImpl->musicLoaded) {
        ma_sound_uninit(&pImpl->music);
    }
    
    ma_result result = ma_sound_init_from_file(&pImpl->engine, filepath.c_str(), 
        MA_SOUND_FLAG_STREAM, NULL, NULL, &pImpl->music);
    
    if (result == MA_SUCCESS) {
        ma_sound_set_looping(&pImpl->music, MA_TRUE);
        ma_sound_set_volume(&pImpl->music, musicVolume);
        ma_sound_start(&pImpl->music);
        pImpl->musicLoaded = true;
        std::cout << "Playing music: " << filepath << std::endl;
    } else {
        std::cerr << "Failed to load music: " << filepath << std::endl;
    }
}

void AudioManager::stopMusic() {
    if (pImpl->musicLoaded) {
        ma_sound_stop(&pImpl->music);
    }
}

void AudioManager::setMusicVolume(float volume) {
    musicVolume = volume;
    if (pImpl->musicLoaded) {
        ma_sound_set_volume(&pImpl->music, volume);
    }
}

void AudioManager::setSoundVolume(float volume) {
    soundVolume = volume;
}
