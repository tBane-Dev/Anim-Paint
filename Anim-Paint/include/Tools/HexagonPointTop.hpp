#pragma once
#include "SFML/Graphics.hpp"
#include "Tools/ResizableShape.hpp"



class HexagonPointTop : public ResizableShape {
public:
	HexagonPointTop();
	~HexagonPointTop();

	virtual void generateImage();
};
