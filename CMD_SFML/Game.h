#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include <iostream>
#include <vector>
#include <ctime>
#include <random>


class Game
{
private:
	//variables
	//window
	sf::RenderWindow* Level1;
	sf::RenderWindow* Level2;

	//Game Logic
	int points;
	int missedEnemy;
	int maxEnemies;
	float enemySpawnTimer;
	float enemySpawnTimerMax;

	int currentLevel;
	bool level1Completed;



	//FPS
	sf::Clock clock;
	float deltaTime;
	float fps;
	
	//Game Objects
	std::vector<sf::RectangleShape> enemies_level1;
	std::vector<sf::RectangleShape> enemies_level2;
	sf::RectangleShape enemy;

	//Mouse Pos
	sf::Vector2i mousePosition;
	sf::Vector2f mousePosView;

	void intVariables();
	void intWindow();
	void intEnemy();
public:
	Game();
	virtual ~Game();

	//Accessors
	const bool running() const;

	//Functions
	void spawnEnemy();
	void pollEvent();

	void render();
	void renderEnemies();

	void update();
	void updateEnemies();
	void updateMousePosition();

	///Level 1
	// create/spawn
	void createLevel1(std::string Title);
	void spawnEnemy_Level1();
	void pollEvent_Level1();

	// render
	void render_Leve1();
	void renderEnemy_Level1();

	// update
	void update_Level1();
	void updateEnemies_Level1();
	void updateMousePos_level1();
	void updateEnemySpeed_Leve1(int i);

	///Level 2
	// create/spawn
	void createLevel2(std::string Title);
	void spawnEnemy_Level2();
	void pollEvent_Level2();

	// render
	void render_Leve2();
	void renderEnemy_Level2();

	// update
	void update_Level2();
	void updateEnemies_Level2();
	void updateMousePos_level2();
	void updateEnemySpeed_Leve2(int i);
};

