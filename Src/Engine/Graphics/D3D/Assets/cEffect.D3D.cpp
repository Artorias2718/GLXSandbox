#include "../../Assets/cEffect.h"

#include "../../../Asserts/Asserts.h"
#include "../../../Logging/Logging.h"
#include "../../../Platform/Platform.h"
#include "../../Functions.h"

#include "../Includes.h"
#include "../Interfaces/D3DInterfaces.h"

bool Engine::Graphics::Assets::cEffect::LoadShaders()
{
	bool wereThereErrors = false;

	// Initialize the graphics objects

	if (!LoadVertexShader())
	{
		wereThereErrors = true;
		ASSERT(false);
		goto OnExit;
	}

	if (!LoadFragmentShader())
	{
		wereThereErrors = true;
		ASSERT(false);
		goto OnExit;
	}

OnExit:
	return !wereThereErrors;
}

bool Engine::Graphics::Assets::cEffect::Bind()
{
	// Set the vertex and fragment shaders
	{
		ID3D11ClassInstance** const noInterfaces = nullptr;
		const unsigned int interfaceCount = 0;
		Interfaces::D3DInterfaces::s_direct3dImmediateContext->VSSetShader(m_vertexShader, noInterfaces, interfaceCount);
		Interfaces::D3DInterfaces::s_direct3dImmediateContext->PSSetShader(m_fragmentShader, noInterfaces, interfaceCount);
	}
	return true;
}

bool Engine::Graphics::Assets::cEffect::CleanUp()
{
	if (Interfaces::D3DInterfaces::s_inputLayout)
	{
		Interfaces::D3DInterfaces::s_inputLayout->Release();
		Interfaces::D3DInterfaces::s_inputLayout = nullptr;
	}

	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = nullptr;
	}
	if (m_fragmentShader)
	{
		m_fragmentShader->Release();
		m_fragmentShader = nullptr;
	}
	return true;
}

bool Engine::Graphics::Assets::cEffect::LoadVertexShader()
{
	bool wereThereErrors = false;

	Platform::sDataFromFile compiledShader;
	std::string shaderFilePath = "data/shaders/" + m_effectName + "/vertex.shader";

	// Load the compiled shader
	{
		std::string errorMessage;
		if (!Platform::LoadBinaryFile(shaderFilePath.c_str(), compiledShader, &errorMessage))
		{
			wereThereErrors = true;
			ASSERTF(false, errorMessage.c_str());
			Logging::OutputError("Failed to load the shader file \"%s\": %s", shaderFilePath.c_str(), errorMessage.c_str());
			goto OnExit;
		}
	}
	// Create the shader object
	{
		ID3D11ClassLinkage* const noInterfaces = NULL;
		const HRESULT result = Graphics::Interfaces::D3DInterfaces::s_direct3dDevice->CreateVertexShader(
			compiledShader.data, compiledShader.size, noInterfaces, &m_vertexShader);
		if (FAILED(result))
		{
			wereThereErrors = true;
			ASSERT(false);
			Logging::OutputError("Direct3D failed to create the shader %s with HRESULT %#010x", shaderFilePath.c_str(), result);
			goto OnExit;
		}
	}

OnExit:

	if (!CreateVertexFormat(compiledShader))
	{
		wereThereErrors = true;
		Logging::OutputError("There was a problem creating the vertex input layout!");
	}
	compiledShader.Free();

	return !wereThereErrors;
}

bool Engine::Graphics::Assets::cEffect::CreateVertexFormat(Engine::Platform::sDataFromFile& i_compiledShader)
{
	// Create the vertex layout
	{
		const unsigned int vertexElementCount = 2;

		D3D11_INPUT_ELEMENT_DESC layoutDescription[vertexElementCount] = { 0 };

		// Initialize the vertex format
		Engine::Graphics::Functions::CreateVertexFormat(layoutDescription);

		const HRESULT result = Graphics::Interfaces::D3DInterfaces::s_direct3dDevice->CreateInputLayout(layoutDescription, vertexElementCount,
			i_compiledShader.data, i_compiledShader.size, &Graphics::Interfaces::D3DInterfaces::s_inputLayout);
		if (FAILED(result))
		{
			ASSERT(false);
			Logging::OutputError("Direct3D failed to create a vertex input layout with HRESULT %#010x", result);
			return false;
		}
	}

	return true;
}

bool Engine::Graphics::Assets::cEffect::LoadFragmentShader()
{
	bool wereThereErrors = false;

	Platform::sDataFromFile compiledShader;
	std::string shaderFilePath = "data/shaders/" + m_effectName + "/fragment.shader";

	// Load the compiled shader
	{
		std::string errorMessage;
		if (!Platform::LoadBinaryFile(shaderFilePath.c_str(), compiledShader, &errorMessage))
		{
			wereThereErrors = true;
			ASSERTF(false, errorMessage.c_str());
			Logging::OutputError("Failed to load the shader file \"%s\": %s", shaderFilePath.c_str(), errorMessage.c_str());
			goto OnExit;
		}
	}
	// Create the shader object
	{
		ID3D11ClassLinkage* const noInterfaces = NULL;
		const HRESULT result = Graphics::Interfaces::D3DInterfaces::s_direct3dDevice->CreatePixelShader(
			compiledShader.data, compiledShader.size, noInterfaces, &m_fragmentShader);
		if (FAILED(result))
		{
			wereThereErrors = true;
			ASSERT(false);
			Logging::OutputError("Direct3D failed to create the shader %s with HRESULT %#010x", shaderFilePath.c_str(), result);
			goto OnExit;
		}
	}

OnExit:

	compiledShader.Free();
	return !wereThereErrors;
}