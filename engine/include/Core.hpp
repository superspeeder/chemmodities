#pragma once
/*

Generic Core includes
    - Exceptions.hpp
    - Macros.hpp
    - Utils.hpp
    - Constants.hpp
    - glad/glad.h
    - GLFW/glfw.h
    - glm/glm.hpp
    - spdlog/spdlog.h
    - spdlog/sinks/stdout_color_sinks.h
    - spdlog/sinks/rotating_file_sink.h
    
define symbols if you want specific includes

CHE_CORE_RENDER
    - Buffer.hpp
    - Shader.hpp
    - Window.hpp

CHE_CORE_APP
    - Application.hpp
    - Window.hpp

CHE_CORE_DEBUG
    - DebugUtils.hpp

CHE_CORE_ECS
    - Entity.hpp
    - Component.hpp

If any are defined as 0, they are ignored and their respective targets are not debugged
CHE_MACROS_DEBUG uses slower, verbose versions of macros in Macros.h
The others enable verbosity within functions for their targets
The use of CHE_CORE_DEBUG activates utilities in DebugUtils.hpp

*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

#include "Utils/Constants.hpp"

#include "Utils/Macros.hpp"
#include "Utils/Utils.hpp"
#include "Utils/Exceptions.hpp"


#ifdef CHE_CORE_DEBUG
#include "Utils/DebugUtils.hpp"
#undef CHE_CORE_DEBUG
#endif

#ifdef CHE_CORE_RENDER
#undef CHE_CORE_RENDER
#include "Renderer/Buffer.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/Window.hpp"
#include "Renderer/Shape.hpp"
#include "Renderer/Framebuffer.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Texture.hpp"
#include "Renderer/SpriteBatch.hpp"
#endif

#ifdef CHE_CORE_APP
#undef CHE_CORE_APP
#include "Application.hpp"
#include "Renderer/Window.hpp"
#endif


#ifdef CHE_CORE_ECS
#undef CHE_CORE_ECS
#include "ECS/Entity.hpp"
#include "ECS/Component.hpp"
#endif