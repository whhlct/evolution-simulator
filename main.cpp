#include <SFML/Graphics.hpp>
#include <iostream>
#include "include/game.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "Evolution Simulator", (sf::Style::Titlebar, sf::Style::Close));
	window.setFramerateLimit(60);
	
	// foad font and text
	sf::Font font;

	if (!font.loadFromFile("Arial.ttf"))
	{
		return EXIT_FAILURE;
	}

	sf::Text text;
	text.setFont(font);
	text.setString("");
	text.setCharacterSize(50);
	text.setFillColor(sf::Color::Black);
	text.setStyle(sf::Text::Bold);
	
	const double dt = 0.0166f;

	World world(2000.0f, 2000.0f);
	Renderer renderer(world, window);
	std::vector<Predator> predators;
	std::vector<Prey> preys;
	
	predators.push_back(Predator(100, 100));
	preys.push_back(Prey(500, 100));
	preys.push_back(Prey(500, 300));
	preys.push_back(Prey(500, 500));
	preys.push_back(Prey(300, 500));
	preys.push_back(Prey(100, 500));
	
	preys.front().rotation = 0.5f;
	Predator& e = predators.front();
	
	float speed = 0.0f;
	float turn = 0.0f;

	while (window.isOpen())
	{
		//event_manager.processEvents();

		//world.initialize();

		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				window.close();
				
		}
		
		
		float time = 0.0f;
		time += dt;

		e.speed = sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ? 1.0f : 0.0f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			e.rotation = -1.0f;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			e.rotation = 1.0f;
		else
			e.rotation = 0.0f;
		//world.update(dt);
		for (auto& predator : predators)
		{
			predator.update(dt);
			if (predator.alive)
			{
				sf::Vector2f diff;
				for (auto& prey : preys)
				{
					if (prey.alive)
					{
						diff = predator.position - prey.position;
						if (diff.x * diff.x + diff.y * diff.y < predator.radius * predator.radius * 4)
						{
							prey.alive = false;
							predator.energy = std::min(predator.energy + 0.1, 1.0);
							//std::cout << dx << ", " << dy << std::endl;
						}
					}
				}
			}
		}
		for (auto& prey : preys)
		{
			prey.update(dt);
		}
		std::remove_if(preys.begin(), preys.end(), [](Prey& prey) { return !prey.alive; });
		// Graphics
		window.clear(sf::Color::White);
		
		renderer.draw(world);
		for (auto& predator : predators)
		{
			renderer.draw(predator);
		}
		
		for (auto& prey : preys)
		{
			renderer.draw(prey);
		}

		renderer.draw_bar(e.energy);
		//text.setString(std::to_string(e.direction.getAngle()));
		//window.draw(text);
		
		window.display();

	}

	return 0;
}