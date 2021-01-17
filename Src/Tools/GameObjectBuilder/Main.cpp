/*
	The main() function is where the program starts execution
*/

// Header Files
//=============

#include "cGameObjectBuilder.h"

// Entry Point
//============

int main(int i_argumentCount, char** i_arguments)
{
	return Tools::AssetBuildLibrary::Build<Tools::GameObjectBuilder::cGameObjectBuilder>(i_arguments, i_argumentCount);
}