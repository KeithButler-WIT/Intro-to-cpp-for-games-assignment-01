#pragma once
#include "enemy.h"

using namespace sf;

int createBackground(VertexArray& rVA, IntRect& arena);
Enemy* createHorde(int numEnemys, IntRect& arena);
