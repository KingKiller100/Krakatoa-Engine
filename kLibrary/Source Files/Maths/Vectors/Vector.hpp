﻿#pragma once
#include "../../HelperMacros.hpp"

#include "../Length_Type.hpp"
#include "../kAlgorithms.hpp"

#include <utility>

namespace kmaths
{
	template<typename T, Length_Type N>
	struct Vector
	{
	public:
		static_assert(N > 0, "Must have at least one dimension to use mathematical vectors");

		using Type = T;
		inline static constexpr Length_Type Length = N;
		inline static constexpr size_t Bytes = Length * sizeof(T);

		constexpr Vector() noexcept
		{}

		template< typename U, Length_Type C>
		constexpr Vector(const Vector<U, C>& other) noexcept
		{
			*this = other;
		}

		template< typename U, Length_Type C>
		constexpr Vector(Vector&& other) noexcept
		{
			*this = std::move(other);
		}

		constexpr Vector(const std::initializer_list<T> l) noexcept
		{
			const auto first_iter = l.begin();
			const auto loops = l.size() < N ? l.size() : N;
			for (auto i = 0; i < loops; ++i)
				dimensions[i] = first_iter[i];
		}

		explicit constexpr Vector(Type _x, Type _y, Type _z = static_cast<Type>(0), Type _w = static_cast<Type>(0)) noexcept
		{
			dimensions[0] = _x;
			dimensions[1] = _y;
			if (_z) dimensions[2] = _z;
			if (_w) dimensions[3] = _w;
		}

		explicit constexpr Vector(const Type& _v) noexcept
		{
			for (auto& val : dimensions)
				val = _v;
		}

		explicit constexpr Vector(const T values[N])
		{
			for (auto i = 0; i < N; ++i)
				dimensions[i] = values[i];
		}

		USE_RESULT constexpr const Type& X() const noexcept
		{
			return dimensions[0];
		}

		USE_RESULT constexpr Type& X() noexcept
		{
			return dimensions[0];
		}

		template<typename U = Type>
		USE_RESULT constexpr std::enable_if_t<N >= 2, const U&> Y() const noexcept
		{
			return dimensions[1];
		}

		template<typename U = Type>
		USE_RESULT constexpr std::enable_if_t<N >= 2, U&> Y() noexcept
		{
			return dimensions[1];
		}

		template<typename U = Type>
		USE_RESULT constexpr std::enable_if_t<N >= 3, const U&> Z() const noexcept
		{
			return dimensions[2];
		}

		template<typename U = Type>
		USE_RESULT constexpr std::enable_if_t<N >= 3, U&> Z() noexcept
		{
			return dimensions[2];
		}


		template<typename U = Type>
		USE_RESULT constexpr std::enable_if_t<N >= 4, const U&> W() const noexcept
		{
			return dimensions[3];
		}

		template<typename U = Type>
		USE_RESULT constexpr std::enable_if_t<N >= 4, U&> W() noexcept
		{
			return dimensions[3];
		}

		USE_RESULT constexpr Type Magnitude() const noexcept
		{
			const auto magSQ = MagnitudeSQ();
			const auto mag = Sqrt(magSQ);
			return static_cast<Type>(mag);
		}

		USE_RESULT constexpr Type MagnitudeSQ() const noexcept
		{
			auto mag = static_cast<Type>(0);
			for (auto val : dimensions)
				mag += val * val;
			return mag;
		}

		template<typename U, Length_Type C>
		USE_RESULT constexpr T DotProduct(const Vector<U, C>& other) const noexcept
		{
			const auto size = (N < C) ? N : C;
			auto dp = static_cast<Type>(0);
			for (auto i = 0; i < size; ++i)
				dp += dimensions[i] * static_cast<Type>(other[i]);
			return dp;
		}

		USE_RESULT constexpr Vector Normalize() const noexcept
		{
			auto mag = Magnitude();
			if (mag == static_cast<Type>(0))
				return Vector();
			else if (mag == static_cast<T>(1))
				return *this;

			mag = static_cast<Type>(1) / mag;

			T temp[N]{ 0 };
			for (auto i = 0; i < N; ++i)
				temp[i] = dimensions[i] * mag;
			return Vector(temp);
		}

		// Restricts vector magnitude to max value
		constexpr void Truncate(const Type max) noexcept
		{
			if (Magnitude() > max)
			{
				*this = Normalize() * max;
			}
		}

