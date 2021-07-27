cmake_minimum_required(VERSION 3.5)

#http://mariobadr.com/creating-a-header-only-library-with-cmake.html
#https://dominikberner.ch/cmake-interface-lib/

project(compile_time_hash
        VERSION 1.0
        DESCRIPTION "This library has implementations for MD5, SHA1 and all SHA2 versions. They run at compile time so the binary doesn't contain the original string. They can also be used at runtime."
        HOMEPAGE_URL "https://github.com/abbbaf/Compile-time-hash-functions")

include(GNUInstallDirs)

add_library(compile_time_hash INTERFACE
        array.hpp
        circular_queue.hpp
        compile_time_hash.hpp
        compile_time_md5.hpp
        compile_time_sha1.hpp
        compile_time_sha224.hpp
        compile_time_sha224_256.hpp
        compile_time_sha256.hpp
        compile_time_sha384.hpp
        compile_time_sha384_512.hpp
        compile_time_sha512.hpp
        compile_time_sha512_224.hpp
        compile_time_sha512_256.hpp
        crypto_hash.hpp
        padded_value.hpp
        section.hpp
        utils.hpp)

target_include_directories(compile_time_hash INTERFACE
        $<BUILD_INTERFACE:${compile_time_hash_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_compile_features(compile_time_hash INTERFACE cxx_std_11)

install(TARGETS compile_time_hash
        EXPORT compile_time_hash_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

include(CMakePackageConfigHelpers)
write_basic_package_version_file("compile_time_hash_ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)

configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/compile_time_hash_config.cmake.in"
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION
        ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(EXPORT ${PROJECT_NAME}_Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(DIRECTORY ${PROJECT_SOURCE_DIR}
        DESTINATION include
        FILES_MATCHING PATTERN "*.hpp")

