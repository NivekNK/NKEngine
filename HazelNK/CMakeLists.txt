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

add_subdirectory(vendor/fmt)
add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/eventpp)

file(GLOB SOURCES CMAKE_CONFIGURE_DEPENDS 
    "src/*.h"
    "src/*.cpp"
    "src/Hazel/*.h"
	"src/Hazel/*.cpp"
    "src/Hazel/Events/*.h"
    "src/Hazel/Events/*.cpp"
)

add_library(HazelNK SHARED ${SOURCES})

target_link_libraries(HazelNK PUBLIC
	fmt::fmt
    spdlog::spdlog_header_only
)

target_include_directories(HazelNK
PUBLIC
    src
    src/Hazel
    
	# Libraries
	vendor/fmt/include
    vendor/spdlog/include
	vendor/eventpp/include
)

target_include_directories(spdlog
PRIVATE
    vendor/fmt/include
)
