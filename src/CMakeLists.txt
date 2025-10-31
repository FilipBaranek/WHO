
add_executable (
    BST
    "main.cpp"
    "Headers/View/Application.h"
    "Source/View/Application.cpp"
    "Headers/Model/Interfaces/IComparable.h"
    "Headers/Model/PCRDatabase/Models/PCRTest.h"
    "Headers/Model/PCRDatabase/Models/Person.h"
    "Headers/Model/PCRDatabase/Database.h" 
    "Headers/Model/Structures/AVL/AVLNode.h" 
    "Headers/Model/Structures/BST/BSTNode.h" 
    "Headers/Model/Structures/BST/BinarySearchTree.h" 
    "Headers/Model/Structures/AVL/AVLTree.h" 
    "Headers/Model/Tests/TestObjects/Number.h"
    "Headers/Model/Tests/FunctionalityTester.h" 
    "Headers/Model/Tests/SpeedTester.h"   
    "Source/Model/Tests/TestObjects/Number.cpp" 
    "Source/Model/Tests/FunctionalityTester.cpp" 
    "Source/Model/Tests/SpeedTester.cpp" 
      
    "Headers/Presenter/Commands/Command.h" 
    
    "Source/Model/PCRDatabase/Database.cpp"
    "Headers/Model/PCRDatabase/ModelWrappers/TestWrapper.h" 
    "Headers/Model/PCRDatabase/ModelWrappers/TestInDateOrderWrapper.h"
    "Headers/Model/PCRDatabase/ModelWrappers/PositiveTestInDistrictOrderWrapper.h"
    "Headers/Model/PCRDatabase/ModelWrappers/NegativeTestInDistrictOrderWrapper.h"
    "Source/Model/PCRDatabase/ModelWrappers/TestWrapper.cpp"
    "Headers/Model/PCRDatabase/Generator/RandomDataGenerator.h" 
    "Source/Model/PCRDatabase/Generator/RandomDataGenerator.cpp" 
    "Headers/Model/PCRDatabase/ModelWrappers/PersonWrapper.h" 
    "Source/Model/PCRDatabase/ModelWrappers/PersonWrapper.cpp"
    "Source/Presenter/Presenter.cpp"
    "Headers/Presenter/Presenter.h"
    "Headers/Presenter/Commands/FindTestResultCommand.h"
    "Source/Presenter/Commands/FindTestResultCommand.cpp"
    "Headers/Presenter/Commands/InsertCommand.h"
    "Source/Presenter/Commands/InsertCommand.cpp" "Source/View/UI/OutputWindow.cpp" "Headers/View/UI/OutputWindow.h" "Headers/View/UI/Window.h"  "Headers/View/UI/PersonWindow.h" "Source/View/UI/PersonWindow.cpp" "Headers/View/UI/TestWindow.h" "Source/View/UI/TestWindow.cpp" "Headers/View/UI/GeneratorWindow.h" "Headers/View/UI/OperationsWindow.h" "Headers/View/UI/FileWindow.h" "Source/View/UI/GeneratorWindow.cpp" "Source/View/UI/FileWindow.cpp" "Source/View/UI/OperationsWindow.cpp")

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