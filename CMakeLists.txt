cmake_minimum_required(VERSION 3.1)
project(gaspar_app LANGUAGES CXX)

# ===========================
# ---   Meta Debugging    ---
# ===========================
# For verbosity
#set(CMAKE_VERBOSE_MAKEFILE ON)
#add_compile_options("--verbose -Wl,--verbose")
#set(CMAKE_TOOLCHAIN_FILE "arm-gcc-toolchain.cmake")



# ===========================
# --- Utilities Functions ---
# ===========================
# Function - finds immediate subdirectories
function(subdirlist result searchdir)
  set(dirlist "")
  # Finds files
  file(GLOB children ${searchdir} "${searchdir}/*")
  # Filter files for subdirectories
  foreach(child IN LISTS children)
    if(IS_DIRECTORY ${child} AND NOT child STREQUAL ${searchdir})
      # Append files to result
      list(APPEND dirlist "${child}")
    endif()
  endforeach()
  # Returns the value inside specified variable in parent scope
  set("${result}" ${dirlist} PARENT_SCOPE)
endfunction()

# Handful macro for filtering folders of files
macro(find_qt_able_files argdir)
  set(dir ${argdir})
  message(STATUS "Searching directory for dependencies of qt-moc: " ${argdir})
  file(GLOB_RECURSE cpps FOLLOW_SYMLINKS ${dir}/*.c[p]*)
  list(APPEND sources ${cpps})
  file(GLOB_RECURSE hpps FOLLOW_SYMLINKS ${dir}/*.h[p]*)
  list(APPEND headers ${hpps})
  file(GLOB_RECURSE uipps FOLLOW_SYMLINKS ${dir}/*.ui)
  list(APPEND forms ${uipps})
  file(GLOB_RECURSE respps FOLLOW_SYMLINKS ${dir}/*.qrc)
  list(APPEND resources ${respps})
endmacro()

macro(filter_crazy_paths pathslist)
  list(FILTER ${pathslist} INCLUDE
       REGEX "/[a-zA-Z0-9-_.]+\\.[a-zA-Z0-9-_.]*")
endmacro()



# ===========================
# ---     Toolchains      ---
# ===========================
# Before declaring project and checking compilers,
# we need to know which compiler use and which libraries/toolkits
# Default build is for desktop
if(NOT DEFINED toolchain)
  set(toolchain "desktop")
endif()

if(UNIX)
  set(ndk_home "$ENV{HOME}/android-ndk-r17/android-ndk-r17")
  set(qt_home "$ENV{HOME}/Qt/5.12.0")
  set(platform_version "android-28")
endif()

if(WIN32)
  set(ndk_home "C:/android-ndk-r17/android-ndk-r17")
  set(qt_home "C:/Qt/5.12.0")
  set(platform_version "android-28")
endif()

if(NOT toolchain MATCHES "desktop")
  set(platform_home "${ndk_home}/platforms/${platform_version}")
endif()

if(toolchain MATCHES "ARM")
  set(ndk_arch "arch-arm")
  set(qt_arch "android_armv7")
  set(QT_MAKE_EXECUTABLE "${qt_home}/${qt_arch}/bin/qmake")
elseif(toolchain MATCHES "X86")
  set(ndk_arch "arch-x86")
  set(qt_arch "android_x86")
  set(QT_MAKE_EXECUTABLE "${qt_home}/${qt_arch}/bin/qmake")
elseif(toolchain MATCHES "desktop")
  if(UNIX)
    set(qt_arch "gcc_64")
  elseif(WIN32 AND MSVC)
    set(qt_arch "msvc2015")
  elseif(WIN32) # Assumes MinGW
    set(qt_arch "mingw49_32")
  endif()

  set(ndk_arch "arch-x86_64")
  set(QT_MAKE_EXECUTABLE "${qt_home}/${qt_arch}/bin/qmake")
  list(APPEND CMAKE_PREFIX_PATH "${qt_home}/${qt_arch}")
endif()

if(NOT toolchain MATCHES "desktop")
  list(APPEND CMAKE_PREFIX_PATH "${qt_home}/${qt_arch}")
  set(CMAKE_SYSROOT "${platform_home}/${ndk_arch}")

  if(NOT toolchain MATCHES ".*64$")
    list(APPEND CMAKE_PREFIX_PATH "${platform_home}/usr/lib")
  else()
    list(APPEND CMAKE_PREFIX_PATH "${platform_home}/usr/lib64")
  endif()
endif()



# ===========================
# --- Qt Modules Config.  ---
# ===========================
set(QtModulesRootDir "${qt_home}/${qt_arch}/lib/cmake")
list(APPEND CMAKE_FRAMEWORK_PATH ${QtModulesRootDir})
message(STATUS "Toolchain:    ${toolchain}")
message(STATUS "Qt Home:      ${qt_home}/${qt_arch}")
message(STATUS "QtModulesDir: ${QtModulesRootDir}")
message(STATUS "Build type:   ${CMAKE_BUILD_TYPE}")




# ===========================
# ---       Project       ---
# ===========================
#list(APPEND CMAKE_CXX_FLAGS "-v -Wl,--verbose --sysroot=/home/superwolf/android-ndk-r17/android-ndk-r17/platforms/android-28/arch-arm/")
#list(APPEND CMAKE_CXX_LINK_FLAGS "--verbose")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Specifies standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# LIFE CAN be easir (or harder) with this
# We only have to manually do AUTOUIC's job
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Find the Qt Core libraries
find_package(Qt5 REQUIRED COMPONENTS Core Quick Widgets Sql Test CONFIG)

# Common prefix
set(THIRDPARTY_FOLDER "${PROJECT_SOURCE_DIR}/src/3rdparty")

# Common files categories/manipulations
set(sources "")
set(headers "")
set(forms "")
set(resources "")
set(includepath "${THIRDPARTY_FOLDER}")

# Adding common project folders
# --- src/code/*/ ---
subdirlist(src__code__subdirs "${PROJECT_SOURCE_DIR}/src/code")
foreach(subdir IN LISTS src__code__subdirs)
  find_qt_able_files(${subdir})
endforeach()
# --- src/gui/*    ---
find_qt_able_files("${PROJECT_SOURCE_DIR}/src/gui")
# --- src/res/*    ---
find_qt_able_files("${PROJECT_SOURCE_DIR}/src/res")

# Tell CMake to create the main executables
add_executable(unit_tests src/code/main_test.cpp)
add_executable(gaspar_app WIN32 src/code/main.cpp)

# Add modules from Qt 5
target_link_libraries(gaspar_app PUBLIC Qt5::Core Qt5::Quick Qt5::Widgets Qt5::Sql)
target_link_libraries(unit_tests PUBLIC Qt5::Core Qt5::Quick Qt5::Widgets Qt5::Sql Qt5::Test)

# Base include path
list(APPEND includepath ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/src/code)

# Adds custom module qt-frameless-window-darkstyle
list(APPEND includepath
  "${THIRDPARTY_FOLDER}/qt-frameless-window-darkstyle/framelesswindow")
find_qt_able_files(
  "${THIRDPARTY_FOLDER}/qt-frameless-window-darkstyle")
# Remove qt-frameless-window-darkstyle main.cpp
list(FILTER sources EXCLUDE REGEX "main\\.cpp")

# Clean any garbage the filesystem throw at us
filter_crazy_paths(headers)
filter_crazy_paths(sources)
filter_crazy_paths(forms)
filter_crazy_paths(resources)

# Do a final mangling of qt files
#qt5_wrap_cpp(headers_moc ${headers} ${sources})
#qt5_add_resources(resources_moc ${resources})
set(forms_moc "")
# Specify ui header file output for ui files
foreach(form IN LISTS forms)
  set(form_moc "")
  get_filename_component(form_path ${form} DIRECTORY)
  get_filename_component(form_name ${form} NAME)
  # See -- https://stackoverflow.com/questions/30049180
  string(REGEX REPLACE "\\.[^.]*$" "" form_basename ${form_name})
  qt5_wrap_ui("${form_path}/ui_${form_basename}.h" ${form})
  list(APPEND forms_moc ${form_moc})
endforeach()

set(dependencies
  ${headers_moc} ${forms_moc} ${resources_moc}
  ${headers} ${sources} ${forms} ${resources}
  )

# Output found files/variables
message(STATUS "The following dependencies were found:")
set(dependencies_sorted_alpha ${dependencies})
list(SORT dependencies_sorted_alpha)
foreach(src IN LISTS dependencies_sorted_alpha)
  message(STATUS "    ${src}")
endforeach()
message(STATUS "The following paths were found for include'ing:")
foreach(path IN LISTS includepath)
  message(STATUS "    ${path}")
endforeach()

# Add files to executables
set(dependencies__no_test ${dependencies})
list(FILTER dependencies__no_test EXCLUDE REGEX "test")
target_sources(gaspar_app PUBLIC
  ${dependencies__no_test}
  )


target_sources(unit_tests PUBLIC
  ${dependencies}
  )

target_include_directories(gaspar_app PUBLIC ${includepath})
target_include_directories(unit_tests PUBLIC ${includepath})


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)

