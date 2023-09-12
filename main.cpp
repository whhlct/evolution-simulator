#include <SFML/Graphics.hpp>
#include <iostream>
//#include <time.h>
//#include <stdlib.h>
#include "include/game.hpp"

int main()
{
	// create window
	float SCREEN_WIDTH = 1920;
	float SCREEN_HEIGHT = 1080;
	float WORLD_WIDTH = 10000.f;
	float WORLD_HEIGHT = 10000.f;
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Evolution Simulator", (sf::Style::None));
	window.setFramerateLimit(60);
	
	sf::View view(sf::Vector2f(WORLD_WIDTH / 2, WORLD_HEIGHT / 2), sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
	view.zoom(2.f);
	srand(time(NULL));
	
	// change in time
	const double dt = 0.0166f;
	// create world, world.predators and prey
	World world(WORLD_WIDTH, WORLD_HEIGHT);
	Renderer renderer(world, window);

	// add predator and prey
	
	world.predators.push_back(Predator(5000.f, 5000.f));
	
	for (int i = 0; i < 14; i++) {
		for (int j = 0; j < 14; j++) {
			world.preys.push_back(Prey(i * 750, j * 750));
		}
	}
	
	
	world.preys.push_back(Prey(5500, 5100, 0));
	world.preys.push_back(Prey(5500, 5300, 10));
	world.preys.push_back(Prey(5500, 5500, 20));
	world.preys.push_back(Prey(5300, 5500, 30));
	world.preys.push_back(Prey(5100, 5500, 40));
	
	
	//world.preys.front().rotation = 0.5f;
	Predator& e = world.predators.front();
	
	float speed = 0.0f;
	float turn = 0.0f;
	float speedGrav = 0.0f;
	
	while (window.isOpen())
	{
		//event_manager.processEvents();

		//world.initialize();
		// create event object and poll events
		
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
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			e.speed = std::min(1.0, e.speed + 0.02);
		}
		else {
			e.speed = std::max(0.0, e.speed - 0.005);
		}
		//e.speed = sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ? 1.0f : 0.0f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			e.rotation = -1.0f;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			e.rotation = 1.0f;
		else
			e.rotation = 0.0f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			view.move(0.f, -10.f);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			view.move(-10.f, 0.f);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			view.move(0.f, 10.f);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			view.move(10.f, 0.f);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			view.zoom(1.02f);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
			view.zoom(1 / 1.02f);
		
		window.setView(view);
		
		world.update(dt);

		
		// Graphics
		window.clear(sf::Color::White);
		
		renderer.draw(world);
		for (auto& predator : world.predators)
		{
			renderer.draw(predator);
		}
		
		for (auto& prey : world.preys)
		{
			renderer.draw(prey);
		}
		//sf::CircleShape closest_circle(10.f);
		//sf::CircleShape contact_circle(10.f);
		//closest_circle.setFillColor(sf::Color::Red);
		//contact_circle.setOrigin(5.f, 5.f);
		//closest_circle.setOrigin(5.f, 5.f);
		world.updateVision();
		window.draw(world.predators[0].lines);
		/*
		for (Prey& prey : world.preys) {
			window.draw(prey.lines);
		}
		*/
		
		window.setView(window.getDefaultView());
		renderer.draw_bar(e.energy, SCREEN_HEIGHT, 400, 40);
		renderer.draw_bar(e.speed, SCREEN_HEIGHT - 40, 400, 40);
		if (e.digestion > 0.f) {
			renderer.draw_bar(e.digestion, SCREEN_HEIGHT, 400, 40, sf::Color::Red, 200);
		}
		renderer.draw_text(std::to_string(world.preys.size()), 0, 0, 64);
		renderer.draw_text(std::to_string(world.predators[0].ray_values[0]), 0, 400);
		
		window.display();

	}

	return 0;
}