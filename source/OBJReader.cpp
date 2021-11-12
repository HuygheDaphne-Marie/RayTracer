#include "OBJReader.h"

#include <fstream>
#include <regex>
#include <sstream>

void OBJReader::ReadOBJ(const std::string& filePath, std::vector<Elite::FPoint3>& vertices, std::vector<unsigned int>& faces) const
{
	std::ifstream file;
	file.open(filePath);

	if (file)
	{
		vertices.reserve(500);
		faces.reserve(500);

		std::string line{};

		std::regex vertexFloatCapture{ "v..(-?\\d+\\.\\d*).(-?\\d+\\.\\d*).(-?\\d+\\.\\d*)" };
		std::regex faceIndexCapture{ "f.(\\d+).(\\d+).(\\d+)" };
		Elite::FPoint3 vertex{};

		while (!file.eof())
		{
			std::getline(file, line);
			std::smatch matches;

			if (line[0] == 'v')
			{
				std::regex_search(line, matches, vertexFloatCapture);
				vertex.x = std::stof(matches[1]);
				vertex.y = std::stof(matches[2]);
				vertex.z = std::stof(matches[3]);
				vertices.push_back(vertex);
			}
			else if(line[0] == 'f')
			{
				std::regex_search(line, matches, faceIndexCapture);
				unsigned int v0Index = std::stoi(matches[1]);
				unsigned int v1Index = std::stoi(matches[2]);
				unsigned int v2Index = std::stoi(matches[3]);
				faces.push_back(v0Index - 1);
				faces.push_back(v1Index - 1);
				faces.push_back(v2Index - 1);
			}
		}
	}
}
