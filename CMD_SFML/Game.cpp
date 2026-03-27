#include "Game.h"

void Game::intVariables()
{
	this->Level1 = nullptr;
	this->Level2 = nullptr;

	//Game Logic
	this->points = 0;
	this->enemySpawnTimerMax = 5.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 1;
	this->missedEnemy = 0;

	this->currentLevel = 1;
	this->level1Completed = false;
	this->enemySpeed = 1;
	
}

void Game::intWindow()
{
	createLevel1("Try To Win (Level 1)");
}

void Game::intEnemy()
{
	this->enemy.setPosition({ 10.f, 10.f });
	this->enemy.setSize(sf::Vector2f( 100.f, 100.f));
	this->enemy.setScale(sf::Vector2f( 0.5f, 0.5f ));
	this->enemy.setFillColor(sf::Color::Red);
	this->enemy.setOutlineColor(sf::Color::Black);
	this->enemy.setOutlineThickness(1.f);
}

Game::Game()
{
	this->intVariables();
	this->intWindow();
	this->intEnemy();
	this->initText();
}

Game::~Game()
{
	delete this->Level1;
	delete this->Level2;
}

const bool Game::running() const
{
	if (this->currentLevel == 1)
	{
		return this->Level1->isOpen();
	}
	else
	{
		return this->Level2->isOpen();
	}
}

void Game::pollEvent()
{
	this->pollEvent_Level1();
	this->pollEvent_Level2();
}

void Game::update()
{
	if (this->currentLevel == 1)
	{
		this->update_Level1();
	}
	else if (this->currentLevel == 2)
	{
		this->update_Level2();
	}
}

void Game::updateEnemies()
{
	this->updateEnemies_Level1();
	this->updateEnemies_Level2();
}


void Game::updateMousePosition()
{
	this->updateMousePos_level1();
	this->updateMousePos_level2();
}

void Game::initText()
{
	try {
		font = sf::Font("arial.ttf");
	}
	catch (const std::exception& e) {
		std::cout << "Failed to load font: " << e.what() << "\n";
	}

	// Construct in-place into the optional
	text.emplace(font, "Points: 0", 24);
	text->setFillColor(sf::Color::Black);
	text->setPosition({ 10.f, 10.f });
}

void Game::updateText()
{
	if (!text) return;

	std::string levelStr = (currentLevel == 1) ? "1/2" : "2/2";
	text->setString(
		std::to_string((int)fps) + "fps\n" +
		"Points: " + std::to_string(points) + "/300\n" +
		"Missed: " + std::to_string(missedEnemy) + "/10\n" +
		"Level: " + levelStr + "\n" +
		"Speed: " + std::to_string(enemySpeed) + "x/6x"
	);
}


void Game::spawnEnemy()
{
	if (this->currentLevel == 1)
	{
		this->spawnEnemy_Level1();
	}
	else if (this->currentLevel == 2)
	{
		this->spawnEnemy_Level2();
	}
}

void Game::render()
{
	if (this->currentLevel == 1)
	{
		this->render_Leve1();
	}
	else if (this->currentLevel == 2)
	{
		this->render_Leve2();
	}
}

void Game::renderEnemies()
{
	this->renderEnemy_Level1();
	this->renderEnemy_Level2();
}



/// Level1
void Game::createLevel1(std::string Title)
{
	this->Level1 = new sf::RenderWindow(sf::VideoMode({ 800, 600 }), Title, sf::Style::Titlebar | sf::Style::Close);
	this->Level1->setFramerateLimit(100);
}

void Game::spawnEnemy_Level1()
{
	this->enemy.setPosition({
		static_cast<float>(rand() % static_cast<int>(this->Level1->getSize().x - this->enemy.getSize().x)),
		0.f
		});

	this->enemy.setFillColor(sf::Color(
		rand() % 256,
		rand() % 256,
		rand() % 256
	));
	this->enemies_level1.push_back(this->enemy);
}

void Game::pollEvent_Level1()
{
	while (const std::optional ev = this->Level1->pollEvent())
	{
		if (ev->is<sf::Event::Closed>())
			this->Level1->close();
	}
}

