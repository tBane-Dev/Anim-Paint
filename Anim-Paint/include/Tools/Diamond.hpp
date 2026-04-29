#pragma once
#include "SFML/Graphics.hpp"
#include "Tools/ResizableShape.hpp"



class Diamond : public ResizableShape {
public:
	Diamond();
	~Diamond();

	virtual void generateImage();
};
