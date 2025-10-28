
add_executable (BST "main.cpp" "Headers/View/Application.h" "Source/View/Application.cpp" "Headers/Model/Interfaces/IComparable.h" "Headers/Model/PCRDatabase/Models/PCRTest.h" "Headers/Model/PCRDatabase/Models/Person.h" "Headers/Model/PCRDatabase/Database.h" "Headers/Model/Structures/AVL/AVLNode.h" "Headers/Model/Structures/BST/BSTNode.h" "Headers/Model/Structures/BST/BinarySearchTree.h" "Headers/Model/Structures/AVL/AVLTree.h" "Headers/Model/Tests/TestObjects/Number.h" "Headers/Model/Tests/FunctionalityTester.h" "Headers/Model/Tests/SpeedTester.h" "Source/Model/PCRDatabase/Models/PCRTest.cpp" "Source/Model/PCRDatabase/Models/Person.cpp" "Source/Model/Tests/TestObjects/Number.cpp" "Source/Model/Tests/FunctionalityTester.cpp" "Source/Model/Tests/SpeedTester.cpp" "Headers/View/UI/FunctionalityTestWindow.h" "Source/View/UI/FunctionalityTestWindow.cpp" "Headers/View/UI/Window.h" "Headers/Presenter/Commands/Command.h" "Headers/Presenter/Commands/Tests/FunctionalityTestCommand.h")

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