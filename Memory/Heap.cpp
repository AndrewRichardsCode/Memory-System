

#include "Heap.h"
#include "Block.h"

namespace Azul
{

	Heap::Heap(Mem* pMem, HANDLE HeapHandle, size_t HeapSize, const char* const Name, const Type HeapType) 
		: pBlkHead(nullptr), mType(HeapType), mWinHeapHandle(HeapHandle), mInfo({}), pFixedFreeHead(nullptr), 
		pOverflow(nullptr), pMem(pMem)
	{

		this->SetName(Name);
		this->mInfo.StartAddr = (size_t)this;
		this->mInfo.EndAddr = (size_t)this + HeapSize;
		this->mInfo.TotalSize = HeapSize;
	}

	const bool Heap::Destroy() const {
	
		return HeapDestroy(this->mWinHeapHandle);
	}

	Heap::Type Heap::GetHeapType() const
	{
		return this->mType;
	}

	// Overflow Heap
	void Heap::SetOverflowHeap(Heap* pHeap)
	{
		this->pOverflow = pHeap;
	}

	Heap* Heap::GetOverflowHeap() const
	{
		return this->pOverflow;
	}

	void Heap::GetInfo(Info& retInfo) const
	{
		retInfo = this->mInfo;
	};

	Heap* Heap::DebugGetNext() const
	{
		return this->GetNext();
	};

	Heap* Heap::DebugGetPrev() const
	{
		return this->GetPrev();
	};

	Block* Heap::GetTrackingHead() const
	{
		return this->pBlkHead;
		
	}

	Block* Heap::GetGlobalTrackingHead() const {
		return this->pMem->DebugGetGlobalTrackingHead();
	}

	void Heap::SetGlobalTrackingHead(Block* BlockIn) {
		assert(this->pMem != nullptr);
		//Downcast to mem selective access pattern
		MemSAP* pMemSAP = (MemSAP*)this->pMem;
		pMemSAP->SetGlobalTrackingHeadSAP(BlockIn);
	}

	void Heap::SetFreeFixedHead(Block* BlockIn) {
	
		this->pFixedFreeHead = BlockIn;
	}

	void Heap::SetName(const char* const pInName) {
	
		assert(pInName != nullptr);

		//or use memcpy
		memcpy_s(this->mInfo.Name, Heap::NameNumChar, pInName, Heap::NameNumChar);
	}

	void* Heap::AllocBlock(Mem::Align Alignment, const char* inName, const size_t lineNum, const size_t inSize) {
	
		
		//Get padding
		size_t Padding = this->GetPadding(Alignment);

		//allocate new memory
		void* NewBlock = HeapAlloc(this->mWinHeapHandle, 0x8, inSize + sizeof(Block) + Padding);
		assert(NewBlock != 0);


		
		//Downcast to mem selective access pattern
		MemSAP* pMemSAP = (MemSAP*)this->pMem;
		//update global stats
		pMemSAP->IncreaseAllocInfoSAP(inSize);
		//update heap stats
		this->IncreaseAllocInfo(inSize);

		Mem::Info info;
		this->pMem->GetInfo(info);

		//placement new block header
		Block* pBlock = (Block*) new(NewBlock) Block(inName, lineNum, info.CurrAllocIndex, inSize);
		assert(pBlock != nullptr);

		//Get where raw mem starts
		size_t* pRawMem = (size_t*)(((size_t)NewBlock + sizeof(Block) + (Padding - 1u)) & ~(Padding - 1u));

		//Set secret pointer in new location for if there is padding
		//Get pointer to secret pointer
		void** pSecret = (void**)((char*)pRawMem - sizeof(pSecret));

		//Set secret pointer to point to NewBlock header
		*pSecret = NewBlock;

		//Add to heap list
		this->PushToFrontHeap(pBlock);

		//Add to global list
		this->PushToFrontGlobal(pBlock);
			

		//Return where raw mem starts
		return pRawMem;
	}