## Dependencies by build type/compiler
set(compiler_options "")
set(linker_options "")

# In order to use clang (llvm), call:
# CC=/usr/bin/clang CXX=/usr/bin/clang++ cmake -H. -Bbuild -DCMAKE_TOOLCHAIN_PREFIX=llvm-
# In order to see triples for the option --target in cross-compilation, use
# llc --version
# and:
# llc -march=arm -mattr=help
# In order to see architectures and subsystem availables

# In order to use mingw32, call:
# cmake -H. -Bbuild -G "MinGW Makefiles"

# In order to use MSVC 2017 targeting WinXp (recommended use of Win10 OS)
# cmake -T v141_xp path/to/src
# msbuild host.vcxproj /p:Configuration=Release
# As stated in https://stackoverflow.com/questions/33188600

if (CMAKE_BUILD_TYPE MATCHES Debug)
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    list(APPEND compiler_options
      -g # Use debbugging symbols
      -Og # Optimize somewhat
      -flto # Use link-time optimization (LTO)
      -march=native # Optmize opcodes for non-portability
      )
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
      list(APPEND compiler_options
        -Wl,-O1 # If possible, tell the linker to use LTO
      )
      list(APPEND linker_options
        -g -Og # Debugging symbols
        -flto -Wl,-O1 # Again, activates LTO
        )
    endif()
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
      list(APPEND linker_options
        -g -O1 # Debugging symbols
        -flto -Wl,-O1 # Again, activates LTO
        )
    endif()
  endif()
