#pragma once
#include "father.h"

class Son :
	public Father
{
public:
	Son(void);
	~Son(void);

	void say (void);

};
