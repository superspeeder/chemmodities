#pragma once

#include <glm/glm.hpp>


namespace che {

    struct Triangle {
        glm::vec2 a,b,c;
    };

    struct Quad {
        glm::vec2 bl, tr;
    };

    struct Grid {
        glm::vec2 offset;
        glm::vec2 cell_size;
        glm::vec2 cell_count;
    };
}