#include "Platform/Platform.hpp"

using namespace sf;

int main()
{
	util::Platform platform;

#if defined(_DEBUG)
	std::cout << "Hello World!" << std::endl;
#endif
	/*
	sf::RenderWindow window;
	// in Windows at least, this must be called before creating the window
	float screenScalingFactor = platform.getScreenScalingFactor(window.getSystemHandle());
	// Use the screenScalingFactor
	window.create(sf::VideoMode(200.0f * screenScalingFactor, 200.0f * screenScalingFactor), "SFML works!");
	platform.setIcon(window.getSystemHandle());

	sf::CircleShape shape(window.getSize().x / 2);
	shape.setFillColor(sf::Color::White);

	sf::Texture shapeTexture;
	shapeTexture.loadFromFile("content/sfml.png");
	shape.setTexture(&shapeTexture);

	sf::Event event;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
	*/
	RenderWindow window(VideoMode(200, 200), "Test!");

	CircleShape MyCircle(100.f);
	MyCircle.setFillColor(Color::Green);

	Texture t;
	t.loadFromFile("fang.png");

	Sprite s;
	s.setTexture(t);
	s.setPosition(50, 100);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		window.clear();
		window.draw(MyCircle);
		window.display();
	}
	return 0;
}
