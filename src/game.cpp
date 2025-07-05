#include <libtcod.hpp>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <filesystem>

#include <iostream>
#include <cstdlib>

#include "../include/Guard.h"
#include "../include/Movable.h"
#include "../include/Player.h"
#include "../include/Screen.h"

using namespace std;

int main(int argc, char* argv[]) {
    // Get the executable directory (implement get_executable_dir as discussed)
    std::string exe_dir = get_executable_dir();
    std::string font_path = exe_dir + "/terminal.png";

    // Load the font tileset
    auto tileset = tcod::load_tilesheet(
        font_path, // path to terminal.png
        std::array<int, 2>{16, 16},  // columns and rows for the tileset
        tcod::CHARMAP_CP437 // character mapping
    );

    auto console = tcod::Console{80, 50};
    auto params = TCOD_ContextParams{};
    params.console = console.get();
    params.window_title = "My Game";
    params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
    params.vsync = true;
    params.argc = argc;
    params.argv = argv;
    params.tileset = tileset.get(); // <-- set the tileset here!

    auto context = tcod::Context(params);

    Screen* current_screen = new Title_Screen();

    while (true) {
        console.clear();
        current_screen->show(console);
        context.present(console);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) return 0;
            current_screen->use_user_input(current_screen, event);
        }
    }
    return 0;
}
