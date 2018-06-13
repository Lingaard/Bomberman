#ifndef CRATE_H
#define CRATE_H
#include "Block.h"
class Crate :public Block
{
private:
	bool mHasItem;
	
public:
	Crate(float xPos, float yPos);
	virtual~Crate();

	bool getHasItem()const;
	void destroy();
};

#endif // !CRATE_H
