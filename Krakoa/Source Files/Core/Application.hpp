#pragma once

#include <Core/EngineCore.hpp>
#include <iWindow.hpp>

#include <memory>

namespace krakoa
{
	#if defined (_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
	EXPIMP_TEMPLATE template class KRAKOA_API std::unique_ptr<iWindow>;
#pragma warning(pop)
#endif

	class KRAKOA_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Initialize();
		void Run();
		constexpr  bool IsRunning() const;
		virtual void Shutdown() = 0;

	protected:
		bool isRunning;
		std::unique_ptr<iWindow> window;
	};

	Application* CreateApplication();
}