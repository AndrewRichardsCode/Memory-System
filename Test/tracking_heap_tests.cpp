//---------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#include "_UnitTestConfiguration.h"
#include "MemEngine.h"
#include "Dog.h"
using namespace Azul;
//---------------------------------------------------------------------------
// Test Files:
// Single step throught, set break points, but do not modify it
//---------------------------------------------------------------------------
#if Add5Allocs_TrackingThroughHeap_enable
TEST(Add5Allocs_TrackingThroughHeap, Tracking_group, Add5Allocs_TrackingThroughHeap_enable)
{
	// Initialize the memory system
	Mem::Create();

	// Test the mem Info structures
	Mem::Info mem_Info;

	// Yes it works.
	Mem::GetInfo(mem_Info);

	// validate it
	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 0);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 0);
	CHECK(mem_Info.PeakNumAlloc == 0);

	// Create a heap pointer (someday make it a handle)
	Heap* heap_A;

	// create the heap by using the memory system
	Mem::Code memResult;
	memResult = Mem::CreateNormalHeap(heap_A, 2 * 1024 * 1024, "HEAP_A");
	CHECK(memResult == Mem::Code::OK);

	CHECK(heap_A);
	CHECK(heap_A->GetHeapType() == Heap::Type::NORMAL);

	// Test the heap Info structures
	Heap::Info heapA_Info;

	// Yes it works.
	heap_A->GetInfo(heapA_Info);

	// start checks
	CHECK((size_t)heap_A >= heapA_Info.StartAddr);
	CHECK((size_t)heap_A < heapA_Info.EndAddr);
	CHECK(heapA_Info.CurrBytesUsed == 0);
	CHECK(heapA_Info.CurrNumAlloc == 0);
	CHECK(heapA_Info.EndAddr == 2 * 1024 * 1024 + heapA_Info.StartAddr);
	CHECK(strcmp(heapA_Info.Name, "HEAP_A") == 0);
	CHECK(heapA_Info.StartAddr == heapA_Info.EndAddr - 2 * 1024 * 1024);
	CHECK(heapA_Info.PeakBytesUsed == 0);
	CHECK(heapA_Info.PeakNumAlloc == 0);
	CHECK(heapA_Info.TotalSize == 2 * 1024 * 1024);

	// Yes it works.
	Mem::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 1);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 1);
	CHECK(mem_Info.PeakNumAlloc == 0);

	// OK at this point, we have a heap create, so let's allocate from it.

	Dog* p0 = new(heap_A, Mem::Align::Byte_4, __FILE__, __LINE__) Dog();
	Dog* p1 = new(heap_A, Mem::Align::Byte_4, __FILE__, __LINE__) Dog();
	Dog* p2 = new(heap_A, Mem::Align::Byte_4, __FILE__, __LINE__) Dog();
	Dog* p3 = new(heap_A, Mem::Align::Byte_4, __FILE__, __LINE__) Dog();
	Dog* p4 = new(heap_A, Mem::Align::Byte_4, __FILE__, __LINE__) Dog();

	// Yes it works.
	Mem::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 5 * sizeof(Dog));
	CHECK(mem_Info.CurrHeapCount == 1);
	CHECK(mem_Info.CurrNumAlloc == 5);
	CHECK(mem_Info.PeakBytesUsed == 5 * sizeof(Dog));
	CHECK(mem_Info.PeakHeapCount == 1);
	CHECK(mem_Info.PeakNumAlloc == 5);

	// OK, now all 5 allocations are created, now look at track info from heap.
	Block* t0 = heap_A->GetTrackingHead();

	CHECK(t0 != nullptr);
	CHECK(t0->GetAllocIndex() == 5);
	CHECK(strcmp(t0->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t0->GetLineNum() == 78);
	CHECK(t0->GetHeapPrev() == nullptr);
	CHECK(t0->GetHeapNext() != nullptr);

	Block* t1 = t0->GetHeapNext();

	CHECK(t1 != nullptr);
	CHECK(t1->GetAllocIndex() == 4);
	CHECK(strcmp(t1->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t1->GetLineNum() == 77);
	CHECK(t1->GetHeapPrev() == t0);
	CHECK(t1->GetHeapNext() != nullptr);
	CHECK(t0->GetHeapNext() == t1);

	Block* t2 = t1->GetHeapNext();

	CHECK(t2 != nullptr);
	CHECK(t2->GetAllocIndex() == 3);
	CHECK(strcmp(t2->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t2->GetLineNum() == 76);
	CHECK(t2->GetHeapPrev() == t1);
	CHECK(t2->GetHeapNext() != nullptr);
	CHECK(t1->GetHeapNext() == t2);

	Block* t3 = t2->GetHeapNext();

	CHECK(t3 != nullptr);
	CHECK(t3->GetAllocIndex() == 2);
	CHECK(strcmp(t3->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t3->GetLineNum() == 75);
	CHECK(t3->GetHeapPrev() == t2);
	CHECK(t3->GetHeapNext() != nullptr);
	CHECK(t2->GetHeapNext() == t3);

	Block* t4 = t3->GetHeapNext();

	CHECK(t4 != nullptr);
	CHECK(t4->GetAllocIndex() == 1);
	CHECK(strcmp(t4->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t4->GetLineNum() == 74);
	CHECK(t4->GetHeapPrev() == t3);
	CHECK(t4->GetHeapNext() == nullptr);
	CHECK(t3->GetHeapNext() == t4);

	delete p0;
	delete p1;
	delete p2;
	delete p3;
	delete p4;
	Mem::Destroy();
} TEST_END
#endif

#if Add4Allocs_RemoveLast_TrackingThroughHeap_enable
TEST(Add4Allocs_RemoveLast_TrackingThroughHeap, Tracking_group, Add4Allocs_RemoveLast_TrackingThroughHeap_enable)
{
	// Initialize the memory system
	Mem::Create();

	// Test the mem Info structures
	Mem::Info mem_Info;

	// Yes it works.
	Mem::GetInfo(mem_Info);

	// validate it
	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 0);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 0);
	CHECK(mem_Info.PeakNumAlloc == 0);

	// Create a heap pointer (someday make it a handle)
	Heap* heap_A;

	// create the heap by using the memory system
	Mem::Code memResult;
	memResult = Mem::CreateNormalHeap(heap_A, 2 * 1024 * 1024, "HEAP_A");
	CHECK(memResult == Mem::Code::OK);

	CHECK(heap_A);
	CHECK(heap_A->GetHeapType() == Heap::Type::NORMAL);

	// Test the heap Info structures
	Heap::Info heapA_Info;

	// Yes it works.
	heap_A->GetInfo(heapA_Info);

	CHECK((size_t)heap_A >= heapA_Info.StartAddr);
	CHECK((size_t)heap_A < heapA_Info.EndAddr);
	CHECK(heapA_Info.CurrBytesUsed == 0);
	CHECK(heapA_Info.CurrNumAlloc == 0);
	CHECK(heapA_Info.EndAddr == 2 * 1024 * 1024 + heapA_Info.StartAddr);
	CHECK(strcmp(heapA_Info.Name, "HEAP_A") == 0);
	CHECK(heapA_Info.StartAddr == heapA_Info.EndAddr - 2 * 1024 * 1024);
	CHECK(heapA_Info.PeakBytesUsed == 0);
	CHECK(heapA_Info.PeakNumAlloc == 0);
	CHECK(heapA_Info.TotalSize == 2 * 1024 * 1024);

	// Yes it works.
	Mem::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 1);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 1);
	CHECK(mem_Info.PeakNumAlloc == 0);

	// OK at this point, we have a heap create, so let's allocate from it.

	Dog* p0 = new(heap_A, Mem::Align::Byte_4, __FILE__, __LINE__) Dog();
	Dog* p1 = new(heap_A, Mem::Align::Byte_4, __FILE__, __LINE__) Dog();
	Dog* p2 = new(heap_A, Mem::Align::Byte_4, __FILE__, __LINE__) Dog();
	Dog* p3 = new(heap_A, Mem::Align::Byte_4, __FILE__, __LINE__) Dog();

	// Yes it works.
	Mem::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 4 * sizeof(Dog));
	CHECK(mem_Info.CurrHeapCount == 1);
	CHECK(mem_Info.CurrNumAlloc == 4);
	CHECK(mem_Info.PeakBytesUsed == 4 * sizeof(Dog));
	CHECK(mem_Info.PeakHeapCount == 1);
	CHECK(mem_Info.PeakNumAlloc == 4);

	// OK, now all 5 allocations are created, now look at track info from heap.
	Block* t0 = heap_A->GetTrackingHead();

	CHECK(t0 != nullptr);
	CHECK(t0->GetAllocIndex() == 4);
	CHECK(strcmp(t0->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t0->GetLineNum() == 212);
	CHECK(t0->GetHeapPrev() == nullptr);
	CHECK(t0->GetHeapNext() != nullptr);

	Block* t1 = t0->GetHeapNext();

	CHECK(t1 != nullptr);
	CHECK(t1->GetAllocIndex() == 3);
	CHECK(strcmp(t1->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t1->GetLineNum() == 211);
	CHECK(t1->GetHeapPrev() == t0);
	CHECK(t1->GetHeapNext() != nullptr);
	CHECK(t0->GetHeapNext() == t1);

	Block* t2 = t1->GetHeapNext();

	CHECK(t2 != nullptr);
	CHECK(t2->GetAllocIndex() == 2);
	CHECK(strcmp(t2->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t2->GetLineNum() == 210);
	CHECK(t2->GetHeapPrev() == t1);
	CHECK(t2->GetHeapNext() != nullptr);
	CHECK(t1->GetHeapNext() == t2);

	Block* t3 = t2->GetHeapNext();

	CHECK(t3 != nullptr);
	CHECK(t3->GetAllocIndex() == 1);
	CHECK(strcmp(t3->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t3->GetLineNum() == 209);
	CHECK(t3->GetHeapPrev() == t2);
	CHECK(t3->GetHeapNext() == nullptr);
	CHECK(t2->GetHeapNext() == t3);

	// Now remove the last one --------------------------------------------

	delete p0;

	// Yes it works.
	Mem::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 3 * sizeof(Dog));
	CHECK(mem_Info.CurrHeapCount == 1);
	CHECK(mem_Info.CurrNumAlloc == 3);
	CHECK(mem_Info.PeakBytesUsed == 4 * sizeof(Dog));
	CHECK(mem_Info.PeakHeapCount == 1);
	CHECK(mem_Info.PeakNumAlloc == 4);

	// OK, now all 4 allocations are created 1 deleted, now look at track info from heap.
	t0 = heap_A->GetTrackingHead();

	CHECK(t0 != nullptr);
	CHECK(t0->GetAllocIndex() == 4);
	CHECK(strcmp(t0->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t0->GetLineNum() == 212);
	CHECK(t0->GetHeapPrev() == nullptr);
	CHECK(t0->GetHeapNext() != nullptr);

	t1 = t0->GetHeapNext();

	CHECK(t1 != nullptr);
	CHECK(t1->GetAllocIndex() == 3);
	CHECK(strcmp(t1->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t1->GetLineNum() == 211);
	CHECK(t1->GetHeapPrev() == t0);
	CHECK(t1->GetHeapNext() != nullptr);
	CHECK(t0->GetHeapNext() == t1);

	t2 = t1->GetHeapNext();

	CHECK(t2 != nullptr);
	CHECK(t2->GetAllocIndex() == 2);
	CHECK(strcmp(t2->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t2->GetLineNum() == 210);
	CHECK(t2->GetHeapPrev() == t1);
	CHECK(t2->GetHeapNext() == nullptr);
	CHECK(t1->GetHeapNext() == t2);

	t3 = t2->GetHeapNext();

	CHECK(t3 == nullptr);

	delete p1;
	delete p2;
	delete p3;
	Mem::Destroy();
} TEST_END
#endif

#if Add4Allocs_RemoveFirst_TrackingThroughHeap_enable
TEST(Add4Allocs_RemoveFirst_TrackingThroughHeap, Tracking_group, Add4Allocs_RemoveFirst_TrackingThroughHeap_enable)
{
	// Initialize the memory system
	Mem::Create();

	// Test the mem Info structures
	Mem::Info mem_Info;

	// Yes it works.
	Mem::GetInfo(mem_Info);

	// validate it
	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 0);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 0);
	CHECK(mem_Info.PeakNumAlloc == 0);

	// Create a heap pointer (someday make it a handle)
	Heap* heap_A;

	// create the heap by using the memory system
	Mem::Code memResult;
	memResult = Mem::CreateNormalHeap(heap_A, 2 * 1024 * 1024, "HEAP_A");
	CHECK(memResult == Mem::Code::OK);

	CHECK(heap_A);
	CHECK(heap_A->GetHeapType() == Heap::Type::NORMAL);

	// Test the heap Info structures
	Heap::Info heapA_Info;

	// Yes it works.
	heap_A->GetInfo(heapA_Info);

	CHECK((size_t)heap_A >= heapA_Info.StartAddr);
	CHECK((size_t)heap_A < heapA_Info.EndAddr);
	CHECK(heapA_Info.CurrBytesUsed == 0);
	CHECK(heapA_Info.CurrNumAlloc == 0);
	CHECK(heapA_Info.EndAddr == 2 * 1024 * 1024 + heapA_Info.StartAddr);
	CHECK(strcmp(heapA_Info.Name, "HEAP_A") == 0);
	CHECK(heapA_Info.StartAddr == heapA_Info.EndAddr - 2 * 1024 * 1024);
	CHECK(heapA_Info.PeakBytesUsed == 0);
	CHECK(heapA_Info.PeakNumAlloc == 0);
	CHECK(heapA_Info.TotalSize == 2 * 1024 * 1024);

	// Yes it works.
	Mem::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 1);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 1);
	CHECK(mem_Info.PeakNumAlloc == 0);

	// OK at this point, we have a heap create, so let's allocate from it.

	Dog* p0 = new(heap_A, Mem::Align::Byte_4, __FILE__, __LINE__) Dog();
	Dog* p1 = new(heap_A, Mem::Align::Byte_4, __FILE__, __LINE__) Dog();
	Dog* p2 = new(heap_A, Mem::Align::Byte_4, __FILE__, __LINE__) Dog();
	Dog* p3 = new(heap_A, Mem::Align::Byte_4, __FILE__, __LINE__) Dog();

	// Yes it works.
	Mem::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 4 * sizeof(Dog));
	CHECK(mem_Info.CurrHeapCount == 1);
	CHECK(mem_Info.CurrNumAlloc == 4);
	CHECK(mem_Info.PeakBytesUsed == 4 * sizeof(Dog));
	CHECK(mem_Info.PeakHeapCount == 1);
	CHECK(mem_Info.PeakNumAlloc == 4);

	// OK, now all 4 allocations are created, now look at track info from heap.
	Block* t0 = heap_A->GetTrackingHead();

	CHECK(t0 != nullptr);
	CHECK(t0->GetAllocIndex() == 4);
	CHECK(strcmp(t0->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t0->GetLineNum() == 382);
	CHECK(t0->GetHeapPrev() == nullptr);
	CHECK(t0->GetHeapNext() != nullptr);

	Block* t1 = t0->GetHeapNext();

	CHECK(t1 != nullptr);
	CHECK(t1->GetAllocIndex() == 3);
	CHECK(strcmp(t1->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t1->GetLineNum() == 381);
	CHECK(t1->GetHeapPrev() == t0);
	CHECK(t1->GetHeapNext() != nullptr);
	CHECK(t0->GetHeapNext() == t1);

	Block* t2 = t1->GetHeapNext();

	CHECK(t2 != 0);
	CHECK(t2->GetAllocIndex() == 2);
	CHECK(strcmp(t2->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t2->GetLineNum() == 380);
	CHECK(t2->GetHeapPrev() == t1);
	CHECK(t2->GetHeapNext() != 0);
	CHECK(t1->GetHeapNext() == t2);

	Block* t3 = t2->GetHeapNext();

	CHECK(t3 != nullptr);
	CHECK(t3->GetAllocIndex() == 1);
	CHECK(strcmp(t3->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t3->GetLineNum() == 379);
	CHECK(t3->GetHeapPrev() == t2);
	CHECK(t3->GetHeapNext() == nullptr);
	CHECK(t2->GetHeapNext() == t3);

	// Now remove the last one --------------------------------------------

	delete p3;

	// Yes it works.
	Mem::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 3 * sizeof(Dog));
	CHECK(mem_Info.CurrHeapCount == 1);
	CHECK(mem_Info.CurrNumAlloc == 3);
	CHECK(mem_Info.PeakBytesUsed == 4 * sizeof(Dog));
	CHECK(mem_Info.PeakHeapCount == 1);
	CHECK(mem_Info.PeakNumAlloc == 4);

	// OK, now all 4 allocations are created 1 deleted, now look at track info from heap.
	t0 = heap_A->GetTrackingHead();

	CHECK(t0 != nullptr);
	CHECK(t0->GetAllocIndex() == 3);
	CHECK(strcmp(t0->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t0->GetLineNum() == 381);
	CHECK(t0->GetHeapPrev() == nullptr);
	CHECK(t0->GetHeapNext() != nullptr);

	t1 = t0->GetHeapNext();

	CHECK(t1 != nullptr);
	CHECK(t1->GetAllocIndex() == 2);
	CHECK(strcmp(t1->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t1->GetLineNum() == 380);
	CHECK(t1->GetHeapPrev() == t0);
	CHECK(t1->GetHeapNext() != nullptr);
	CHECK(t0->GetHeapNext() == t1);

	t2 = t1->GetHeapNext();

	CHECK(t2 != nullptr);
	CHECK(t2->GetAllocIndex() == 1);
	CHECK(strcmp(t2->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t2->GetLineNum() == 379);
	CHECK(t2->GetHeapPrev() == t1);
	CHECK(t2->GetHeapNext() == nullptr);
	CHECK(t1->GetHeapNext() == t2);

	t3 = t2->GetHeapNext();

	CHECK(t3 == nullptr);

	delete p0;
	delete p1;
	delete p2;
	Mem::Destroy();
} TEST_END
#endif

#if Add4Allocs_RemoveMiddle_TrackingThroughHeap_enable
TEST(Add4Allocs_RemoveMiddle_TrackingThroughHeap, Tracking_group, Add4Allocs_RemoveMiddle_TrackingThroughHeap_enable)
{
	// Initialize the memory system
	Mem::Create();

	// Test the mem Info structures
	Mem::Info mem_Info;

	// Yes it works.
	Mem::GetInfo(mem_Info);

	// validate it
	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 0);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 0);
	CHECK(mem_Info.PeakNumAlloc == 0);

	// Create a heap pointer (someday make it a handle)
	Heap* heap_A;

	// create the heap by using the memory system
	Mem::Code memResult;
	memResult = Mem::CreateNormalHeap(heap_A, 2 * 1024 * 1024, "HEAP_A");
	CHECK(memResult == Mem::Code::OK);

	CHECK(heap_A);
	CHECK(heap_A->GetHeapType() == Heap::Type::NORMAL);

	// Test the heap Info structures
	Heap::Info heapA_Info;

	// Yes it works.
	heap_A->GetInfo(heapA_Info);

	CHECK((size_t)heap_A >= heapA_Info.StartAddr);
	CHECK((size_t)heap_A < heapA_Info.EndAddr);
	CHECK(heapA_Info.CurrBytesUsed == 0);
	CHECK(heapA_Info.CurrNumAlloc == 0);
	CHECK(heapA_Info.EndAddr == 2 * 1024 * 1024 + heapA_Info.StartAddr);
	CHECK(strcmp(heapA_Info.Name, "HEAP_A") == 0);
	CHECK(heapA_Info.StartAddr == heapA_Info.EndAddr - 2 * 1024 * 1024);
	CHECK(heapA_Info.PeakBytesUsed == 0);
	CHECK(heapA_Info.PeakNumAlloc == 0);
	CHECK(heapA_Info.TotalSize == 2 * 1024 * 1024);

	// Yes it works.
	Mem::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 1);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 1);
	CHECK(mem_Info.PeakNumAlloc == 0);

	// OK at this point, we have a heap create, so let's allocate from it.

	Dog* p0 = new(heap_A, Mem::Align::Byte_4, __FILE__, __LINE__) Dog();
	Dog* p1 = new(heap_A, Mem::Align::Byte_4, __FILE__, __LINE__) Dog();
	Dog* p2 = new(heap_A, Mem::Align::Byte_4, __FILE__, __LINE__) Dog();
	Dog* p3 = new(heap_A, Mem::Align::Byte_4, __FILE__, __LINE__) Dog();

	// Yes it works.
	Mem::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 4 * sizeof(Dog));
	CHECK(mem_Info.CurrHeapCount == 1);
	CHECK(mem_Info.CurrNumAlloc == 4);
	CHECK(mem_Info.PeakBytesUsed == 4 * sizeof(Dog));
	CHECK(mem_Info.PeakHeapCount == 1);
	CHECK(mem_Info.PeakNumAlloc == 4);

	// OK, now all 4 allocations are created, now look at track info from heap.
	Block* t0 = heap_A->GetTrackingHead();

	CHECK(t0 != nullptr);
	CHECK(t0->GetAllocIndex() == 4);
	CHECK(strcmp(t0->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t0->GetLineNum() == 552);
	CHECK(t0->GetHeapPrev() == nullptr);
	CHECK(t0->GetHeapNext() != nullptr);

	Block* t1 = t0->GetHeapNext();

	CHECK(t1 != nullptr);
	CHECK(t1->GetAllocIndex() == 3);
	CHECK(strcmp(t1->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t1->GetLineNum() == 551);
	CHECK(t1->GetHeapPrev() == t0);
	CHECK(t1->GetHeapNext() != nullptr);
	CHECK(t0->GetHeapNext() == t1);

	Block* t2 = t1->GetHeapNext();

	CHECK(t2 != nullptr);
	CHECK(t2->GetAllocIndex() == 2);
	CHECK(strcmp(t2->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t2->GetLineNum() == 550);
	CHECK(t2->GetHeapPrev() == t1);
	CHECK(t2->GetHeapNext() != nullptr);
	CHECK(t1->GetHeapNext() == t2);

	Block* t3 = t2->GetHeapNext();

	CHECK(t3 != nullptr);
	CHECK(t3->GetAllocIndex() == 1);
	CHECK(strcmp(t3->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t3->GetLineNum() == 549);
	CHECK(t3->GetHeapPrev() == t2);
	CHECK(t3->GetHeapNext() == nullptr);
	CHECK(t2->GetHeapNext() == t3);

	// Now remove the last one --------------------------------------------

	delete p2;

	// Yes it works.
	Mem::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 3 * sizeof(Dog));
	CHECK(mem_Info.CurrHeapCount == 1);
	CHECK(mem_Info.CurrNumAlloc == 3);
	CHECK(mem_Info.PeakBytesUsed == 4 * sizeof(Dog));
	CHECK(mem_Info.PeakHeapCount == 1);
	CHECK(mem_Info.PeakNumAlloc == 4);

	// OK, now all 4 allocations are created 1 deleted, now look at track info from heap.
	t0 = heap_A->GetTrackingHead();

	CHECK(t0 != nullptr);
	CHECK(t0->GetAllocIndex() == 4);
	CHECK(strcmp(t0->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t0->GetLineNum() == 552);
	CHECK(t0->GetHeapPrev() == nullptr);
	CHECK(t0->GetHeapNext() != nullptr);

	t1 = t0->GetHeapNext();

	CHECK(t1 != nullptr);
	CHECK(t1->GetAllocIndex() == 2);
	CHECK(strcmp(t1->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t1->GetLineNum() == 550);
	CHECK(t1->GetHeapPrev() == t0);
	CHECK(t1->GetHeapNext() != nullptr);
	CHECK(t0->GetHeapNext() == t1);

	t2 = t1->GetHeapNext();

	CHECK(t2 != nullptr);
	CHECK(t2->GetAllocIndex() == 1);
	CHECK(strcmp(t2->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t2->GetLineNum() == 549);
	CHECK(t2->GetHeapPrev() == t1);
	CHECK(t2->GetHeapNext() == nullptr);
	CHECK(t1->GetHeapNext() == t2);

	t3 = t2->GetHeapNext();

	CHECK(t3 == nullptr);

	delete p0;
	delete p1;
	delete p3;
	Mem::Destroy();
} TEST_END
#endif

#if Add4Allocs_RemoveAll_TrackingThroughHeap_enable
TEST(Add4Allocs_RemoveAll_TrackingThroughHeap, Tracking_group, Add4Allocs_RemoveAll_TrackingThroughHeap_enable)
{
	// Initialize the memory system
	Mem::Create();

	// Test the mem Info structures
	Mem::Info mem_Info;

	// Yes it works.
	Mem::GetInfo(mem_Info);

	// validate it
	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 0);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 0);
	CHECK(mem_Info.PeakNumAlloc == 0);

	// Create a heap pointer (someday make it a handle)
	Heap* heap_A;

	// create the heap by using the memory system
	Mem::Code memResult;
	memResult = Mem::CreateNormalHeap(heap_A, 2 * 1024 * 1024, "HEAP_A");
	CHECK(memResult == Mem::Code::OK);

	CHECK(heap_A);
	CHECK(heap_A->GetHeapType() == Heap::Type::NORMAL);

	// Test the heap Info structures
	Heap::Info heapA_Info;

	// Yes it works.
	heap_A->GetInfo(heapA_Info);

	CHECK((size_t)heap_A >= heapA_Info.StartAddr);
	CHECK((size_t)heap_A < heapA_Info.EndAddr);
	CHECK(heapA_Info.CurrBytesUsed == 0);
	CHECK(heapA_Info.CurrNumAlloc == 0);
	CHECK(heapA_Info.EndAddr == 2 * 1024 * 1024 + heapA_Info.StartAddr);
	CHECK(strcmp(heapA_Info.Name, "HEAP_A") == 0);
	CHECK(heapA_Info.StartAddr == heapA_Info.EndAddr - 2 * 1024 * 1024);
	CHECK(heapA_Info.PeakBytesUsed == 0);
	CHECK(heapA_Info.PeakNumAlloc == 0);
	CHECK(heapA_Info.TotalSize == 2 * 1024 * 1024);

	// Yes it works.
	Mem::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 1);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 1);
	CHECK(mem_Info.PeakNumAlloc == 0);

	// OK at this point, we have a heap create, so let's allocate from it.

	Dog* p0 = new(heap_A, Mem::Align::Byte_4, __FILE__, __LINE__) Dog();
	Dog* p1 = new(heap_A, Mem::Align::Byte_4, __FILE__, __LINE__) Dog();
	Dog* p2 = new(heap_A, Mem::Align::Byte_4, __FILE__, __LINE__) Dog();
	Dog* p3 = new(heap_A, Mem::Align::Byte_4, __FILE__, __LINE__) Dog();

	// Yes it works.
	Mem::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 4 * sizeof(Dog));
	CHECK(mem_Info.CurrHeapCount == 1);
	CHECK(mem_Info.CurrNumAlloc == 4);
	CHECK(mem_Info.PeakBytesUsed == 4 * sizeof(Dog));
	CHECK(mem_Info.PeakHeapCount == 1);
	CHECK(mem_Info.PeakNumAlloc == 4);

	// OK, now all 4 allocations are created, now look at track info from heap.
	Block* t0 = heap_A->GetTrackingHead();

	CHECK(t0 != nullptr);
	CHECK(t0->GetAllocIndex() == 4);
	CHECK(strcmp(t0->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t0->GetLineNum() == 722);
	CHECK(t0->GetHeapPrev() == nullptr);
	CHECK(t0->GetHeapNext() != nullptr);

	Block* t1 = t0->GetHeapNext();

	CHECK(t1 != nullptr);
	CHECK(t1->GetAllocIndex() == 3);
	CHECK(strcmp(t1->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t1->GetLineNum() == 721);
	CHECK(t1->GetHeapPrev() == t0);
	CHECK(t1->GetHeapNext() != nullptr);
	CHECK(t0->GetHeapNext() == t1);

	Block* t2 = t1->GetHeapNext();

	CHECK(t2 != nullptr);
	CHECK(t2->GetAllocIndex() == 2);
	CHECK(strcmp(t2->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t2->GetLineNum() == 720);
	CHECK(t2->GetHeapPrev() == t1);
	CHECK(t2->GetHeapNext() != nullptr);
	CHECK(t1->GetHeapNext() == t2);

	Block* t3 = t2->GetHeapNext();

	CHECK(t3 != nullptr);
	CHECK(t3->GetAllocIndex() == 1);
	CHECK(strcmp(t3->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t3->GetLineNum() == 719);
	CHECK(t3->GetHeapPrev() == t2);
	CHECK(t3->GetHeapNext() == nullptr);
	CHECK(t2->GetHeapNext() == t3);

	// Now remove the last one --------------------------------------------

	delete p0;

	// Yes it works.
	Mem::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 3 * sizeof(Dog));
	CHECK(mem_Info.CurrHeapCount == 1);
	CHECK(mem_Info.CurrNumAlloc == 3);
	CHECK(mem_Info.PeakBytesUsed == 4 * sizeof(Dog));
	CHECK(mem_Info.PeakHeapCount == 1);
	CHECK(mem_Info.PeakNumAlloc == 4);

	// OK, now all 4 allocations are created 1 deleted, now look at track info from heap.
	t0 = heap_A->GetTrackingHead();

	CHECK(t0 != nullptr);
	CHECK(t0->GetAllocIndex() == 4);
	CHECK(strcmp(t0->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t0->GetLineNum() == 722);
	CHECK(t0->GetHeapPrev() == nullptr);
	CHECK(t0->GetHeapNext() != nullptr);

	t1 = t0->GetHeapNext();

	CHECK(t1 != nullptr);
	CHECK(t1->GetAllocIndex() == 3);
	CHECK(strcmp(t1->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t1->GetLineNum() == 721);
	CHECK(t1->GetHeapPrev() == t0);
	CHECK(t1->GetHeapNext() != nullptr);
	CHECK(t0->GetHeapNext() == t1);

	t2 = t1->GetHeapNext();

	CHECK(t2 != nullptr);
	CHECK(t2->GetAllocIndex() == 2);
	CHECK(strcmp(t2->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t2->GetLineNum() == 720);
	CHECK(t2->GetHeapPrev() == t1);
	CHECK(t2->GetHeapNext() == nullptr);
	CHECK(t1->GetHeapNext() == t2);

	t3 = t2->GetHeapNext();

	CHECK(t3 == nullptr);

	// Now remove the last one --------------------------------------------

	delete p1;

	// Yes it works.
	Mem::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 2 * sizeof(Dog));
	CHECK(mem_Info.CurrHeapCount == 1);
	CHECK(mem_Info.CurrNumAlloc == 2);
	CHECK(mem_Info.PeakBytesUsed == 4 * sizeof(Dog));
	CHECK(mem_Info.PeakHeapCount == 1);
	CHECK(mem_Info.PeakNumAlloc == 4);

	// OK, now all 4 allocations are created 1 deleted, now look at track info from heap.
	t0 = heap_A->GetTrackingHead();

	CHECK(t0 != nullptr);
	CHECK(t0->GetAllocIndex() == 4);
	CHECK(strcmp(t0->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t0->GetLineNum() == 722);
	CHECK(t0->GetHeapPrev() == nullptr);
	CHECK(t0->GetHeapNext() != nullptr);

	t1 = t0->GetHeapNext();

	CHECK(t1 != nullptr);
	CHECK(t1->GetAllocIndex() == 3);
	CHECK(strcmp(t1->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t1->GetLineNum() == 721);
	CHECK(t1->GetHeapPrev() == t0);
	CHECK(t1->GetHeapNext() == nullptr);
	CHECK(t0->GetHeapNext() == t1);

	t2 = t1->GetHeapNext();

	CHECK(t2 == 0);

	// Now remove the last one --------------------------------------------

	delete p2;

	// Yes it works.
	Mem::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 1 * sizeof(Dog));
	CHECK(mem_Info.CurrHeapCount == 1);
	CHECK(mem_Info.CurrNumAlloc == 1);
	CHECK(mem_Info.PeakBytesUsed == 4 * sizeof(Dog));
	CHECK(mem_Info.PeakHeapCount == 1);
	CHECK(mem_Info.PeakNumAlloc == 4);

	// OK, now all 4 allocations are created 1 deleted, now look at track info from heap.
	t0 = heap_A->GetTrackingHead();

	CHECK(t0 != 0);
	CHECK(t0->GetAllocIndex() == 4);
	CHECK(strcmp(t0->GetFileName(), "tracking_heap_tests.cpp") == 0);
	CHECK(t0->GetLineNum() == 722);
	CHECK(t0->GetHeapPrev() == 0);
	CHECK(t0->GetHeapNext() == 0);

	t1 = t0->GetHeapNext();

	CHECK(t1 == 0);

	// Now remove the last one --------------------------------------------

	delete p3;

	// Yes it works.
	Mem::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 1);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 4 * sizeof(Dog));
	CHECK(mem_Info.PeakHeapCount == 1);
	CHECK(mem_Info.PeakNumAlloc == 4);

	// OK, now all 4 allocations are created 1 deleted, now look at track info from heap.
	t0 = heap_A->GetTrackingHead();

	CHECK(t0 == nullptr);
	Mem::Destroy();
} TEST_END
#endif

// ---  End of File ---------------
