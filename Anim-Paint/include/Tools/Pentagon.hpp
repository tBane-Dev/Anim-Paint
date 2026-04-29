#pragma once
#include "SFML/Graphics.hpp"
#include "Tools/ResizableShape.hpp"



class Pentagon : public ResizableShape {
public:
	Pentagon();
	~Pentagon();

	virtual void generateImage();
};
