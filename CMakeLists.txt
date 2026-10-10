cmake_minimum_required(VERSION 3.20)
project(msdos_cpp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(msdos_core
    src/machine.cpp
    src/fcb_host.cpp
    src/fat12.cpp
    generated/v1.25/command_scan.cpp
    generated/v1.25/command_numbers.cpp
    generated/v1.25/hex2bin.cpp
    generated/v1.25/trans.cpp
    generated/v1.25/fat_pack.cpp
    generated/v1.25/fcb_records.cpp
    generated/v1.25/setvect.cpp
    generated/v2.0/dsum.cpp
    generated/v4.0/mapper/getver.cpp
    generated/v4.0/mapper/qverify.cpp
    generated/v4.0/mapper/sverify.cpp
    generated/v4.0/mapper/mkdir.cpp
    generated/v4.0/mapper/rmdir.cpp
    generated/v4.0/mapper/chdir.cpp
    generated/v4.0/mapper/delete.cpp
    generated/v4.0/mapper/close.cpp
    generated/v4.0/mapper/read.cpp
    generated/v4.0/mapper/write.cpp
    generated/v4.0/mapper/lseek.cpp
    generated/v4.0/mapper/sel_disk.cpp
    generated/v4.0/mapper/qfilemod.cpp
    generated/v4.0/mapper/sfilemod.cpp
    generated/v4.0/mapper/qhandtyp.cpp
    generated/v4.0/mapper/freeseg.cpp
    generated/v4.0/mapper/allocseg.cpp
    generated/v4.0/mapper/reallseg.cpp
    generated/v4.0/mapper/get_tod.cpp
    generated/v4.0/mapper/set_tod.cpp
    generated/v4.0/mapper/putmsg.cpp
    generated/v4.0/mapper/ioctl.cpp
    generated/v4.0/mapper/exit.cpp
    generated/v4.0/mapper/scurpos.cpp
    generated/v4.0/mapper/scrollup.cpp
    generated/v4.0/mapper/dbcs.cpp
    generated/v4.0/mapper/casemap.cpp
    generated/v4.0/mapper/devconfg.cpp
    generated/v4.0/mapper/sfileinf.cpp
    generated/v4.0/mapper/wchstra.cpp)
target_include_directories(msdos_core PUBLIC include generated)
if(MSVC)
    add_compile_options(/W4 /permissive- /utf-8 /wd4702 /wd4267 /wd4244 /EHsc /Zc:__cplusplus)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wconversion)
endif()

add_executable(hex2bin src/hex2bin_main.cpp)
target_link_libraries(hex2bin PRIVATE msdos_core)

add_executable(trans src/trans_main.cpp)
target_link_libraries(trans PRIVATE msdos_core)

include(CTest)
if(BUILD_TESTING)
    add_executable(msdos_tests tests/core_tests.cpp)
    target_link_libraries(msdos_tests PRIVATE msdos_core)
    add_test(NAME core COMMAND msdos_tests)
    add_executable(msdos_v4_tests tests/v4_mapper_tests.cpp)
    target_link_libraries(msdos_v4_tests PRIVATE msdos_core)
    add_test(NAME v4_mapper COMMAND msdos_v4_tests)
    find_program(NODE_EXECUTABLE node)
    if(NODE_EXECUTABLE)
        add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/native_flow_fixture.cpp"
            COMMAND ${NODE_EXECUTABLE} tests/native_flow_fixtures.mjs "${CMAKE_CURRENT_BINARY_DIR}/native_flow_fixture.cpp"
            DEPENDS tests/native_flow_fixtures.mjs tools/convert.mjs
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
        add_executable(msdos_native_flow_tests tests/native_flow_tests.cpp "${CMAKE_CURRENT_BINARY_DIR}/native_flow_fixture.cpp")
        target_link_libraries(msdos_native_flow_tests PRIVATE msdos_core)
        add_test(NAME native_control_flow COMMAND msdos_native_flow_tests)
        add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/boot/test-fixture.img"
            COMMAND ${NODE_EXECUTABLE} tools/boot-fixture.mjs "${CMAKE_CURRENT_BINARY_DIR}/boot/test-fixture.img"
            DEPENDS tools/boot-fixture.mjs tools/build-boot.mjs
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
        add_custom_target(boot_test_fixture DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/boot/test-fixture.img")
        add_executable(msdos_boot_tests tests/boot_kernel_tests.cpp)
        add_dependencies(msdos_boot_tests boot_test_fixture)
        add_test(NAME boot_kernel COMMAND msdos_boot_tests "${CMAKE_CURRENT_BINARY_DIR}/boot/test-fixture.img")
        add_test(NAME boot_image_layout COMMAND ${NODE_EXECUTABLE} --test tests/boot_image.test.mjs)
        set_tests_properties(boot_image_layout PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
        add_test(NAME conversion COMMAND ${NODE_EXECUTABLE} --test tests/converter.test.mjs tests/v4_audit_resources.test.mjs)
        set_tests_properties(conversion PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
        add_test(NAME hex2bin_cli COMMAND ${NODE_EXECUTABLE} --test tests/hex2bin.test.mjs)
        set_tests_properties(hex2bin_cli PROPERTIES
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            ENVIRONMENT "HEX2BIN_EXE=$<TARGET_FILE:hex2bin>")
        add_test(NAME trans_cli COMMAND ${NODE_EXECUTABLE} --test tests/trans.test.mjs)
        set_tests_properties(trans_cli PROPERTIES
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            ENVIRONMENT "TRANS_EXE=$<TARGET_FILE:trans>")
    endif()
endif()

find_program(NODE_EXECUTABLE node)
if(NODE_EXECUTABLE)
    add_custom_target(boot_image
        COMMAND ${NODE_EXECUTABLE} tools/build-boot.mjs
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMENT "Building the freestanding C++ kernel and FAT12 boot image")
    add_custom_target(regenerate
        COMMAND ${NODE_EXECUTABLE} tools/convert.mjs generate
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMENT "Regenerating C++ from the original assembly")
    add_custom_target(conversion_audit
        COMMAND ${NODE_EXECUTABLE} tools/convert.mjs audit
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
    if(TARGET msdos_v4_tests)
        add_custom_target(v4_audit
            COMMAND ${NODE_EXECUTABLE} tools/audit-v4.mjs --validate --test-executable $<TARGET_FILE:msdos_v4_tests>
            DEPENDS msdos_v4_tests
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            COMMENT "Auditing every DOS 4.0 source file and validating translated mapper modules")
    else()
        add_custom_target(v4_audit
            COMMAND ${NODE_EXECUTABLE} tools/audit-v4.mjs
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            COMMENT "Auditing every DOS 4.0 source file without runtime validation")
    endif()
    add_custom_target(boot_smoke_v86
        COMMAND ${NODE_EXECUTABLE} tools/boot-smoke-v86.mjs
        DEPENDS boot_image
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMENT "Booting the disk in v86 and testing native DOS programs")
    add_custom_target(boot_smoke_virtualbox
        COMMAND ${NODE_EXECUTABLE} tools/boot-smoke-vbox.mjs
        DEPENDS boot_image
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMENT "Booting the disk in the installed VirtualBox and testing DOS programs")
endif()
