//---------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#ifndef GLOBAL_NEW_H
#define GLOBAL_NEW_H

#include "Mem.h"
#include "HeapSAP.h"

//-------------------------------------------------------------------------
// Namespace Weird thing:
//    Namespaces are not allowed for new/delete
//    Also forward declaring here, causes hell
//    So include both Mem.h and Heap.h
//                  - you are welcome (you just saved 8 hrs)
//-------------------------------------------------------------------------

	// For Normal Heap
	void* operator new(size_t inSize, Azul::Heap* const pHeap, Azul::Mem::Align align, const char* inName, size_t lineNum);
	void operator delete(void* p, Azul::Heap* const pHeap, Azul::Mem::Align align, const char* inName, size_t lineNum);

	void* operator new[](size_t inSize, Azul::Heap* const pHeap, Azul::Mem::Align align, const char* inName, size_t lineNum);
	void operator delete[](void* p, Azul::Heap* const pHeap, Azul::Mem::Align align, const char* inName, size_t lineNum);

	// For Fixed Heap
	void* operator new(size_t inSize, Azul::Heap* const pHeap, const char* inName, size_t lineNum);
	void operator delete(void* p, Azul::Heap* const pHeap, const char* inName, size_t lineNum);

// -----------------------------------------------------------------
// Your new system needs to coexist with existing new/delete()
//    Simple new/delete is used in the unit test fixture
//    Welcome to memory system development
// -----------------------------------------------------------------
	void* operator new(size_t inSize);
	void operator delete(void* p);

	void* operator new[](size_t inSize);
	void operator delete[](void* p);

#endif

// ---  End of File ---------------
