// Header Files
//=============

#include "../../Interfaces/cSprite.h"

// <windows.h> is #inluded before <D3D11.h> so that it is #included consistently.
// I didn't think about this and should change this next year so that D3D has it's own Includes.h file like OpenGL does :(
#include "../../../Windows/Includes.h"
#include "../../Functions.h"

#include <D3D11.h>
#include "../../../Asserts/Asserts.h"
#include "../../../Logging/Logging.h"
#include "../../Structures/sVertex.h"

#include "D3DInterfaces.h"
// Static Data Initialization
//===========================

ID3D11Buffer* Engine::Graphics::Interfaces::cSprite::m_vertexBufferId = NULL;

namespace
{
	// When the topology is a quad Direct3D only needs 4 vertices
	const unsigned int vertexCount = 4;
}

// Interface
//==========

// Render
//-------

void Engine::Graphics::Interfaces::cSprite::Render() const
{
	ASSERT(m_vertexBufferId != NULL);

	// Update the vertex buffer
	{
		// Get a pointer from Direct3D that can be written to
		void* memoryToWriteTo;
		{
			D3D11_MAPPED_SUBRESOURCE mappedSubResource;
			{
				// Discard previous contents when writing
				const unsigned int noSubResources = 0;
				const D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
				const unsigned int noFlags = 0;
				const HRESULT result = D3DInterfaces::s_direct3dImmediateContext->Map(m_vertexBufferId, noSubResources, mapType, noFlags, &mappedSubResource);
				if (FAILED(result))
				{
					ASSERT(false);
					Logging::OutputError("Direct3D failed to map the sprite vertex buffer");
					return;
				}
			}
			memoryToWriteTo = mappedSubResource.pData;
		}
		// Fill in the memory that Direct3D has provided
		{
			Structures::sVertex* const vertexData = reinterpret_cast<Structures::sVertex*>(memoryToWriteTo);
			// Texture Coordinates
			{
				// Lower Left
				{
					Structures::sVertex& vertex = vertexData[0];
					vertex.texture.u = m_texturePosition.x;
					vertex.texture.v = 1.0f - m_texturePosition.w;
				}
				// Upper Left
				{
					Structures::sVertex& vertex = vertexData[1];
					vertex.texture.u = m_texturePosition.x;
					vertex.texture.v = 1.0f - m_texturePosition.z;
				}
				// Lower Right
				{
					Structures::sVertex& vertex = vertexData[2];
					vertex.texture.u = m_texturePosition.y;
					vertex.texture.v = 1.0f - m_texturePosition.w;
				}
				// Upper Right
				{
					Structures::sVertex& vertex = vertexData[3];
					vertex.texture.u = m_texturePosition.y;
					vertex.texture.v = 1.0f - m_texturePosition.z;
				}
			}
			// Position
			{
				// Lower Left
				{
					Structures::sVertex& vertex = vertexData[0];
					vertex.position.x = m_scale * m_screenPosition.x;
					vertex.position.y = m_scale * m_screenPosition.w;
				}
				// Upper Left
				{
					Structures::sVertex& vertex = vertexData[1];
					vertex.position.x = m_scale * m_screenPosition.x;
					vertex.position.y = m_scale * m_screenPosition.z;
				}
				// Lower Right
				{
					Structures::sVertex& vertex = vertexData[2];
					vertex.position.x = m_scale * m_screenPosition.y;
					vertex.position.y = m_scale * m_screenPosition.w;
				}
				// Upper Right
				{
					Structures::sVertex& vertex = vertexData[3];
					vertex.position.x = m_scale * m_screenPosition.y;
					vertex.position.y = m_scale * m_screenPosition.z;
				}
			}			// Colors
			for (unsigned int i = 0; i < vertexCount; ++i)
			{
				Structures::sVertex& vertex = vertexData[i];
				vertex.color.r = m_color.r;
				vertex.color.g = m_color.g;
				vertex.color.b = m_color.b;
				vertex.color.a = m_color.a;
			}
		}
		// Let Direct3D know that the memory contains the data
		// (the pointer will be invalid after this call)
		{
			const unsigned int noSubResources = 0;
			D3DInterfaces::s_direct3dImmediateContext->Unmap(m_vertexBufferId, noSubResources);
		}
	}
	// Bind the vertex buffer to the device as a data source
	{
		const unsigned int startingSlot = 0;
		const unsigned int vertexBufferCount = 1;
		// The "stride" defines how large a single vertex is in the stream of data
		const unsigned int bufferStride = sizeof(Structures::sVertex);
		// It's possible to start streaming data in the middle of a vertex buffer
		const unsigned int bufferOffset = 0;
		D3DInterfaces::s_direct3dImmediateContext->IASetVertexBuffers(startingSlot, vertexBufferCount, &m_vertexBufferId, &bufferStride, &bufferOffset);
	}
	// Specify what kind of data the vertex buffer holds
	{
		// Set the topology (which defines how to interpret multiple vertices as a single "primitive";
		// we have defined the vertex buffer as a quad, which means that a triangle "strip" will work
		// (meaning that the first primitive will be a triangle defined by three vertices
		// and the second primitive will be a triangle defined by the two most recent vertices and one new vertex)
		D3DInterfaces::s_direct3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	}
	// Render triangles from the currently-bound vertex buffer
	{
		// It's possible to start rendering primitives in the middle of the stream
		const unsigned int indexOfFirstVertexToUse = 0;
		D3DInterfaces::s_direct3dImmediateContext->Draw(vertexCount, indexOfFirstVertexToUse);
	}
}

