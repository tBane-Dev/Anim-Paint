#pragma once
#include "SFML/Graphics.hpp"
#include "Tools/ResizableShape.hpp"



class Circle : public ResizableShape {
public:
	Circle();
	~Circle();

	virtual void generateImage();
};
