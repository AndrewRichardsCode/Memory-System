#ifndef MEM_SAP_H
#define MEM_SAP_H

#include "Mem.h"

//-----Selective Access Pattern-----

//Don't distribute this class publicly
//Only include in heap.h
//Then distrubte that publicly

namespace Azul {

	class MemSAP : public Mem
	{
	public:
		void IncreaseAllocInfoSAP(const size_t inSize);
		void DecreaseAllocInfoSAP(const size_t inSize);

		void SetGlobalTrackingHeadSAP(Block* pBlockIn);

	};

}

#endif