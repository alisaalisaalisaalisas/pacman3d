#ifndef AUDIO_H
#define AUDIO_H

#include <string>

/**
 * Simple audio manager using miniaudio.
 * Plays sound effects and background music.
 */
class AudioManager {
public:
    AudioManager();
    ~AudioManager();
    
    bool init();
    void shutdown();
    
    void playSound(const std::string& filepath);
    void playMusic(const std::string& filepath);
    void stopMusic();
    
    void setMusicVolume(float volume);
    void setSoundVolume(float volume);
    
private:
    struct Impl;
    Impl* pImpl;
    
    float musicVolume;
    float soundVolume;
};

#endif // AUDIO_H
