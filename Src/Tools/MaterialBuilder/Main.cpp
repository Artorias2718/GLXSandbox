/*
	The main() function is where the program starts execution
*/

// Header Files
//=============

#include "cMaterialBuilder.h"

// Entry Point
//============

int main(int i_argumentCount, char** i_arguments)
{
	return Tools::AssetBuildLibrary::Build<Tools::MaterialBuilder::cMaterialBuilder>(i_arguments, i_argumentCount);
}