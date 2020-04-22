#pragma once

template<class T>
class BoundingPrim
{

private:
	T boundingPrimitive;
public:
	BoundingPrim(T boundingPrim)
	{
		this->boundingPrimitive = boundingPrim;
	}
	T getBoundingPrim()
	{
		return boundingPrimitive;
	}

};