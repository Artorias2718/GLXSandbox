// Header Files
//=============

#include "cTexture.h"
#include "Internal.h"
#include "../../../Asserts/Asserts.h"
#include "../../../Logging/Logging.h"
#include "../../../Platform/Platform.h"
#include "../../../../External/DirectXTex/DirectXTex/DDS.h"
#include <cmath>

// Interface
//==========

// Initialization / Clean Up
//--------------------------

bool Engine::Graphics::Assets::cTexture::Load(const std::string& i_fileName)
{
	bool wereThereErrors = false;

	Platform::sDataFromFile dataFromFile;

	// Load the binary data
	{
		// Load the binary data
		std::string errorMessage;
		if (!Platform::LoadBinaryFile(i_fileName.c_str(), dataFromFile, &errorMessage))
		{
			wereThereErrors = true;
			ASSERTF(false, errorMessage.c_str());
			Logging::OutputError("Failed to load texture data from file %s: %s", i_fileName.c_str(), errorMessage.c_str());
			goto OnExit;
		}
	}

	// DDS files must begin with a "magic number" that spells "DDS " (4 characters in a uint32_t)
	const uint8_t* currentPosition = reinterpret_cast<uint8_t*>(dataFromFile.data);
	size_t remainingSize = dataFromFile.size;
	if ((remainingSize >= sizeof(DirectX::DDS_MAGIC))
		&& (*reinterpret_cast<const uint32_t*>(currentPosition) == DirectX::DDS_MAGIC))
	{
		currentPosition += sizeof(DirectX::DDS_MAGIC);
		remainingSize -= sizeof(DirectX::DDS_MAGIC);
	}
	else
	{
		wereThereErrors = true;
		ASSERTF(false, "Invalid DDS file %s", i_fileName.c_str());
		Logging::OutputError("The DDS file %s doesn't start with the magic number", i_fileName.c_str());
		goto OnExit;
	}
	// A header follows the magic number
	const DirectX::DDS_HEADER* header;
	if (remainingSize >= sizeof(DirectX::DDS_HEADER))
	{
		header = reinterpret_cast<const DirectX::DDS_HEADER*>(currentPosition);
		currentPosition += sizeof(DirectX::DDS_HEADER);
		remainingSize -= sizeof(DirectX::DDS_HEADER);
		// Validate the header
		if ((header->dwSize != sizeof(DirectX::DDS_HEADER))
			|| (header->ddspf.dwSize != sizeof(DirectX::DDS_PIXELFORMAT)))
		{
			wereThereErrors = true;
			ASSERTF(false, "Invalid DDS file %s", i_fileName.c_str());
			Logging::OutputError("The header in the DDS file %s has the wrong struct sizes", i_fileName.c_str());
			goto OnExit;
		}
	}
	else
	{
		wereThereErrors = true;
		ASSERTF(false, "Invalid DDS file %s", i_fileName.c_str());
		Logging::OutputError("The DDS file %s isn't big enough for a header", i_fileName.c_str());
		goto OnExit;
	}
	// Newer DDS files can have an extra header with more advanced information
	const DirectX::DDS_HEADER_DXT10* header_dxt10 = NULL;
	if ((header->ddspf.dwFlags & DDS_FOURCC)
		&& (header->ddspf.dwFourCC == MAKEFOURCC('D', 'X', '1', '0')))
	{
		if (remainingSize >= sizeof(DirectX::DDS_HEADER_DXT10))
		{
			header_dxt10 = reinterpret_cast<const DirectX::DDS_HEADER_DXT10*>(currentPosition);
			currentPosition += sizeof(DirectX::DDS_HEADER_DXT10);
			remainingSize -= sizeof(DirectX::DDS_HEADER_DXT10);
		}
		else
		{
			wereThereErrors = true;
			ASSERTF(false, "Invalid DDS file %s", i_fileName.c_str());
			Logging::OutputError("The DDS file %s isn't big enough for a DXT10 header", i_fileName.c_str());
			goto OnExit;
		}
	}

	// The code above should work for any DDS files,
	// but the next block validates specific constraints for our class
	// (these errors should never occur since we create the DDS files ourselves in TextureBuilder).
	// If you keep this code after the class is over you may want to expand your texture handling
	// and remove some of these constraints.
	{
		// We only support the old style of DDS header
		if (header_dxt10)
		{
			wereThereErrors = true;
			ASSERT(false);
			Logging::OutputError("The DDS file %s uses a DXT10 header, which is unsupported", i_fileName.c_str());
			goto OnExit;
		}
		// We only support 2D textures
		if (header->dwFlags & DDS_HEADER_FLAGS_VOLUME)
		{
			wereThereErrors = true;
			ASSERT(false);
			Logging::OutputError("The DDS file %s is a volume texture, which is unsupported", i_fileName.c_str());
			goto OnExit;
		}
		else if (header->dwCaps & DDS_CUBEMAP)
		{
			wereThereErrors = true;
			ASSERT(false);
			Logging::OutputError("The DDS file %s is a cube texture, which is unsupported", i_fileName.c_str());
			goto OnExit;
		}
		// We only support BC1 ("DXT1") and BC3 ("DXT5") formats
		if (header->ddspf.dwFlags & DDS_FOURCC)
		{
			if ((header->ddspf.dwFourCC != DirectX::DDSPF_DXT1.dwFourCC)
				&& (header->ddspf.dwFourCC != DirectX::DDSPF_DXT5.dwFourCC))
			{
				wereThereErrors = true;
				ASSERT(false);
				Logging::OutputError("The DDS file %s isn't one of the supported formats: BC1, BC3", i_fileName.c_str());
				goto OnExit;
			}
		}
		else
		{
			wereThereErrors = true;
			ASSERT(false);
			Logging::OutputError("The DDS file %s doesn't use a FourCC pixel format, which is unsupported", i_fileName.c_str());
			goto OnExit;
		}
	}

	// Initialize the API object
	{
		// Populate a struct with the necessary information
		Texture::sDdsInfo ddsInfo;
		{
			// The remaining bits in the DDS file are the actual image data
			{
				ddsInfo.imageData = currentPosition;
				ddsInfo.imageDataSize = remainingSize;
			}
			if (header->dwWidth < (1u << (sizeof(m_width) * 8u)))
			{
				m_width = static_cast<uint16_t>(header->dwWidth);
				ddsInfo.width = m_width;
			}
			else
			{
				wereThereErrors = true;
				ASSERTF(false, "Texture width (%u) too big", header->dwWidth);
				Logging::OutputError("The width (%u) of the DDS file %s is too big", header->dwWidth, i_fileName.c_str());
				goto OnExit;
			}
			if (header->dwHeight < (1u << (sizeof(m_height) * 8u)))
			{
				m_height = static_cast<uint16_t>(header->dwHeight);
				ddsInfo.height = m_height;
			}
			else
			{
				wereThereErrors = true;
				ASSERTF(false, "Texture width (%u) too big", header->dwHeight);
				Logging::OutputError("The width (%u) of the DDS file %s is too big", header->dwHeight, i_fileName.c_str());
				goto OnExit;
			}

			ddsInfo.mipLevelCount = static_cast<unsigned int>(header->dwMipMapCount);
			// Format
			{
				if (header->ddspf.dwFourCC == DirectX::DDSPF_DXT1.dwFourCC)
				{
					ddsInfo.format = Texture::Internal::BC1;
				}
				else if (header->ddspf.dwFourCC == DirectX::DDSPF_DXT5.dwFourCC)
				{
					ddsInfo.format = Texture::Internal::BC3;
				}
				else
				{
					// This can never happen since a check was already made above,
					// but if you ever change the code...
					ddsInfo.format = Texture::Internal::Unknown;
				}
			}
		}
		if (!Initialize(i_fileName.c_str(), ddsInfo))
		{
			goto OnExit;
		}
	}

OnExit:

	dataFromFile.Free();

	return !wereThereErrors;
}

Engine::Graphics::Assets::cTexture::cTexture(const std::string& i_fileName)
	:
#if defined( D3D_API )
	m_textureView(NULL),
#elif defined( GL_API )
	m_textureId(0),
#endif
	m_width(0), m_height(0)
{
	if (!Load(i_fileName))
	{
		ASSERTF(false, "There was a problem loading the texture!");
	}
}

Engine::Graphics::Assets::cTexture::~cTexture()
{
	CleanUp();
}
