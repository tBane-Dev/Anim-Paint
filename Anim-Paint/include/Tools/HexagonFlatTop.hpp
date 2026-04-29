#pragma once
#include "SFML/Graphics.hpp"
#include "Tools/ResizableShape.hpp"



class HexagonFlatTop : public ResizableShape {
public:
	HexagonFlatTop();
	~HexagonFlatTop();

	virtual void generateImage();
};
