//---------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#ifndef UNIT_TEST_CONFIGURATION_H
#define UNIT_TEST_CONFIGURATION_H

//--------------------------------------------------------------
// Make sure all are enabled for final submission...
//      I grade what ever is set in this file - Keenan
//--------------------------------------------------------------

//--------------------------------------------------------------
// TESTS below Mem::Align::Byte_4 is default... 
//       You can ignore alignment until you begin that section
//--------------------------------------------------------------
																   
#define version_enable                                             true 
																   
#define Initialize_MemorySystem_enable	                           true
																   
#define No_Initialize_MemorySystem_enable                          true
#define No_Initialize_Destroy_enable							   true 
																   
#define No_Initialize_HeapCreate_enable	                           true 
#define Initialize_HeapCreate_enable							   true 
																   
#define No_Initialize_NullHeap_Destroy_enable					   true 
#define No_Initialize_Heap_Destroy_enable						   true
																   
#define Initialize_NullHeap_Destroy_enable                         true 
#define Initialize_WrongHeap_Destroy_enable                        true 
#define Initialize_Heap_Destroy_enable                             true 
																   
#define AddOneHeap_enable				                           true 
#define Add4Heaps_enable				                           true 
																   
#define Add4Heaps_RemoveLastHeap_enable	                           true 
#define Add4Heaps_RemoveMiddleHeap_enable                          true 
#define Add4Heaps_RemoveFirstHeap_enable                           true 
#define Add4Heaps_RemoveAllHeaps_enable	                           true 
																   
#define No_Initialize_GetHeapByAddr_enable                         true 
#define Initialize_GetHeapByAddr_badPtr_enable                     true 
																   
#define allocate_from_heap_enable                                  true 
#define allocate_from_heapARRAY_enable                             true 
#define allocate_from_heapARRAY2_enable                            true 
#define allocate_from_heapARRAY3_enable                            true 
#define allocate_5_from_heap_enable                                true 
                     											   
#define test1_heapcCheck_enable                                    true 
#define test1_heapcCheck2_enable                                   true
#define test1_heapcCheck3_enable                                   true 
										                           
#define test1_trackCheck_enable                                    true 
#define test1_trackCheck2_enable                                   true 
#define test1_trackCheck3_enable                                   true 
#define test1_trackCheck4_enable                                   true 
														           
#define Add5Allocs_TrackingThroughHeap_enable                      true 
#define Add4Allocs_RemoveLast_TrackingThroughHeap_enable           true 
#define Add4Allocs_RemoveFirst_TrackingThroughHeap_enable          true 
#define Add4Allocs_RemoveMiddle_TrackingThroughHeap_enable         true 
#define Add4Allocs_RemoveAll_TrackingThroughHeap_enable            true 
																   
#define Add5Allocs_TrackingThroughHeap_Global_enable               true 
#define Add4Allocs_RemoveLast_TrackingThroughHeap_Global_enable    true 
#define Add4Allocs_RemoveFirst_TrackingThroughHeap_Global_enable   true 
#define Add4Allocs_RemoveMiddle_TrackingThroughHeap_Global_enable  true 
#define Add4Allocs_RemoveAll_TrackingThroughHeap_Global_enable     true 
																   
//--------------------------------------------------------------   
// TESTS below Require Alignment to be working					   
//--------------------------------------------------------------   
																   
#define align_4byte_test_enable                                    true 
#define align_8byte_test_enable                                    true 
#define align_16byte_test_enable                                   true 
#define align_32byte_test_enable                                   true 
#define align_64byte_test_enable                                   true 
#define align_128byte_test_enable                                  true 
#define align_256byte_test_enable                                  true 
																   
#define test1_AlignCheck_enable                                    true 
#define test1_alignCheck2_enable                                   true 
																   
//--------------------------------------------------------------   
// TESTS below require Fixed heaps								   
//--------------------------------------------------------------   
																   
#define fixed_test1_enable                                         true
#define fixed_test2_enable                                         true
																   
#define fixed_test1_overflow_enable                                true
#define fixed_test2_overflow_enable                                true
																   
#define Fixed_Tracking_enable                                      true
#define Fixed2_Tracking_enable                                     true

#endif

// ---  End of File ---------------
