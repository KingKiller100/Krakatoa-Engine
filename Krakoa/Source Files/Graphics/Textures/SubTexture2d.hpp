﻿#pragma once

#include "../../Memory/MemoryTypes.hpp"
#include <Maths/Vectors/Vector2.hpp>
#include <vector>

#include "../Primitives 2D/BatchRendererData.hpp"

namespace krakoa::graphics
{
	class iTexture2D;

	using TexCoordsList = std::vector<kmaths::Vector2f>;

	enum class GeometryType : uint8_t
	{
		QUAD = batch::limits::quad::vertices,
		CIRCLE = batch::limits::circle::vertices,
		TRIANGLE = batch::limits::triangle::vertices,
		UNKNOWN = 255,
	};

	class SubTexture2D
	{
	public:
		struct TexCoordData
		{
			kmaths::Vector2f coordIndex;
			kmaths::Vector2f spriteDimensions;
			TexCoordsList baseCoords;
		};

	public:
		SubTexture2D();
		SubTexture2D(iTexture2D* texture, const TexCoordData& data);
		SubTexture2D(const std::shared_ptr<iTexture2D>& texture, const TexCoordData& data);
		~SubTexture2D() noexcept;

		USE_RESULT const Multi_Ptr<iTexture2D>& GetTexture() const noexcept;
		USE_RESULT Multi_Ptr<iTexture2D>& GetTexture() noexcept;
		USE_RESULT const kmaths::Vector2f* GetTexCoord() const noexcept;

		void SetTexture(iTexture2D* tex) noexcept;
		SETTER(std::shared_ptr<iTexture2D>, SetTexture, texture)

		CONST_GETTER(TexCoordData, GetTexCoordData, texCoordData)

		USE_RESULT GeometryType GetGeometryType() const noexcept;
		
		// Only for quads
		static SubTexture2D* Create(iTexture2D* texture, const TexCoordData& texCoordData);
		static SubTexture2D* Create(const std::shared_ptr<iTexture2D > & texture, const TexCoordData& data);

	private:
		void CreateTexCoords();

	private:
		Multi_Ptr<iTexture2D> texture;
		std::vector<kmaths::Vector2f> texCoords;
		TexCoordData texCoordData;
	};
}
