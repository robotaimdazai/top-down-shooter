#ifndef GAME_STATE_H
#define GAME_STATE_H
#include "game.h"


class i_game_state
{
public:
    virtual ~i_game_state() = default;
    virtual auto load()->void =0;
    virtual auto clean()->void =0;
    virtual auto update(uint32_t delta_time)->void=0;
    virtual auto render()->void = 0;
    virtual auto pause()->void = 0;
    virtual auto resume()->void = 0;
    virtual auto set_game(game* game)->void=0;
    virtual auto handle_event(const input_state&)->void = 0;
    
};
#endif