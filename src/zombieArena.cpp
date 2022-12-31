#include "zombieArena.h"
#include "player.h"
#include "textureHolder.h"
#include "turret.h"
#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
	TextureHolder holder;

	// The game will always be in one of four states
	enum class State
	{
		PAUSED,
		LEVELING_UP,
		GAME_OVER,
		PLAYING
	};

	// Start with the GAME_OVER state
	State state = State::GAME_OVER;

	// Get the screen resolution and create an SFML window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width / 2;
	resolution.y = VideoMode::getDesktopMode().height / 2;
	RenderWindow window(VideoMode(resolution.x, resolution.y), "Assignment", Style::Fullscreen);

	// Create an SFML View for the main action
	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));
	// Clock for timing everything
	Clock clock;
	// How long has the PLAYING state been active?
	Time gameTimeTotal;
	// Where is the mouse in relation to world coordinates?
	Vector2f mouseWorldPosition;
	// Where is the mouse in relation to screen coordinates?
	Vector2i mouseScreenPosition;

	// Create an instance of the Player class
	Player player;

	// Create an instance of the Turret class
	Turret turret;

	// The boundaries of the arena
	IntRect arena;
	IntRect& rArena = arena;

	// Create the background
	VertexArray background;
	// Load the texture for our background vertex array
	Texture textureBackground = TextureHolder::GetTexture("content/graphics/Tiles/tile_background.png");

	// Prepare for a horde of zombies
	int numZombies = 0; // Number at start of wave
	// int numZombiesAlive = 0; // Number to be killed
	Zombie* zombies = nullptr;

	// When was the fire button last pressed?
	Time lastPressed;

	// Hide the mouse pointer and replace it with crosshair
	window.setMouseCursorVisible(false);
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::GetTexture("content/graphics/Tiles/tile_0060.png");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(8, 8);

	//The main game loop
	while (window.isOpen())
	{

		/*
		************************************
		Handle input
		************************************
		*/

		// Handle events
		Event event;
		while (window.pollEvent(event))
		{

			if (event.type == Event::KeyPressed)
			{

				// Pause game while playing
				if (event.key.code == Keyboard::Return && state == State::PLAYING)
				{
					state = State::PAUSED;
				}

				// Restart while paused
				else if (event.key.code == Keyboard::Return && state == State::PAUSED)
				{
					state = State::PLAYING;
					// Reset the clock so there isn't a frame jump
					clock.restart();
				}

				// Start a new game while in GAME_OVER state
				else if (event.key.code == Keyboard::Return && state == State::GAME_OVER)
				{
					state = State::LEVELING_UP;
				}

				if (state == State::PLAYING)
				{
					// TODO fix dash
					// Dash
					if (event.key.code == Keyboard::Space)
					{
						player.startDash();
						// check for collision with a zombie

						for (int i = 0; i < numZombies; i++)
						{
							if (player.checkCollision(zombies[i]))
							{
								player.stopDash();
								// add point
							}
						}

						// check for collision with Turret
						if (player.checkCollision(turret))
						{
							// reload turret when dashed through
						}

						lastPressed = gameTimeTotal;

					} //End if (event.key.code == Keyboard::Space)

					// // Reloading
					// if (event.key.code == Keyboard::R)
					// {
					// 	if (bulletsSpare >= clipSize)
					// 	{
					// 		// Plenty of bullets. Reload.
					// 		bulletsInClip = clipSize;
					// 		bulletsSpare -= clipSize;
					// 	}
					// }
					// else if (bulletsSpare > 0)
					// {
					// 	// Only few bullets left
					// 	bulletsInClip = bulletsSpare;
					// 	bulletsSpare = 0;
					// }
					// else
					// {
					// 	// More here later reload failed sound
					// }

				} //End if (event.key.code == Keyboard::R)
			}	  //End if (event.type == Event::KeyPressed)

		} // End event polling

		// Stops the player dashing after a set time
		if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / 3)
			player.stopDash();

		// Handle the player quitting
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		// Handle controls while playing
		if (state == State::PLAYING)
		{
			// Handle the pressing and releasing of the WASD keys
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				player.moveUp();
			}
			else
			{
				player.stopUp();
			}

			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				player.moveDown();
			}
			else
			{
				player.stopDown();
			}

			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				player.moveLeft();
			}
			else
			{
				player.stopLeft();
			}

			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				player.moveRight();
			}
			else
			{
				player.stopRight();
			}
			// // Dash
			// if (Keyboard::isKeyPressed(Keyboard::Space))
			// {
			// 	player.startDash();
			// 	// if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / 1)
			// 	// {
			// 	// player.stopDash();
			// 	// lastPressed = gameTimeTotal;
			// 	// }
			// }
			// else
			// {
			// 	player.stopDash();
			// } //End if (event.key.code == Keyboard::Space)
			if (Keyboard::isKeyPressed(Keyboard::F))
			{
				// TODO
				// Spawn the turret at players location
				turret.spawn(player.getCenter(), resolution);

			} //End if (event.key.code == Keyboard::F)

			// Fire a bullet
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				// if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0)
				// {
				// 	// Pass the centre of the player and the centre of the cross-hair to the shoot function
				// 	bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y, mouseWorldPosition.x, mouseWorldPosition.y);
				// 	currentBullet++;

				// 	if (currentBullet > 99)
				// 	{
				// 		currentBullet = 0;
				// 	}

				// 	lastPressed = gameTimeTotal;
				// 	bulletsInClip--;
				// }
			} // End fire a bullet

		} // End WASD while playing

		// Handle the levelling up state
		if (state == State::LEVELING_UP)
		{

			// Handle the player levelling up
			if (event.key.code == Keyboard::Num1)
			{
				// increase turret damage
				turret.upgradeDamage();
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num2)
			{
				// increase dash distance
				player.upgradeDash();
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num3)
			{
				// increase run speed
				player.upgradeSpeed();
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num4)
			{
				// increase player health
				player.upgradeHealth();
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num5)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num6)
			{
				state = State::PLAYING;
			}

			if (state == State::PLAYING)
			{
				// Prepare the level – we will update this later
				rArena.width = 1600;
				rArena.height = 1600;
				rArena.left = 0;
				//int tileSize = 50; // we will update this later
				rArena.top = 0;

				// Pass the vertex array by reference to the createBackground function
				int tileSize = createBackground(background, rArena);

				// Spawn the player in the middle of the arena
				player.spawn(rArena, resolution, tileSize);

				// Create a horde of zombies
				numZombies = 10;

				// Delete the previously allocated memory (if it exists)
				delete[] zombies; // Note use of delete[] – should use [] when deleting arrays from heap.
				zombies = createHorde(numZombies, rArena);
				// numZombiesAlive = numZombies;

				// Reset the clock so there isn't a frame jump
				clock.restart();
			}
		} // End levelling up

		/*
		************************************
		UPDATE THE FRAME
		************************************
		*/
		if (state == State::PLAYING)
		{

			// Update the delta time
			Time dt = clock.restart();
			// Update the total game time
			gameTimeTotal += dt;
			// Make a decimal fraction of 1 from the delta time
			float dtAsSeconds = dt.asSeconds();

			// Find the mouse pointer
			mouseScreenPosition = Mouse::getPosition(window);

			// Convert mouse position to world coordinates of mainView
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);

			// Set the crosshair to the mouse world location
			spriteCrosshair.setPosition(mouseWorldPosition.x, mouseWorldPosition.y);

			// Update the player
			// player.update(dtAsSeconds, Mouse::getPosition());
			player.update(dtAsSeconds);

			// turret.update(zombies[1].getCenter());

			// Make a note of the players new position
			Vector2f playerPosition(player.getCenter());

			// Make the view centre around the player
			mainView.setCenter(player.getCenter());

			// Convert mouse position to world coordinates of mainView
			// Loop through each Zombie and update them if alive
			for (int i = 0; i < numZombies; i++)
			{
				if (zombies[i].isAlive())
				{
					zombies[i].update(dt.asSeconds(), playerPosition);
					// FloatRect zombieWorldPosition = zombies[i].getPosition();
					// Vector2f zombieWorldPosition = window.mapPixelToCoords(zombies[i].getCenter());
					// turret.update(zombieWorldPosition);
					turret.update(zombies[i].getCenter());
				}
			}

			// Update any bullets that are in-flight
			for (int i = 0; i < 100; i++)
			{
				if (turret.getBullet(i).isInFlight())
				{
					turret.getBullet(i).update(dtAsSeconds);
				}
			}

		} // End updating the scene

		/*
		************************************
		Draw the scene
		************************************
		*/

		if (state == State::PLAYING)
		{
			window.clear();

			// set the mainView to be displayed in the window
			// And draw everything related to it
			window.setView(mainView);

			// Draw the background
			window.draw(background, &textureBackground);

			// Draw the zombies
			for (int i = 0; i < numZombies; i++)
			{
				window.draw(zombies[i].getSprite());
			}

			// Draw the bullets
			for (int i = 0; i < 100; i++)
			{
				if (turret.getBullet(i).isInFlight())
				{
					window.draw(turret.getBullet(i).getSprite());
				}
			}

			// Draw the turret
			// window.draw(Sprite(TextureHolder::GetTexture("content/graphics/Tiles/turret_base.png")));
			window.draw(turret.getSprite());

			// Draw the player
			window.draw(player.getSprite());

			//Draw the crosshair
			window.draw(spriteCrosshair);
		}

		if (state == State::LEVELING_UP)
		{
		}

		if (state == State::PAUSED)
		{
		}

		if (state == State::GAME_OVER)
		{
		}

		window.display();
	}

	// Delete the previously allocated memory (if it exists)
	delete[] zombies;

	return 0;
}