endif()

# This app will be deployed in the Portable Executable format,
# therefore it needs to be restricted to that Instruction Set
# (i.e. IA-32 (i386), the first x86 instruction set). However,
# it can be optmized to newer incarnations of that ISA.
if (CMAKE_BUILD_TYPE MATCHES Rel)
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    list(APPEND compiler_options
      -s # Strip symbols during compilation/linking
      -flto # Use link-time optimization (LTO)
      -march=i386 -mtune=generic # Gerate win32 PE (.exe)
      -Wl,-O1 # If possible, tell the linker to use LTO
      )
    list(APPEND linker_options
      -s # Strip all symbols
      -flto -Wl,-O1 # Again, activates LTO
      )
  endif()
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    list(APPEND compiler_options
      -march=i386 -mtune=generic # Gerate win32 PE (.exe)
      )
  endif()
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    list(APPEND compiler_options
      # Will attempt to generate ABI compatible with MSVC
      --target=x86i386-win32 -mtune=generic # Gerate win32 PE (.exe)
      )
  endif()
endif()

if (CMAKE_BUILD_TYPE MATCHES Release)
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    list(APPEND compiler_options
      -O3 # Optimize aggressively
      )
    list(APPEND linker_options
      -O3 # Optimize it!
      )
  endif()
endif()

if (CMAKE_BUILD_TYPE MATCHES MinSizeRel)
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    list(APPEND compiler_options
      -Os # Optimize for size
      )
    list(APPEND linker_options
      -Os
      )
  endif()
endif()


# Universal CRT dlls/dependencies are a pain in the ass on WinXp
# so static link them
# see https://stackoverflow.com/questions/35805113
if(MSVC)
  if(CMAKE_BUILD_TYPE MATCHES Rel)
    #list(APPEND compiler_options "/MT")
    #target_link_libraries(gaspar_app STATIC mincore_downlevel.lib)
    #target_link_libraries(unit_tests STATIC mincore_downlevel.lib)
  elseif(CMAKE_BUILD_TYPE MATHCES Deb)
    #list(APPEND compiler_options "/MTd")
  endif()
endif()

message(STATUS "Extra compiler options:    ${compiler_options}")
message(STATUS "Extra linker options:      ${linker_options}")

target_compile_options(unit_tests PRIVATE ${compiler_options})
target_link_libraries(unit_tests PRIVATE ${linker_options})
target_compile_options(gaspar_app PRIVATE ${compiler_options})
target_link_libraries(gaspar_app PRIVATE ${linker_options})
