//---------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#include "_UnitTestConfiguration.h"
#include "MemEngine.h"
using namespace Azul;
//---------------------------------------------------------------------------
// Test Files:
// Single step through, set break points, but do not modify it
//---------------------------------------------------------------------------

#if version_enable
TEST(version, mem_group, version_enable)
{
	CHECK(Mem::MAJOR_VERSION == 4);
	CHECK(Mem::MINOR_VERSION == 0);
} TEST_END
#endif

#if Initialize_MemorySystem_enable	 
TEST(Initialize_MemorySystem, mem_group, Initialize_MemorySystem_enable)
{
	// Initialize the memory system
	Mem::Create();

	// Test the mem Info structures
	Mem::Info mem_Info;

	// Yes it works.
	Mem::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 0);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 0);
	CHECK(mem_Info.PeakNumAlloc == 0);

	Mem::Destroy();

}TEST_END
#endif

#if No_Initialize_MemorySystem_enable
TEST(No_Initialize_MemorySystem, mem_group, No_Initialize_MemorySystem_enable)
{
	// Initialize the memory system
	//Mem::Initialize(); --> commented out on purpose

	// Test the mem Info structures
	Mem::Info mem_Info;

	// get info
	Mem::Code memResult;
	memResult = Mem::GetInfo(mem_Info);

	CHECK(memResult == Mem::Code::ERROR_Mem_Not_Initialized);

}TEST_END
#endif

#if No_Initialize_Destroy_enable
TEST(No_Initialize_Destroy, mem_group, No_Initialize_Destroy_enable)
{
	// Initialize the memory system
	//Mem::Initialize();  <--- commented out on purpose

	// get info
	Mem::Code memResult;
	memResult = Mem::Destroy();

	CHECK(memResult == Mem::Code::ERROR_Mem_Not_Initialized);

}TEST_END
#endif

#if No_Initialize_HeapCreate_enable
TEST(No_Initialize_HeapCreate, mem_group, No_Initialize_HeapCreate_enable)
{
	// Initialize the memory system
	//Mem::Initialize();  <--- commented out on purpose

	// Create a heap pointer (someday make it a handle)
	Heap* heap_A;

	// create the heap by using the memory system
	Mem::Code memResult;
	memResult = Mem::CreateNormalHeap(heap_A, 2 * 1024 * 1024, "HEAP_A");

	CHECK(memResult == Mem::Code::ERROR_Mem_Not_Initialized);

	CHECK(heap_A == nullptr);

}TEST_END
#endif

#if Initialize_HeapCreate_enable
TEST(Initialize_HeapCreate, mem_group, No_Initialize_HeapCreate_enable)
{
	// Initialize the memory system
	Mem::Create();

	// Create a heap pointer (someday make it a handle)
	Heap* heap_A;

	// create the heap by using the memory system
	Mem::Code memResult;
	memResult = Mem::CreateNormalHeap(heap_A, 2 * 1024 * 1024, "HEAP_A");
	CHECK(memResult == Mem::Code::OK);
	CHECK(heap_A != nullptr);

	Mem::Destroy();

}TEST_END
#endif

#if No_Initialize_NullHeap_Destroy_enable
TEST(No_Initialize_NullHeap_Destroy, mem_group, No_Initialize_NullHeap_Destroy_enable)
{
	// Initialize the memory system
	//Mem::Initialize();  <--- commented out on purpose

	// Create a heap pointer (someday make it a handle)
	Heap* heap_A = nullptr; //  just for this test

	// create the heap by using the memory system
	Mem::Code memResult;
	memResult = Mem::DestroyHeap(heap_A);

	CHECK(memResult == Mem::Code::ERROR_Mem_Not_Initialized);

}TEST_END
#endif

#if No_Initialize_Heap_Destroy_enable	
TEST(No_Initialize_Heap_Destroy, mem_group, No_Initialize_Heap_Destroy_enable)
{
	// Initialize the memory system
	//Mem::Initialize();  <--- commented out on purpose

	// Create a heap pointer (someday make it a handle)
	Heap* heap_A = (Heap*)0x1; //  just for this test (bogus pointer)

	// create the heap by using the memory system
	Mem::Code memResult;
	memResult = Mem::DestroyHeap(heap_A);

	CHECK(memResult == Mem::Code::ERROR_Mem_Not_Initialized);

}TEST_END
#endif

