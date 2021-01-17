#ifndef ENGINE_GRAPHICS_INTERFACES_CCONSTANTBUFFER_H
#define ENGINE_GRAPHICS_INTERFACES_CCONSTANTBUFFER_H

#include "eConstantBufferType.h"

#if defined D3D_API 
struct ID3D11Buffer;
struct D3D11_BUFFER_DESC;
struct D3D11_SUBRESOURCE_DATA;
typedef unsigned int UINT;
#elif defined OGL_API 
#include "../OGL/Includes.h"
#endif

namespace Engine
{
	namespace Graphics
	{
		namespace Interfaces 
		{
			class cConstantBuffer
			{
			public:
				//cConstantBuffer();
				//~cConstantBuffer();

#if defined D3D_API 
				cConstantBuffer(eConstantBufferType i_bufferType, D3D11_BUFFER_DESC& i_buffer_description,
					UINT i_bufferSize, D3D11_SUBRESOURCE_DATA& i_sub_resource_data, void* i_constantBufferData = 0);
				bool Initialize(eConstantBufferType i_bufferType, D3D11_BUFFER_DESC& i_buffer_description,
					UINT i_bufferSize, D3D11_SUBRESOURCE_DATA& i_sub_resource_data, void* i_constantBufferData = 0);
#elif defined OGL_API 
				cConstantBuffer(eConstantBufferType i_bufferType, size_t i_bufferSize, void* i_constantBufferData = 0);
				bool Initialize(eConstantBufferType i_bufferType, size_t i_bufferSize, void* i_constantBufferData = 0);
#endif
				bool Bind(eConstantBufferType i_bufferType);
				bool Update(eConstantBufferType i_bufferType, void* i_constantBufferData = 0);
				bool CleanUp();
			private:
#if defined D3D_API 
				ID3D11Buffer* m_constantBufferId;
#elif defined OGL_API 
				GLuint m_constantBufferId;
#endif
			};
		}
	}
}

#endif // !ENGINE_GRAPHICS_INTERFACES_CCONSTANTBUFFER_H
