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

void move_entities_down(LinkedList *entities, const Screen *screen) {
    register const float STEP = MOVEMENT.ENTITY_MOVE_C * screen->height;
    LinkedList *prev = entities;
    entities = entities->next;
    while (entities != nullptr) {
        float new_position = entities->entity->position.y + STEP;
        if (new_position > entities->entity->size.height) {
            entities->entity->position.y = new_position;
        } else {
            prev->next = entities->next;
            entities = prev;
        }
        prev = entities;
        entities = entities->next;
    }
}

void move_blasts_up(LinkedList *blasts, const Screen *screen) {
    register const float STEP = MOVEMENT.BLAST_MOVE_C * screen->height;
    LinkedList *prev = blasts;
    blasts = blasts->next;
    while (blasts != nullptr) {
        float new_position = blasts->entity->position.y + STEP;
        if (new_position < screen->width) {
            blasts->entity->position.y = new_position;
        } else {
            prev->next = blasts->next;
            blasts = prev;
        }
        prev = blasts;
        blasts = blasts->next;
    }
}

inline bool two_rect_collide(__SizedPoint *rect1, __SizedPoint *rect2) {
    if (rect1->position.x < rect2->position.x + rect2->size.width &&
   rect1->position.x + rect1->size.width > rect2->size.width &&
   rect1->position.y < rect2->position.y + rect2->size.height &&
   rect1->position.y + rect1->size.height > rect2->position.y)
        return true;
    return false;
}

bool detect_user_entity_collision(Player *player, LinkedList *entities) {
    while (entities != nullptr) {
        if (two_rect_collide(player, entities->entity))
            return true;
        entities = entities->next;
    }
    return false;
}

inline bool single_blast_entity_collision(Blast *blast, LinkedList *entities, bool is_small) {
    LinkedList *prev = entities;
    entities = entities->next;
    while (entities != nullptr) {
        if (two_rect_collide(blast, entities->entity)) {
            Asteroid *entity = entities->entity;
            if (is_small || entity->cracked) {
                prev->next = entities->next;
                return true;
            } else {
                entity->cracked = true;
            }
        }
        prev = entities;
        entities = entities->next;
    }
    return false;
}

void process_asteroid_blast_collision(LinkedList *blasts, LinkedList *entities, bool is_small = false) {
    LinkedList *prev = blasts;
    blasts = blasts->next;
    while (blasts != nullptr) {
        if (single_blast_entity_collision(blasts->entity, entities, is_small)) {
            prev->next = blasts->next;
            blasts = prev;
        }
        prev = blasts;
        blasts = blasts->next;
    }
}