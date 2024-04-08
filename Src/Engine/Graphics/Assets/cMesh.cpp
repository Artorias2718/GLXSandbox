#include "cMesh.h"
#include "../Structures/sVertex.h"

#include "../../Asserts/Asserts.h"
#include "../../Logging/Logging.h"
#include "../../Platform/Platform.h"

Engine::Graphics::Assets::cMesh::cMesh(std::string i_fileName)
{
	Platform::sDataFromFile binaryFileData;
	std::string errorMsg;

	char* binPtr;

	const std::string fullPath = "data/meshes/" + i_fileName + ".mesh";
	uint32_t fileOffset = 0;

	Platform::LoadBinaryFile(fullPath.c_str(), binaryFileData, &errorMsg);
	{
		// Index Type

		binPtr = reinterpret_cast<char*>(binaryFileData.data);

		m_isShort = *reinterpret_cast<bool*>(binPtr);

		fileOffset += sizeof(bool);

		binPtr = reinterpret_cast<char*>(reinterpret_cast<char*>(binaryFileData.data) + fileOffset);

		// Vertex Count 

		m_vertexCount = *reinterpret_cast<uint32_t*>(binPtr);

		fileOffset += sizeof(uint32_t);

		binPtr = reinterpret_cast<char*>(reinterpret_cast<char*>(binaryFileData.data) + fileOffset);

		// Index Count

		m_indexSetCount = *reinterpret_cast<uint32_t*>(binPtr);

		fileOffset += sizeof(uint32_t);
		binPtr = reinterpret_cast<char*>(reinterpret_cast<char*>(binaryFileData.data) + fileOffset);

		// Vertex Data

		fileOffset += m_vertexCount * sizeof(Structures::sVertex);

		m_vertices = reinterpret_cast<Structures::sVertex*>(binPtr);
		binPtr = reinterpret_cast<char*>(reinterpret_cast<char*>(binaryFileData.data) + fileOffset);

		if (m_isShort)
		{
			// Index Data

			m_indexSet16 = reinterpret_cast<Structures::sIndexSet16*>(binPtr);
			binPtr = reinterpret_cast<char*>(reinterpret_cast<char*>(binaryFileData.data) + fileOffset);

			fileOffset += m_indexSetCount * sizeof(Structures::sIndexSet16);
		}
		else
		{
			// Index Data

			m_indexSet32 = reinterpret_cast<Structures::sIndexSet32*>(binPtr);
			binPtr = reinterpret_cast<char*>(reinterpret_cast<char*>(binaryFileData.data) + fileOffset);

			fileOffset += m_indexSetCount * sizeof(Structures::sIndexSet32);
		}
	}
	Initialize();
}

Engine::Graphics::Assets::cMesh::~cMesh()
{
	CleanUp();
}