#pragma once
#include <SFML/Graphics.hpp>
//#include <math.h>
#include "direction.hpp"

//#define PI 3.14159265
bool lineCircle(float x1, float y1, float x2, float y2, float cx, float cy, float r, float& closestX, float& closestY);
float dist(float x1, float y1, float x2, float y2);

struct World;

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
	float reproductionEnergy = 4.f;
	float digestion = 1.0f;
	bool alive = true;
	sf::Vector2f position;
	Direction direction;
	float ray_angles[24];
	float ray_distance;
	float ray_values[24];
	sf::VertexArray lines;
	
	Entity(float x, float y, float angle = 0.0f)
		: position(x, y)
		, direction(angle)
	{
		lines = sf::VertexArray(sf::Lines, 48);
	}

protected:
	void updatePosition(float dt)
	{
		direction.update(dt, rotation);
		position += direction.getVector() * (speed * speedLimit * dt);
	}
public:
	// Update rays of entity
	/*
	void updateVision(std::vector<Entity> *entityList)
	{
		for (int i = 0; i < 24; i++) {
			ray_values[i] = ray_distance;
			// Draw ray lines
			lines[i*2].position = position;
			lines[i * 2 + 1].position = position + sf::Vector2f(std::cos((direction + ray_angles[i]) * 3.14159 / 180), std::sin((direction + ray_angles[i]) * 3.14159 / 180)) * ray_distance;
			float px, py;
			for (Entity& entity : *entityList) {
				if (lineCircle(lines[i * 2].position.x, lines[i * 2].position.y, lines[i * 2 + 1].position.x, lines[i * 2 + 1].position.y, entity.position.x, entity.position.y, entity.radius, px, py)) {
					float distance = dist(position.x, position.y, px, py);
					//contact_circle.setPosition(px, py);
					if (distance < ray_values[i])
					{
						ray_values[i] = distance;
					}
					//window.draw(contact_circle);
				}
			}
			//closest_circle.setPosition(position + sf::Vector2f(50.f, 50.f));
			//closest_circle.setPosition(position + sf::Vector2f(std::cos((direction + ray_angles[i]) * 3.14159 / 180), std::sin((direction + ray_angles[i]) * 3.14159 / 180)) * ray_values[i]);
			lines[i * 2 + 1].position = position + sf::Vector2f(std::cos((direction + ray_angles[i]) * 3.14159 / 180), std::sin((direction + ray_angles[i]) * 3.14159 / 180)) * ray_values[i];
			//window.draw(closest_circle);
		}

		//window.draw(lines);
	}
	*/
};

struct Predator : Entity
{
	double energy_cost = 0; // (1 / 15.0);
	
	Predator(float x, float y, float angle = 0.0f)
		: Entity(x, y, angle)
	{
		type = Type::Predator;
		ray_distance = 4000.f;
		for (int i = 0; i < 24; i++) {
			ray_angles[i] = i * 2 - 23;
		}
	}
	
	void update(double dt)
	{
		if (alive)
		{
			double cost = energy_cost * dt; // calculate energy expense, predators don't lose energy from running
			energy -= cost; // update energy
			digestion = std::max(0.0, digestion - dt); // update digestion
			updatePosition(dt); // update position
			if (energy <= 0.0)
				alive = false; // predator dies if energy runs out
		}
	}
	
	Predator reproduce() {
		reproductionEnergy = 1.f;
		float randAngle = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * PI * 2;
		return Predator(position.x + cos(randAngle) * 100, position.y + sin(randAngle) * 100);
	}
};

struct Prey : Entity
{
	Prey(float x, float y, float angle = 0.0f)
		: Entity(x, y, angle)
	{
		type = Type::Prey;
		ray_distance = 2000.f;
		for (int i = 0; i < 24; i++) {
			ray_angles[i] = i * 15 - 180;
		}
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
		reproductionEnergy = std::max(reproductionEnergy - dt, 0.0); // update reproduction energy. Should take prey 5-7s to reproduce
	}
	
	Prey reproduce() {
		// set reproduction timer to 5-7 seconds
		reproductionEnergy = 5.f + static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2.f;
		// create random angle for child to spawn
		float randAngle = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * PI * 2;
		// create child
		return Prey(position.x + cos(randAngle) * 100, position.y + sin(randAngle) * 100);
	}
};

