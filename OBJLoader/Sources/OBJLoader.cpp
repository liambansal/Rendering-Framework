#include "OBJLoader.h" // File's header.
#include <fstream>
#include <iostream>
#include <sstream>

void OBJVertex::SetPosition(glm::vec4 a_position)
{
	m_position = a_position;
}

void OBJVertex::SetNormal(glm::vec4 a_normal)
{
	m_normal = a_normal;
}

void OBJVertex::SetUVCoordinate(glm::vec2 a_uvCoordinate)
{
	m_uvCoordinate = a_uvCoordinate;
}

const glm::vec4 OBJVertex::GetPosition() const
{
	return m_position;
}

const glm::vec4 OBJVertex::GetNormal() const
{
	return m_normal;
}

const glm::vec2 OBJVertex::GetUVCoordinate() const
{
	return m_uvCoordinate;
}

void OBJMesh::SetName(std::string a_name)
{
	m_name = a_name;
}

void OBJMesh::SetVertices(std::vector<OBJVertex> a_vertices)
{
	m_vertices = a_vertices;
}

void OBJMesh::SetIndices(std::vector<unsigned int> a_indices)
{
	m_indices = a_indices;
}

const std::string OBJMesh::GetName() const
{
	return m_name;
}

std::vector<OBJVertex>* OBJMesh::GetVertices()
{
	return &m_vertices;
}

std::vector<unsigned int>* OBJMesh::GetIndices()
{
	return &m_indices;
}

bool OBJModel::Load(const char* a_filename)
{
	std::cout << "Attempting to open file: " << a_filename << std::endl;
	// Get an fstream to read in the file data.
	std::fstream file;
	file.open(a_filename, std::ios_base::in | std::ios_base::binary);

	// Test to see if the file has opened correctly.
	if (file.is_open())
	{
		std::cout << "Successfully Opened." << std::endl;
		// Success file has been opened, verify contents of file.
		file.ignore(std::numeric_limits<std::streamsize>::max());
		std::streamsize fileSize = file.gcount();
		file.clear();
		file.seekg(0, std::ios_base::beg);

		if (fileSize == 0)
		{
			std::cout << "File contains no data, closing file." << std::endl;
			file.close();
		}

		int bytesInKilobyte = 1024;
		std::cout << "File size: " << fileSize / bytesInKilobyte << " KB" << std::endl;

		OBJMesh* currentMesh = nullptr;
		std::string fileLine;
		std::vector<glm::vec4> vertexData;
		std::vector<glm::vec4> normalData;
		std::vector<glm::vec2> uvData;

		// Set up reading in chunks of a file at a time.
		while (!file.eof())
		{
			if (std::getline(file, fileLine))
			{
				if (fileLine.size() > 0)
				{
					std::string dataType = LineType(fileLine);

					// If dataType has a 0 length then skip all tests and 
					// continue to next line.
					if (dataType.length() == 0)
					{
						continue;
					}

					std::string data = LineData(fileLine);

					if (dataType == "#")
					{
						// This is a comment line.
						std::cout << data << std::endl;
						continue;
					}

					if (dataType == "mtllib")
					{
						std::cout << "Material File: " << data << std::endl;
						continue;
					}

					if (dataType == "g")
					{
						std::cout << "OBJ Group Found: " << data << std::endl;
						
						// We can use group tags to split our model into 
						// smaller mesh components.
						if (currentMesh != nullptr)
						{
							m_meshes.push_back(currentMesh);
						}

						currentMesh = new OBJMesh();
						currentMesh->SetName(data);
					}

					if (dataType == "v") // Data is a vector.
					{
						std::cout << "Saving vertex data.\n";
						glm::vec4 vertex = ProcessVectorString(data);
						// As this is positional data ensure the w component 
						// is set to 1.0f;
						vertex.w = 1.f;
						vertexData.push_back(vertex);
						continue;
					}

					if (dataType == "vt") // Data is a UV coordinate.
					{
						std::cout << "Saving UV coordinate data.\n";
						glm::vec4 uvCoordinateV4 = ProcessVectorString(data);
						glm::vec2 uvCoordinate =
							glm::vec2(uvCoordinateV4.x, uvCoordinateV4.y);
						uvData.push_back(uvCoordinate);
						continue;
					}

					if (dataType == "vn") // Data is a normal.
					{
						std::cout << "Saving normal data.\n";
						glm::vec4 normal = ProcessVectorString(data);
						normal.w = 0.f;
						normalData.push_back(normal);
						continue;
					}

					if (dataType == "f") // Data is a face.
					{
						std::cout << "Saving face data.\n";
						
						// We have entered processing faces without having 
						// hit a 'o' or 'g' tag.
						if (currentMesh == nullptr)
						{
							currentMesh = new OBJMesh();
						}

						// Face consists of 3 -> more vertices split at ' ' 
						// then at '/' character.
						std::vector<std::string> faceData =
							SplitStringAtCharacter(data, ' ');
						unsigned int ci = (unsigned int)currentMesh->GetVertices()->size();

						for (auto iterator = faceData.begin();
							iterator != faceData.end();
							++iterator)
						{
							// Process face triplet.
							OBJFaceTriplet triplet = ProcessTriplet(*iterator);
							// Triplet processed new set vertex data from 
							// position/normal/texture data.
							OBJVertex currentVertex;
							currentVertex.SetPosition(vertexData[triplet.vertex - 1]);

							if (triplet.normalVertex != 0)
							{
								currentVertex.SetNormal(normalData[triplet.normalVertex - 1]);
							}

							if (triplet.uvCoordinate != 0)
							{
								currentVertex.SetUVCoordinate(uvData[triplet.uvCoordinate - 1]);
							}

							currentMesh->GetVertices()->push_back(currentVertex);
						}

						// All face information for the tri/quad/fan have been 
						// collected. Time to index these into the current mesh.
						for (unsigned int offset = 1; offset < (faceData.size() - 1); ++offset)
						{
							currentMesh->GetIndices()->push_back(ci);
							currentMesh->GetIndices()->push_back(ci + offset);
							currentMesh->GetIndices()->push_back(ci + 1 + offset);
						}

						continue;
					}
				}
			}
		}

		if (currentMesh != nullptr)
		{
			m_meshes.push_back(currentMesh);
		}

		file.close();
		return true;
	}

	return false;
}