#if Initialize_NullHeap_Destroy_enable 
TEST(Initialize_NullHeap_Destroy, mem_group, Initialize_NullHeap_Destroy_enable)
{
	// Initialize the memory system
	Mem::Create();

	// Create a heap pointer (someday make it a handle)
	Heap* heap_A = nullptr;

	// create the heap by using the memory system
	Mem::Code memResult;
	memResult = Mem::DestroyHeap(heap_A);

	CHECK(memResult == Mem::Code::ERROR_Illegal_Heap);

	Mem::Destroy();

}TEST_END
#endif

#if Initialize_WrongHeap_Destroy_enable
TEST(Initialize_WrongHeap_Destroy, mem_group, Initialize_WrongHeap_Destroy_enable)
{
	// Initialize the memory system
	Mem::Create();

	// Create a heap pointer (someday make it a handle)
	Heap* heap_A = (Heap*)0x33; //  just for this test (bogus pointer)

	// create the heap by using the memory system
	Mem::Code memResult;
	memResult = Mem::DestroyHeap(heap_A);

	CHECK(memResult == Mem::Code::ERROR_Illegal_Heap);

	Mem::Destroy();

}TEST_END
#endif

#if Initialize_Heap_Destroy_enable
TEST(Initialize_Heap_Destroy, mem_group, Initialize_Heap_Destroy_enable)
{
	// Initialize the memory system
	Mem::Create();

	// Create a heap pointer (someday make it a handle)
	Heap* heap_A;

	// create the heap by using the memory system
	Mem::Code memResult;
	memResult = Mem::CreateNormalHeap(heap_A, 2 * 1024 * 1024, "HEAP_A");
	CHECK(memResult == Mem::Code::OK);
	CHECK(heap_A != nullptr);

	memResult = Mem::DestroyHeap(heap_A);
	CHECK(memResult == Mem::Code::OK);

	Mem::Destroy();

}TEST_END
#endif

#if AddOneHeap_enable
TEST(AddOneHeap, mem_group, AddOneHeap_enable)
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
	Mem::Code  memResult;
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

	Mem::Destroy();

}TEST_END
#endif

#if Add4Heaps_enable
TEST(Add4Heaps, mem_group, Add4Heaps_enable)
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

	// create the heap by using the memory system
	Mem::Code  memResult;
	Heap* heap_A;
	memResult = Mem::CreateNormalHeap(heap_A, 1 * 1024 * 1024, "HEAP_A");
	CHECK(memResult == Mem::Code::OK);

	CHECK(heap_A);
	CHECK(heap_A->GetHeapType() == Heap::Type::NORMAL);

	// create the heap by using the memory system
	Heap* heap_B;
	memResult = Mem::CreateNormalHeap(heap_B, 2 * 1024 * 1024, "HEAP_B");
	CHECK(memResult == Mem::Code::OK);

	CHECK(heap_B);
	CHECK(heap_B->GetHeapType() == Heap::Type::NORMAL);

	// create the heap by using the memory system
	Heap* heap_C;
	memResult = Mem::CreateNormalHeap(heap_C, 3 * 1024 * 1024, "HEAP_C");
	CHECK(memResult == Mem::Code::OK);

	CHECK(heap_C);
	CHECK(heap_C->GetHeapType() == Heap::Type::NORMAL);

	// create the heap by using the memory system
	Heap* heap_D;
	memResult = Mem::CreateNormalHeap(heap_D, 4 * 1024 * 1024, "HEAP_D");
	CHECK(memResult == Mem::Code::OK);

	CHECK(heap_D);
	CHECK(heap_D->GetHeapType() == Heap::Type::NORMAL);

	// Get the stats
	Mem::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 4);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 4);
	CHECK(mem_Info.PeakNumAlloc == 0);

	// Now walk through the heaps, make sure there is 4 of them

	Heap* hTmp;

	// get the head
	hTmp = Mem::DebugGetHeapHead();

	CHECK(hTmp != 0);

	CHECK(hTmp == heap_D);
	CHECK(hTmp->DebugGetNext() == heap_C);
	CHECK(hTmp->DebugGetPrev() == 0);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_C);
	CHECK(hTmp->DebugGetNext() == heap_B);
	CHECK(hTmp->DebugGetPrev() == heap_D);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_B);
	CHECK(hTmp->DebugGetNext() == heap_A);
	CHECK(hTmp->DebugGetPrev() == heap_C);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_A);
	CHECK(hTmp->DebugGetNext() == 0);
	CHECK(hTmp->DebugGetPrev() == heap_B);

	Mem::Destroy();

}TEST_END
#endif

