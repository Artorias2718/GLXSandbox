/*
	This file provides configurable settings
	that can be used to control assert behavior
*/

#ifndef ENGINE_ASSERTS_CONFIGURATION_H
#define ENGINE_ASSERTS_CONFIGURATION_H

// By default asserts are only enabled for debug builds,
// but you can #define it differently as necessary
#ifdef _DEBUG
	#define ASSERTS_AREENABLED
#endif

#endif	// !ENGINE_ASSERTS_CONFIGURATION_H