// Unloads and frees memory.
void OBJModel::Unload()
{
	m_meshes.clear();
}

const char* OBJModel::GetFilePath() const
{
	return m_filePath.c_str();
}

unsigned int OBJModel::GetMeshCount() const
{
	return (unsigned int)m_meshes.size();
}

const glm::mat4& OBJModel::GetWorldMatrix() const
{
	return m_worldMatrix;
}

//OBJMesh* OBJModel::GetMeshByName(const char* a_name)
//{
//
//}

OBJMesh* OBJModel::GetMeshByIndex(unsigned int a_index)
{
	unsigned int meshCount = (unsigned int)m_meshes.size();

	if (meshCount > 0 && a_index < meshCount)
	{
		return m_meshes[a_index];
	}

	return nullptr;
}

std::string OBJModel::LineType(const std::string& a_in)
{
	if (!a_in.empty())
	{
		// Gets the token part of the line.
		size_t token_start = a_in.find_first_not_of(" \t");
		size_t token_end = a_in.find_first_of(" \t", token_start);

		if (token_start != std::string::npos && token_end != std::string::npos)
		{
			return a_in.substr(token_start, token_end - token_start);
		}
		else if (token_start != std::string::npos)
		{
			return a_in.substr(token_start);
		}
	}

	return "";
}

std::string OBJModel::LineData(const std::string& a_in)
{
	// Gets the token part of the line.
	size_t token_start = a_in.find_first_not_of(" \t");
	size_t token_end = a_in.find_first_of(" \t", token_start);
	// Get the data part of the line.
	size_t data_start = a_in.find_first_not_of(" \t", token_end);
	size_t data_end = a_in.find_last_not_of(" \t\n\r");

	if (data_start != std::string::npos && token_end != std::string::npos)
	{
		return a_in.substr(data_start, data_end - data_start + 1);
	}
	else if (data_start != std::string::npos)
	{
		return a_in.substr(data_start);
	}

	return "";
}

glm::vec4 OBJModel::ProcessVectorString(const std::string a_data)
{
	// Split the line data at each space character and store this as a float 
	// value within a glm::vec4.
	std::stringstream iss(a_data);
	glm::vec4 vectorData = glm::vec4(0.f);
	int i = 0;
	
	for (std::string value; iss>> value; ++i)
	{
		float fValue = std::stof(value);
		vectorData[i] = fValue;
	}

	return vectorData;
}

std::vector<std::string> OBJModel::SplitStringAtCharacter(std::string a_data, char a_character)
{
	std::vector<std::string> lineData;
	// Split the line data at each space character and store this as float 
	// value within a glm::vec4.
	std::stringstream iss(a_data);
	std::string lineSegment;

	while (std::getline(iss, lineSegment, a_character))
	{
		lineData.push_back(lineSegment);
	}

	return lineData;
}

OBJModel::OBJFaceTriplet OBJModel::ProcessTriplet(std::string a_triplet)
{
	std::vector<std::string> vertexIndices = SplitStringAtCharacter(a_triplet, '/');
	OBJFaceTriplet faceTriplet;
	faceTriplet.vertex = 0;
	faceTriplet.normalVertex = 0;
	faceTriplet.uvCoordinate = 0;
	faceTriplet.vertex = std::stoi(vertexIndices[0]);

	if (vertexIndices.size() >= 2)
	{
		if (vertexIndices[1].size() > 0)
		{
			faceTriplet.uvCoordinate = std::stoi(vertexIndices[1]);
		}

		if (vertexIndices.size() >= 3)
		{
			faceTriplet.normalVertex = std::stoi(vertexIndices[2]);
		}
	}

	return faceTriplet;
}