#if Add4Heaps_RemoveLastHeap_enable
TEST(Add4Heaps_RemoveLastHeap, mem_group, Add4Heaps_RemoveLastHeap_enable)
{
	// Initialize the memory system
	Mem::Create();

	// Test the mem Info structures
	Mem::Info mem_Info;

	// Yes it works.
	Mem::GetInfo(mem_Info);

	// create the heap by using the memory system
	Mem::Code memResult;
	Heap* heap_A;
	memResult = Mem::CreateNormalHeap(heap_A, 1 * 1024 * 1024, "HEAP_A");
	CHECK(memResult == Mem::Code::OK);

	CHECK(heap_A);
	CHECK(heap_A->GetHeapType() == Heap::Type::NORMAL);

	// create the heap by using the memory system
	Heap* heap_B;
	memResult = Mem::CreateNormalHeap(heap_B, 2 * 1024 * 1024, "HEAP_B");
	CHECK(memResult == Mem::Code::OK);

	CHECK(heap_B);
	CHECK(heap_B->GetHeapType() == Heap::Type::NORMAL);

	// create the heap by using the memory system
	Heap* heap_C;
	memResult = Mem::CreateNormalHeap(heap_C, 3 * 1024 * 1024, "HEAP_C");
	CHECK(memResult == Mem::Code::OK);

	CHECK(heap_C);
	CHECK(heap_C->GetHeapType() == Heap::Type::NORMAL);

	// create the heap by using the memory system
	Heap* heap_D;
	memResult = Mem::CreateNormalHeap(heap_D, 4 * 1024 * 1024, "HEAP_D");
	CHECK(memResult == Mem::Code::OK);

	CHECK(heap_D);
	CHECK(heap_D->GetHeapType() == Heap::Type::NORMAL);

	// Get the stats
	Mem::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 4);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 4);
	CHECK(mem_Info.PeakNumAlloc == 0);

	// Now walk through the heaps, make sure there is 4 of them
	Heap* hTmp;

	// get the head
	hTmp = Mem::DebugGetHeapHead();

	CHECK(hTmp != nullptr);

	CHECK(hTmp == heap_D);
	CHECK(hTmp->DebugGetNext() == heap_C);
	CHECK(hTmp->DebugGetPrev() == nullptr);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_C);
	CHECK(hTmp->DebugGetNext() == heap_B);
	CHECK(hTmp->DebugGetPrev() == heap_D);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_B);
	CHECK(hTmp->DebugGetNext() == heap_A);
	CHECK(hTmp->DebugGetPrev() == heap_C);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_A);
	CHECK(hTmp->DebugGetNext() == nullptr);
	CHECK(hTmp->DebugGetPrev() == heap_B);

	// --------------------------------------
	// Now remove last
	// --------------------------------------
	Mem::DestroyHeap(heap_A);

	// Get the stats
	Mem::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 3);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 4);
	CHECK(mem_Info.PeakNumAlloc == 0);

	// Now walk through the heaps, make sure there is 3 of them

	// get the head
	hTmp = Mem::DebugGetHeapHead();

	CHECK(hTmp != 0);

	CHECK(hTmp == heap_D);
	CHECK(hTmp->DebugGetNext() == heap_C);
	CHECK(hTmp->DebugGetPrev() == nullptr);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_C);
	CHECK(hTmp->DebugGetNext() == heap_B);
	CHECK(hTmp->DebugGetPrev() == heap_D);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_B);
	CHECK(hTmp->DebugGetNext() == nullptr);
	CHECK(hTmp->DebugGetPrev() == heap_C);

	Mem::Destroy();

}TEST_END
#endif

