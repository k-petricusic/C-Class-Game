#include "../include/MusicManager.h"

MusicManager::MusicManager() {}
MusicManager::~MusicManager() {}

void MusicManager::play_menu_music(const std::string&) {}
void MusicManager::play_chase_music(const std::string&, float) {}
void MusicManager::set_chase_speed(float) {}
void MusicManager::stop_music() {}
void MusicManager::update() {}
bool MusicManager::is_chase_playing() const { return false; }
bool MusicManager::is_menu_playing() const { return false; }