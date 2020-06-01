﻿#pragma once

#include "Memory Structures/MemoryTypes.hpp"

#include <HelperMacros.hpp>
#include <Maths/BytesTypes.hpp>

#include <array>
#include <string>

namespace memory
{
	struct SubPool
	{
		void* pHead;
		kmaths::Byte_Type* pNextFree;
		size_t capacity;
#ifndef KRAKOA_RELEASE
		size_t remainingSpace;
#endif


		
		SubPool() noexcept
			: pHead(nullptr),
			pNextFree(nullptr),
			capacity(0)
#ifndef KRAKOA_RELEASE
		, remainingSpace(capacity)
#endif
		{}
		
		SubPool(const SubPool&) = delete;
	};

	class MemoryPool
	{
		struct Token {};
		static constexpr size_t SubPoolSize = 4;
		using SubPoolList = std::array<SubPool, SubPoolSize>;

	public:
		MemoryPool(Token&) noexcept;
		MemoryPool(const MemoryPool&) = delete;
		~MemoryPool() noexcept;

		void Initialize(const size_t volume, const kmaths::BytesUnits units);

		kmaths::Byte_Type* Allocate(const size_t requestedBytes);
		void Deallocate(AllocHeader* pHeader, const size_t bytesToDelete);

		USE_RESULT size_t GetTotalBytes() const;
		USE_RESULT size_t GetMaxBytes() const;

		USE_RESULT size_t WalkTheHeap() const;
		USE_RESULT std::string GetStatus() const;

		static MemoryPool& Reference();

	private:
		/**
		 * \brief
		 *		Inform's current pool if there is enough space inside
		 * \param requestedBytes
		 *		Bytes to allocate
		 * \return
		 *		TRUE if this pool has space for this object or FALSE if this pool has no more space
		 */
		USE_RESULT SubPool& GetSubPoolIndex(const size_t requestedBytes);

		void CreateNewPool(const size_t capacity, const size_t index);

		SubPool& FindPointerOwner(void* pHeader);

		static void DefragHeap(SubPool& pool, const size_t deletedBytes);
		
		void ShutDown();
		
	private:
		SubPoolList subPoolList;
		const size_t poolIncrementBytes;
	};
}