#if Add4Heaps_RemoveMiddleHeap_enable
TEST(Add4Heaps_RemoveMiddleHeap, mem_group, Add4Heaps_RemoveMiddleHeap_enable)
{
	// Initialize the memory system
	Mem::Create();

	// Test the mem Info structures
	Mem::Info mem_Info;

	// Yes it works.
	Mem::GetInfo(mem_Info);

	Mem::Code memResult;
	// create the heap by using the memory system
	Heap* heap_A;
	memResult = Mem::CreateNormalHeap(heap_A, 1 * 1024 * 1024, "HEAP_A");
	CHECK(memResult == Mem::Code::OK);

	CHECK(heap_A);
	CHECK(heap_A->GetHeapType() == Heap::Type::NORMAL);

	// create the heap by using the memory system
	Heap* heap_B;
	memResult = Mem::CreateNormalHeap(heap_B, 2 * 1024 * 1024, "HEAP_B");
	CHECK(memResult == Mem::Code::OK);

	CHECK(heap_B);
	CHECK(heap_B->GetHeapType() == Heap::Type::NORMAL);

	// create the heap by using the memory system
	Heap* heap_C;
	memResult = Mem::CreateNormalHeap(heap_C, 3 * 1024 * 1024, "HEAP_C");
	CHECK(memResult == Mem::Code::OK);

	CHECK(heap_C);
	CHECK(heap_C->GetHeapType() == Heap::Type::NORMAL);

	// create the heap by using the memory system
	Heap* heap_D;
	memResult = Mem::CreateNormalHeap(heap_D, 4 * 1024 * 1024, "HEAP_D");
	CHECK(memResult == Mem::Code::OK);

	CHECK(heap_D);
	CHECK(heap_D->GetHeapType() == Heap::Type::NORMAL);

	// Get the stats
	Mem::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 4);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 4);
	CHECK(mem_Info.PeakNumAlloc == 0);

	// Now walk through the heaps, make sure there is 4 of them
	Heap* hTmp;

	// get the head
	hTmp = Mem::DebugGetHeapHead();

	CHECK(hTmp != 0);

	CHECK(hTmp == heap_D);
	CHECK(hTmp->DebugGetNext() == heap_C);
	CHECK(hTmp->DebugGetPrev() == nullptr);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_C);
	CHECK(hTmp->DebugGetNext() == heap_B);
	CHECK(hTmp->DebugGetPrev() == heap_D);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_B);
	CHECK(hTmp->DebugGetNext() == heap_A);
	CHECK(hTmp->DebugGetPrev() == heap_C);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_A);
	CHECK(hTmp->DebugGetNext() == nullptr);
	CHECK(hTmp->DebugGetPrev() == heap_B);

	// Now remove middle
	Mem::DestroyHeap(heap_B);

	// Get the stats
	Mem::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 3);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 4);
	CHECK(mem_Info.PeakNumAlloc == 0);

	// Now walk through the heaps, make sure there is 3 of them

	// get the head
	hTmp = Mem::DebugGetHeapHead();

	CHECK(hTmp != 0);

	CHECK(hTmp == heap_D);
	CHECK(hTmp->DebugGetNext() == heap_C);
	CHECK(hTmp->DebugGetPrev() == nullptr);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_C);
	CHECK(hTmp->DebugGetNext() == heap_A);
	CHECK(hTmp->DebugGetPrev() == heap_D);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_A);
	CHECK(hTmp->DebugGetNext() == nullptr);
	CHECK(hTmp->DebugGetPrev() == heap_C);

	Mem::Destroy();

}TEST_END
#endif

