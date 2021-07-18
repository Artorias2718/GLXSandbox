#include "../../../Assets/Debug/cCapsule.h"
#include "../../../Assets/Debug/cSphere.h"

#include "../../Includes.h"
#include "../../../../Asserts/Asserts.h"
#include "../../../../Logging/Logging.h"
#include "../../Interfaces/D3DInterfaces.h"
#include "../../../../Math/Functions.h"
#include "../../../Structures/sVertex.h"

bool Engine::Graphics::Assets::Debug::cCapsule::Render()
{
	m_top->Render();

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
			Interfaces::D3DInterfaces::s_direct3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}
	}

	// Index Buffer
	{
		// Bind a specific index buffer to the device as a data source
		{
			ASSERT(m_indexBufferId != NULL);
			// Every index is a 16 bit unsigned integer
			const DXGI_FORMAT format = DXGI_FORMAT_R16_UINT;
			// The indices start at the beginning of the buffer
			const unsigned int offset = 0;
			Interfaces::D3DInterfaces::s_direct3dImmediateContext->IASetIndexBuffer(m_indexBufferId, format, offset);
		}
	}

	// Render triangles from the currently-bound vertex buffer
	{
		// It's possible to start rendering primitives in the middle of the stream
		const unsigned int indexOfFirstVertexToRender = 0;
		const unsigned int offsetPerIndex = 0;

		// There are three indices per triangle
		const unsigned int indicesPerTriangle = 3;
		Interfaces::D3DInterfaces::s_direct3dImmediateContext->DrawIndexed(indicesPerTriangle * m_indexSetCount, indexOfFirstVertexToRender, offsetPerIndex);
	}

	m_bottom->Render();
	return true;
}
