project (stb LANGUAGES C)

set (STB_IMPLEMENTATIONS
    src/stb_c_lexer.c
    src/stb_connected_components.c
    src/stb_divide.c
    src/stb_ds.c
    src/stb_dxt.c
    src/stb_herringbone_wang_tile.c
    src/stb_image_resize.c
    src/stb_image_write.c
    src/stb_image.c
    src/stb_include.c
    src/stb_leakcheck.c
    src/stb_perlin.c
    src/stb_rect_pack.c
    src/stb_sprintf.c
    src/stb_truetype.c
)

add_library(stb ${STB_IMPLEMENTATIONS})
add_library(stb::stb ALIAS stb)

target_include_directories(stb PUBLIC ${PROJECT_SOURCE_DIR}/include)
