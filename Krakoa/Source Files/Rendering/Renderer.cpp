﻿#include "Precompile.hpp"
#include "Renderer.hpp"
#include "RenderCommand.hpp"

#include "../Rendering/Rendering Resources/iShader.hpp"

namespace krakoa::graphics
{
	Renderer::Renderer(Token &&)
	{
		KRK_BANNER("Rendering Architecture Info", "GRAPHICS");
		RenderCommand::Initialize();
	}

	Renderer::~Renderer()
		= default;

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		camera_VPMat = &camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene() const
	{

	}

	void Renderer::Submit(iShader& shader, const iVertexArray& vertexArray, const kmaths::TransformMatrix<float>& transform) const
	{
		KRK_FATAL(!vertexArray.GetVertexBuffers().empty(), "No vertex buffer attached to this vertex array!");

		shader.Bind();
		shader.UploadUniformMatrix4x4("u_VpMat", *camera_VPMat);
		shader.UploadUniformMatrix4x4("u_TransformMat", transform);
		vertexArray.Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}