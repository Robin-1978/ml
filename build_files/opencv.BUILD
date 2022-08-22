load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

filegroup(
    name = "all_srcs",
    srcs = glob(["**"]),
    visibility = ["//visibility:public"],
)

cmake(
    name = "opencv",
    cache_entries = {
        "CMAKE_C_FLAGS": "-fPIC",
    },
    lib_source = ":all_srcs",
    includes = ["include/opencv4"],
    out_include_dir = "include/opencv4",
    out_shared_libs = [
        "libopencv_core.so",
        "libopencv_highgui.so",
        "libopencv_imgproc.so", 
        "libopencv_imgcodecs.so"
        ],
    visibility = ["//visibility:public"],
)