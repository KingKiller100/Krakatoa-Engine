﻿#pragma once

#include <Maths/Matrices/Matrix4x2.hpp>

#include <memory>

namespace krakoa::graphics
{
	class iTexture2D;

	class SubTexture2D
	{
	public:
		// Only for quads
		SubTexture2D(const std::shared_ptr<iTexture2D>& texture, const kmaths::Vector2f& minCoord = kmaths::constants::Zero<kmaths::Vector2f>(), const kmaths::Vector2f& maxCoord = kmaths::constants::One<kmaths::Vector2f>()); 
		~SubTexture2D();
		
		USE_RESULT const std::shared_ptr<iTexture2D> GetTexture() const noexcept;
		USE_RESULT const kmaths::Vector2f* GetTexCoord() const noexcept;

		// Only for quads
		static SubTexture2D* Create(const std::shared_ptr<iTexture2D > & texture, const kmaths::Vector2f& coordIndex, const kmaths::Vector2f& spriteDimensions, const kmaths::Vector2f& increment = {1, 1});
		
	private:
		std::weak_ptr<iTexture2D> texture;
		kmaths::Matrix4x2f texCoords;
	};
}