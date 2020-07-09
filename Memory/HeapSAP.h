#ifndef HEAP_SAP_H
#define HEAP_SAP_H

#include "Heap.h"

//-----Selective Access Pattern-----

//Don't distribute this class publicly
//Only include in mem.h and GlobalNew.h
//Then distrubte those publicly

namespace Azul
{
	class HeapSAP : public Heap
	{
	public:

		//Specialized Constructor
		HeapSAP(Mem* pMem, HANDLE HeapHandle, size_t HeapSize, const char* const Name, const Type HeapType);

		//accessors
		void SetFreeFixedHeadSAP(Block* BlockIn);

		//destory heap
		const bool DestroySAP() const;

		//alloc block mem
		void* AllocBlockSAP(Mem::Align Alignment, const char* inName, const size_t lineNum, const size_t inSize);
		void* AllocFixedBlockSAP(const char* inName, const size_t lineNum, const size_t inSize);

		//remove block
		void FreeBlockSAP(void* p);
		void FreeBlockSAP(Block* pDeadBlock);

	};

}

#endif