void Game::render_Leve1()
{
	this->Level1->clear(sf::Color::Cyan);

	if (text)                            // ✅ guard
		this->Level1->draw(*text);

	this->renderEnemy_Level1();
	this->Level1->display();
}

void Game::renderEnemy_Level1()
{
	for (auto& e : this->enemies_level1)
	{
		this->Level1->draw(e);
	}
}

void Game::update_Level1()
{
	//FPS
	this->deltaTime = this->clock.restart().asSeconds();
	this->fps = 1.f / this->deltaTime;
	std::cout << "FPS: " << (int)this->fps << "\n";

	//Points
	std::cout << "Points: " << this->points << "\n";
	std::cout << "Missed: " << this->missedEnemy << "\n";
	this->updateText();
	this->pollEvent_Level1();
	this->updateMousePos_level1();
	this->updateEnemies_Level1();
}

void Game::updateEnemies_Level1()
{
	// Spawn normally
	if (this->enemies_level1.size() < this->maxEnemies)
	{
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
		{
			spawnEnemy();
			this->enemySpawnTimer = 0.f;
		}
		else
		{
			this->enemySpawnTimer += 1.f;
		}
	}

	if (this->missedEnemy >= 10)
	{
		this->Level1->close();
		std::cout << "!!You Loose!!\n";
	}

	// Move enemies
	for (int i = 0; i < this->enemies_level1.size(); i++)
	{
		this->updateEnemySpeed_Leve1(i);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			if (this->enemies_level1[i].getGlobalBounds().contains(this->mousePosView))
			{
				this->enemies_level1.erase(this->enemies_level1.begin() + i);
				this->points += 1;
			}
		}
	}

	// Remove enemies outside Level1
	for (size_t i = 0; i < this->enemies_level1.size(); )
	{
		if (this->enemies_level1[i].getPosition().y > this->Level1->getSize().y)
		{
			this->enemies_level1.erase(this->enemies_level1.begin() + i);
			this->missedEnemy += 1;
		}
		else
		{
			++i;
		}
	}

	// If all enemies are gone → respawn batch
	/*if (this->enemies.empty())
	{
		for (int i = 0; i < this->maxEnemies; i++)
		{
			spawnEnemy();
		}
	}*/
}

void Game::updateMousePos_level1()
{
	this->mousePosition = sf::Mouse::getPosition(*this->Level1);
	this->mousePosView = this->Level1->mapPixelToCoords(this->mousePosition);
}

void Game::updateEnemySpeed_Leve1(int i)
{
	if (this->points < 50)
	{
		this->enemies_level1[i].move({ 0.f, 1.f });
	}
	else if (this->points < 100)
	{
		this->enemies_level1[i].move({ 0.f, 2.f });
		this->enemySpeed = 2;
	}
	else if (this->points < 150)
	{
		this->enemies_level1[i].move({ 0.f, 3.f });
		this->enemySpeed = 3;
	}
	else if (this->points < 200)
	{
		this->enemies_level1[i].move({ 0.f, 4.f });
		this->enemySpeed = 4;
	}
	else if (this->points < 250)
	{
		this->enemies_level1[i].move({ 0.f, 5.f });
		this->enemySpeed = 5;
	}
	else if (this->points < 300)
	{
		this->enemies_level1[i].move({ 0.f, 6.f });
		this->enemySpeed = 6;
	}
	else
	{
		std::cout << "!You Win Level 1!\n";

		this->Level1->close();
		this->level1Completed = true;

		this->currentLevel = 2;
		this->maxEnemies = 2;
		this->enemySpawnTimerMax = 6.f;

		// Create Level2 window
		createLevel2("Level 2");

		// Reset stats for level 2
		this->missedEnemy = 0;
		this->points = 0;
	}
}


///Level 2
void Game::createLevel2(std::string Title)
{
	this->Level2 = new sf::RenderWindow(sf::VideoMode({ 800, 600 }), Title, sf::Style::Titlebar | sf::Style::Close);
	this->Level2->setFramerateLimit(100);
}

