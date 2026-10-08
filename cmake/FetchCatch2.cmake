include(FetchContent)


set(FETCHCONTENT_CMAKE_ARGS
  -DCMAKE_CXX_FLAGS="-w"  # Disable all warnings
)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.4
)

FetchContent_MakeAvailable(Catch2)

if (TARGET Catch2)
  if (MSVC)
    # Disable warnings for MSVC (adjust codes if needed)
    target_compile_options(Catch2 PRIVATE /wd4061) # example warning disable
  else()
    # For GCC/Clang, disable the specific warning, e.g. -Wswitch-default
    target_compile_options(Catch2 PRIVATE -Wno-switch-default -w)
  endif()
endif()