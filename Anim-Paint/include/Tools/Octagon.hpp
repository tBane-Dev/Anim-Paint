#pragma once
#include "SFML/Graphics.hpp"
#include "Tools/ResizableShape.hpp"



class Octagon : public ResizableShape {
public:
	Octagon();
	~Octagon();

	virtual void generateImage();
};
