#pragma once
#include <string>

class MusicManager {
public:
    MusicManager();
    ~MusicManager();

    void play_menu_music(const std::string& path);
    void play_chase_music(const std::string& path, float speed = 1.0f);
    void set_chase_speed(float speed);
    void stop_music();
    void update();

    bool is_chase_playing() const;
    bool is_menu_playing() const;
};