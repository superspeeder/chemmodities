#include "Application.h"

#include <iostream>
#include <glm/vec3.hpp>

Application::Application() {

    glm::vec3 test(0,0,1);

    std::cout << "EEEE\n" << test.x << " " << test.y << " " << test.z << "\n";
}