﻿#include "Precompile.hpp"
#include "MemoryPool.hpp"

#include "MemoryErrors.hpp"
#include "Memory Structures/MemoryTypes.hpp"

#include "../Core/Logging/MemoryLogger.hpp"

#include <Maths/kAlgorithms.hpp>

namespace memory
{
	using namespace kmaths;

	constexpr size_t deadBlockSize = 1 << 17;

	void* MemoryPool::exampleDeadBlock = nullptr;

	MemoryPool::MemoryPool(const size_t initialVolume, const size_t typeSize)
		: typeSize(typeSize)
	{
		if (!exampleDeadBlock)
		{
			exampleDeadBlock = malloc(deadBlockSize);
			memset(exampleDeadBlock, 0, deadBlockSize);

			if (!exampleDeadBlock)
				throw debug::MemoryPoolError("Cannot allocate space for example dead memory block");
		}

		const auto capacity = initialVolume * typeSize;;
		CreateNewPool(capacity, 0);
	}

	MemoryPool::~MemoryPool() noexcept
	{
		ShutDown();
	}

	void MemoryPool::ShutDown()
	{
		for (auto& subPool : subPoolList)
		{
			if (!subPool.pHead)
				continue;

			free(subPool.pHead);
			subPool.pHead = subPool.pNextFree = nullptr;
		}
	}

	kmaths::Byte_Type* MemoryPool::Allocate(const size_t requestedBytes)
	{
		auto* pBlockStart = GetBlockStartPtr(requestedBytes);
		return pBlockStart;
	}

	kmaths::Byte_Type* MemoryPool::GetBlockStartPtr(const size_t requestedBytes)
	{
		for (int index = 0; index < SubPoolSize; ++index)
		{
			auto& currentPool = subPoolList[index];

			if (!currentPool.pHead)
			{
				const auto nextCapacity = subPoolList[index - 1].capacity >> 1; // Next pool has half the capacity as the previous
				CreateNewPool(nextCapacity, index);
			}

			auto* pBlockStart = FindFreeBlock(currentPool, requestedBytes);

			if (!pBlockStart)
				continue;

			currentPool.remainingSpace -= requestedBytes;
			return pBlockStart;
		}

		throw debug::MemoryFullError();
	}

	void MemoryPool::CreateNewPool(const size_t capacity, const size_t index)
	{
		static std::array<bool, SubPoolSize> usedIndex{};
		if (usedIndex[index]) // Attempting to create a pool that already exists
			throw debug::MemoryPoolError("Attempting to initialize a pool already in uses");

		auto& pool = subPoolList[index];
		pool = SubPool(capacity);
		pool.pHead = malloc(capacity);
		pool.pNextFree = CAST(kmaths::Byte_Type*, pool.pHead);
		memset(pool.pHead, 0, capacity);

		MEM_ASSERT(pool.pHead);
		usedIndex[index] = true;
	}

	kmaths::Byte_Type* MemoryPool::FindFreeBlock(SubPool& pool, const size_t requestedBytes) const
	{
		if (CheckBlockIsDead(pool.pNextFree, requestedBytes))
		{
			auto* pBlockStart = pool.pNextFree;
			pool.pNextFree += requestedBytes;
			MoveNextFreePointer(pool.pNextFree);
			return pBlockStart;
		}

		if (IsNegative(pool.pNextFree - static_cast<Byte_Type*>(pool.pHead)))
			throw debug::MemoryPoolError("Distance from pool's head to the"
				" pool's next free space pointers is negative!");

		auto* pNextFree = pool.pNextFree;
		auto* const prevFree = pNextFree;

		do {
			auto* pHeader = reinterpret_cast<AllocHeader*>(pNextFree);

			if (AllocHeader::VerifyHeader(pHeader, false))
				pNextFree += pHeader->bytes + ControlBlockSize;
			else
			{
				auto maxLoops = requestedBytes;
				auto* const pEndAddress = static_cast<Byte_Type*>(pool.pHead) + pool.capacity;

				while (maxLoops-- > 0 && (pNextFree + requestedBytes) <= pEndAddress)
				{
					pHeader = reinterpret_cast<AllocHeader*>(pNextFree);

					if (!AllocHeader::VerifyHeader(pHeader, false))
						pNextFree++;
					else
					{
						pNextFree += pHeader->bytes + ControlBlockSize;
						break;
					}
				}

				if ((pNextFree + requestedBytes) <= pEndAddress)
				{
					pool.pNextFree = prevFree;
					return nullptr;
				}
			}
		} while (!CheckBlockIsDead(pNextFree, requestedBytes));

		MoveNextFreePointer(pool.pNextFree);

		return pNextFree;
	}

	bool MemoryPool::CheckBlockIsDead(const Byte_Type* pNextFree, const size_t requestedBytes) const
	{
		if (requestedBytes <= deadBlockSize)
		{
			return (memcmp(
				pNextFree,
				exampleDeadBlock,
				requestedBytes)
				== 0);
		}

		auto initialLoops = requestedBytes / deadBlockSize;
		const auto remainingSize = requestedBytes % deadBlockSize;

		while (initialLoops-- > 0)
		{
			if (memcmp(
				pNextFree + (deadBlockSize * initialLoops),
				exampleDeadBlock,
				deadBlockSize)
				!= 0)
				return false;
		}
		
		return (memcmp(pNextFree, exampleDeadBlock, remainingSize) != 0);
	}

	void MemoryPool::MoveNextFreePointer(kmaths::Byte_Type*& pNextFree)
	{
		auto* pHeader = reinterpret_cast<AllocHeader*>(pNextFree);
		while (AllocHeader::VerifyHeader(pHeader, false))
		{
			pNextFree += pHeader->bytes + ControlBlockSize;
			pHeader = reinterpret_cast<AllocHeader*>(pNextFree);
		}
	}

	void MemoryPool::Deallocate(void* pBlockStart, const size_t bytesToDelete)
	{
		auto* pFreeAddress = REINTERPRET(Byte_Type*, pBlockStart);

		auto& pool = FindPointerOwner(pBlockStart);
		memset(pBlockStart, 0, bytesToDelete);

		if (pFreeAddress < pool.pNextFree)
			pool.pNextFree = pFreeAddress;

		pool.remainingSpace += bytesToDelete;
	}

	SubPool& MemoryPool::FindPointerOwner(void* pHeader)
	{
		const auto* const pAddress = CAST(kmaths::Byte_Type*, pHeader);

		for (auto i = 0; i < SubPoolSize; ++i)
		{
			const auto& pool = subPoolList[i];
			const auto* pStartAddress = CAST(kmaths::Byte_Type*, pool.pHead);
			const auto* pEndAddress = pStartAddress + pool.capacity;

			if (pStartAddress <= pAddress
				&& pEndAddress > pAddress)
				return subPoolList[i];
		}

		throw debug::UnknownPointerError();
	}

	size_t MemoryPool::GetBytes() const
	{
		size_t currentStorage = 0;
		for (auto& pool : subPoolList)
			currentStorage += (pool.capacity - pool.remainingSpace);
		return currentStorage;
	}

	size_t MemoryPool::GetMaxBytes() const
	{
		size_t maxBytes = 0;
		for (auto& pool : subPoolList)
			maxBytes += pool.capacity;

		return maxBytes;
	}

	std::string MemoryPool::GetStatus() const
	{
		return "";
	}
}
