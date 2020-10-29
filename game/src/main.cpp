#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Application.h>
#include <glm/vec3.hpp>



int main() {

    Application();
    glm::vec3 test(1,2,3);
	std::cout << "FUCKe\n" << test.x << " " << test.y << " " << test.z << "\n";

    GLFWwindow* window;

    if (!glfwInit()) {
        std::cerr << "FUCK GLFW DID THE EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEee\n";
        return -1;
    }

    window = glfwCreateWindow(800, 800, "Fuck Glasser 2.0", NULL, NULL);

    if (!window) {
        glfwTerminate();
        std::cerr << "FUCK GLFW FUCKED ITS WINDOW UP THE ASS\n";
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGL()) {
        glfwTerminate();
        std::cerr << "GLAD IS FUCKEROO\n";
    }

    int fuck = 0;


    while (!glfwWindowShouldClose(window)) {
        fuck++;
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(
            (float)(fuck % 200) / 200.0f,
            (float)(fuck % 100) / 100.0f,
            (float)(fuck %  50) /  50.0f,
            1);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}