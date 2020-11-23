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

OBJMaterial::OBJMaterial() : m_textureIDs(),
	m_name(),
	m_textureFileNames(),
	m_kA(),
	m_kD(),
	m_kS()
{}

OBJMaterial::~OBJMaterial()
{}

void OBJMaterial::SetTextureID(unsigned int a_texture, unsigned int a_newID)
{
	m_textureIDs[a_texture] = a_newID;
}

void OBJMaterial::SetName(std::string a_name)
{
	m_name = a_name;
}

void OBJMaterial::SetTextureFileName(OBJMaterial::TEXTURE_TYPES a_texture, std::string a_newName)
{
	m_textureFileNames[a_texture] = a_newName;
}

// Sets the ambient light colour.
void OBJMaterial::SetKA(glm::vec4 a_kA)
{
	m_kA = a_kA;
}

// Sets the diffuse light colour.
void OBJMaterial::SetKD(glm::vec4 a_kD)
{
	m_kD = a_kD;
}

// Sets the specular light colour.
void OBJMaterial::SetKS(glm::vec4 a_kS)
{
	m_kS = a_kS;
}

unsigned int OBJMaterial::GetTextureID(OBJMaterial::TEXTURE_TYPES a_texture)
{
	return m_textureIDs[a_texture];
}

const std::string OBJMaterial::GetName() const
{
	return m_name;
}

const std::string OBJMaterial::GetTextureFileName(unsigned int a_texture) const
{
	if (a_texture >= 0 && a_texture < (unsigned int)m_textureFileNames->size())
	{
		return m_textureFileNames[a_texture];
	}

	std::cout << "Warning: Could not retrieve texture file name" << std::endl;
	return "";
}

// Returns the ambient light colour.
glm::vec4* OBJMaterial::GetKA()
{
	return &m_kA;
}

// Returns the diffuse light colour.
glm::vec4* OBJMaterial::GetKD()
{
	return &m_kD;
}

// Returns the specular light colour.
glm::vec4* OBJMaterial::GetKS()
{
	return &m_kS;
}

// Calculates the face normal for a set of vertex indices.
glm::vec4 OBJMesh::CalculateFaceNormal(const unsigned int& a_indexA,
	const unsigned int& a_indexB,
	const unsigned int& a_indexC) const
{
	glm::vec3 a = m_vertices[a_indexA].GetPosition();
	glm::vec3 b = m_vertices[a_indexB].GetPosition();
	glm::vec3 c = m_vertices[a_indexC].GetPosition();
	glm::vec3 ab = glm::normalize(b - a);
	glm::vec3 ac = glm::normalize(c - a);
	return glm::vec4(glm::cross(ab, ac), 0.f);
}