// cell = 75*75 px
// entity = 100*100 px
// small grid square = 2 cells, 150*150 px
// large grid square = 5 small, 10 cells, 750*750 px

struct World
{
	float width;
	float height;
	sf::RenderTexture world_texture;
	sf::Sprite world_sprite;
	std::vector<Predator> predators;
	std::vector<Prey> preys;
	int PREY_MAX = 2000;
	
	World(float width_, float height_)
		: width(width_)
		, height(height_)
	{
		int squareSize = 150;
		int vertical_lines = (int)width / squareSize;
		int horizontal_lines = (int)height / squareSize;
		
		world_texture.create(squareSize * 5, squareSize * 5);
		world_texture.setRepeated(true);
		sf::RectangleShape line(sf::Vector2f(squareSize * 5, 4.f));
		line.setOrigin(0.f, 2.f);
		line.setFillColor(sf::Color::Color(140, 140, 140, 255));
		
		for (int i = 1; i <= 4; i ++) {
			line.setRotation(0);
			line.setPosition(0, i * squareSize);
			world_texture.draw(line);
			line.setRotation(90);
			line.setPosition(i * squareSize, 0);
			world_texture.draw(line);
		}
		
		line.setRotation(0);
		line.setSize(sf::Vector2f(squareSize * 5, 10.f));
		line.setOrigin(0.f, 5.f);
		line.setPosition(0, 0);
		world_texture.draw(line);
		line.setPosition(0, squareSize * 5);
		world_texture.draw(line);
		
		line.setRotation(90);
		line.setPosition(0.f, 0.f);
		world_texture.draw(line);
		line.setPosition(squareSize * 5, 0.f);
		world_texture.draw(line);
		world_sprite.setTexture(world_texture.getTexture());
		world_sprite.setTextureRect(sf::IntRect(0, 0, width, height));
	}
	
