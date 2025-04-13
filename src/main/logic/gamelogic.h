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

#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "utils/structs.h"

typedef struct {
    Point position;
    Size size;
    short hp;
    int score;
} Player;

typedef struct {
    Point position;
    Size size;
    bool cracked;
} Asteroid;

typedef struct {
    Point position;
    Size size;
} __SizedPoint;

typedef __SizedPoint SmallAsteroid;
typedef __SizedPoint HealthKit;

typedef struct LinkedList LinkedList;
struct LinkedList {
    __SizedPoint *entity;
    LinkedList *next;
};

#endif //GAMELOGIC_H
