# Copyright (c) 2020 PotatoTech
# SPDX-License-Identifier: Apache-2.0

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::Main SDL2::Image)
if(NOT MSVC)
    target_link_libraries(${PROJECT_NAME} PRIVATE m)
endif()

target_sources(${PROJECT_NAME}
    PRIVATE
        main.c
        logic.c
        render.c
        font.c
        default_font_data.c
)

