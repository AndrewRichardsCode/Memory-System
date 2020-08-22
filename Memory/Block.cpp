

#include "Block.h"

namespace Azul
{
	//For Normal Heap
	Block::Block(const char* inName, size_t lineNum, size_t index, size_t inSize) 
		: hNext(nullptr), hPrev(nullptr), gNext(nullptr), gPrev(nullptr), 
		mLineNum(lineNum), mAllocIndex(index), mAllocSize(inSize), pSecret(this)
	{
		this->SetName(inName);
	}

	//For Fixed Heap
	Block::Block(size_t inSize) 
		: hNext(nullptr), hPrev(nullptr), gNext(nullptr), gPrev(nullptr),
		mFileName(), mLineNum(), mAllocIndex(), mAllocSize(inSize), pSecret(this)
	{

	}

	void Block::Set(const char* inName, size_t lineNum, size_t index) {
		
		this->SetName(inName);
		this->mLineNum = lineNum;
		this->mAllocIndex = index;
	}

	void Block::SetHeapNext(Block* hNextIn) {
		this->hNext = hNextIn;
	}

	void Block::SetHeapPrev(Block* hPrevIn) {
		this->hPrev = hPrevIn;
	}

	void Block::SetGlobalNext(Block* gNextIn) {
		this->gNext = gNextIn;
	}

	void Block::SetGlobalPrev(Block* gPrevIn) {
		this->gPrev = gPrevIn;
	}


	Block* Block::GetHeapNext() const
	{
		return this->hNext;
	}

	Block* Block::GetHeapPrev() const
	{
		return this->hPrev;
	}

	Block* Block::GetGlobalNext() const
	{
		return this->gNext;
	}

	Block* Block::GetGlobalPrev() const
	{
		return this->gPrev;
	}

	char* Block::GetFileName()
	{
		assert(this->mFileName != nullptr);
		return this->mFileName;
	}

	size_t Block::GetLineNum() const
	{
		return this->mLineNum;
	}

	size_t Block::GetAllocIndex() const
	{
		return this->mAllocIndex;
	}

	size_t Block::GetAllocSize() const
	{
		return this->mAllocSize;
	}

	void Block::SetName(const char* const pInName) {

		assert(pInName != nullptr);

		const char* p = strrchr(pInName, '\\');
		p++;

		//use memcpy
		memcpy_s(this->mFileName, Block::NameLength, p, Block::NameLength);
	}

}

// ---  End of File ---------------
