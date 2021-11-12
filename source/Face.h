#pragma once
#include "EMath.h"

struct Face // Helper Struct
{
	static constexpr int vertexCount{ 3 };
	unsigned int verticesIndex[vertexCount];
	Elite::FVector3 normal;
	Elite::FVector3 transformedNormal; 
};
