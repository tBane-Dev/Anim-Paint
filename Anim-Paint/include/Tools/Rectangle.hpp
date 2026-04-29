#pragma once
#include "SFML/Graphics.hpp"
#include "Tools/ResizableShape.hpp"



class Rectangle : public ResizableShape {
public:
	Rectangle();
	~Rectangle();

	virtual void generateImage();
};