void Game::spawnEnemy_Level2()
{
	this->enemy.setPosition({
		static_cast<float>(rand() % static_cast<int>(this->Level2->getSize().x - this->enemy.getSize().x)),
		0.f
		});

	this->enemy.setFillColor(sf::Color(
		rand() % 256,
		rand() % 256,
		rand() % 256
	));
	this->enemies_level2.push_back(this->enemy);
}

void Game::pollEvent_Level2()
{
	if (!this->Level2) return;  // ✅ guard
	while (const std::optional ev = this->Level2->pollEvent())
	{
		if (ev->is<sf::Event::Closed>())
			this->Level2->close();
	}
}

void Game::render_Leve2()
{
	// background color
	this->Level2->clear(sf::Color::Cyan);

	if (text)                            // ✅ guard
		this->Level2->draw(*text);

	this->renderEnemy_Level2();
	this->Level2->display();
}

void Game::renderEnemy_Level2()
{
	for (auto& e : this->enemies_level2)
	{
		this->Level2->draw(e);
	}
}

void Game::update_Level2()
{
	//FPS
	this->deltaTime = this->clock.restart().asSeconds();
	this->fps = 1.f / this->deltaTime;
	std::cout << "FPS: " << (int)this->fps << "\n";

	//Points
	std::cout << "Points: " << this->points << "\n";
	std::cout << "Missed: " << this->missedEnemy << "\n";

	this->updateText();  // ✅ add this
	this->pollEvent_Level2();
	this->updateMousePos_level2();
	this->updateEnemies_Level2();
}

void Game::updateEnemies_Level2()
{
	// Spawn normally
	if (this->enemies_level2.size() < this->maxEnemies)
	{
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
		{
			spawnEnemy();
			this->enemySpawnTimer = 0.f;
		}
		else
		{
			this->enemySpawnTimer += 1.f;
		}
	}

	if (this->missedEnemy >= 10)
	{
		this->Level2->close();
		std::cout << "!!You Loose!!\n";
	}

	// Move enemies
	for (int i = 0; i < this->enemies_level2.size(); i++)
	{
		this->updateEnemySpeed_Leve2(i);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			if (this->enemies_level2[i].getGlobalBounds().contains(this->mousePosView))
			{
				this->enemies_level2.erase(this->enemies_level2.begin() + i);
				this->points += 1;
			}
		}
	}

	// Remove enemies outside Level2
	for (size_t i = 0; i < this->enemies_level2.size(); )
	{
		if (this->enemies_level2[i].getPosition().y > this->Level2->getSize().y)
		{
			this->enemies_level2.erase(this->enemies_level2.begin() + i);
			this->missedEnemy += 1;
		}
		else
		{
			++i;
		}
	}

	// If all enemies are gone → respawn batch
	/*if (this->enemies.empty())
	{
		for (int i = 0; i < this->maxEnemies; i++)
		{
			spawnEnemy();
		}
	}*/
}

void Game::updateMousePos_level2()
{
	if (!this->Level2) return;  // ✅ guard
	this->mousePosition = sf::Mouse::getPosition(*this->Level2);
	this->mousePosView = this->Level2->mapPixelToCoords(this->mousePosition);
}

void Game::updateEnemySpeed_Leve2(int i)
{
	if (this->points < 50)
	{
		this->enemies_level2[i].move({ 0.f, 2.f });
		this->enemySpeed = 2;
	}
	else if (this->points < 100)
	{
		this->enemies_level2[i].move({ 0.f, 3.f });
		this->enemySpeed = 3;
	}
	else if (this->points < 150)
	{
		this->enemies_level2[i].move({ 0.f, 4.f });
		this->enemySpeed = 4;
	}
	else if (this->points < 200)
	{
		this->enemies_level2[i].move({ 0.f, 5.f });
		this->enemySpeed = 5;
	}
	else if (this->points < 250)
	{
		this->enemies_level2[i].move({ 0.f, 6.f });
		this->enemySpeed = 6;
	}
	else if (this->points < 300)
	{
		this->enemies_level2[i].move({ 0.f, 7.f });
		this->enemySpeed = 7;
	}
	else
	{
		this->Level2->close();
		std::cout << "!You Win!";
	}
}
