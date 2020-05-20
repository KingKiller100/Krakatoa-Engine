#pragma once

// kLibrary
#include <kLib.hpp>

// For client game applications

// Core
#include "Instrumentor.hpp"
#include "Core/EngineConfig.hpp"
#include "Core/Application.hpp"
#include "Core/Logging/Logger.hpp"

// Input
#include "Input/KeyCode.hpp"
#include "Input/InputManager.hpp"
#include "Input/MouseButtonCode.hpp"

// Camera
#include "Camera/OrthographicCamera.hpp"
#include "Camera/OrthographicCameraController.hpp"

// Colours
#include "Rendering/Colour.hpp"

// Rendering
#include "Rendering/Renderer.hpp"
#include "Rendering/Renderer2D.hpp"
#include "Rendering/RenderCommand.hpp"

// Textures
#include "Rendering/Textures/iTexture.hpp"
#include "Rendering/Textures/iTexture2D.hpp"
#include "Rendering/Textures/SubTexture2d.hpp"

// Rendering Resources
#include "Rendering/ShaderLibrary.hpp"
#include "Rendering/Rendering Resources/iShader.hpp"
#include "Rendering/Rendering Resources/iBuffers.hpp"
#include "Rendering/Rendering Resources/iVertexArray.hpp"
#include "Rendering/Rendering Resources/BufferLayout.hpp"

// Layers
#include "Layers/LayerBase.hpp"
#include "Layers/ImGui/ImGuiLayer.hpp"
