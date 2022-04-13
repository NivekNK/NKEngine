project("Sandbox")

add_executable(Sandbox src/SandboxApp.cpp)

target_compile_definitions(Sandbox PRIVATE
    $<$<CONFIG:Debug>:NK_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:NK_RELEASE>
    $<$<CONFIG:Release>:NK_DIST>
)

if (WIN32)
    target_compile_definitions(Sandbox PRIVATE
        NK_PLATFORM_WINDOWS
    )
endif()

target_link_libraries(Sandbox PRIVATE HazelNK)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sandbox)
