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

#include "Constants.hpp"

#include "Macros.hpp"
#include "Utils.hpp"
#include "Exceptions.hpp"


#ifdef CHE_CORE_DEBUG
#include "DebugUtils.hpp"
#undef CHE_CORE_DEBUG
#endif

#ifdef CHE_CORE_RENDER
#undef CHE_CORE_RENDER
#include "Buffer.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#endif

#ifdef CHE_CORE_APP
#undef CHE_CORE_APP
#include "Application.hpp"
#include "Window.hpp"
#endif


#ifdef CHE_CORE_ECS
#undef CHE_CORE_ECS
#include "Entity.hpp"
#include "Component.hpp"
#endif