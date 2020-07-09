#ifndef DLINK_H
#define DLINK_H

namespace Azul 
{

	//forward declare
	class Heap;

	class DLink
	{
	public:

		DLink();
		DLink(const DLink& in) = delete;
		const DLink& operator=(const DLink& LinkIn) = delete;
		~DLink();

		void SetNext(Heap* pIn);
		void SetPrev(Heap* pIn);
		Heap* GetNext() const;
		Heap* GetPrev() const;

	private:
		Heap* pNext;
		Heap* pPrev;

	};
}

#endif DLINK_H
