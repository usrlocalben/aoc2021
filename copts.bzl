MSVC_FLAGS = [
    "/EHsc",  # C++ exceptions: enabled
    "/GS-",  # security checks: disabled
    "/std:c++20",
    "/permissive-",  # standard conformance: strict

    "/utf-8",

    "/W4",
    "/WX",
    "/wd4324",  # structure padded due to alignment specifier
    "/wd4706",  # assignment in if expr
    "/wd4267",  # conversion from 'size_t' to 'type', possible loss of data (backward-cpp)
	"/wd4244",  # conversion loss of data (fast_io)
    # "/wd4305",  # "truncation from 'double' to 'float'
    # "/wd4244",  # "conversion from 'double' to 'float' possible loss of data
    # "/wd4005",  # macro redefinition
    # "/wd4127",  # conditional expr is constant
    # "/wd4459",  # jsoncons
    # "/wd4505",  # unreferenced function with internal linkage removed

    "/DUNICODE",
    "/D_UNICODE",
    "/D_ENABLE_EXTENDED_ALIGNED_STORAGE",
    "/DNOMINMAX",]  # Don't define min and max macros (windows.h)

CLANG_CL_FLAGS = [
    "-msse4.1",
    # "-Wno-unused-parameter",
    # "-Wno-unused-variable",
    # "-Wno-unused-function",
    # "-Wno-builtin-macro-redefined",
    # "-Wno-unused-command-line-argument",
    # "-Wno-missing-field-initializers",
    # "-Wno-unused-but-set-variable",
    # "-Wno-unused-private-field",
    ]

MSVC_RELEASE_FLAGS = [
    "/fp:fast", ]

RQDQ_DEFAULT_COPTS = MSVC_FLAGS + MSVC_RELEASE_FLAGS + \
                     select({"//:clang_cl_compiler": CLANG_CL_FLAGS,
                             "//conditions:default": []})
