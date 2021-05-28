#include "../../../Assets/Debug/cBox.h"

#include "../../Includes.h"
#include "../../../../Asserts/Asserts.h"
#include "../../../../Logging/Logging.h"
#include "../../Interfaces/D3DInterfaces.h"
#include "../../../../Math/Functions.h"
#include "../../../Structures/sVertex.h"

bool Engine::Graphics::Assets::Debug::cBox::Initialize()
{
	bool wereThereErrors = false;

	const unsigned int bufferSize = m_vertexCount * sizeof(Engine::Graphics::Structures::sVertex);

	// Intialize the vertex buffer
	{
		D3D11_BUFFER_DESC bufferDescription = { 0 };
		{
			bufferDescription.ByteWidth = bufferSize;
			bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;	// In our class the buffer will never change after it's been created
			bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDescription.CPUAccessFlags = 0;	// No CPU access is necessary
			bufferDescription.MiscFlags = 0;
			bufferDescription.StructureByteStride = 0;	// Not used
		}
		D3D11_SUBRESOURCE_DATA initialData = { 0 };
		{
			initialData.pSysMem = m_vertices;
			// (The other data members are ignored for non-texture buffers)
		}

		const HRESULT result = Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dDevice->CreateBuffer(&bufferDescription, &initialData, &m_vertexBufferId);
		if (FAILED(result))
		{
			ASSERT(false);
			Engine::Logging::OutputError("Direct3D failed to create the vertex buffer with HRESULT %#010x", result);
			return false;
		}
	}

	// Intialize the index buffer
	{
		const unsigned int bufferSize = m_indexSetCount * sizeof(Structures::sIndexSet16);
		D3D11_BUFFER_DESC bufferDescription = { 0 };
		{
			bufferDescription.ByteWidth = bufferSize;
			bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;	// In our class the buffer will never change after it's been created
			bufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bufferDescription.CPUAccessFlags = 0;	// No CPU access is necessary
			bufferDescription.MiscFlags = 0;
			bufferDescription.StructureByteStride = 0;	// Not used
		}
		D3D11_SUBRESOURCE_DATA initialData = { 0 };
		{
			initialData.pSysMem = m_indices16;
			// (The other data members are ignored for non-texture buffers)
		}

		const HRESULT result = Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dDevice->CreateBuffer(&bufferDescription, &initialData, &m_indexBufferId);
		if (FAILED(result))
		{
			ASSERT(false);
			Engine::Logging::OutputError("Direct3D failed to create the index buffer with HRESULT %#010x", result);
			return false;
		}
	}

	return !wereThereErrors;

}

bool Engine::Graphics::Assets::Debug::cBox::Render()
{
	// Vertex Buffer
	{
		// Bind a specific vertex buffer to the device as a data source
		{
			const unsigned int startingSlot = 0;
			const unsigned int vertexBufferCount = 1;
			// The "stride" defines how large a single vertex is in the stream of data
			const unsigned int bufferStride = sizeof(Structures::sVertex);
			// It's possible to start streaming data in the middle of a vertex buffer
			const unsigned int bufferOffset = 0;
			Interfaces::D3DInterfaces::s_direct3dImmediateContext->IASetVertexBuffers(startingSlot, vertexBufferCount, &m_vertexBufferId, &bufferStride, &bufferOffset);
		}
		// Specify what kind of data the vertex buffer holds
		{
			// Set the layout (which defines how to interpret a single vertex)
			Interfaces::D3DInterfaces::s_direct3dImmediateContext->IASetInputLayout(Interfaces::D3DInterfaces::s_inputLayout);
			// Set the topology (which defines how to interpret multiple vertices as a single "primitive";
			// we have defined the vertex buffer as a triangle list
			// (meaning that every primitive is a triangle and will be defined by three vertices)
			Interfaces::D3DInterfaces::s_direct3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		}
	}

	// Index Buffer
	{
		// Bind a specific index buffer to the device as a data source
		{
			ASSERT(m_indexBufferId != NULL);
			// Every index is a 16 bit (or 32-bit) unsigned integer
			const DXGI_FORMAT format = DXGI_FORMAT_R16_UINT;
			// The indices start at the beginning of the buffer
			const unsigned int bufferOffset = 0;
			Interfaces::D3DInterfaces::s_direct3dImmediateContext->IASetIndexBuffer(m_indexBufferId, format, bufferOffset);
		}
	}

	// Render triangles from the currently-bound vertex buffer
	{
		// It's possible to start rendering primitives in the middle of the stream
		const unsigned int indexOfFirstVertexToRender = 0;
		const unsigned int offsetPerIndex = 0;

		// There are three indices per triangle
		Interfaces::D3DInterfaces::s_direct3dImmediateContext->DrawIndexed(m_indexSetCount, indexOfFirstVertexToRender, offsetPerIndex);
	}

	return true;
}

bool Engine::Graphics::Assets::Debug::cBox::CleanUp()
{
	bool wereThereErrors = false;

	if (m_vertexBufferId)
	{
		m_vertexBufferId->Release();
		m_vertexBufferId = NULL;
	}

	if (m_indexBufferId)
	{
		m_indexBufferId->Release();
		m_indexBufferId = NULL;
	}

	return !wereThereErrors;
}