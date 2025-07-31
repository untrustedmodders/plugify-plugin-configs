include(FetchContent)

message(STATUS "Pulling and configuring glaze")

if (CMAKE_CXX_STANDARD GREATER 20)
    set(GLAZE_TAG "v5.5.4" CACHE STRING "")
else()
    set(GLAZE_TAG "v2.9.5" CACHE STRING "")
endif()

FetchContent_Declare(
        glaze
        GIT_REPOSITORY https://github.com/stephenberry/glaze.git
        GIT_TAG ${GLAZE_TAG}
)

FetchContent_MakeAvailable(glaze)