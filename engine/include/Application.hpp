#pragma once
#include "glad/glad.h"
#include "Renderer/Window.hpp"
#include "Utils/Utils.hpp"
#include <vector>

namespace che {
	class Application {
	protected:
	    che::Window* m_Window;
	    che::WindowSettings m_WindowSettings;
	    che::Logging m_Logging;

		std::vector<che::Disposable*> m_Disposables;
	public:

		Application();

		int launch();

		virtual void create() = 0;
		virtual void render(double dt) = 0;
		virtual void destroy() = 0;

		inline void registerDisposableObject(che::Disposable* object) { m_Disposables.push_back(object); };
		void disposeAll();

		inline che::Window* getWindow() { return m_Window; };
		inline che::WindowSettings getWindowSettings() { return m_WindowSettings; };
		inline che::Logging getLogging() { return m_Logging; };

		inline void enableAutoDisposer() { che::Disposable::setAutoDisposer(this); };
		void removeDisposable(che::Disposable* disposable);
	};
}