// Cycles through a model and generates its faces' normals.
void OBJMesh::CalculateFaceNormals()
{
	// As our indexed triangle array contains a tri for each three points we 
	// can iterate through this vector and calculate a face normal.
	for (int i = 0; i < m_indices.size(); i += 3)
	{
		glm::vec4 normal = CalculateFaceNormal(i, i + 1, i + 2);
		// Set face normal to each vertex for the tri.
		m_vertices[i].SetNormal(normal);
		m_vertices[i + 1].SetNormal(normal);
		m_vertices[i + 2].SetNormal(normal);
	}
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

void OBJMesh::SetMaterial(OBJMaterial* a_material)
{
	m_pMaterial = a_material;
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

OBJMaterial* OBJMesh::GetMaterial()
{
	return m_pMaterial;
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
		// Get file path information.
		std::string filePath = a_filename;
		size_t pathEnd = filePath.find_last_of("\/\\");

		if (pathEnd != std::string::npos)
		{
			filePath = filePath.substr(0, pathEnd + 1);
		}
		else
		{
			filePath = "";
		}

		m_filePath = filePath;

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

		const unsigned int kilobyte = 1024;
		std::cout << "File size: " << fileSize / kilobyte << " KB" << std::endl;

		OBJMesh* pCurrentMesh = nullptr;
		std::string fileLine;
		std::vector<glm::vec4> vertexData;
		std::vector<glm::vec4> normalData;
		std::vector<glm::vec2> uvData;
		// Store out material is a string as face data is not generated prior 
		// to material assignment and may not have a mesh.
		OBJMaterial* pCurrentMtl = nullptr;

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
						// Load in material fle so that materials can be used as required.
						LoadMaterialLibrary(data);
						continue;
					}

					if (dataType == "g" || dataType == "o")
					{
						std::cout << "OBJ Group Found: " << data << std::endl;
						
						// We can use group tags to split our model into 
						// smaller mesh components.
						if (pCurrentMesh != nullptr)
						{
							m_meshes.push_back(pCurrentMesh);
						}

						pCurrentMesh = new OBJMesh();
						pCurrentMesh->SetName(data);

						// If we have a material name.
						if (pCurrentMtl)
						{
							pCurrentMesh->SetMaterial(pCurrentMtl);
							pCurrentMtl = nullptr;
						}

						continue;
					}

					if (dataType == "v") // Data is a vector.
					{
						glm::vec4 vertex = ProcessVectorString(data);
						// Multiply by passed in vector to allow scaling of the model.
						vertex *= m_modelScale;
						// As this is positional data ensure the w component 
						// is set to 1.0f;
						vertex.w = 1.f;
						vertexData.push_back(vertex);
						continue;
					}

					if (dataType == "vt") // Data is a UV coordinate.
					{
						glm::vec4 uvCoordinateV4 = ProcessVectorString(data);
						glm::vec2 uvCoordinate =
							glm::vec2(uvCoordinateV4.x, uvCoordinateV4.y);
						uvData.push_back(uvCoordinate);
						continue;
					}

					if (dataType == "vn") // Data is a normal.
					{
						glm::vec4 normal = ProcessVectorString(data);
						normal.w = 0.f;
						normalData.push_back(normal);
						continue;
					}

					if (dataType == "f") // Data is a face.
					{
						// We have entered processing faces without having 
						// hit a 'o' or 'g' tag.
						if (!pCurrentMesh)
						{
							pCurrentMesh = new OBJMesh();
							
							if (pCurrentMtl)
							{
								pCurrentMesh->SetMaterial(pCurrentMtl);
								pCurrentMtl = nullptr;
							}
						}

						// Face consists of 3 -> more vertices split at ' ' 
						// then at '/' character.
						std::vector<std::string> faceData =
							SplitStringAtCharacter(data, ' ');
						unsigned int ci = (unsigned int)pCurrentMesh->GetVertices()->size();

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

							pCurrentMesh->GetVertices()->push_back(currentVertex);
						}

						bool calculateNormals = normalData.empty();

						// All face information for the tri/quad/fan have been collected.
						// Time to index these into the current mesh.
						for (unsigned int offset = 1; offset < (faceData.size() - 1); ++offset)
						{
							pCurrentMesh->GetIndices()->push_back(ci);
							pCurrentMesh->GetIndices()->push_back(ci + offset);
							pCurrentMesh->GetIndices()->push_back(ci + offset + 1);

							// Test to see if the OBJ file contains normal data, 
							// if normalData is empty then there are no normals.
							if (calculateNormals)
							{
								glm::vec4 normal = pCurrentMesh->CalculateFaceNormal(ci,
									ci + offset,
									ci + offset + 1);
								(*pCurrentMesh->GetVertices())[ci].SetNormal(normal);
								(*pCurrentMesh->GetVertices())[ci + offset].SetNormal(normal);
								(*pCurrentMesh->GetVertices())[ci + offset + 1].SetNormal(normal);
							}
						}

						continue;
					}

					if (dataType == "usemtl")
					{
						// We have a material to use on the current mesh.
						OBJMaterial* pMtl = GetMaterialByName(data.c_str());

						if (pMtl)
						{
							pCurrentMtl = pMtl;

							if (pCurrentMesh)
							{
								pCurrentMesh->SetMaterial(pCurrentMtl);
							}
						}

						continue;
					}
				}
			}
		}

		if (pCurrentMesh)
		{
			m_meshes.push_back(pCurrentMesh);
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

unsigned int OBJModel::GetMaterialCount() const
{
	return (unsigned int)m_materials.size();
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

OBJMaterial* OBJModel::GetMaterialByName(const char* a_name)
{
	for (auto iterator = m_materials.begin();
		iterator != m_materials.end();
		++iterator)
	{
		OBJMaterial* pMaterial = (*iterator);
		
		if (pMaterial->GetName() == a_name)
		{
			return pMaterial;
		}
	}

	return nullptr;
}

OBJMaterial* OBJModel::GetMaterialByIndex(unsigned int a_index)
{
	unsigned int materialCount = (unsigned int)m_materials.size();
	
	if (materialCount > 0 && a_index < materialCount)
	{
		return m_materials[a_index];
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

void OBJModel::LoadMaterialLibrary(std::string a_mtllib)
{
	std::string materialFile = m_filePath + a_mtllib;
	std::cout << "Attempting to load material file: " << materialFile << std::endl;
	// Get an fstream to read in the file data.
	std::fstream file;
	file.open(materialFile, std::ios_base::in | std::ios_base::binary);

	// Test to see if the file has opened correctly.
	if (file.is_open())
	{
		std::cout << "Material Library Successfully Opened\n";
		// Verify contents of file.
		// Attempt to read the highest number of bytes from the file.
		file.ignore(std::numeric_limits<std::streamsize>::max());
		// gCount will have reached EDF marker, letting us know number of bytes.
		std::streamsize fileSize = file.gcount();
		// Clear EDF marker from being read.
		file.clear();
		// Seek back to the beginning of the file.
		file.seekg(0, std::ios_base::beg);

		// If our file has no data close the file and return early.
		if (fileSize == 0)
		{
			std::cout << "File contains no data, closing file.\n";
			file.close();
			return;
		}

		const unsigned int kilobyte = 1024;
		std::cout << "Material File Size: " << fileSize / kilobyte << " KB" << std::endl;
		// Stores file data as its read line by line.
		std::string fileLine;
		OBJMaterial* pCurrentMaterial = nullptr;

		while (!file.eof())
		{
			if (std::getline(file, fileLine))
			{
				if (fileLine.size() > 0)
				{
					std::string dataType = LineType(fileLine);
					
					// Skip all tests if dataType has a length of 0.
					if (dataType.length() == 0)
					{
						continue;
					}

					std::string data = LineData(fileLine);

					if (dataType == "#")
					{
						std::cout << data << std::endl;
						continue;
					}

					if (dataType == "newmtl")
					{
						std::cout << "New material found: " << data << std::endl;

						if (m_pCurrentMaterial)
						{
							m_materials.push_back(m_pCurrentMaterial);
						}

						m_pCurrentMaterial = new OBJMaterial();
						m_pCurrentMaterial->SetName(data);
						continue;
					}

					if (dataType == "Ns")
					{
						if (m_pCurrentMaterial)
						{
							// Ns is guaranteed to be a single float value.
							m_pCurrentMaterial->GetKS()->a = std::stof(data);
						}

						continue;
					}

					if (dataType == "Ka")
					{
						if (m_pCurrentMaterial)
						{
							// Process kA as vector string.
							// Store alpha channel as it may contain refractive index.
							float kAD = m_pCurrentMaterial->GetKA()->a;
							m_pCurrentMaterial->SetKA(ProcessVectorString(data));
							m_pCurrentMaterial->GetKA()->a = kAD;
						}

						continue;
					}

					if (dataType == "Kd")
					{
						if (m_pCurrentMaterial)
						{
							// Process kD as vector string.
							// Store alpha as it may contain dissolve value.
							float kDA = m_pCurrentMaterial->GetKD()->a;
							m_pCurrentMaterial->SetKD(ProcessVectorString(data));
							m_pCurrentMaterial->GetKD()->a = kDA;
						}

						continue;
					}

					if (dataType == "Ks")
					{
						if (m_pCurrentMaterial)
						{
							// Process Ks as vector string.
							// Store alpha as it may contain specular component.
							float kSA = m_pCurrentMaterial->GetKS()->a;
							m_pCurrentMaterial->SetKS(ProcessVectorString(data));
							m_pCurrentMaterial->GetKS()->a = kSA;
						}

						continue;
					}

					if (dataType == "Ke")
					{
						// Ke is for emissive properties.
						// Don't need to support this for our purposes.
						continue;
					}

					if (dataType == "Ni")
					{
						if (m_pCurrentMaterial)
						{
							// This is the refractive index of the mesh (how 
							// light bends as it passes through the material).
							// We'll store this in the alpha component of the 
							// ambient light values (Ka).
							m_pCurrentMaterial->GetKA()->a = std::stof(data);
						}

						continue;
					}

					if (dataType == "d" || dataType == "Tr")
					{
						if (m_pCurrentMaterial)
						{
							// This is the dissolve or alpha value of the 
							// material. We'll store this in the Kd alpha 
							// channel.
							m_pCurrentMaterial->GetKD()->a = std::stof(data);

							if (dataType == "Tr")
							{
								m_pCurrentMaterial->GetKD()->a = 1.f - m_pCurrentMaterial->GetKD()->a;
							}
						}

						continue;
					}

					if (dataType == "illum")
					{
						// Illum describes the illumintation model used to 
						// light the model. Ignoe this as we'll light the 
						// scene our own way.
						continue;
					}

					// Diffuse texture.
					if (dataType == "map_Kd")
					{
						std::vector<std::string> mapData = SplitStringAtCharacter(data, ' ');
						m_pCurrentMaterial->SetTextureFileName(OBJMaterial::TEXTURE_TYPES::TEXTURE_TYPES_DIFFUSE,
							m_filePath + mapData[mapData.size() - 1]);
						continue;
					}

					// Specular texture.
					if (dataType == "map_Ks")
					{
						std::vector<std::string> mapData = SplitStringAtCharacter(data, ' ');
						m_pCurrentMaterial->SetTextureFileName(OBJMaterial::TEXTURE_TYPES::TEXTURE_TYPES_SPECULAR,
							m_filePath + mapData[mapData.size() - 1]);
						continue;
					}

					// Normal map texture. Map_bump or bump for OBJ files.
					if (dataType == "map_bump" || dataType == "bump")
					{
						std::vector<std::string> mapData = SplitStringAtCharacter(data, ' ');
						m_pCurrentMaterial->SetTextureFileName(OBJMaterial::TEXTURE_TYPES::TEXTURE_TYPES_NORMAL,
							m_filePath + mapData[mapData.size() - 1]);
						continue;
					}
				}
			}
		}

		if (m_pCurrentMaterial)
		{
			m_materials.push_back(m_pCurrentMaterial);
		}

		file.close();
	}
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
