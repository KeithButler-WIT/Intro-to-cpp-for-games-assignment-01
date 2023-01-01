#include "zombieArena.h"
#include "player.h"
#include "textureHolder.h"
#include "turret.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>

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
	int numZombies = 0;		 // Number at start of wave
	int numZombiesAlive = 0; // Number to be killed
	Zombie* zombies = nullptr;

	int score = 0;
	int hiScore = 0;

	// When was the fire button last pressed?
	Time lastPressed;

	// Hide the mouse pointer and replace it with crosshair
	window.setMouseCursorVisible(false);
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::GetTexture("content/graphics/Tiles/tile_0060.png");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(8, 8);

	// Background for the home/game over screen
	// Sprite spriteGameOver;
	// Texture textureGameOver = TextureHolder::GetTexture("content/graphics/background.png");
	// spriteGameOver.setTexture(textureGameOver);
	// spriteGameOver.setPosition(0, 0);

	// // Image is 1920x1080 - needs scaling for large displays
	// spriteGameOver.setScale(resolution.x / 1920, resolution.y / 1080);

	// Create a view for the HUD
	View HudView(FloatRect(0, 0, resolution.x, resolution.y));

	// Load the font
	Font font;
	font.loadFromFile("content/fonts/Kenney Pixel.ttf");

	// Paused
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(155);
	pausedText.setFillColor(Color::White);
	pausedText.setString("Press Enter \nto continue");

	// Place in middle of screen
	FloatRect pausedRect = pausedText.getLocalBounds();
	pausedText.setOrigin(pausedRect.left + pausedRect.width / 2.0f, pausedRect.top + pausedRect.height / 2.0f);
	pausedText.setPosition(resolution.x / 2, resolution.y / 6);

	// Game Over
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(125);
	gameOverText.setFillColor(sf::Color::White);
	gameOverText.setString("Press Enter to play");

	FloatRect gameOverRect = gameOverText.getLocalBounds();
	gameOverText.setOrigin(gameOverRect.left + gameOverRect.width / 2.0f, gameOverRect.top + gameOverRect.height / 2.0f);
	gameOverText.setPosition(resolution.x / 2, resolution.y / 2);

	// Levelling up
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(80);
	levelUpText.setFillColor(Color::Black);
	levelUpText.setPosition(resolution.x / 2, resolution.y / 2);
	std::stringstream levelUpStream;
	levelUpStream << "1- Increased damage"
				  << "\n2- Increased rate of fire"
				  << "\n3- Increased dash distance"
				  << "\n4- Increased run speed"
				  << "\n5- Increased max health";
	levelUpText.setString(levelUpStream.str());

	FloatRect levelUpRect = levelUpText.getLocalBounds();
	levelUpText.setOrigin(levelUpRect.left + levelUpRect.width / 2.0f, levelUpRect.top + levelUpRect.height / 2.0f);
	levelUpText.setPosition(resolution.x / 2, resolution.y / 2);

	// Ammo
	// Text ammoText;
	// ammoText.setFont(font);
	// ammoText.setCharacterSize(55);
	// ammoText.setFillColor(Color::White);
	// ammoText.setPosition(200, resolution.y - 200);

	// Score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(55);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(resolution.x / 2, (resolution.y / 2) - 200);

	// Load the high score from a text file
	std::ifstream inputFile("gamedate/scores.txt");
	if (inputFile.is_open())
	{
		inputFile >> hiScore;
		inputFile.close();
	}

	// Hi Score
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(55);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(resolution.x / 2, (resolution.y / 2) - 100);
	std::stringstream s;
	s << "Hi Score:" << hiScore;
	hiScoreText.setString(s.str());

	// Zombies remaining
	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(55);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(resolution.x - 200, resolution.y - 200);
	zombiesRemainingText.setString("Zombies: 100");

	// Wave number
	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(55);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(resolution.x * 0.66, resolution.y - 200);
	waveNumberText.setString("Wave: 0");

	// Health bar
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	// healthBar.setOutlineColor(Color::Black);
	// healthBar.setOutlineThickness(2.0f);
	healthBar.setScale(0.1f, 0.1f);
	healthBar.setPosition((resolution.x / 2) - 16, (resolution.y / 2) + 12);

	// When did we last update the HUD?
	int framesSinceLastHUDUpdate = 0;

	// How often (in frames) should we update the HUD?
	int fpsMeasurementFrameInterval = 1000;

	// // Prepare the hit sound
	// SoundBuffer hitBuffer;
	// hitBuffer.loadFromFile("content/sound/hit.wav");
	// Sound hit;
	// hit.setBuffer(hitBuffer);

	// // Prepare the splat sound
	// SoundBuffer splatBuffer;
	// splatBuffer.loadFromFile("content/sound/splat.wav");
	// sf::Sound splat;
	// splat.setBuffer(splatBuffer);

	// // Prepare the shoot sound
	// SoundBuffer shootBuffer;
	// shootBuffer.loadFromFile("content/sound/shoot.wav");
	// Sound shoot;
	// shoot.setBuffer(shootBuffer);

	// // Prepare the reload sound
	// SoundBuffer reloadBuffer;
	// reloadBuffer.loadFromFile("content/sound/reload.wav");
	// Sound reload;
	// reload.setBuffer(reloadBuffer);

	// // Prepare the failed sound
	// SoundBuffer reloadFailedBuffer;
	// reloadFailedBuffer.loadFromFile("content/sound/reload_failed.wav");
	// Sound reloadFailed;
	// reloadFailed.setBuffer(reloadFailedBuffer);

	// Prepare the powerup sound
	// SoundBuffer powerupBuffer;
	// powerupBuffer.loadFromFile("content/sound/powerup.wav");
	// Sound powerup;
	// powerup.setBuffer(powerupBuffer);

	// Prepare the pickup sound
	// SoundBuffer pickupBuffer;
	// pickupBuffer.loadFromFile("content/sound/pickup.wav");
	// Sound pickup;
	// pickup.setBuffer(pickupBuffer);

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
					// Prepare the level – we will update this later
					rArena.width = 1600;
					rArena.height = 1600;
					rArena.left = 0;
					//int tileSize = 50; // we will update this later
					rArena.top = 0;

					// Pass the vertex array by reference to the createBackground function
					int tileSize = createBackground(background, rArena);

					player.spawn(rArena, resolution, tileSize);

					wave = 1;
					score = 0;

					player.resetPlayerStats();
					turret.resetBullets();
					turret.resetTurretStats();

					// Create a horde of zombies
					numZombies = 20 * wave;

					// Delete the previously allocated memory (if it exists)
					delete[] zombies; // Note use of delete[] – should use [] when deleting arrays from heap.
					zombies = createHorde(numZombies, rArena);
					numZombiesAlive = numZombies;

					// Reset the clock so there isn't a frame jump
					clock.restart();

					state = State::PLAYING;
				}

				if (state == State::PLAYING)
				{
					// TODO fix dash
					// Dash
					if (event.key.code == Keyboard::Space)
					{
						player.startDash();
						// Play sound
						// BUG holding space gives infinite dash

						// check for collision with a zombie
						for (int i = 0; i < numZombies; i++)
						{
							if (player.checkCollision(zombies[i]))
							{
								// Play sound
								// add point
							}
						}

					} //End if (event.key.code == Keyboard::Space)

				} //End if (event.key.code == Keyboard::R)
			}	  //End if (event.type == Event::KeyPressed)

		} // End event polling

		// Stops the player dashing after a set time
		if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / 3)
		{
			player.stopDash();
			lastPressed = gameTimeTotal;
		}

		// Handle Collisions while Playing
		if (state == State::PLAYING)
		{
			for (int i = 0; i < numZombies; i++)
			{
				// Player is immune while dashing
				if (player.checkCollision(zombies[i]) && !player.isDashing())
				{
					player.hit(gameTimeTotal);
					// Play sound
					// add point
				}

				// Loops through all the bullets for each zombie and check for collision
				for (int j = 0; j < 100; j++)
				{
					// Skip checking collision if bullet is not moving
					if (turret.getBullet(j).isInFlight())
						continue;

					if (zombies[i].checkCollision(turret.getBullet(j)))
					{
						// Play sound
						zombies[i].hit();
					}
				}
			} // End zombies for loop

			// check for collision with Turret
			if (player.checkCollision(turret))
			{
				// reload turret when dashed through
				// TODO if player had enough points refil turrets ammo
				// if (player.getPoints() > 0)
				// {
				// 	int temp = player.getPoints();
				// 	player.setPoints(temp - 1);
				// 	// Play sound
				// }
				// else
				// {
				// 	// Play sound
				// }
				turret.resetBullets();
			}

			// Check if the player is dead or not
			if (!player.isAlive())
			{
				state = State::GAME_OVER;

				std::ofstream outputFile("gamedata/scores.txt");
				outputFile << hiScore;
				outputFile.close();
			}
		} // End handle collision

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
			if (Keyboard::isKeyPressed(Keyboard::F))
			{
				// TODO
				// Spawn the turret at players location
				turret.spawn(player.getCenter(), resolution);

			} //End if (event.key.code == Keyboard::F)

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
				// Increase turret fireRate
				turret.upgradeFireRate();
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num3)
			{
				// increase dash distance
				player.upgradeDash();
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num4)
			{
				// increase run speed
				player.upgradeSpeed();
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num5)
			{
				// increase player health
				player.upgradeHealth();
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num6)
			{
				// increase total allowed turrets
				state = State::PLAYING;
			}

			if (state == State::PLAYING)
			{
				// Increments the level by 1
				player.increaseLevel();

				// Increaes the wave number
				wave++;

				// // Create a horde of zombies
				// numZombies = 20 * wave;

				// // Delete the previously allocated memory (if it exists)
				// delete[] zombies; // Note use of delete[] – should use [] when deleting arrays from heap.
				// zombies = createHorde(numZombies, rArena);
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
			mainView.setCenter(playerPosition);

			// Convert mouse position to world coordinates of mainView
			// Loop through each Zombie and update them if alive
			bool hasTarget = false;
			for (int i = 0; i < numZombies; i++)
			{
				if (zombies[i].isAlive())
				{
					zombies[i].update(dt.asSeconds(), playerPosition);
					if (hasTarget)
						break;
					hasTarget = true;
					// FloatRect zombieWorldPosition = zombies[i].getPosition();
					// Vector2f zombieWorldPosition = window.mapPixelToCoords(zombies[i].getCenter());
					// turret.update(zombieWorldPosition);
					turret.update(gameTimeTotal, zombies[i].getCenter());
				}
				// else if (hasTarget)
				else
				{
					hasTarget = false;
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

			// Size up the health bar
			healthBar.setSize(Vector2f(player.getHealth() * 3, 70));

			// Increment the number of frames since the last HUD calculation
			framesSinceLastHUDUpdate++;

			// Calculate FPS every fpsMeasurementFrameInterval frames
			if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval)
			{
				std::stringstream ssScore;
				std::stringstream ssHiScore;
				std::stringstream ssWave;
				std::stringstream ssZombiesAlive;

				// Update the score text
				ssScore << "Score:" << score;
				scoreText.setString(ssScore.str());

				// Update the high score text
				ssHiScore << "Hi Score:" << hiScore;
				hiScoreText.setString(ssHiScore.str());

				// Update the wave text
				ssWave << "Wave:" << wave;
				waveNumberText.setString(ssWave.str());

				// Update the Zombies text
				ssZombiesAlive << "Zombies:" << numZombiesAlive;
				zombiesRemainingText.setString(ssZombiesAlive.str());

				framesSinceLastHUDUpdate = 0;
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

			window.setView(HudView);
			// Draw all the HUD elements
			window.draw(healthBar);
			// window.draw(waveNumberText);
			// window.draw(zombiesRemainingText);
		}

		if (state == State::LEVELING_UP)
		{
			window.draw(pausedText);
			window.draw(levelUpText);
		}

		if (state == State::PAUSED)
		{
			window.draw(pausedText);
		}

		if (state == State::GAME_OVER)
		{
			// window.draw(spriteGameOver);
			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(gameOverText);
		}

		window.display();
	}

	// Delete the previously allocated memory (if it exists)
	delete[] zombies;

	return 0;
}