// Initialization / Clean Up
//--------------------------

bool Engine::Graphics::Interfaces::cSprite::Initialize()
{
	D3D11_BUFFER_DESC bufferDescription = { 0 };
	{
		const size_t bufferSize = static_cast<size_t>(vertexCount) * sizeof(Structures::sVertex);
		ASSERT(bufferSize < (uint64_t(1u) << (sizeof(bufferDescription.ByteWidth) * 8)));
		bufferDescription.ByteWidth = static_cast<unsigned int>(bufferSize);
		bufferDescription.Usage = D3D11_USAGE_DYNAMIC;	// The buffer will change frequently
		bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// The CPU will write to the buffer
		bufferDescription.MiscFlags = 0;
		bufferDescription.StructureByteStride = 0;	// Not used
	}

	Structures::sVertex vertexData[vertexCount];
	{
		// Lower Left
		{
			Structures::sVertex& vertex = vertexData[0];
			vertex.texture.u = 0.0f;
			vertex.texture.v = 1.0f;
			vertex.position.x = -1.0f;
			vertex.position.y = -1.0f;
			vertex.position.z = 0.0f;
		}
		// Upper Left
		{
			Structures::sVertex& vertex = vertexData[1];
			vertex.texture.u = 0.0f;
			vertex.texture.v = 0.0f;
			vertex.position.x = -1.0f;
			vertex.position.y = 1.0f;
			vertex.position.z = 0.0f;
		}
		// Lower Right
		{
			Structures::sVertex& vertex = vertexData[2];
			vertex.texture.u = 1.0f;
			vertex.texture.v = 1.0f;
			vertex.position.x = 1.0f;
			vertex.position.y = -1.0f;
			vertex.position.z = 0.0f;
		}
		// Upper Right
		{
			Structures::sVertex& vertex = vertexData[3];
			vertex.texture.u = 1.0f;
			vertex.texture.v = 0.0f;
			vertex.position.x = 1.0f;
			vertex.position.y = 1.0f;
			vertex.position.z = 0.0f;
		}
		// Colors
		for (unsigned int i = 0; i < vertexCount; ++i)
		{
			Structures::sVertex& vertex = vertexData[i];
			vertex.color.r = vertex.color.g = vertex.color.b = vertex.color.a = 255;
		}
	}
	D3D11_SUBRESOURCE_DATA initialData = { 0 };
	{
		initialData.pSysMem = vertexData;
		// (The other data members are ignored for non-texture buffers)
	}
	const HRESULT result = D3DInterfaces::s_direct3dDevice->CreateBuffer(&bufferDescription, &initialData, &m_vertexBufferId);
	if (SUCCEEDED(result))
	{
		return true;
	}
	else
	{
		ASSERT(false);
		Logging::OutputError("Direct3D failed to create the vertex buffer for sprites with HRESULT %#010x", result);
		return false;
	}
}

bool Engine::Graphics::Interfaces::cSprite::CleanUp()
{
	if (m_vertexBufferId)
	{
		m_vertexBufferId->Release();
		m_vertexBufferId = NULL;
	}

	return true;
}