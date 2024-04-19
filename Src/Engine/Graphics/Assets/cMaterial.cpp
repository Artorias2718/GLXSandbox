#include "cMaterial.h"
#include "../../Asserts/Asserts.h"
#include "../../Logging/Logging.h"
#include "../../Platform/Platform.h"

#include "../Assets/cEffect.h"
#include "../Interfaces/cConstantBuffer.h"

#include "../Structures/sMaterial.h"

#if defined D3D_API 
#include "../D3D/Interfaces/D3DInterfaces.h"
#endif


Engine::Graphics::Assets::cMaterial::cMaterial(const std::string& i_materialFileName)
{
	Platform::sDataFromFile binaryFileData;
	std::string errorMessage;

	if (!(Platform::LoadBinaryFile(("data/materials/" + i_materialFileName + ".mat").c_str(), binaryFileData, &errorMessage)))
	{
		ASSERTF(false, errorMessage.c_str());
		Logging::OutputError("Error while loading file \"%s\":%s", i_materialFileName, errorMessage.c_str());
	}
	else
	{
		char* dataPtr = reinterpret_cast<char*> (binaryFileData.data);
		size_t fileOffset = 0;

		m_materialData = reinterpret_cast<Structures::sMaterial*>(dataPtr);
		fileOffset += sizeof(Structures::sMaterial);

		dataPtr = reinterpret_cast<char*>(reinterpret_cast<char*>(binaryFileData.data) + fileOffset);

		std::string effect = dataPtr;
		fileOffset += effect.length() + 1;

		m_effect = new Assets::cEffect(effect);
	}

#if defined D3D_API 
	m_materialBuffer = new Interfaces::cConstantBuffer(Interfaces::MATERIAL, m_materialBufferDescription, sizeof(Structures::sMaterial), m_initialMaterialData, m_materialData);
#elif defined OGL_API 
	m_materialBuffer = new Interfaces::cConstantBuffer(Interfaces::MATERIAL, sizeof(Structures::sMaterial), m_materialData);
#endif
}

bool Engine::Graphics::Assets::cMaterial::Bind()
{
	bool wereThereErrors = false;

	if (!m_materialBuffer->Bind(Interfaces::MATERIAL))
	{
		wereThereErrors = true;
		Logging::OutputError("Failed to bind the material constant buffer!");
		goto OnExit;
	}
	if (!m_effect->Bind())
	{
		wereThereErrors = true;
		Logging::OutputError("Failed to bind the effect data!");
		goto OnExit;
	}
OnExit:
	return !wereThereErrors;

}

bool Engine::Graphics::Assets::cMaterial::CleanUp()
{
	bool wereThereErrors = false;

	if (m_materialBuffer)
	{
		if (!m_materialBuffer->CleanUp())
		{
			wereThereErrors = true;
			Logging::OutputError("Failed to clean up the material constant buffer!");
			goto OnExit;
		}
	}

	if (!m_effect)
	{
		if (!m_effect->CleanUp())
		{
			wereThereErrors = true;
			Logging::OutputError("Failed to clean up the material constant buffer!");
			goto OnExit;
		}
	}
OnExit:
	return !wereThereErrors;
}

Engine::Graphics::Assets::cMaterial::~cMaterial()
{
}