	void update(double dt)
	{
		// Call update for all predators and prey
		for (auto& predator : predators)
		{
			predator.update(dt);
			if (predator.alive)
			{
				sf::Vector2f diff;
				for (auto& prey : preys)
				{
					diff = predator.position - prey.position;
					if (diff.x * diff.x + diff.y * diff.y < predator.radius * predator.radius * 4) // if predator and prey collide
					{
						prey.alive = false; // kill prey
						if (predator.digestion <= 0.f) {
							predator.energy = std::min(predator.energy + 0.1, 1.0);
							predator.digestion = 1.f;
						}
					}
				}
			}
		}
		for (auto& prey : preys)
		{
			prey.update(dt);
		}
		
		// remove dead predators
		predators.erase(
			std::remove_if(predators.begin(), predators.end(), [](Predator& predator) { return !predator.alive; }),
			predators.end());
		// remove dead prey
		preys.erase(
			std::remove_if(preys.begin(), preys.end(), [](Prey& prey) { return !prey.alive; }),
			preys.end());

		// reproduce
		int preysize = preys.size();
		for (int i = 0; i < preysize; i++) {
			if (preys[i].reproductionEnergy <= 0.f && preys.size() < PREY_MAX) {
				preys.push_back(preys[i].reproduce());
			}
		}
	}
	void updateVision()
	{
		// UPDATE PREDATOR RAYS
		for (Predator& predator : predators)
		{
			for (int i = 0; i < 24; i++)
			{
				predator.ray_values[i] = predator.ray_distance;
				// Draw ray lines
				predator.lines[i*2].position = predator.position;
				predator.lines[i * 2 + 1].position = predator.position + sf::Vector2f(std::cos((predator.direction + predator.ray_angles[i]) * 3.14159 / 180), std::sin((predator.direction + predator.ray_angles[i]) * 3.14159 / 180)) * predator.ray_distance;
				float px, py;
				for (Prey& prey : preys) {
					if (lineCircle(predator.lines[i * 2].position.x, predator.lines[i * 2].position.y, predator.lines[i * 2 + 1].position.x, predator.lines[i * 2 + 1].position.y, prey.position.x, prey.position.y, prey.radius, px, py)) {
						float distance = dist(predator.position.x, predator.position.y, px, py);
						//contact_circle.setPosition(px, py);
						if (distance < predator.ray_values[i])
						{
							predator.ray_values[i] = distance;
						}
						//window.draw(contact_circle);
					}
				}
				//closest_circle.setPosition(position + sf::Vector2f(50.f, 50.f));
				//closest_circle.setPosition(position + sf::Vector2f(std::cos((direction + ray_angles[i]) * 3.14159 / 180), std::sin((direction + ray_angles[i]) * 3.14159 / 180)) * ray_values[i]);
				predator.lines[i * 2 + 1].position = predator.position + sf::Vector2f(std::cos((predator.direction + predator.ray_angles[i]) * 3.14159 / 180), std::sin((predator.direction + predator.ray_angles[i]) * 3.14159 / 180)) * predator.ray_values[i];
				//window.draw(closest_circle);
			}
		}

		// UPDATE PREY RAYS
		for (Prey& prey : preys)
		{
			for (int i = 0; i < 24; i++)
			{
				prey.ray_values[i] = prey.ray_distance;
				// Draw ray lines
				prey.lines[i*2].position = prey.position;
				prey.lines[i * 2 + 1].position = prey.position + sf::Vector2f(std::cos((prey.direction + prey.ray_angles[i]) * 3.14159 / 180), std::sin((prey.direction + prey.ray_angles[i]) * 3.14159 / 180)) * prey.ray_distance;
				float px, py;
				for (Predator& predator : predators) {
					if (lineCircle(prey.lines[i * 2].position.x, prey.lines[i * 2].position.y, prey.lines[i * 2 + 1].position.x, prey.lines[i * 2 + 1].position.y, predator.position.x, predator.position.y, predator.radius, px, py)) {
						float distance = dist(prey.position.x, prey.position.y, px, py);
						//contact_circle.setPosition(px, py);
						if (distance < prey.ray_values[i])
						{
							prey.ray_values[i] = distance;
						}
						//window.draw(contact_circle);
					}
				}
				//closest_circle.setPosition(position + sf::Vector2f(50.f, 50.f));
				//closest_circle.setPosition(position + sf::Vector2f(std::cos((direction + ray_angles[i]) * 3.14159 / 180), std::sin((direction + ray_angles[i]) * 3.14159 / 180)) * ray_values[i]);
				prey.lines[i * 2 + 1].position = prey.position + sf::Vector2f(std::cos((prey.direction + prey.ray_angles[i]) * 3.14159 / 180), std::sin((prey.direction + prey.ray_angles[i]) * 3.14159 / 180)) * prey.ray_values[i];
				//window.draw(closest_circle);
			}
		}
	}
};

struct Renderer
{
	const World& world;
	sf::RenderTarget& target;
	float zoom = 1.0f; // render pixels : real pixels, lower = zoomed out more
	sf::Vector2f cameraOffset; // in terms of real pixels
	sf::Font font;

	Renderer(const World& world_, sf::RenderTarget& target_)
		: world(world_)
		, target(target_)
	{
		if (!font.loadFromFile("..\\..\\Arial.ttf"))
		{
			exit(-1);
		}
	}

	void draw(const Entity& entity)
	{
		sf::Color color = (entity.type == Entity::Type::Predator) ? sf::Color::Red : sf::Color::Green;
		// Entity main body
		sf::CircleShape circle(entity.radius * zoom);
		circle.setOrigin(entity.radius * zoom, entity.radius * zoom);
		circle.setPosition(entity.position * zoom - cameraOffset);
		circle.setFillColor(color);
		circle.setOutlineThickness(5.f * zoom);
		circle.setOutlineColor(sf::Color::Black);

		// Entity angle indicator
		sf::RectangleShape rectangle(sf::Vector2f(entity.radius * zoom, 5.0f * zoom));
		rectangle.setOrigin(0.0f, 2.5f * zoom);
		rectangle.setPosition(entity.position * zoom - cameraOffset);
		rectangle.rotate(entity.direction.getAngle());
		rectangle.setFillColor(sf::Color::Black);
		
		target.draw(circle);
		target.draw(rectangle);
	}