		// Reassigns values to be positives
		constexpr void ToPositives() noexcept
		{
			for (auto& d : dimensions)
				if (d < 0)
					d = -d;
		}

		// Calculates distance between two 3D objects
		template<typename U, Length_Type C>
		USE_RESULT constexpr T Distance(const Vector<U, C>& v) const noexcept
		{
			const auto distanceVec = v - *this;
			return static_cast<Type>(distanceVec.Magnitude());
		}

		// Returns vector times by -1 - does not reassign values (except w element)
		USE_RESULT constexpr Vector ReverseVector() const noexcept
		{
			T copy[N]{ 0 };
			for (auto i = 0; i < N; ++i)
				copy[i] = dimensions[i] * -1;
			return Vector(copy);
		}

		USE_RESULT constexpr Vector Inverse() const noexcept
		{
			if _CONSTEXPR_IF(!std::is_floating_point_v<T>)
				return *this;

			T copy[N]{ 0 };
			for (auto i = 0; i < N; ++i)
				copy[i] = (constants::One<T>() / dimensions[i]);
			return Vector(copy);
		}

		// Sets all values of the vector to zero
		constexpr void Zero() noexcept
		{
			for (auto i = 0; i < N; ++i)
				dimensions[i] = CAST(Type, 0);
		}

		USE_RESULT constexpr bool IsZero() const noexcept
		{
			for (auto val : dimensions)
				if (val != CAST(Type, 0))
					return false;
			return true;
		}

		// Compilers earlier than C++20 will not work in constexpr
		USE_RESULT constexpr Type* GetPointerToData() const
		{
			Type& first = (Type)dimensions[0];
			return std::addressof<Type>(first);
		}

		USE_RESULT constexpr inline auto GetLength() const noexcept
		{
			return N;
		}

		template<typename U = Type>
		USE_RESULT constexpr std::enable_if_t<!std::is_unsigned_v<U>
			&& N == 2,
			Vector> Perpendicular() const noexcept
		{
			return Vector(-dimensions[1], dimensions[0]);
		}

		template<typename X, typename U = T>
		USE_RESULT constexpr std::enable_if_t<!std::is_unsigned_v<U>
			&& !std::is_unsigned_v<X>
			&& N == 3,
			Vector> CrossProduct(const Vector<X, N>& v) const noexcept
		{
			return Vector(
				(dimensions[1] * v[2] - dimensions[2] * v[1]),
				(dimensions[2] * v[0] - dimensions[0] * v[2]),
				(dimensions[0] * v[1] - dimensions[1] * v[0])
			);
		}

		USE_RESULT constexpr Type& operator[](const size_t index)
		{
			return dimensions[index];
		}

		USE_RESULT constexpr const Type& operator[](const size_t index) const
		{
			return dimensions[index];
		}

		USE_RESULT constexpr Vector operator-() const noexcept
		{
			T copy[N]{ 0 };
			for (size_t i = 0; i < N; ++i)
				copy[i] = dimensions[i] * -1;
			return Vector(copy);
		}

		template<typename U, Length_Type C>
		USE_RESULT constexpr Vector operator+(const Vector<U, C>& other) const noexcept
		{
			T copy[N]{ 0 };
			for (auto i = size_t(0); i < N; ++i)
			{
				copy[i] = (other.GetLength() > i)
					? dimensions[i] + static_cast<Type>(other[i])
					: dimensions[i];
			}
			return Vector(copy);
		}

		template<typename U, Length_Type C>
		USE_RESULT constexpr Vector operator-(const Vector<U, C>& other) const noexcept
		{
			T copy[N]{ 0 };
			for (auto i = size_t(0); i < N; ++i)
			{
				copy[i] = (other.GetLength() > i)
					? dimensions[i] - static_cast<Type>(other[i])
					: dimensions[i];
			}
			return Vector(copy);
		}

		template<typename U, Length_Type C>
		USE_RESULT constexpr Vector operator*(const Vector<U, C>& other) const noexcept
		{
			T copy[N]{ 0 };
			for (auto i = size_t(0); i < N; ++i)
			{
				copy[i] = (other.GetLength() > i)
					? dimensions[i] * static_cast<Type>(other[i])
					: dimensions[i];
			}
			return Vector(copy);
		}

