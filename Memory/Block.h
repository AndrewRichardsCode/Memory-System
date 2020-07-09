//---------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#ifndef BLOCK_H
#define BLOCK_H

namespace Azul
{

	//-------------------------------------------------------------------------
	// Weird thing
	//   Please declare your forward declarations INSIDE namespace
	//   Outside the namespace cause all kinds of hell
	//-------------------------------------------------------------------------

	class Block
	{
	public:
		// defines for the heap name length
		static const uint32_t NameLength = 40;
		static const uint32_t NameNumChar = (Block::NameLength - 1);

	public:

		Block() = delete;
		Block(const Block&) = delete;
		Block& operator = (const Block& tmp) = delete;
		~Block() = default;

		//specialized constructor
		Block(const char* inName, size_t lineNum, size_t index, size_t inSize);
		Block(size_t inSize);

		//Set Block
		void Set(const char* inName, size_t lineNum, size_t index);

		// Public methods

		void SetHeapNext(Block* hNextIn);
		void SetHeapPrev(Block* hPrevIn);
		void SetGlobalNext(Block* gNextIn);
		void SetGlobalPrev(Block* gPrevIn);

		Block* GetHeapNext() const;
		Block* GetHeapPrev() const;
		Block* GetGlobalNext() const;
		Block* GetGlobalPrev() const;
		char* GetFileName();
		size_t  GetLineNum() const;
		size_t  GetAllocIndex() const;
		size_t  GetAllocSize() const;

	private:

		// Helpers
		void SetName(const char* const pInName);

		// Add tracking links 
		Block* hNext;
		Block* hPrev;
		Block* gNext;
		Block* gPrev;

		// stats
		char	mFileName[Block::NameLength];
		size_t	mLineNum;
		size_t	mAllocIndex;
		size_t	mAllocSize;

		// back pointer
		Block* pSecret;   // back pointer to the top of the trackingblock
	};

}

#endif

// ---  End of File ---------------