	void* Heap::AllocFixedBlock(const char* inName, const size_t lineNum, const size_t inSize) {
	
		//check that size equals fixed size per block
		assert((inSize + sizeof(Block) / this->mInfo.TotalSize));

		//check if heap full
		if (this->pFixedFreeHead == nullptr) {

			return this->GetOverflowHeap()->AllocBlock(Mem::Align::Byte_16, inName, lineNum, inSize);
		}
		
		//Downcast to mem selective access pattern
		MemSAP* pMemSAP = (MemSAP*)this->pMem;
		//update global stats
		pMemSAP->IncreaseAllocInfoSAP(inSize);
		//update heap stats
		this->IncreaseAllocInfo(inSize);

		//Get Mem stats
		Mem::Info info;
		this->pMem->GetInfo(info);

		//Get Free block header
		Block* pBlock = this->pFixedFreeHead;

		//Set next free block
		this->pFixedFreeHead = this->pFixedFreeHead->GetHeapNext();

		//Set block instead of placement new
		pBlock->Set(inName, lineNum, info.CurrAllocIndex);

		//Add to heap list
		this->PushToFrontHeap(pBlock);

		//Add to global list
		this->PushToFrontGlobal(pBlock);

		//Return where raw mem starts
		return (size_t*)((size_t)pBlock + sizeof(Block));
	}

	void Heap::FreeBlock(void* p) {
		
		assert(p != nullptr);
		
		//p is start of raw memory... move up to secret ptr to top of block
		size_t* pSecret = (size_t*)p;
		pSecret--;

		Block* DeadBlock = (Block*)*pSecret;

		//remove from heap list
		this->RemoveFromHeap(DeadBlock);

		//remove from global list
		this->RemoveFromGlobal(DeadBlock);

		//Downcast to mem selective access pattern
		MemSAP* pMemSAP = (MemSAP*)this->pMem;
		//update global stats
		pMemSAP->DecreaseAllocInfoSAP(DeadBlock->GetAllocSize());
		//update heap stats
		this->DecreaseAllocInfo(DeadBlock->GetAllocSize());

		if (this->mType == Type::NORMAL) {
			//free win32 block
			assert(HeapFree(this->mWinHeapHandle, 0x0, DeadBlock));
		}
		else {
			//ReAlloc fixed block with placement new
			size_t size = DeadBlock->GetAllocSize();
			Block* pBlock = new(DeadBlock) Block(size);

			//Add to free block list
			pBlock->SetHeapNext(this->pFixedFreeHead);
			this->pFixedFreeHead = pBlock;
		}
	}

	void Heap::FreeBlock(Block* pDeadBlock) {

		assert(pDeadBlock != nullptr);

		//remove from heap list
		this->RemoveFromHeap(pDeadBlock);

		//remove from global list
		this->RemoveFromGlobal(pDeadBlock);

		//Downcast to mem selective access pattern
		MemSAP* pMemSAP = (MemSAP*)this->pMem;
		//update global stats
		pMemSAP->DecreaseAllocInfoSAP(pDeadBlock->GetAllocSize());
		//update heap stats
		this->DecreaseAllocInfo(pDeadBlock->GetAllocSize());


		//free win32 block
		//bool status = HeapFree(this->mWinHeapHandle, 0x0, pDeadBlock);
		//assert(status == true);
		assert(HeapFree(this->mWinHeapHandle, 0x0, pDeadBlock));
	}

	void Heap::IncreaseAllocInfo(const size_t inSize) {

		this->mInfo.CurrBytesUsed += inSize;
		this->mInfo.CurrNumAlloc++;
		

		if (this->mInfo.CurrBytesUsed > this->mInfo.PeakBytesUsed) {
			this->mInfo.PeakBytesUsed = this->mInfo.CurrBytesUsed;
		}

		if (this->mInfo.CurrNumAlloc > this->mInfo.PeakNumAlloc) {
			this->mInfo.PeakNumAlloc = this->mInfo.CurrNumAlloc;
		}
	}

