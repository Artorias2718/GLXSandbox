/*
	The main() function is where the program starts execution
*/

// Header Files
//=============

#include "cMeshBuilder.h"

// Entry Point
//============

int main(int i_argumentCount, char** i_arguments)
{
	return Tools::AssetBuildLibrary::Build<Tools::MeshBuilder::cMeshBuilder>(i_arguments, i_argumentCount);
}