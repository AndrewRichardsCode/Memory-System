//---------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#ifndef MEM_H
#define MEM_H



namespace Azul
{

	//-------------------------------------------------------------------------
	// Namespace Weird thing
	//   Please declare your forward declarations INSIDE namespace
	//   Outside the namespace cause all kinds of hell
	//-------------------------------------------------------------------------

	// forward declaration
	class Heap;
	class Block;

	// Singleton
	class Mem
	{
	public:

		// Constants for the library
		static const uint32_t MAJOR_VERSION = 4;
		static const uint32_t MINOR_VERSION = 0;

		// alignment 
		enum class Align
		{
			Byte_4,
			Byte_8,
			Byte_16,
			Byte_32,
			Byte_64,
			Byte_128,
			Byte_256,
			Default = Byte_4
		};

		// Return codes
		enum class Code
		{
			OK,
			ERROR_Heap_Create,
			ERROR_Illegal_Heap,
			ERROR_Mem_Not_Initialized,
			ERROR_Invalid_Addr,
		};

		// info structure
		struct Info
		{
			size_t CurrAllocIndex;
			size_t PeakHeapCount;
			size_t CurrHeapCount;
			size_t PeakNumAlloc;
			size_t CurrNumAlloc;
			size_t PeakBytesUsed;
			size_t CurrBytesUsed;
		};

	public:
		// Public Interface ---------------- (for the customers)

		// Initialize the mem system
		static Code Create();
		static Code Destroy();

		// Create an Heap from the heap system
		static Code CreateNormalHeap(Heap*& newHeap, unsigned int heapSize, const char* const Name);
		static Code CreateFixedHeap(Heap*& newHeap, unsigned int numBlocks, int sizePerBlock, const char* const Name);

		// Create an Heap from the heap system
		static Code DestroyHeap(Heap* inHeap);

		// Information
		static Code GetInfo(Info& info);

		static Code GetHeapByAddr(Heap*& pHeap, void* p);
		static Block* DebugGetGlobalTrackingHead(void);

		// Needed for Testings (Not for Customers)
		static Heap* DebugGetHeapHead(void);

		// -----------------------------------------------------------------------------------
		// Add extra data or methods below this line
		// -----------------------------------------------------------------------------------

		Mem(const Mem&) = delete;
		Mem& operator = (const Mem& tmp) = delete;
		~Mem();

	protected:

		void IncreaseAllocInfo(const size_t inSize);
		void DecreaseAllocInfo(const size_t inSize);

		void SetGlobalTrackingHead(Block* pBlockIn);

	private:

		Mem();
		static Mem* GetInstance();

		Code PushToFront(Heap* pNode);
		Code Remove(Heap* pNode);

		// data -----------------------------------------
		static Mem* pInstance; //singleton instance

		Heap* poHead;
		Block* pGlobalHead; //for tracking blocks
	protected:
		Info	mInfo;

		//bool	mInitialized;// remove this?
		//char    pad[7];
	};

}

#endif 

// ---  End of File ---------------
