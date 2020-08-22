

#include "Mem.h"
#include "GlobalNew.h"
#include "Block.h"

//-------------------------------------------------------------------------
// Namespace Weird thing:
//   Don't use namespace here...
//   Since new/delete isn't allowed namespace
//   do all declaraton with full name Azul::Heap... or Azul::Mem::Align...
//-------------------------------------------------------------------------

// --------------------------------------------
// For Normal Heap
// --------------------------------------------

	void* operator new(size_t inSize, Azul::Heap* const pHeap, Azul::Mem::Align align, const char* inName, size_t lineNum)
	{
		//Downcast to selective access pattern
		Azul::HeapSAP* pHeapSAP = (Azul::HeapSAP*)pHeap;

		//Allocate space on heap
		void* pMemLocation = pHeapSAP->AllocBlockSAP(align, inName, lineNum, inSize);// plus size of block plus padding?
		assert(pMemLocation != nullptr);
		
		return pMemLocation;
	}

	void operator delete(void* p, Azul::Heap* const pHeap, Azul::Mem::Align align, const char* inName, size_t lineNum)
	{
		// -----------------------------------------
		// You don't need to implement this... 
		// its here to shut up compiler warnings
		// just this function
		//------------------------------------------
		REPLACE_ME(p);
		REPLACE_ME(pHeap);
		REPLACE_ME(align);
		REPLACE_ME(inName);
		REPLACE_ME(lineNum);
	}

	void* operator new[](size_t inSize, Azul::Heap* const pHeap, Azul::Mem::Align align, const char* inName, size_t lineNum)
	{
		Azul::HeapSAP* heap = (Azul::HeapSAP*)pHeap;

		//Allocate space on heap
		void* pMemLocation = heap->AllocBlockSAP(align, inName, lineNum, inSize);
		assert(pMemLocation != nullptr);

		return pMemLocation;
	}

	void operator delete[](void* p, Azul::Heap* const pHeap, Azul::Mem::Align align, const char* inName, size_t lineNum)
	{
		// -----------------------------------------
		// You don't need to implement this... 
		// its here to shut up compiler warnings
		// just this function
		//------------------------------------------
		REPLACE_ME(p);
		REPLACE_ME(pHeap);
		REPLACE_ME(align);
		REPLACE_ME(inName);
		REPLACE_ME(lineNum);
	}

// --------------------------------------------
// For Fixed Heap
// --------------------------------------------

	void* operator new(size_t inSize, Azul::Heap* const pHeap, const char* inName, size_t lineNum)
	{
		//Downcast to selective access pattern
		Azul::HeapSAP* pHeapSAP = (Azul::HeapSAP*)pHeap;

		void* pMemLocation = pHeapSAP->AllocFixedBlockSAP(inName, lineNum, inSize);
		assert(pMemLocation != nullptr);

		return pMemLocation;
	}

	void operator delete(void* p, Azul::Heap* const pHeap, const char* inName, size_t lineNum)
	{
		// -----------------------------------------
		// You don't need to implement this... 
		// its here to shut up compiler warnings
		// just this function
		//------------------------------------------
		REPLACE_ME(p);
		REPLACE_ME(pHeap);
		REPLACE_ME(inName);
		REPLACE_ME(lineNum);
	}

// --------------------------------------------
// For Global Standard new/delete
// --------------------------------------------

	void* operator new(size_t inSize)
	{
		void* p = malloc(inSize);
		return p;
	}

	void operator delete(void* p)
	{
		Azul::Heap* pHeap = nullptr;
		Azul::Mem::GetHeapByAddr(pHeap, p);

		if (pHeap != nullptr) {
			//Downcast to selective access pattern
			Azul::HeapSAP* pHeapSAP = (Azul::HeapSAP*)pHeap;
			pHeapSAP->FreeBlockSAP(p);	

		}
		else {
			free(p);
		}
	}

	void* operator new[](size_t inSize)
	{
		void* p = malloc(inSize);
		return p;
	}

	void operator delete[](void* p)
	{
		Azul::Heap* pHeap = nullptr;
		Azul::Mem::GetHeapByAddr(pHeap, p);

		if (pHeap != nullptr) {
			//Downcast to selective access pattern
			Azul::HeapSAP* pHeapSAP = (Azul::HeapSAP*)pHeap;
			pHeapSAP->FreeBlockSAP(p);

		}
		else {
			free(p);
		}
	}


// ---  End of File ---------------
