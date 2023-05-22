
#include "window_sdl.h"
window_sdl::window_sdl(const std::string& title): m_title_(title)
{
    
}

window_sdl::~window_sdl()
{
    SDL_Quit();
    printf("Bye");
}

auto window_sdl::init(const int x_pos, const int y_pos, const int width, const int height, const bool is_fullscreen) -> bool
{
    bool success = true;
    int flags = SDL_WINDOW_SHOWN;
    if(is_fullscreen)flags|=SDL_WINDOW_FULLSCREEN;
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        printf("subsystems init: success");
        m_window_ = std::unique_ptr<SDL_Window,sdl_window_destroyer>(
            SDL_CreateWindow(m_title_.c_str(),x_pos,y_pos,width,height,flags));
        
        if(m_window_)
            printf("window created");
        else
            success = false;
       
    }
    else
    {
        success = false;
    }

    return success;
}

auto iwindow::create(const std::string& title) -> std::unique_ptr<iwindow>
{
    return std::make_unique<window_sdl>(title);
}


