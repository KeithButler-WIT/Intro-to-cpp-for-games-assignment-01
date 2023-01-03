#include "enemy.h"
#include "enemyArena.h"

Enemy* createHorde(int numEnemys, IntRect& arena)
{
	Enemy* enemys = new Enemy[numEnemys];

	int maxY = arena.height - 20;
	int minY = arena.top + 20;
	int maxX = arena.width - 20;
	int minX = arena.left + 20;

	for (int i = 0; i < numEnemys; i++)
	{

		// Which side should the enemy spawn
		int side = (rand() % 4);
		float x = 0, y = 0;

		switch (side)
		{
			case 0:
				// left
				x = minX;
				y = (rand() % maxY) + minY;
				break;

			case 1:
				// right
				x = maxX;
				y = (rand() % maxY) + minY;
				break;

			case 2:
				// top
				x = (rand() % maxX) + minX;
				y = minY;
				break;

			case 3:
				// bottom
				x = (rand() % maxX) + minX;
				y = maxY;
				break;
			default:
				break;
		}

		// Bloater, crawler or runner
		int type = (rand() % 3);

		// Spawn the new enemy into the array
		enemys[i].spawn(x, y, type, i);
	}
	return enemys;
}
