#include "MemSAP.h"


namespace Azul {



	void MemSAP::IncreaseAllocInfoSAP(const size_t inSize) {



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

	void MemSAP::DecreaseAllocInfoSAP(const size_t inSize) {
		this->mInfo.CurrBytesUsed -= inSize;
		this->mInfo.CurrNumAlloc--;
	}

	void MemSAP::SetGlobalTrackingHeadSAP(Block* pBlockIn) {
		this->SetGlobalTrackingHead(pBlockIn);
	}




}
