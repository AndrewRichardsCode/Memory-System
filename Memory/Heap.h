

#ifndef HEAP_H
#define HEAP_H

#include "MemSAP.h"
#include "DLink.h"

namespace Azul
{

	//-------------------------------------------------------------------------
	// Namespace Weird thing
	//   Please declare your forward declarations INSIDE namespace
	//   Outside the namespace cause all kinds of hell
	//-------------------------------------------------------------------------

	// Forward declaration
	class Block;

	// Heap class
	class Heap : public DLink
	{
	public:
		// Constants 
		static const uint32_t NameLength = 40;
		static const uint32_t NameNumChar = (NameLength - 1);

		// Heap Types
		enum class Type : size_t
		{
			NORMAL,
			FIXED
		};

		// Info structure
		struct Info
		{
			char Name[Heap::NameLength];
			size_t    StartAddr;
			size_t    EndAddr;
			size_t    TotalSize;
			size_t	  PeakNumAlloc;
			size_t	  CurrNumAlloc;
			size_t	  PeakBytesUsed;
			size_t	  CurrBytesUsed;
		};

	public:
		// Prevent from calling
		Heap() = delete;
		Heap(const Heap&) = delete;
		const Heap& operator = (const Heap& tmp) = delete;

		// Public Interface (For Customers)
		void GetInfo(Info& info) const;

		// Tracking block 
		Block* GetTrackingHead() const;
		Heap::Type GetHeapType() const;

		Block* GetGlobalTrackingHead() const;

		// Overflow Heap
		void SetOverflowHeap(Heap*);
		Heap* GetOverflowHeap() const;

		// Needed for Testings (Not for Customers)
		Heap* DebugGetNext() const;
		Heap* DebugGetPrev() const;

	protected:
		//Specialized Constructor
		Heap(Mem* pMem, HANDLE HeapHandle, size_t HeapSize, const char* const Name, const Type HeapType);
		
		//destory heap
		const bool Destroy() const;
	
		//alloc block mem
		void* AllocBlock(Mem::Align Alignment, const char* inName, const size_t lineNum, const size_t inSize);
	
		void* AllocFixedBlock(const char* inName, const size_t lineNum, const size_t inSize);
		
		//remove block
		void FreeBlock(void* p);
		void FreeBlock(Block* pDeadBlock);

		//accessor
		void SetFreeFixedHead(Block* BlockIn);

	private:

		// Helpers
		void SetName(const char* const pInName);
		void IncreaseAllocInfo(const size_t inSize);
		void DecreaseAllocInfo(const size_t inSize);

		const unsigned int GetPadding(const Mem::Align Alignment) const;

		void SetGlobalTrackingHead(Block* BlockIn);

		void PushToFrontHeap(Block* pBlock);
		void PushToFrontGlobal(Block* pBlock);

		void RemoveFromHeap(Block* pBlock);
		void RemoveFromGlobal(Block* pBlock);

		// data ------------------------------------------------
		Block* pBlkHead;
		Type	mType;
		HANDLE  mWinHeapHandle;
		Info	mInfo;

		// null for Normal, used only in FixedHeap
		Block* pFixedFreeHead;

		// Overflow heap
		Heap* pOverflow;

		// Back link to the memory system
		Mem* pMem;
	};

}

#endif

// ---  End of File ---------------