#if Add4Heaps_RemoveFirstHeap_enable
TEST(Add4Heaps_RemoveFirstHeap, mem_group, Add4Heaps_RemoveFirstHeap_enable)
{
	// Initialize the memory system
	Mem::Create();

	// Test the mem Info structures
	Mem::Info mem_Info;

	// Yes it works.
	Mem::GetInfo(mem_Info);

	Mem::Code memResult;
	// create the heap by using the memory system
	Heap* heap_A;
	memResult = Mem::CreateNormalHeap(heap_A, 1 * 1024 * 1024, "HEAP_A");
	CHECK(memResult == Mem::Code::OK);

	CHECK(heap_A);
	CHECK(heap_A->GetHeapType() == Heap::Type::NORMAL);

	// create the heap by using the memory system
	Heap* heap_B;
	memResult = Mem::CreateNormalHeap(heap_B, 2 * 1024 * 1024, "HEAP_B");
	CHECK(memResult == Mem::Code::OK);

	CHECK(heap_B);
	CHECK(heap_B->GetHeapType() == Heap::Type::NORMAL);

	// create the heap by using the memory system
	Heap* heap_C;
	memResult = Mem::CreateNormalHeap(heap_C, 3 * 1024 * 1024, "HEAP_C");
	CHECK(memResult == Mem::Code::OK);

	CHECK(heap_C);
	CHECK(heap_C->GetHeapType() == Heap::Type::NORMAL);

	// create the heap by using the memory system
	Heap* heap_D;
	memResult = Mem::CreateNormalHeap(heap_D, 4 * 1024 * 1024, "HEAP_D");
	CHECK(memResult == Mem::Code::OK);

	CHECK(heap_D);
	CHECK(heap_D->GetHeapType() == Heap::Type::NORMAL);

	// Get the stats
	Mem::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 4);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 4);
	CHECK(mem_Info.PeakNumAlloc == 0);

	// Now walk through the heaps, make sure there is 4 of them
	Heap* hTmp;

	// get the head
	hTmp = Mem::DebugGetHeapHead();

	CHECK(hTmp != 0);

	CHECK(hTmp == heap_D);
	CHECK(hTmp->DebugGetNext() == heap_C);
	CHECK(hTmp->DebugGetPrev() == nullptr);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_C);
	CHECK(hTmp->DebugGetNext() == heap_B);
	CHECK(hTmp->DebugGetPrev() == heap_D);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_B);
	CHECK(hTmp->DebugGetNext() == heap_A);
	CHECK(hTmp->DebugGetPrev() == heap_C);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_A);
	CHECK(hTmp->DebugGetNext() == nullptr);
	CHECK(hTmp->DebugGetPrev() == heap_B);

	// Now remove last
	Mem::DestroyHeap(heap_D);

	// Get the stats
	Mem::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 3);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 4);
	CHECK(mem_Info.PeakNumAlloc == 0);

	// Now walk through the heaps, make sure there is 3 of them

	// get the head
	hTmp = Mem::DebugGetHeapHead();

	CHECK(hTmp != 0);

	CHECK(hTmp == heap_C);
	CHECK(hTmp->DebugGetNext() == heap_B);
	CHECK(hTmp->DebugGetPrev() == nullptr);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_B);
	CHECK(hTmp->DebugGetNext() == heap_A);
	CHECK(hTmp->DebugGetPrev() == heap_C);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_A);
	CHECK(hTmp->DebugGetNext() == nullptr);
	CHECK(hTmp->DebugGetPrev() == heap_B);

	Mem::Destroy();

} TEST_END
#endif

