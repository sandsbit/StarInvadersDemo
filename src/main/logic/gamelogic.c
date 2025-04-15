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
#include <stdlib.h>

#include "gamelogic.h"
#include "setup/movement.h"
#include "setup/logic.h"

/**
 * Moves player to the left [for if the LEFT_ARROW key is pressed].
 */
inline void move_player_left(Player *player, const Screen *screen) {
    register float new_position = player->position.x - MOVEMENT.PLAYER_MOVE_C * screen->width;
    player->position.x = fmaxf(new_position, 0.0f);
}

/**
 * Moves player to the right [for if the RIGHT_ARROW key is pressed].
 */
inline void move_player_right(Player *player, const Screen *screen) {
    register float new_position = player->position.x + MOVEMENT.PLAYER_MOVE_C * screen->width;
    player->position.x = fminf(new_position, screen->width - player->size.width);
}

/**
 * Moves entities down. Should be called 10 thimes a second.
 *
 * First element in the linked list must point to a NULL entity.
 * Entities that are out of view are deleted using free, so they
 * must be allocated with malloc (both LinkedList and LinkedList.entity).
 */
void move_entities_down(LinkedList *entities, const Screen *screen) {
    register const float STEP = MOVEMENT.ENTITY_MOVE_C * screen->height;
    LinkedList *prev = entities;
    entities = entities->next;  // first entity is always NULL
    while (entities != nullptr) {
        float new_position = entities->entity->position.y + STEP;
        if (new_position > entities->entity->size.height) {
            entities->entity->position.y = new_position;
        } else {
            free(entities->entity);
            prev->next = entities->next;
            free(entities);
            entities = prev;
        }
        prev = entities;
        entities = entities->next;
    }
}

/**
 * Moves blasts up. Should be called 10 thimes a second.
 *
 * First element in the linked list must point to a NULL entity.
 * Entities that are out of view are deleted using free, so they
 * must be allocated with malloc (both LinkedList and LinkedList.entity).
 */
void move_blasts_up(LinkedList *blasts, const Screen *screen) {
    register const float STEP = MOVEMENT.BLAST_MOVE_C * screen->height;
    LinkedList *prev = blasts;
    blasts = blasts->next;  // first entity is always NULL
    while (blasts != nullptr) {
        float new_position = blasts->entity->position.y + STEP;
        if (new_position < screen->width) {
            blasts->entity->position.y = new_position;
        } else {
            free(blasts->entity);
            prev->next = blasts->next;
            free(blasts);
            blasts = prev;
        }
        prev = blasts;
        blasts = blasts->next;
    }
}

/**
 * Checks if rect1 and rect2 collide.
 */
inline bool two_rect_collide(__SizedPoint *rect1, __SizedPoint *rect2) {
    if (rect1->position.x < rect2->position.x + rect2->size.width &&
   rect1->position.x + rect1->size.width > rect2->size.width &&
   rect1->position.y < rect2->position.y + rect2->size.height &&
   rect1->position.y + rect1->size.height > rect2->position.y)
        return true;
    return false;
}

/**
 * Detects if user hit an entity and deletes that entity.
 *
 * Returns true if one collide was detected and false if no
 * were detected. After first collide is detected, function
 * is stopped.
 *
 * First element in the linked list must point to a NULL entity.
 * Entities that are out of view are deleted using free, so they
 * must be allocated with malloc (both LinkedList and LinkedList.entity).
 */
bool detect_user_entity_collision(Player *player, LinkedList *entities) {
    LinkedList *prev = entities;
    entities = entities->next;  // first entity is always NULL
    while (entities != nullptr) {
        if (two_rect_collide(player, entities->entity)) {
            free(entities->entity);
            prev->next = entities->next;
            free(entities);
            return true;
        }
        prev = entities;
        entities = entities->next;
    }
    return false;
}

/**
 * Detects if blast hit an asteroid.
 *
 * Entity is deleted if is_small = true or Asteroid.cracked
 * is true. Otherwise cracked is set to true.
 *
 * Returns true if one collide was detected and false if no
 * were detected. After first collide is detected, function
 * is stopped.
 *
 * First element in the linked list must point to a NULL entity.
 * Entities that are out of view are deleted using free, so they
 * must be allocated with malloc (both LinkedList and LinkedList.entity).
 */
bool single_blast_entity_collision(Blast *blast, LinkedList *entities, bool is_small) {
    LinkedList *prev = entities;
    entities = entities->next;  // first entity is always NULL
    while (entities != nullptr) {
        if (two_rect_collide(blast, entities->entity)) {
            Asteroid *entity = entities->entity;
            if (is_small || entity->cracked) {
                free(entities->entity);
                prev->next = entities->next;
                free(entities);
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

/**
 * Detects if any blast hit an asteroid and deletes that blast.
 *
 * Entity is deleted if is_small = true or Asteroid.cracked
 * is true. Otherwise cracked is set to true.
 * See docs for single_blast_entity_collision().
 *
 * First element in the linked list must point to a NULL entity.
 * Entities that are out of view are deleted using free, so they
 * must be allocated with malloc (both LinkedList and LinkedList.entity).
 */
void process_asteroid_blast_collision(LinkedList *blasts, LinkedList *entities, bool is_small = false) {
    LinkedList *prev = blasts;
    blasts = blasts->next;  // first entity is always NULL
    while (blasts != nullptr) {
        if (single_blast_entity_collision(blasts->entity, entities, is_small)) {
            free(blasts->entity);
            prev->next = blasts->next;
            free(blasts);
            blasts = prev;
        }
        prev = blasts;
        blasts = blasts->next;
    }
}

// see docs in gamelogic.h
bool game_loop(GameObjects *objects, bool LEFT_ARROW, bool RIGHT_ARROW) {
    // -- MOVEMENT --
    if (LEFT_ARROW)
        move_player_left(objects->player, objects->screen);
    if (RIGHT_ARROW)
        move_player_right(objects->player, objects->screen);

    move_entities_down(objects->asteroids, objects->screen);
    move_entities_down(objects->small_asteroids, objects->screen);
    move_entities_down(objects->health_kits, objects->screen);

    // -- COLLISIONS --
    process_asteroid_blast_collision(objects->blasts, objects->asteroids);
    process_asteroid_blast_collision(objects->blasts, objects->small_asteroids, true);

    if (detect_user_entity_collision(objects->player, objects->asteroids))
        objects->player->hp -= LOGIC.HIT_ASTEROID_HP;
    if (detect_user_entity_collision(objects->player, objects->small_asteroids))
        objects->player->hp -= LOGIC.HIT_SMALL_ASTEROID_HP;
    if (detect_user_entity_collision(objects->player, objects->health_kits))
        objects->player->hp += LOGIC.HIT_HEALTH_KIT_HP;

    // -- LOST OR NOT --
    if (objects->player->hp <= 0)
        return GAME_LOOP_DEAD;
    return GAME_LOOP_ALIVE;
}
