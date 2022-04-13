project("HazelNK")

add_compile_definitions(
    $<$<CONFIG:Debug>:NK_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:NK_RELEASE>
    $<$<CONFIG:Release>:NK_DIST>
    NK_BUILD_DLL
)

if (WIN32)
    add_compile_definitions(
        NK_PLATFORM_WINDOWS
    )
endif()

file(GLOB SOURCES CMAKE_CONFIGURE_DEPENDS
    "src/*.h"
    "src/*.cpp"
)

add_library(HazelNK SHARED ${SOURCES})

target_include_directories(HazelNK
PUBLIC
    src
)
