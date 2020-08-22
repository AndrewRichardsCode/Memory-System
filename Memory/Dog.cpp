
#include "Dog.h"

// Default constructor
Dog::Dog()
	: a(0), b(0), c(0)
{
	//printf("Dog(%p):default constuctor\n",this);
}

// Specialized constructor
Dog::Dog(int inA, int inB, int inC)
	: a(inA), b(inB), c(inC)
{
	//printf("Dog(%p):specialized constuctor\n",this);
}

// Copy constructor
Dog::Dog(const Dog& inDog)
	: a(inDog.a), b(inDog.b), c(inDog.c)
{
	//printf("Dog(%p):copy constuctor\n",this);
}

// assignment operator
const Dog& Dog::operator = (const Dog& inDog)
{
	//printf("Dog(%p):assignment operator\n",this);

	this->a = inDog.a;
	this->b = inDog.b;
	this->c = inDog.c;

	return *this;
}

// destructor
Dog::~Dog()
{
	//printf("Dog(%p):destructor\n",this);
}

// ---  End of File ---------------
