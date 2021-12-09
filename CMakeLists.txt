cmake_minimum_required(VERSION 3.0.0)
project(gold-rush VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(PROJECT_SRC_DIR src)
set(PROJECT_INCLUDE_DIR include)
set(PROJECT_LIBRARY_DIR libraries)

set(APPLICATION_SRC
    ${PROJECT_SRC_DIR}/Application/glad.c
    ${PROJECT_SRC_DIR}/Application/Window.cpp
    ${PROJECT_SRC_DIR}/Application/Window.h)

set(BUFFERS_SRC
    ${PROJECT_SRC_DIR}/Buffers/UniformBuffer.h)

set(GAME_SRC
    ${PROJECT_SRC_DIR}/Game/Entity.cpp
    ${PROJECT_SRC_DIR}/Game/Entity.h
    ${PROJECT_SRC_DIR}/Game/Game.cpp
    ${PROJECT_SRC_DIR}/Game/Game.h
    ${PROJECT_SRC_DIR}/Game/Player.cpp
    ${PROJECT_SRC_DIR}/Game/Player.h)

set(GUI_SRC
    ${PROJECT_SRC_DIR}/GUI/imgui_demo.cpp
    ${PROJECT_SRC_DIR}/GUI/imgui_draw.cpp
    ${PROJECT_SRC_DIR}/GUI/imgui_impl_glfw.cpp
    ${PROJECT_SRC_DIR}/GUI/imgui_impl_opengl3.cpp
    ${PROJECT_SRC_DIR}/GUI/imgui_tables.cpp
    ${PROJECT_SRC_DIR}/GUI/imgui_widgets.cpp
    ${PROJECT_SRC_DIR}/GUI/imgui.cpp)

set(RENDERER_SRC
    ${PROJECT_SRC_DIR}/Renderer/Camera.cpp
    ${PROJECT_SRC_DIR}/Renderer/Camera.h
    ${PROJECT_SRC_DIR}/Renderer/Mesh.cpp
    ${PROJECT_SRC_DIR}/Renderer/Mesh.h
    ${PROJECT_SRC_DIR}/Renderer/Model.cpp
    ${PROJECT_SRC_DIR}/Renderer/Model.h
    ${PROJECT_SRC_DIR}/Renderer/Renderer.cpp
    ${PROJECT_SRC_DIR}/Renderer/Renderer.h
    ${PROJECT_SRC_DIR}/Renderer/Shader.cpp
    ${PROJECT_SRC_DIR}/Renderer/Shader.h
    ${PROJECT_SRC_DIR}/Renderer/Skybox.cpp
    ${PROJECT_SRC_DIR}/Renderer/Skybox.h)

set(TERRAIN_SRC
    ${PROJECT_SRC_DIR}/Terrain/NoiseGenerator.cpp
    ${PROJECT_SRC_DIR}/Terrain/NoiseGenerator.h
    ${PROJECT_SRC_DIR}/Terrain/Terrain.cpp
    ${PROJECT_SRC_DIR}/Terrain/Terrain.h
    ${PROJECT_SRC_DIR}/Terrain/TerrainGenerator.cpp
    ${PROJECT_SRC_DIR}/Terrain/TerrainGenerator.h)

set(TYPES_SRC
    ${PROJECT_SRC_DIR}/Types/AABB.h
    ${PROJECT_SRC_DIR}/Types/EMovement.h
    ${PROJECT_SRC_DIR}/Types/EShader.h
    ${PROJECT_SRC_DIR}/Types/ESkybox.h
    ${PROJECT_SRC_DIR}/Types/ETexture.h
    ${PROJECT_SRC_DIR}/Types/FWindow.h)

set(WORLD_SRC
    ${PROJECT_SRC_DIR}/World/GameWorld.cpp
    ${PROJECT_SRC_DIR}/World/GameWorld.h
    ${PROJECT_SRC_DIR}/World/GObject.cpp
    ${PROJECT_SRC_DIR}/World/GObject.h
    ${PROJECT_SRC_DIR}/World/QuadTree.cpp
    ${PROJECT_SRC_DIR}/World/QuadTree.h
    ${PROJECT_SRC_DIR}/World/TerrainElement.cpp
    ${PROJECT_SRC_DIR}/World/TerrainElement.h)

# file(GLOB_RECURSE INCLUDE_SRC CONFIGURE_DEPENDS ${PROJECT_INCLUDE_DIR}/*)

set(SRC
    ${APPLICATION_SRC}
    ${BUFFERS_SRC}
    ${GAME_SRC}
    ${GUI_SRC}
    ${RENDERER_SRC}
    ${TERRAIN_SRC}
    ${TYPES_SRC}
    ${WORLD_SRC}
    ${INCLUDE_SRC}
    ${PROJECT_SRC_DIR}/Main.cpp)

add_executable(gold-rush ${SRC})

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

target_include_directories(gold-rush PUBLIC include src)
target_link_directories(gold-rush PUBLIC libraries)
target_link_libraries(gold-rush OpenGL::GL assimp glfw GLEW ${CMAKE_DL_LIBS})
