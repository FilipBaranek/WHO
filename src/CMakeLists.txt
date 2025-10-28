
add_executable (BST "main.cpp")

target_include_directories(BST PRIVATE ${glm_SOURCE_DIR})

target_link_libraries(BST PRIVATE
    glfw
    glad
    glm
    imgui_backend
)

if (WIN32)
    target_link_libraries(BST PRIVATE opengl32)
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
	set_property(TARGET BST PROPERTY CXX_STANDARD 20)
endif()