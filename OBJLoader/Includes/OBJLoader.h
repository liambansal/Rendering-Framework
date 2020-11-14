#ifndef OBJLOADER_H
#define OBJLOADER_H

// Includes.
#include <GLM/glm.hpp>
#include <vector>
#include <string>

class OBJVertex
{
public:
	enum VertexAttributeFlags
	{
		// The position of the vertex.
		POSITION = (1 << 0),
		// The normal for the vertex.
		NORMAL = (1 << 1),
		// The UV coordinates for the vertex.
		UVCOORDINATE = (1 << 2)
	};

	enum Offsets
	{
		PositionOffset = 0,
		NormalOffset = PositionOffset + sizeof(glm::vec4),
		UVCoordinateOffset = NormalOffset + sizeof(glm::vec4)
	};

	OBJVertex();
	~OBJVertex();

	bool operator == (const OBJVertex& a_rhs) const;
	bool operator < (const OBJVertex& a_rhs) const;

	void SetPosition(glm::vec4 a_position);
	void SetNormal(glm::vec4 a_normal);
	void SetUVCoordinate(glm::vec2 a_uvCoordinate);
	const glm::vec4 GetPosition() const;
	const glm::vec4 GetNormal() const;
	const glm::vec2 GetUVCoordinate() const;

private:
	glm::vec4 m_position;
	glm::vec4 m_normal;
	glm::vec2 m_uvCoordinate;
};

inline OBJVertex::OBJVertex() : m_position(0, 0, 0, 1),
	m_normal(0, 0, 0, 0),
	m_uvCoordinate(0, 0)
{}

inline OBJVertex::~OBJVertex()
{}

inline bool OBJVertex::operator == (const OBJVertex& a_rhs) const
{
	return memcmp(this, &a_rhs, sizeof(OBJVertex)) == 0;
}

inline bool OBJVertex::operator < (const OBJVertex& a_rhs) const
{
	return memcmp(this, &a_rhs, sizeof(OBJVertex)) < 0;
}

// Materials have properties such as lights, textures and roughness.
class OBJMaterial
{
public:
	OBJMaterial();
	~OBJMaterial();

	enum TEXTURE_TYPES
	{
		TEXTURE_TYPES_DIFFUSE = 0,
		TEXTURE_TYPES_SPECULAR,
		TEXTURE_TYPES_NORMAL,
		TEXTURE_TYPES_COUNT
	};

	void SetTextureID(unsigned int a_texture, unsigned int a_newID);
	void SetName(std::string a_name);
	void SetTextureFileName(OBJMaterial::TEXTURE_TYPES a_texture,
		std::string a_newName);
	// Sets the ambient light colour.
	void SetKA(glm::vec4 a_kA);
	// Sets the diffuse light colour.
	void SetKD(glm::vec4 a_kD);
	// Sets the specular light colour.
	void SetKS(glm::vec4 a_kS);
	unsigned int GetTextureID(OBJMaterial::TEXTURE_TYPES a_texture);
	const std::string GetName() const;
	const std::string GetTextureFileName(unsigned int a_texture) const;
	// Gets the ambient light colour.
	glm::vec4* GetKA();
	// Gets the diffuse light colour.
	glm::vec4* GetKD();
	// Gets the specular light colour.
	glm::vec4* GetKS();
	
private:

	unsigned int m_textureIDs[TEXTURE_TYPES_COUNT];
	std::string m_name;
	std::string m_textureFileNames[TEXTURE_TYPES_COUNT];
	// Colour and illumination variables.
	// Ambient Light Colour - alpha componenet stores Optical Density (Ni)
	// (Refraction Index 0.001 - 10).
	glm::vec4 m_kA;
	// Diffuse Light Colour - alpha component stores dissolve (d)(0 - 1).
	glm::vec4 m_kD;
	// Specular Light Colour (exponent stored in alpha).
	glm::vec4 m_kS;
};

// An OBJ Model can be composed of many meshes. Much like any 3D model.
// Use a class to store individual mesh data.
class OBJMesh
{
public:
	OBJMesh();
	~OBJMesh();

	// Calculates the face normal for a set of vertex indices.
	glm::vec4 CalculateFaceNormal(const unsigned int& a_indexA,
		const unsigned int& a_indexB,
		const unsigned int& a_indexC) const;
	// Cycles through a model and generates its faces' normals.
	void CalculateFaceNormals();
	void SetName(std::string a_name);
	void SetVertices(std::vector<OBJVertex> a_vertices);
	void SetIndices(std::vector<unsigned int> a_indices);
	void SetMaterial(OBJMaterial* a_material);
	const std::string GetName() const;
	std::vector<OBJVertex>* GetVertices();
	std::vector<unsigned int>* GetIndices();
	OBJMaterial* GetMaterial();

private:
	std::string m_name;
	std::vector<OBJVertex> m_vertices;
	std::vector<unsigned int> m_indices;
	OBJMaterial* m_pMaterial;
};

inline OBJMesh::OBJMesh()
{}

inline OBJMesh::~OBJMesh()
{}

class OBJModel
{
public:
	OBJModel();
	~OBJModel();

	bool Load(const char* a_filename);
	// Unloads and frees memory.
	void Unload();
	const char* GetFilePath() const;
	unsigned int GetMeshCount() const;
	unsigned int GetMaterialCount() const;
	const glm::mat4& GetWorldMatrix() const;
	//OBJMesh* GetMeshByName(const char* a_name);
	OBJMesh* GetMeshByIndex(unsigned int a_index);
	OBJMaterial* GetMaterialByName(const char* a_name);
	OBJMaterial* GetMaterialByIndex(unsigned int a_index);

private:
	std::string LineType(const std::string& a_in);
	std::string LineData(const std::string& a_in);
	glm::vec4 ProcessVectorString(const std::string a_data);
	std::vector<std::string> SplitStringAtCharacter(std::string a_data, char a_character);
	void LoadMaterialLibrary(std::string a_mtllib);

	typedef struct OBJFaceTriplet
	{
		unsigned int vertex;
		unsigned int uvCoordinate;
		unsigned int normalVertex;
	} objFaceTriplet;

	OBJFaceTriplet ProcessTriplet(std::string a_triplet);

	float m_modelScale;
	OBJMaterial* m_pCurrentMaterial;
	std::vector<OBJMesh*> m_meshes;
	std::vector<OBJMaterial*> m_materials;
	std::string m_filePath;
	glm::mat4 m_worldMatrix;
};

inline OBJModel::OBJModel() : m_modelScale(0.5f),
	m_pCurrentMaterial(nullptr),
	m_meshes(),
	m_filePath(),
	m_worldMatrix(glm::mat4(1.f))
{}

inline OBJModel::~OBJModel()
{
	Unload();
}

#endif // OBJLOADER_H.