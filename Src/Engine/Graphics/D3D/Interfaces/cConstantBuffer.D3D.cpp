#include "../../Interfaces/cConstantBuffer.h"
#include "../Interfaces/D3DInterfaces.h"
#include "../Includes.h"

#include "../../../Asserts/Asserts.h"
#include "../../../Logging/Logging.h"
#include "../../../Time/Time.h"
#include "../../../Math/Functions.h"

#include "../../Structures/sFrame.h"
#include "../../Structures/sDrawCall.h"

Engine::Graphics::Interfaces::cConstantBuffer::cConstantBuffer(Interfaces::eConstantBufferType i_bufferType, D3D11_BUFFER_DESC& i_bufferDescription,
	UINT i_bufferSize, D3D11_SUBRESOURCE_DATA& i_subResourceData, void* i_constantBufferData)
{
	if (!Initialize(i_bufferType, i_bufferDescription, i_bufferSize, i_subResourceData, i_constantBufferData))
	{
		switch (i_bufferType)
		{
		case FRAME:
			ASSERTF(false, "There was a problem initializing the frame constant buffer!");
			break;
		case DRAWCALL:
			ASSERTF(false, "There was a problem initializing the drawcall constant buffer!");
			break;
		}
	}
}

bool Engine::Graphics::Interfaces::cConstantBuffer::Initialize(Interfaces::eConstantBufferType i_bufferType, D3D11_BUFFER_DESC& i_bufferDescription,
	UINT i_bufferSize, D3D11_SUBRESOURCE_DATA& i_subResourceData, void* i_constantBufferData)
{
	bool wereThereErrors = false;
	{
		// The byte width must be rounded up to a multiple of 16
		i_bufferDescription.ByteWidth = Engine::Math::RoundUpToMultiple_PowerOf2(static_cast<unsigned int>(i_bufferSize), 16u);
		i_bufferDescription.Usage = D3D11_USAGE_DYNAMIC;	// The CPU must be able to update the buffer
		i_bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		i_bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// The CPU must write, but doesn't read
		i_bufferDescription.MiscFlags = 0;
		i_bufferDescription.StructureByteStride = 0;	// Not used

		// Fill in the constant buffer

		switch (i_bufferType)
		{
		case FRAME:
			i_subResourceData.pSysMem = reinterpret_cast<Structures::sFrame*>(i_constantBufferData);
			break;
		case DRAWCALL:
			i_subResourceData.pSysMem = reinterpret_cast<Structures::sDrawCall*>(i_constantBufferData);
			break;
		}
	}

	const HRESULT result = D3DInterfaces::s_direct3dDevice->CreateBuffer(&i_bufferDescription, &i_subResourceData, &m_constantBufferId);
	if (FAILED(result))
	{
		Logging::OutputError("Failed to initialize the constant buffer!");
	}
	return !wereThereErrors;
}

bool Engine::Graphics::Interfaces::cConstantBuffer::Bind(eConstantBufferType i_bufferType)
{
	bool wereThereErrors = false;

	// Bind the constant buffer to the shader
	{
		const unsigned int bufferCount = 1;
		switch (i_bufferType)
		{
		case FRAME:
		{
			const unsigned int registerAssignedInShader = 0;
			D3DInterfaces::s_direct3dImmediateContext->VSSetConstantBuffers(registerAssignedInShader, bufferCount, &m_constantBufferId);
			D3DInterfaces::s_direct3dImmediateContext->PSSetConstantBuffers(registerAssignedInShader, bufferCount, &m_constantBufferId);
			break;
		}
		case DRAWCALL:
		{
			const unsigned int registerAssignedInShader = 1;
			D3DInterfaces::s_direct3dImmediateContext->VSSetConstantBuffers(registerAssignedInShader, bufferCount, &m_constantBufferId);
			break;
		}
		}
	}
	return !wereThereErrors;
}

bool Engine::Graphics::Interfaces::cConstantBuffer::Update(eConstantBufferType i_bufferType, void* i_constantBufferData)
{
	bool wereThereErrors = false;
	// Update the constant buffer
	{
		// Update the struct (i.e. the memory that we own)

		// Get a pointer from Direct3D that can be written to
		void* memoryToWriteTo = NULL;
		{
			D3D11_MAPPED_SUBRESOURCE mappedSubResource;
			{
				// Discard previous contents when writing
				const unsigned int noSubResources = 0;
				const D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
				const unsigned int noFlags = 0;
				const HRESULT result = D3DInterfaces::s_direct3dImmediateContext->Map(m_constantBufferId, noSubResources, mapType, noFlags, &mappedSubResource);
				if (SUCCEEDED(result))
				{
					memoryToWriteTo = mappedSubResource.pData;
				}
				else
				{
					wereThereErrors = true;
					goto OnExit;
				}
			}
		}
		if (memoryToWriteTo)
		{
			// Copy the new data to the memory that Direct3D has provided

			switch (i_bufferType)
			{
			case FRAME:
				memcpy(memoryToWriteTo, i_constantBufferData, sizeof(Structures::sFrame));
				break;
			case DRAWCALL:
				memcpy(memoryToWriteTo, i_constantBufferData, sizeof(Structures::sDrawCall));
				break;
			}

			// Let Direct3D know that the memory contains the data
			// (the pointer will be invalid after this call)
			const unsigned int noSubResources = 0;
			D3DInterfaces::s_direct3dImmediateContext->Unmap(m_constantBufferId, noSubResources);
			memoryToWriteTo = NULL;
		}
	OnExit:
		return !wereThereErrors;
	}
}


bool Engine::Graphics::Interfaces::cConstantBuffer::CleanUp()
{
	bool wereThereErrors = false;
	if (m_constantBufferId)
	{
		m_constantBufferId->Release();
		m_constantBufferId = NULL;
	}
	return !wereThereErrors;
}