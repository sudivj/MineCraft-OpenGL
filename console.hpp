#pragma once


#include <stdio.h>
#include <stdlib.h>
#include "src/player/player_data.hpp"

void clear() {
    //system("clear");
}

void position(player_data &player) {
    std::cout << "x - " << player.x << " y - " << player.y << " z - " << player.z << " block count - " << player.w_block_count << std::endl;
}

void display_player_postion(player_data &player) {
    if (player.pre_x != player.x) { position(player); player.update_position(); }
    if (player.pre_y != player.y) { position(player); player.update_position(); }
    if (player.pre_z != player.z) { position(player); player.update_position(); }
}

void console(player_data &player) {
    display_player_postion(player);
}