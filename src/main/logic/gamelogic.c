/*
 * Copyright (C) 2025 Nikita Serba. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the “Software”), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to
 * do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <math.h>

#include "gamelogic.h"
#include "setup/movement.h"

inline void move_player_left(Player *player, const Screen *screen) {
    register float new_position = player->position.x - MOVEMENT.PLAYER_MOVE_C * screen->width;
    player->position.x = fmaxf(new_position, 0.0f);
}

inline void move_player_right(Player *player, const Screen *screen) {
    register float new_position = player->position.x + MOVEMENT.PLAYER_MOVE_C * screen->width;
    player->position.x = fminf(new_position, screen->width - player->size.width);
}

inline void move_asteroids_down(AsteroidLinkedList *asteroids, const Screen *screen) {
    register const float STEP = MOVEMENT.ENTITY_MOVE_C * screen->height;
    AsteroidLinkedList *prev = nullptr;
    while (asteroids != nullptr) {
        float new_position = asteroids->asteroid.position.y + STEP;
        if (new_position > asteroids->asteroid.size.height) {
            asteroids->asteroid.position.y = new_position;
        } else if (prev) {
            prev->next = asteroids->next;
            asteroids = prev;
        }
        prev = asteroids;
        asteroids = asteroids->next;
    }
}