	void draw(const World& world)
	{
		sf::RectangleShape shape(sf::Vector2f(world.width, world.height));
		shape.setFillColor(sf::Color::Color(127, 127, 127, 255));
		shape.setPosition(sf::Vector2f(0.0f, 0.0f));
		target.draw(shape);
		target.draw(world.world_sprite);
	}
	
	void draw_bar(double value, float y = 800, double width = 500, float height = 50, sf::Color color = sf::Color::Green, int alpha = 255)
	{
		sf::RectangleShape blackShape(sf::Vector2f(width, height));
		blackShape.setOrigin(0, height);
		blackShape.setPosition(0, y);
		sf::Color background = sf::Color::Black;
		background.a = alpha,
		blackShape.setFillColor(background);
		sf::RectangleShape shape(sf::Vector2f(value * width, height));
		shape.setOrigin(0, height);
		shape.setPosition(0, y);
		sf::Color foreground = color;
		foreground.a = alpha,
		shape.setFillColor(foreground);
		
		target.draw(blackShape);
		target.draw(shape);
	}
	void draw_text(std::string text, float x, float y, int size = 64, sf::Color color = sf::Color::White)
	{
		sf::Text textObj;
		textObj.setFont(font);
		textObj.setString(text);
		textObj.setCharacterSize(size);
		textObj.setFillColor(color);
		textObj.setPosition(x, y);
		textObj.setStyle(sf::Text::Bold);
		target.draw(textObj);
	}
};


// POINT/CIRCLE
bool pointCircle(float px, float py, float cx, float cy, float r) {

	// get distance between the point and circle's center
	// using the Pythagorean Theorem
	float distX = px - cx;
	float distY = py - cy;
	float distance = sqrt((distX * distX) + (distY * distY));

	// if the distance is less than the circle's
	// radius the point is inside!
	if (distance <= r) {
		return true;
	}
	return false;
}

// calculate distance between two points
float dist(float x1, float y1, float x2, float y2) {
	float distX = x1 - x2;
	float distY = y1 - y2;
	float distance = sqrt((distX * distX) + (distY * distY));
	return distance;
}

// LINE/POINT
bool linePoint(float x1, float y1, float x2, float y2, float px, float py) {

	// get distance from the point to the two ends of the line
	float d1 = dist(px, py, x1, y1);
	float d2 = dist(px, py, x2, y2);

	// get the length of the line
	float lineLen = dist(x1, y1, x2, y2);

	// since floats are so minutely accurate, add
	// a little buffer zone that will give collision
	float buffer = 0.1;    // higher # = less accurate

	// if the two distances are equal to the line's
	// length, the point is on the line!
	// note we use the buffer here to give a range,
	// rather than one #
	if (d1 + d2 >= lineLen - buffer && d1 + d2 <= lineLen + buffer) {
		return true;
	}
	return false;
}
// LINE/CIRCLE
bool lineCircle(float x1, float y1, float x2, float y2, float cx, float cy, float r, float& closestX, float& closestY) {

	// is either end INSIDE the circle?
	// if so, return true immediately
	bool inside1 = pointCircle(x1, y1, cx, cy, r);
	bool inside2 = pointCircle(x2, y2, cx, cy, r);
	if (inside1 || inside2) return true;

	// get length of the line
	float distX = x1 - x2;
	float distY = y1 - y2;
	float len = sqrt((distX * distX) + (distY * distY));

	// get dot product of the line and circle
	float dot = (((cx - x1) * (x2 - x1)) + ((cy - y1) * (y2 - y1))) / pow(len, 2);

	// find the closest point on the line
	closestX = x1 + (dot * (x2 - x1));
	closestY = y1 + (dot * (y2 - y1));

	// is this point actually on the line segment?
	// if so keep going, but if not, return false
	bool onSegment = linePoint(x1, y1, x2, y2, closestX, closestY);
	if (!onSegment) return false;

	// get distance to closest point
	distX = closestX - cx;
	distY = closestY - cy;
	float distance = sqrt((distX * distX) + (distY * distY));

	if (distance <= r) {
		return true;
	}
	return false;
}
