﻿#include "Renderer2DLayer.hpp"

#include <imgui/imgui.h>

#include <Krakoa.hpp>

Renderer2DLayer::Renderer2DLayer() noexcept
	: LayerBase("Renderer"),
	cameraController(CAST(float, krakoa::Application::Reference().GetWindow().GetWidth()) // Aspect ratio from window size
		/ CAST(float, krakoa::Application::Reference().GetWindow().GetHeight()),
		true), // Aspect ratio from window size
	position({ 0.f, 0.f })
{
	cameraController.SetRotationSpeed(180.f);
	cameraController.SetTranslationSpeed(5.f);
}

void Renderer2DLayer::OnAttach()
{
	KRK_PROFILE_FUNCTION();
	pWinTexture = std::shared_ptr<krakoa::graphics::iTexture2D>(
		krakoa::graphics::iTexture2D::Create("Assets/Win.png")
		);
}

void Renderer2DLayer::OnDetach()
{
	KRK_PROFILE_FUNCTION();
}

void Renderer2DLayer::OnUpdate(float deltaTime)
{
	KRK_PROFILE_FUNCTION();

	constexpr float moveSpeed = 2.f;

	cameraController.OnUpdate(deltaTime);
	SendRendererCommands();

	if (krakoa::input::InputManager::IsKeyPressed(KRK_KEY_RIGHT))
		position.X() += moveSpeed * deltaTime;

	if (krakoa::input::InputManager::IsKeyPressed(KRK_KEY_LEFT))
		position.X() -= moveSpeed * deltaTime;

	if (krakoa::input::InputManager::IsKeyPressed(KRK_KEY_UP))
		position.Y() += moveSpeed * deltaTime;

	if (krakoa::input::InputManager::IsKeyPressed(KRK_KEY_DOWN))
		position.Y() -= moveSpeed * deltaTime;

	rotation -= 5 * moveSpeed * deltaTime;
}

void Renderer2DLayer::SendRendererCommands() noexcept
{
	KRK_PROFILE_FUNCTION();
	constexpr auto rotScale = kmaths::Vector2f(0.25f);

	krakoa::graphics::Renderer2D::BeginScene(cameraController.GetCamera());

	{
		KRK_PROFILE_SCOPE("Renderer coloured triangle");

		for (auto y = 0; y < 5; ++y)
			for (auto x = -755.f; x < 750.f; ++x)
				if (kmaths::Modulus<float>(x, y))
					krakoa::graphics::Renderer2D::DrawTriangle(geometryColour, kmaths::Vector3f(0.5f, 0.f, -.95f), { 0.25f, 0.25f });
				else
					krakoa::graphics::Renderer2D::DrawTriangle(pWinTexture, kmaths::Vector3f(-0.5f, 0.f, -.95f), { 0.25f, 0.25f });
	}

	//krakoa::graphics::Renderer2D::DrawQuad({ 0, 0, 0, 1 }, kmaths::Vector3f(0, 0), kmaths::Vector2f(20.f));

	{
		KRK_PROFILE_SCOPE("Textured quad");
		for (auto y = 0; y < 5; ++y) {
			for (auto x = -750.f; x < 750.f; ++x)
			{
				const auto miniSquarePos = kmaths::Vector3f{ x * .5f, y * .5f, -0.2f };
				krakoa::graphics::Renderer2D::DrawQuad({ 1, 1, 1, 0.25f }, miniSquarePos, kmaths::Vector2f(0.25f));
			}
		}
	}

	{
		KRK_PROFILE_SCOPE("Renderer coloured quad");
		krakoa::graphics::Renderer2D::DrawQuad(geometryColour, kmaths::Vector3f(-0.5f, 0.f, -0.75f), { 0.2f, 0.2f });
		krakoa::graphics::Renderer2D::DrawQuad({ 1, 0, 0, 1 }, kmaths::Vector3f(0.5f, 0.f, -0.75f), { 0.2f, 0.2f });
		krakoa::graphics::Renderer2D::DrawQuad({ 0, 1, 0, 1 }, kmaths::Vector3f(0.f, 0.5f, -0.75f), { 0.2f, 0.2f });
		krakoa::graphics::Renderer2D::DrawQuad({ 0, 0, 1, 1 }, kmaths::Vector3f(0.f, -0.5f, -0.75f), { 0.2f, 0.2f });

		krakoa::graphics::Renderer2D::DrawRotatedQuad(pWinTexture, position, rotation, rotScale, geometryColour, 3.f);
	}

	krakoa::graphics::Renderer2D::EndScene();
}

void Renderer2DLayer::OnRender()
{
	KRK_PROFILE_FUNCTION();

	ImGui::Begin("Geometry Colour Settings");
	ImGui::ColorEdit4("Geometry Colour", geometryColour.GetPointerToData());
	ImGui::End();

	RenderZoomControls();
}

void Renderer2DLayer::RenderZoomControls() noexcept
{
	constexpr kmaths::Vector2f in = { 0.f, 1.f };
	constexpr kmaths::Vector2f out = { 0.f, -1.f };

	ImGui::Begin("Zoom Controls");

	if (ImGui::ArrowButton("Zoom In", ImGuiDir_Up))
	{
		auto zoomInEvent = krakoa::events::MouseScrolledEvent(in);
		cameraController.OnEvent(zoomInEvent);
	}
	else if (ImGui::ArrowButton("Zoom Out", ImGuiDir_Down))
	{
		auto zoomOutEvent = krakoa::events::MouseScrolledEvent(out);
		cameraController.OnEvent(zoomOutEvent);
	}

	ImGui::End();
}

void Renderer2DLayer::OnEvent(krakoa::events::Event& e)
{
	KRK_PROFILE_FUNCTION();
	cameraController.OnEvent(e);
}
