# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary
cc_binary(
    name = "demo1",
    srcs = glob(["src/**"]),
    includes = ["include"],
    copts = ["--std=c++2a"],
    deps = ["@fmt//:fmt", "@magic_enum//:magic_enum", "@opencv//:opencv"],
)