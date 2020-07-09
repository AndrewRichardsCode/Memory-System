#include "DLink.h"

namespace Azul
{
	DLink::DLink() {

		this->pNext = nullptr;
		this->pPrev = nullptr;
	}

	DLink::~DLink() {



	}

	void DLink::SetNext(Heap* pIn) {
		this->pNext = pIn;
	}

	void DLink::SetPrev(Heap* pIn) {
		this->pPrev = pIn;
	}

	Heap* DLink::GetNext() const {
		return this->pNext;
	}

	Heap* DLink::GetPrev() const {
		return this->pPrev;
	}

}