//---------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#include "Mem.h"
#include "Block.h"
#include "HeapSAP.h"

namespace Azul
{

	Mem* Mem::pInstance = nullptr;

	//Private Constructor
	Mem::Mem() 
		:poHead(nullptr), pGlobalHead(nullptr), mInfo({})
	{
	
	}

	Mem::~Mem() {
		Mem* pMem = Mem::GetInstance();
		if (pMem != nullptr) {
			pMem->Destroy();
		}
	}

	// Create the heap.
	Mem::Code Mem::CreateNormalHeap(Heap*& newHeap, unsigned int heapSize, const char* const Name)
	{
		Mem* pMem = Mem::GetInstance();


		if (pMem != nullptr) {
		
		
			//Create win32 heap
			HANDLE HeapHandle = HeapCreate(0, 0, heapSize*2u);
			assert(HeapHandle != 0);

			//Allocate memory for win32 heap header and init to 0 wtih parameter flag
			LPVOID HeapHeader = HeapAlloc(HeapHandle, 0x8, sizeof(Heap));
			assert(HeapHeader != 0);

			//placement new heap object
			newHeap = (Heap*) new((unsigned int*)HeapHeader) HeapSAP(pMem, HeapHandle , heapSize, Name, Heap::Type::NORMAL);
			assert(newHeap != nullptr);

			//Add to list
			pMem->PushToFront(newHeap);

			//update stats
			pMem->mInfo.CurrHeapCount++;
			if (pMem->mInfo.CurrHeapCount > pMem->mInfo.PeakHeapCount) {
				pMem->mInfo.PeakHeapCount = pMem->mInfo.CurrHeapCount;
			}

			return Code::OK;
		}


		newHeap = nullptr;

		return Code::ERROR_Mem_Not_Initialized;
	}

	Mem::Code Mem::CreateFixedHeap(Heap*& newHeap, unsigned int numBlocks, int sizePerBlock, const char* const Name)
	{

		Mem* pMem = Mem::GetInstance();

		if (pMem != nullptr) {

			//Calculate heap size
			size_t HeapSize = numBlocks * (sizePerBlock + sizeof(Block));

			//create win32 heap
			HANDLE HeapHandle = HeapCreate(0, 0, HeapSize*2u);
			assert(HeapHandle != 0);

			//Allocate memory for win32 heap header and init to 0 wtih parameter flag
			void* HeapHeader = HeapAlloc(HeapHandle, 0x8, sizeof(Heap));
			assert(HeapHeader != 0);

			//placement new heap header object
			newHeap = (Heap*) new((size_t*)HeapHeader) HeapSAP(pMem, HeapHandle, HeapSize, Name, Heap::Type::FIXED);
			assert(newHeap != nullptr);

			//allocate raw mem for ENTIRE heap
			void* RawMem = HeapAlloc(HeapHandle, 0x8, HeapSize);
			assert(RawMem != 0);
			
			//Downcast to selective access pattern
			HeapSAP* newHeapSAP = (HeapSAP*)newHeap;

			//placement new all block headers
			void* BlockHeader = RawMem;
			Block* pCurrent = new(BlockHeader) Block(sizePerBlock);
			newHeapSAP->SetFreeFixedHeadSAP(pCurrent);

			for (unsigned int i = 0u; i < numBlocks-1; i++) {
				
				//Set next free block header location
				BlockHeader = (Block*)((size_t)BlockHeader + sizeof(Block) + sizePerBlock);

				//Placement new block header
				Block* pBlock = new(BlockHeader) Block(sizePerBlock);

				//Add block to free list
				pCurrent->SetHeapNext(pBlock);
				pCurrent = pBlock;
			}


			//Add heap to list
			pMem->PushToFront(newHeap);

			//update stats
			pMem->mInfo.CurrHeapCount++;
			if (pMem->mInfo.CurrHeapCount > pMem->mInfo.PeakHeapCount) {
				pMem->mInfo.PeakHeapCount = pMem->mInfo.CurrHeapCount;
			}

			return Code::OK;
		}

		
		return Code::ERROR_Mem_Not_Initialized;
	}

	// Initialize the memory system  (Public method)
	Mem::Code Mem::Create(void)
	{
		if (pInstance == nullptr) {
			pInstance = new Mem();
			return Code::OK;
		}

		pInstance->Destroy();
		pInstance->Create();

		return Code::ERROR_Heap_Create;
	}

	Mem::Code Mem::Destroy(void)
	{
		
		Mem* pMem = Mem::GetInstance();
		if (pMem == nullptr) {
			return Code::ERROR_Mem_Not_Initialized;
		}

		//Need to walk heap list and delete them
		Heap* current = pMem->poHead;
		Heap* dead;
		while (current != nullptr) {
			
			dead = current;
			current = current->GetNext();

			pMem->DestroyHeap(dead);
		}
		
		pInstance->poHead = nullptr;
		pInstance->pGlobalHead = nullptr;
		pInstance->mInfo = {};
		pInstance = nullptr;

		return Code::OK;
	}

	Mem::Code Mem::GetInfo(Mem::Info& retInfo)
	{

		Mem* pMem = Mem::GetInstance();

		if (pMem != nullptr) {
			retInfo = pInstance->mInfo;
			return Code::OK;
		}

		return Code::ERROR_Mem_Not_Initialized;
	}

