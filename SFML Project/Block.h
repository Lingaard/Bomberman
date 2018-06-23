#ifndef BLOCK_H
#define BLOCK_H
#include "Graphics.h"

class Block :public Graphics
{
private:
	// Class variables
	bool mSolid;

public:
	Block(float xPos = 0.0f, float yPos = 0.0f);
	Block(std::string texDir, bool solid, float xPos = 0.0f, float yPos = 0.0f);
	Block(sf::Vector2f position);
	virtual~Block();

	bool getIsSolid()const;
};


#endif // !BLOCK_H
