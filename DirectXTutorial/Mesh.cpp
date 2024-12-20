#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <locale>
#include <codecvt>

#include "GraphicsEngine.h"
#include "VertexMesh.h"

#include "Debugger.h"

Mesh::Mesh(const wchar_t* full_path) : Resource(full_path)
{
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	// RESPONSE
	std::string warn;
	std::string err;

	// USE AS STRING
	std::string inputfile = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(full_path);

	bool res = tinyobj::LoadObj(&attribs, &shapes, &materials, &warn, &err, inputfile.c_str());

	if (!err.empty() || !res) Debugger::Error("[Mesh] Failed to load mesh");
	if (shapes.size() > 1) throw std::exception("[Mesh] Failed to load mesh");

	std::vector<VertexMesh> list_vertices;
	std::vector<unsigned int> list_indices;

	// SHAPE
	for (size_t s = 0; s < shapes.size(); s++)
	{
		size_t index_offset = 0;
		list_vertices.reserve(shapes[s].mesh.indices.size());
		list_indices.reserve(shapes[s].mesh.indices.size());

		// FACE
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			unsigned char num_face_verts = shapes[s].mesh.num_face_vertices[f];

			// VERTICES
			for (unsigned char v = 0; v < num_face_verts; v++)
			{
				tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

				// VERTEX COORD
				tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
				tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
				tinyobj::real_t vz = attribs.vertices[index.vertex_index * 3 + 2];

				// TEX COORD

				tinyobj::real_t tx = 0;
				tinyobj::real_t ty = 0;

				// EXCEPTION OCCURS FROM MISSING TEX COORDS
				if (attribs.texcoords.size() > index.texcoord_index * 2 + 1) {
					tx = attribs.texcoords[index.texcoord_index * 2 + 0];
					ty = attribs.texcoords[index.texcoord_index * 2 + 1];
				}
				
				VertexMesh vertex(Vector3D(vx, vy, vz), Vector2D(tx, ty));
				// STORE VERTEX MESH DATA
				list_vertices.push_back(vertex);

				list_indices.push_back((unsigned int)index_offset + v);
			}

			index_offset += num_face_verts;
		}
	}

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	
	GraphicsEngine::get()->getVertexMeshLayoutShaderByteCodeAndSize(&shader_byte_code, &size_shader);
	m_vertex_buffer = GraphicsEngine::get()->getRenderSystem()->
		createVertexBuffer(
			&list_vertices[0], sizeof(VertexMesh),
			(UINT)list_vertices.size(), 
			shader_byte_code, (UINT)size_shader, 
			ShaderType::BASIC_TEXTURE);
	
	m_index_buffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(&list_indices[0], (UINT)list_indices.size());
}


Mesh::~Mesh()
{
}

const VertexBufferPtr& Mesh::getVertexBuffer()
{
	return m_vertex_buffer;
}

const IndexBufferPtr& Mesh::getIndexBuffer()
{
	return m_index_buffer;
}