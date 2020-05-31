﻿#include "Precompile.hpp"
#include "MemoryOperators.hpp"

#include "MemoryPool.hpp"
#include "Memory Structures/HeapBase.hpp"
#include "Memory Structures/MemoryTypes.hpp"
#include "Memory Structures/HeapFactory.hpp"

#include "../Core/Logging/MemoryLogger.hpp"

#include <cassert>

using namespace memory;

auto& memPool = MemoryPool::Reference();

void* operator new(const size_t bytes, memory::HeapBase* pHeap) // Pads Control Blocks
{
	MEM_ASSERT((bytes != 0) || bytes < CAST(size_t, -1));

	memPool.Initialize(1, kmaths::BytesUnits::KILO);
	
	const size_t requestedBytes = AllocHeaderBytes + bytes + SignatureBytes; // Alignment in memory
	auto* pBlock = memPool.Allocate(requestedBytes);
	auto* pHeader = REINTERPRET(AllocHeader*, pBlock);

	pHeader->signature = KRK_MEMSYSTEM_START_SIG;
	pHeader->pHeap = pHeap;
	pHeader->bytes = bytes;
	pHeader->pPrev = pHeader->pNext = nullptr;

	if (pHeader->pHeap->GetPrevAddress())
	{
		auto* pPrevHeader = pHeader->pHeap->GetPrevAddress();
		pHeader->pPrev = pPrevHeader;
		pPrevHeader->pNext = pHeader;
	}

	pHeader->pHeap->SetPrevAddress(pHeader);

	auto* pMemStart = pBlock + AllocHeaderBytes;
	auto* pMemEnd = REINTERPRET(AllocHeader::Signature_Ptr_Type, pMemStart + bytes);
	*pMemEnd = KRK_MEMSYSTEM_END_SIG;

	pHeap->Allocate(requestedBytes);

	return pMemStart; // Returns pointer to the start of the object's data
}

void* operator new [](const size_t bytes, memory::HeapBase* pHeap)
{
	return operator new(bytes, pHeap);
}

void* operator new(const size_t bytes)
{
	return operator new(bytes, memory::HeapFactory::GetDefaultHeap());
}

void* operator new [](const size_t bytes)
{
	return operator new(bytes, memory::HeapFactory::GetDefaultHeap());
}

void operator delete(void* ptr)
{
	if (!ptr)
		return;

	auto* pHeader = AllocHeader::GetHeaderFromPointer(ptr);
	
	auto& pHeap = pHeader->pHeap;
	const auto bytes = pHeader->bytes;
	auto& pPrev = pHeader->pPrev;
	auto& pNext = pHeader->pNext;

	const auto totalBytes = AllocHeaderBytes + bytes + SignatureBytes;

	if (!pPrev && !pNext) // Both null
		pHeap->SetPrevAddress(nullptr);
	else // Only one null
	{
		if (pNext)
			pNext->pPrev = pHeader->pPrev;
		else
			pHeap->SetPrevAddress(pPrev);

		if (pPrev)
			pPrev->pNext = pHeader->pNext;
	}

	pHeap->Deallocate(totalBytes);

	memPool.Deallocate(pHeader, totalBytes);
	//free(pHeader);

	ptr = nullptr;
}

void operator delete [](void* ptr)
{
	operator delete(ptr);
}

