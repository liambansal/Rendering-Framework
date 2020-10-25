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

inline OBJVertex::OBJVertex() : m_position(0, 0, 0, 1), m_normal(0, 0, 0, 0), m_uvCoordinate(0, 0)
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

class OBJMesh
{
public:
	OBJMesh();
	~OBJMesh();

	void SetName(std::string a_name);
	void SetVertices(std::vector<OBJVertex> a_vertices);
	void SetIndices(std::vector<unsigned int> a_indices);
	const std::string GetName() const;
	std::vector<OBJVertex>* GetVertices();
	std::vector<unsigned int>* GetIndices();

private:
	std::string m_name;
	std::vector<OBJVertex> m_vertices;
	std::vector<unsigned int> m_indices;
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
	const glm::mat4& GetWorldMatrix() const;
	//OBJMesh* GetMeshByName(const char* a_name);
	OBJMesh* GetMeshByIndex(unsigned int a_index);

private:
	std::string LineType(const std::string& a_in);
	std::string LineData(const std::string& a_in);
	glm::vec4 ProcessVectorString(const std::string a_data);
	std::vector<std::string> SplitStringAtCharacter(std::string a_data, char a_character);

	typedef struct OBJFaceTriplet
	{
		unsigned int vertex;
		unsigned int uvCoordinate;
		unsigned int normalVertex;
	} objFaceTriplet;

	OBJFaceTriplet ProcessTriplet(std::string a_triplet);

	std::vector<OBJMesh*> m_meshes;
	std::string m_filePath;
	glm::mat4 m_worldMatrix;
};

inline OBJModel::OBJModel() : m_meshes(),
	m_filePath(),
	m_worldMatrix(glm::mat4(0.f))
{

}

inline OBJModel::~OBJModel()
{
	Unload();
}

#endif // OBJLOADER_H.