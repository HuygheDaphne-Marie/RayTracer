#pragma once
#include <string>
#include <vector>

#include "EMath.h"
#include "Singleton.h"
#include "Face.h"

class OBJReader final : public Singleton<OBJReader>
{
public:
	OBJReader(const OBJReader& other) = delete;
	OBJReader(OBJReader&& other) = delete;
	OBJReader& operator=(const OBJReader& other) = delete;
	OBJReader& operator=(OBJReader&& other) = delete;
	~OBJReader() override = default;

	void ReadOBJ(const std::string& filePath, std::vector<Elite::FPoint3>& vertices, std::vector<Face>& faces) const;

private:
	friend class Singleton<OBJReader>;
	OBJReader() = default;
};