	Heap* Mem::DebugGetHeapHead()
	{
		Mem* pMem = Mem::GetInstance();
		assert(pMem != nullptr);

		return pMem->poHead;
	}

	Block* Mem::DebugGetGlobalTrackingHead()
	{
		Mem* pMem = Mem::GetInstance();
		assert(pMem != nullptr);

		return pMem->pGlobalHead;
	}

	void Mem::SetGlobalTrackingHead(Block* pBlockIn) {
		this->pGlobalHead = pBlockIn;
	}

	Mem::Code Mem::DestroyHeap(Heap* pInHeap)
	{
		//Get Mem Singleton
		Mem* pMem = Mem::GetInstance();
		
		//Check Mem not created
		if (pMem == nullptr) {
			return Code::ERROR_Mem_Not_Initialized;
		}

		//Check Heap not created
		if (pInHeap == nullptr){ 
			return Code::ERROR_Illegal_Heap;
		}

		//Walk Heap List and Check Heap is valid address
		Heap* current = pMem->poHead;
		bool OnList = false;
		while (current != nullptr) {
			if (current == pInHeap) {
				OnList = true;
				break;
			}
			current = current->GetNext();
		}
		if (OnList == false) { 
			return Code::ERROR_Illegal_Heap;
		}

		//Downcast to selective access pattern
		HeapSAP* pInHeapSAP = (HeapSAP*)pInHeap;


		if (pInHeap->GetHeapType() == Heap::Type::NORMAL) {
			//Free all allocations on this heap
			Block* CurrentBlock = pInHeap->GetTrackingHead();
			Block* DeadBlock;
			while (CurrentBlock != nullptr) {
				DeadBlock = CurrentBlock;
				CurrentBlock = CurrentBlock->GetHeapNext();
				pInHeapSAP->FreeBlockSAP(DeadBlock);
			}
		}
		

		//Remove Heap from Heap List
		pMem->Remove(pInHeap);

		//Destory win32 heap
		assert(pInHeapSAP->DestroySAP());

		//update stats
		pMem->mInfo.CurrHeapCount--;

		return Code::OK;
	}

	Mem::Code Mem::GetHeapByAddr(Heap*& pHeap, void* p)
	{
		Mem* pMem = Mem::GetInstance();
		
		if (pMem == nullptr) {
			return Code::ERROR_Mem_Not_Initialized;
		}

		//Walk Heap List and check p is valid address
		Heap* current = pMem->poHead;
		Heap::Info info;

		while (current != nullptr) {

			current->GetInfo(info);

			size_t addr = (size_t)p;
			//Check if address is within beginning and end of heap
			if (addr > info.StartAddr && addr < info.EndAddr) {
				
				pHeap = current;
				return Code::OK;
			}
			current = current->GetNext();
		}
	
		return Code::ERROR_Invalid_Addr;
	}

	Mem* Mem::GetInstance() {

		return pInstance;
	}

	Mem::Code Mem::PushToFront(Heap* pNode) {

		assert(pNode != nullptr);

		Mem* pMem = Mem::GetInstance();
		if (pMem == nullptr) { return Code::ERROR_Mem_Not_Initialized; }

		// add node
		if (pMem->poHead == nullptr)
		{
			// push to the front
			pMem->poHead = pNode;
			pNode->SetNext(nullptr);
			pNode->SetPrev(nullptr);
		}
		else
		{
			// push to front
			//pNode->pPrev = nullptr;
			pNode->SetNext(pMem->poHead);
			// update head
			pMem->poHead->SetPrev(pNode);
			pMem->poHead = pNode;
		}
		assert(pMem->poHead != nullptr);

		return Code::OK;
	}

	Mem::Code Mem::Remove(Heap* pNode) {

		assert(pNode != nullptr);

		Mem* pMem = Mem::GetInstance();
		if (pMem == nullptr) { return Code::ERROR_Mem_Not_Initialized; }

		//if not head
		if (pNode->GetPrev() != nullptr)
		{
			pNode->GetPrev()->SetNext(pNode->GetNext());
		}
		//else if head
		else
		{
			pMem->poHead = pNode->GetNext();
		}

		if (pMem->poHead != nullptr) {
			pMem->poHead->SetPrev(nullptr);
		}

		if (pNode->GetNext() != nullptr)
		{
			pNode->GetNext()->SetPrev(pNode->GetPrev());
		}
		// remove any lingering links
		pNode->SetNext(nullptr);
		pNode->SetPrev(nullptr);

		return Code::OK;
	}

	void Mem::IncreaseAllocInfo(const size_t inSize) {
	
		this->mInfo.CurrBytesUsed += inSize;
		this->mInfo.CurrNumAlloc++;
		this->mInfo.CurrAllocIndex++;

		if (this->mInfo.CurrBytesUsed > this->mInfo.PeakBytesUsed) {
			this->mInfo.PeakBytesUsed = this->mInfo.CurrBytesUsed;
		}

		if (this->mInfo.CurrNumAlloc > this->mInfo.PeakNumAlloc) {
			this->mInfo.PeakNumAlloc = this->mInfo.CurrNumAlloc;
		}
	}

	void Mem::DecreaseAllocInfo(const size_t inSize) {

		this->mInfo.CurrBytesUsed -= inSize;
		this->mInfo.CurrNumAlloc--;
	}


}

// ---  End of File ---------------
