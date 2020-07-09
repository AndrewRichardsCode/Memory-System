#include "HeapSAP.h"

namespace Azul {

	//Specialized Constructor
	HeapSAP::HeapSAP(Mem* pMem, HANDLE HeapHandle, size_t HeapSize, const char* const Name, const Type HeapType) 
		:Heap(pMem, HeapHandle, HeapSize, Name, HeapType)
	{
	
	}

	//accessors
	
	void HeapSAP::SetFreeFixedHeadSAP(Block* BlockIn) {
		this->SetFreeFixedHead(BlockIn);
	}

	//destory heap
	const bool HeapSAP::DestroySAP() const {
		return this->Destroy();
	}

	//alloc block mem
	void* HeapSAP::AllocBlockSAP(Mem::Align Alignment, const char* inName, const size_t lineNum, const size_t inSize) {
		return this->AllocBlock(Alignment, inName, lineNum, inSize);
	}

	void* HeapSAP::AllocFixedBlockSAP(const char* inName, const size_t lineNum, const size_t inSize) {
		return this->AllocFixedBlock(inName, lineNum, inSize);
	}

	//remove block
	void HeapSAP::FreeBlockSAP(void* p) {
		this->FreeBlock(p);
	}

	void HeapSAP::FreeBlockSAP(Block* pDeadBlock) {
		this->FreeBlock(pDeadBlock);
	}

}