	void Heap::DecreaseAllocInfo(const size_t inSize) {

		this->mInfo.CurrBytesUsed -= inSize;
		this->mInfo.CurrNumAlloc--;
	}

	void Heap::PushToFrontHeap(Block* pBlock) {

		assert(pBlock != nullptr);

		// add node
		if (this->pBlkHead == nullptr)
		{
			// push to the front
			this->pBlkHead = pBlock;
			pBlock->SetHeapNext(nullptr);
			pBlock->SetHeapPrev(nullptr);
		}
		else
		{
			// push to front
			pBlock->SetHeapNext(this->pBlkHead);
			// update head
			this->pBlkHead->SetHeapPrev(pBlock);
			this->pBlkHead = pBlock;
		}
		assert(this->pBlkHead != nullptr);
	}

	void Heap::PushToFrontGlobal(Block* pBlock) {
		assert(pBlock != nullptr);

		// add node
		if (this->GetGlobalTrackingHead() == nullptr)
		{
			// push to the front
			this->SetGlobalTrackingHead(pBlock);
			pBlock->SetGlobalNext(nullptr);
			pBlock->SetGlobalPrev(nullptr);
		}
		else
		{
			// push to front
			pBlock->SetGlobalNext(this->GetGlobalTrackingHead());
			// update head
			this->GetGlobalTrackingHead()->SetGlobalPrev(pBlock);
			this->SetGlobalTrackingHead(pBlock);
		}
		assert(this->GetGlobalTrackingHead() != nullptr);
	}

	void Heap::RemoveFromHeap(Block* pBlock) {
		assert(pBlock != nullptr);

		//if not head
		if (pBlock->GetHeapPrev() != nullptr)
		{
			pBlock->GetHeapPrev()->SetHeapNext(pBlock->GetHeapNext());
		}
		//else if head
		else
		{
			this->pBlkHead = pBlock->GetHeapNext();
		}

		if (this->pBlkHead != nullptr) {
			this->pBlkHead->SetHeapPrev(nullptr);
		}

		if (pBlock->GetHeapNext() != nullptr)
		{
			pBlock->GetHeapNext()->SetHeapPrev(pBlock->GetHeapPrev());
		}
		// remove any lingering links
		pBlock->SetHeapNext(nullptr);
		pBlock->SetHeapPrev(nullptr);
	}

	void Heap::RemoveFromGlobal(Block* pBlock) {
		assert(pBlock != nullptr);

		//if not head
		if (pBlock->GetGlobalPrev() != nullptr)
		{
			pBlock->GetGlobalPrev()->SetGlobalNext(pBlock->GetGlobalNext());
		}
		//else if head
		else
		{
			this->SetGlobalTrackingHead(pBlock->GetGlobalNext());
		}

		if (this->GetGlobalTrackingHead() != nullptr) {
			this->GetGlobalTrackingHead()->SetGlobalPrev(nullptr);
		}

		if (pBlock->GetGlobalNext() != nullptr)
		{
			pBlock->GetGlobalNext()->SetGlobalPrev(pBlock->GetGlobalPrev());
		}
		// remove any lingering links
		pBlock->SetGlobalNext(nullptr);
		pBlock->SetGlobalPrev(nullptr);
	}

	const unsigned int Heap::GetPadding(const Mem::Align Alignment) const {
	
		unsigned int out = 4u;

		switch (Alignment) {
		
			case Mem::Align::Byte_4:
				//out = 4u;
				break;
			case Mem::Align::Byte_8:
				out = 8u;
				break;
			case Mem::Align::Byte_16:
				out = 16u;
				break;
			case Mem::Align::Byte_32:
				out = 32u;
				break;
			case Mem::Align::Byte_64:
				out = 64u;
				break;
			case Mem::Align::Byte_128:
				out = 128u;
				break;
			case Mem::Align::Byte_256:
				out = 256u;
				break;
		}
		return out;
	}

}

// ---  End of File ---------------
