include("../../libcogra/buildutils/CreateApp.cmake")
project(DeCasteljau)
set(ContribLibraries GLEW GLFW GLM IMGUI COGRA)
CreateApp(ContribLibraries)