		template<typename U, Length_Type C>
		USE_RESULT constexpr Vector operator/(const Vector<U, C>& other) const noexcept
		{
			T copy[N]{ 0 };
			for (auto i = size_t(0); i < N; ++i)
			{
				copy[i] = (other.GetLength() > i)
					? dimensions[i] / static_cast<Type>(other[i])
					: dimensions[i];
			}
			return Vector(copy);
		}

		template<typename U>
		USE_RESULT constexpr Vector operator*(const U scalar) const noexcept
		{
			T copy[N]{ 0 };
			for (auto i = size_t(0); i < N; ++i)
				copy[i] = dimensions[i] * CAST(Type, scalar);
			return Vector(copy);
		}

		template<typename U>
		USE_RESULT constexpr Vector operator/(const U scalar) const noexcept
		{
			T copy[N]{ 0 };
			for (auto i = size_t(0); i < N; ++i)
				copy[i] = dimensions[i] / CAST(Type, scalar);
			return Vector(copy);
		}

		template<typename U, Length_Type C>
		constexpr Vector& operator+=(const Vector<U, C>& other) noexcept
		{
			*this = *this + other;
			return *this;
		}

		template<typename U, Length_Type C>
		constexpr Vector operator-=(const Vector<U, C>& other) noexcept
		{
			*this = *this - other;
			return *this;
		}

		template<typename U>
		constexpr Vector operator*=(const U scalar) noexcept
		{
			*this = *this * scalar;
			return *this;
		}

		template<typename U, Length_Type C>
		constexpr Vector operator*=(const Vector<U, C>& other) noexcept
		{
			*this = *this * other;
			return *this;
		}

		template<typename U>
		constexpr Vector operator/=(const U scalar) noexcept
		{
			*this = *this / scalar;
			return *this;
		}

		template<typename U, Length_Type C>
		constexpr Vector operator/=(const Vector<U, C>& other) noexcept
		{
			*this = *this / other;
			return *this;
		}

		// bool operator == returns true if both Vector values are equal
		USE_RESULT constexpr bool operator==(const Vector& v) const
		{
			for (size_t i = 0; i < N; ++i)
				if (dimensions[i] != v[i])
					return false;
			return true;
		}

		// bool operator == returns true if both Vector values are equal
		USE_RESULT constexpr bool operator==(Vector&& v) const
		{
			for (size_t i = 0; i < N; ++i)
				if (dimensions[i] != v[i])
					return false;
			return true;
		}

		// bool operator != returns true if both Vector values are NOT equal
		USE_RESULT constexpr bool operator!=(const Vector& v) const
		{
			return !(*this == v);
		}

		template<typename U, Length_Type C>
		constexpr Vector& operator=(const Vector<U, C>& other) noexcept
		{
			constexpr auto size = (N < C) ? N : C;

			for (auto i = 0; i < size; ++i)
				dimensions[i] = static_cast<Type>(other[i]);

			return *this;
		}

		template<typename U, Length_Type C>
		constexpr Vector& operator=(Vector<U, C>&& other) noexcept
		{
			constexpr auto size = (N < C) ? N : C;

			for (auto i = 0; i < size; ++i)
				dimensions[i] = std::move(other[i]);

			return *this;
		}

		// Deleted version of functions (under certain circumstances
		template<typename U = Type>
		std::enable_if_t < N < 2, const U&> Y() const noexcept = delete;
		template<typename U = Type>
		std::enable_if_t < N < 2, U&> Y() noexcept = delete;
		template<typename U = Type>
		std::enable_if_t < N < 3, U&> Z() noexcept = delete;
		template<typename U = Type>
		std::enable_if_t < N < 3, const U&> Z() const noexcept = delete;
		template<typename U = Type>
		std::enable_if_t < N < 4, const U&> W() const noexcept = delete;
		template<typename U = Type>
		std::enable_if_t < N < 4, U&> W() noexcept = delete;

		template<typename U = T>
		USE_RESULT constexpr inline std::enable_if_t<std::is_unsigned_v<U>
			|| N != 2,
			Vector> Perpendicular() const
			= delete;

		template<typename X, typename U = T>
		USE_RESULT constexpr std::enable_if_t<std::is_unsigned_v<U>
			|| std::is_unsigned_v<X>
			|| N != 3,
			Vector> CrossProduct(const Vector& v) const noexcept
			= delete;

	private:
		T dimensions[N]{};
	};
}