#if Add4Heaps_RemoveAllHeaps_enable
TEST(Add4Heaps_RemoveAllHeaps, mem_group, Add4Heaps_RemoveAllHeaps_enable)
{
	// Initialize the memory system
	Mem::Create();

	// Test the mem Info structures
	Mem::Info mem_Info;

	// Yes it works.
	Mem::GetInfo(mem_Info);

	// create the heap by using the memory system
	Mem::Code memResult;
	Heap* heap_A;
	memResult = Mem::CreateNormalHeap(heap_A, 1 * 1024 * 1024, "HEAP_A");
	CHECK(memResult == Mem::Code::OK);

	CHECK(heap_A);
	CHECK(heap_A->GetHeapType() == Heap::Type::NORMAL);

	// create the heap by using the memory system
	Heap* heap_B;
	memResult = Mem::CreateNormalHeap(heap_B, 2 * 1024 * 1024, "HEAP_B");
	CHECK(memResult == Mem::Code::OK);

	CHECK(heap_B);
	CHECK(heap_B->GetHeapType() == Heap::Type::NORMAL);

	// create the heap by using the memory system
	Heap* heap_C;
	memResult = Mem::CreateNormalHeap(heap_C, 3 * 1024 * 1024, "HEAP_C");
	CHECK(memResult == Mem::Code::OK);

	CHECK(heap_C);
	CHECK(heap_C->GetHeapType() == Heap::Type::NORMAL);

	// create the heap by using the memory system
	Heap* heap_D;
	memResult = Mem::CreateNormalHeap(heap_D, 4 * 1024 * 1024, "HEAP_D");
	CHECK(memResult == Mem::Code::OK);

	CHECK(heap_D);
	CHECK(heap_D->GetHeapType() == Heap::Type::NORMAL);

	// Get the stats
	Mem::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 4);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 4);
	CHECK(mem_Info.PeakNumAlloc == 0);

	// Now walk through the heaps, make sure there is 4 of them
	Heap* hTmp;

	// get the head
	hTmp = Mem::DebugGetHeapHead();

	CHECK(hTmp != 0);

	CHECK(hTmp == heap_D);
	CHECK(hTmp->DebugGetNext() == heap_C);
	CHECK(hTmp->DebugGetPrev() == nullptr);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_C);
	CHECK(hTmp->DebugGetNext() == heap_B);
	CHECK(hTmp->DebugGetPrev() == heap_D);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_B);
	CHECK(hTmp->DebugGetNext() == heap_A);
	CHECK(hTmp->DebugGetPrev() == heap_C);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_A);
	CHECK(hTmp->DebugGetNext() == nullptr);
	CHECK(hTmp->DebugGetPrev() == heap_B);

	// Now remove last -----------------------------------------------------------
	Mem::DestroyHeap(heap_A);

	// Get the stats
	Mem::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 3);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 4);
	CHECK(mem_Info.PeakNumAlloc == 0);

	// Now walk through the heaps, make sure there is 3 of them

	// get the head
	hTmp = Mem::DebugGetHeapHead();

	CHECK(hTmp != 0);

	CHECK(hTmp == heap_D);
	CHECK(hTmp->DebugGetNext() == heap_C);
	CHECK(hTmp->DebugGetPrev() == nullptr);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_C);
	CHECK(hTmp->DebugGetNext() == heap_B);
	CHECK(hTmp->DebugGetPrev() == heap_D);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_B);
	CHECK(hTmp->DebugGetNext() == nullptr);
	CHECK(hTmp->DebugGetPrev() == heap_C);

	// Now remove last  ------------------------------------------------
	Mem::DestroyHeap(heap_B);

	// Get the stats
	Mem::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 2);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 4);
	CHECK(mem_Info.PeakNumAlloc == 0);

	// Now walk through the heaps, make sure there is 2 of them

	// get the head
	hTmp = Mem::DebugGetHeapHead();

	CHECK(hTmp != 0);

	CHECK(hTmp == heap_D);
	CHECK(hTmp->DebugGetNext() == heap_C);
	CHECK(hTmp->DebugGetPrev() == nullptr);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_C);
	CHECK(hTmp->DebugGetNext() == nullptr);
	CHECK(hTmp->DebugGetPrev() == heap_D);

	// Now remove last------------------------------------------------
	Mem::DestroyHeap(heap_C);

	// Get the stats
	Mem::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 1);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 4);
	CHECK(mem_Info.PeakNumAlloc == 0);

	// Now walk through the heaps, make sure there is 1 of them

	// get the head
	hTmp = Mem::DebugGetHeapHead();

	CHECK(hTmp != 0);

	CHECK(hTmp == heap_D);
	CHECK(hTmp->DebugGetNext() == nullptr);
	CHECK(hTmp->DebugGetPrev() == nullptr);

	// Now remove last------------------------------------------------
	Mem::DestroyHeap(heap_D);

	// Get the stats
	Mem::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 0);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 4);
	CHECK(mem_Info.PeakNumAlloc == 0);

	// Now walk through the heaps, make sure there is 1 of them

	// get the head
	hTmp = Mem::DebugGetHeapHead();

	CHECK(hTmp == nullptr);

	Mem::Destroy();

}TEST_END
#endif


#if No_Initialize_GetHeapByAddr_enable
TEST(No_Initialize_GetHeapByAddr, mem_group, No_Initialize_GetHeapByAddr_enable)
{
	// Initialize the memory system
	//Mem::Initialize();  <--- uninitialized on purpose

	// Create a heap pointer (someday make it a handle)
	Heap* heap_A = nullptr;

	// create the heap by using the memory system
	Mem::Code memResult;
	memResult = Mem::GetHeapByAddr(heap_A, (void*)0x1);

	CHECK(memResult == Mem::Code::ERROR_Mem_Not_Initialized);

}TEST_END
#endif

#if Initialize_GetHeapByAddr_badPtr_enable
TEST(Initialize_GetHeapByAddr_badPtr, mem_group, Initialize_GetHeapByAddr_badPtr_enable)
{
	// Initialize the memory system
	Mem::Create();

	// Create a heap pointer (someday make it a handle)
	Heap* heap_A = nullptr;

	// create the heap by using the memory system
	Mem::Code memResult;
	memResult = Mem::GetHeapByAddr(heap_A, (void*)0x1);

	CHECK(memResult == Mem::Code::ERROR_Invalid_Addr);

	Mem::Destroy();

}TEST_END
#endif


// ---  End of File ---------------
