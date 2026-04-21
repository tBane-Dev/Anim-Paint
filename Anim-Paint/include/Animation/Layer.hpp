#pragma once
#include <SFML/Graphics.hpp>

class Layer {
public:
	std::wstring _name;
	sf::Image _image;
	sf::Texture _texture;

	Layer(std::wstring name, sf::Vector2i size, sf::Color color);
	Layer(std::wstring name, sf::Image image);
	Layer(std::shared_ptr<Layer> layer);
	~Layer();

	void generateTexture();
};
