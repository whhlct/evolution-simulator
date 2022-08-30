#pragma once
#include <SFML/Graphics.hpp>
#include "direction.hpp"

struct Entity
{
	enum class Type
	{
		Prey,
		Predator
	};
	Type type;
	float radius = 50.0f;
	float speed = 0.0f;
	float rotation = 0.0f;
	float speedLimit = 500.0f;
	double energy = 1.0;
	bool alive = true;
	sf::Vector2f position;
	Direction direction;

	Entity(float x, float y, float angle = 0.0f)
		: position(x, y)
		, direction(angle)
	{}
	/*
	void update(double dt)
	{
		double energy_cost = (0.05 + 0.2 * speed) * dt;
		if (energy >= energy_cost)
		{
			energy -= energy_cost;
			updatePosition(dt);
		}
		else
		{
			alive = false;
		}
	} */

protected:
	void updatePosition(float dt)
	{
		direction.update(dt, rotation);
		position += direction.getVector() * (speed * speedLimit * dt);
	}
};

struct Predator : Entity
{
	Predator(float x, float y, float angle = 0.0f)
		: Entity(x, y, angle)
	{
		type = Type::Predator;
	}
	
	void update(double dt)
	{
		if (alive)
		{
			double energy_cost = (0.05 + 0.2 * speed) * dt; // calculate energy expense
			energy -= energy_cost; // update energy
			updatePosition(dt); // update position
			if (energy <= 0.0)
				alive = false; // predator dies if energy runs out
		}
	}
};

struct Prey : Entity
{
	Prey(float x, float y, float angle = 0.0f)
		: Entity(x, y, angle)
	{
		type = Type::Prey;
	}

	void update(double dt)
	{
		double energy_cost = (-0.05 + 0.2 * speed) * dt; // calculate energy expense
		
		if (energy >= energy_cost) // check if prey has energy to move
		{
			energy -= energy_cost; // update energy
			energy = energy <= 1.0 ? energy : 1.0; // prevent energy from exceeding 100%
			updatePosition(dt); // update position
		}
		else
			energy += 0.05 * dt;
	}
	
};

struct World
{
	float width;
	float height;
	
	World() : width(2000.0f), height(2000.0f) {};
	World(float width_, float height_)
		: width(width_)
		, height(height_)
	{}
};

struct Renderer
{
	const World& world;
	sf::RenderTarget& target;

	Renderer(const World& world_, sf::RenderTarget& target_)
		: world(world_)
		, target(target_)
	{}

	void draw(const Entity& entity)
	{
		sf::Color color = (entity.type == Entity::Type::Predator) ? sf::Color::Red : sf::Color::Green;
		// Entity main body
		sf::CircleShape circle(entity.radius);
		circle.setOrigin(entity.radius, entity.radius);
		circle.setPosition(entity.position);
		circle.setFillColor(color);

		// Entity angle indicator
		sf::RectangleShape rectangle(sf::Vector2f(entity.radius * 2, 10.0f));
		rectangle.setOrigin(0.0f, 5.0f);
		rectangle.setPosition(entity.position);
		rectangle.rotate(entity.direction.getAngle());
		rectangle.setFillColor(color);
		
		target.draw(circle);
		target.draw(rectangle);
	}

	void draw(const World& world)
	{
		sf::RectangleShape shape(sf::Vector2f(world.width, world.height));
		shape.setFillColor(sf::Color::White);
		shape.setPosition(sf::Vector2f(0.0f, 0.0f));
		target.draw(shape);
	}
	
	void draw_bar(double value, float y = 800, double scale = 500)
	{
		sf::RectangleShape blackShape(sf::Vector2f(scale, 50));
		blackShape.setOrigin(0, 50);
		blackShape.setPosition(0, y);
		blackShape.setFillColor(sf::Color::Black);
		sf::RectangleShape shape(sf::Vector2f(value * scale, 50));
		shape.setOrigin(0, 50);
		shape.setPosition(0, y);
		shape.setFillColor(sf::Color::Green);
		
		target.draw(blackShape);
		target.draw(shape);
	}
};