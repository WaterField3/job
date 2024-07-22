#include "Mesh.h"

const std::vector<D3D::Vertex>& Mesh::GetVertices()
{
	return m_vertices;
}

const std::vector<unsigned int>& Mesh::GetIndices()
{
	return m_indices;
}
