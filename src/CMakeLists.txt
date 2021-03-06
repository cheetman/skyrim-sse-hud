set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")

set(SOURCE_DIR "${ROOT_DIR}/src")
set(SOURCE_FILES
	"${SOURCE_DIR}/main.h"
	"${SOURCE_DIR}/papyrus.cpp"
	"${SOURCE_DIR}/main.cpp"
	"${SOURCE_DIR}/PCH.h"
	"${SOURCE_DIR}/sse-imgui/cimgui.h"
	"${SOURCE_DIR}/sse-imgui/imgui_wrapped.h"
	"${SOURCE_DIR}/sse-imgui/platform.h"
	"${SOURCE_DIR}/sse-imgui/sse-imgui.h"
	"${SOURCE_DIR}/event/BSTMenuEvent.h"
	"${SOURCE_DIR}/event/BSTScriptEvent.h"
	"${SOURCE_DIR}/event/BSTCrosshairRefEvent.h"
	"${SOURCE_DIR}/event/BSTCrosshairRefEvent.cpp"
	
	"${SOURCE_DIR}/hook/hudhook.h"
	"${SOURCE_DIR}/hook/hudhook.cpp"
	"${SOURCE_DIR}/utils/NameUtil.h"
	"${SOURCE_DIR}/utils/NameUtil.cpp"
	"${SOURCE_DIR}/utils/PlayerDataProvider.h"
	"${SOURCE_DIR}/utils/utils.h"
	"${SOURCE_DIR}/utils/GeneralUtil.h"
	"${SOURCE_DIR}/utils/GeneralUtil.cpp"
	"${SOURCE_DIR}/nlohmann/adl_serializer.hpp"
	"${SOURCE_DIR}/nlohmann/json.hpp"
	"${SOURCE_DIR}/nlohmann/json_fwd.hpp"
	"${SOURCE_DIR}/nlohmann/detail/exceptions.hpp"
	"${SOURCE_DIR}/nlohmann/detail/json_pointer.hpp"
	"${SOURCE_DIR}/nlohmann/detail/json_ref.hpp"
	"${SOURCE_DIR}/nlohmann/detail/macro_scope.hpp"
	"${SOURCE_DIR}/nlohmann/detail/macro_unscope.hpp"
	"${SOURCE_DIR}/nlohmann/detail/value_t.hpp"
	"${SOURCE_DIR}/nlohmann/detail/conversions/from_json.hpp"
	"${SOURCE_DIR}/nlohmann/detail/conversions/to_chars.hpp"
	"${SOURCE_DIR}/nlohmann/detail/conversions/to_json.hpp"
	"${SOURCE_DIR}/nlohmann/detail/input/binary_reader.hpp"
	"${SOURCE_DIR}/nlohmann/detail/input/input_adapters.hpp"
	"${SOURCE_DIR}/nlohmann/detail/input/json_sax.hpp"
	"${SOURCE_DIR}/nlohmann/detail/input/lexer.hpp"
	"${SOURCE_DIR}/nlohmann/detail/input/parser.hpp" 
	"${SOURCE_DIR}/nlohmann/detail/input/position_t.hpp" 
	"${SOURCE_DIR}/nlohmann/detail/iterators/internal_iterator.hpp" 
	"${SOURCE_DIR}/nlohmann/detail/iterators/iter_impl.hpp" 
	"${SOURCE_DIR}/nlohmann/detail/iterators/iteration_proxy.hpp" 
	"${SOURCE_DIR}/nlohmann/detail/iterators/iterator_traits.hpp" 
	"${SOURCE_DIR}/nlohmann/detail/iterators/json_reverse_iterator.hpp" 
	"${SOURCE_DIR}/nlohmann/detail/iterators/primitive_iterator.hpp" 
	"${SOURCE_DIR}/nlohmann/detail/meta/cpp_future.hpp" 
	"${SOURCE_DIR}/nlohmann/detail/meta/detected.hpp" 
	"${SOURCE_DIR}/nlohmann/detail/meta/is_sax.hpp" 
	"${SOURCE_DIR}/nlohmann/detail/meta/type_traits.hpp" 
	"${SOURCE_DIR}/nlohmann/detail/meta/void_t.hpp" 
	"${SOURCE_DIR}/nlohmann/detail/output/binary_writer.hpp" 
	"${SOURCE_DIR}/nlohmann/detail/output/output_adapters.hpp" 
	"${SOURCE_DIR}/nlohmann/detail/output/serializer.hpp" 
)

source_group(TREE "${ROOT_DIR}" FILES ${SOURCE_FILES})

set(VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/Plugin.h")
configure_file(
	"${ROOT_DIR}/cmake/Plugin.h.in"
	"${VERSION_HEADER}"
	@ONLY
)

source_group("src" FILES "${VERSION_HEADER}")

configure_file(
	"${ROOT_DIR}/cmake/version.rc.in"
	"${CMAKE_CURRENT_BINARY_DIR}/version.rc"
	@ONLY
)

add_library(
	"${PROJECT_NAME}"
	SHARED
	${SOURCE_FILES}
	"${VERSION_HEADER}"
	"${CMAKE_CURRENT_BINARY_DIR}/version.rc"
	"${ROOT_DIR}/.clang-format"
	"${ROOT_DIR}/.editorconfig"
)

target_compile_features(
	"${PROJECT_NAME}"
	PRIVATE
		cxx_std_20
)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	target_compile_options(
		"${PROJECT_NAME}"
		PRIVATE
			"/sdl"	# Enable Additional Security Checks
			"/utf-8"	# Set Source and Executable character sets to UTF-8
			"/Zi"	# Debug Information Format

			"/permissive-"	# Standards conformance
			"/Zc:preprocessor"	# Enable preprocessor conformance mode

			"/wd4200" # nonstandard extension used : zero-sized array in struct/union

			"$<$<CONFIG:DEBUG>:>"
			"$<$<CONFIG:RELEASE>:/Zc:inline;/JMC-;/Ob3>"
	)

	target_link_options(
		"${PROJECT_NAME}"
		PRIVATE
			"$<$<CONFIG:DEBUG>:/INCREMENTAL;/OPT:NOREF;/OPT:NOICF>"
			"$<$<CONFIG:RELEASE>:/INCREMENTAL:NO;/OPT:REF;/OPT:ICF;/DEBUG:FULL>"
	)
endif()

target_include_directories(
	"${PROJECT_NAME}"
	PRIVATE
		"${CMAKE_CURRENT_BINARY_DIR}/src"
		"${SOURCE_DIR}"
)

add_subdirectory("${ROOT_DIR}/external/CommonLibSSE" CommonLibSSE)

target_link_libraries(
	"${PROJECT_NAME}"
	PRIVATE
		CommonLibSSE::CommonLibSSE
)

target_precompile_headers(
	"${PROJECT_NAME}"
	PRIVATE
		"${SOURCE_DIR}/PCH.h"
)

#追加 20220429
target_compile_definitions(
	"${PROJECT_NAME}"
	PUBLIC
		_CRT_SECURE_NO_WARNINGS
)
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX-)
else(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif(MSVC)


if("${COPY_OUTPUT}")
	add_custom_command(
		TARGET "${PROJECT_NAME}"
		POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" "${Skyrim64Path}/Data/SKSE/Plugins/"
		COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_PDB_FILE:${PROJECT_NAME}>" "${Skyrim64Path}/Data/SKSE/Plugins/"
		VERBATIM
	)
endif()
