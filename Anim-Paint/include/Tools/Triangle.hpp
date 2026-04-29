#pragma once
#include "SFML/Graphics.hpp"
#include "Tools/ResizableShape.hpp"



class Triangle : public ResizableShape {
public:
	Triangle();
	~Triangle();

	virtual void generateImage();
};
