﻿#pragma once

#include "Memory Structures/MemoryTypes.hpp"

#include <HelperMacros.hpp>
#include <Maths/BytesUnits.hpp>

#include <array>
#include <string>

namespace memory
{
	struct SubPool
	{
	public:
		explicit SubPool(const size_t capacity = 0) noexcept
			: pHead(nullptr),
			pNextFree(nullptr),
			capacity(capacity)
			, remainingSpace(capacity)
		{}

		SubPool(SubPool&& other) noexcept
			: pHead(other.pHead),
			pNextFree(other.pNextFree),
			capacity(other.capacity),
			remainingSpace(other.remainingSpace)
		{ }

		SubPool& operator=(SubPool&& other) noexcept
		{
			pHead = other.pHead;
			pNextFree = other.pNextFree;
			remainingSpace = other.remainingSpace;

			auto* capPtr = const_cast<size_t*>(&capacity);
			*capPtr = other.capacity;

			return *this;
		}

		SubPool(const SubPool&) = delete;

	public:
		void* pHead;
		kmaths::Byte_Type* pNextFree;
		const size_t capacity;
		size_t remainingSpace;
	};

	class MemoryPool
	{
		static constexpr size_t SubPoolSize = 4;
		using SubPoolList = std::array<SubPool, SubPoolSize>;

		struct MemLinkedList
		{
		public:
			size_t bytes;
			void* pPrev;
			void* pNext;
		};
		
	public:

		MemoryPool(const size_t initialVolume, const size_t typeSize);
		~MemoryPool() noexcept;

		kmaths::Byte_Type* Allocate(const size_t requestedBytes);
		void Deallocate(void* pBlockStart, const size_t bytesToDelete);

		USE_RESULT size_t GetBytes() const;
		USE_RESULT size_t GetMaxBytes() const;

		USE_RESULT std::string GetStatus() const;

		MemoryPool(const MemoryPool&) = delete;
		MemoryPool& operator=(const MemoryPool&) noexcept = delete;
	private:
		void ShutDown();

		kmaths::Byte_Type* FindFreeBlock(SubPool& pool, const size_t requestedBytes) const;
		bool CheckBlockIsDead(const kmaths::Byte_Type* pNextFree, size_t requestedBytes) const;

		/**
		 * \brief
		 *		Inform's current pool if there is enough space inside
		 * \param requestedBytes
		 *		Bytes to allocate
		 * \return
		 *		TRUE if this pool has space for this object or FALSE if this pool has no more space
		 */
		USE_RESULT kmaths::Byte_Type* GetBlockStartPtr(const size_t requestedBytes);

		void CreateNewPool(const size_t capacity, const size_t index);
		static void MoveNextFreePointer(kmaths::Byte_Type*& pNextFree);

		SubPool& FindPointerOwner(void* pHeader);


	private:
		SubPoolList subPoolList;
		size_t typeSize;
		
		static void* exampleDeadBlock;
	};
}

