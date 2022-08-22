workspace(name = "ml")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "rules_cc",
    urls = ["https://github.com/bazelbuild/rules_cc/archive/0.0.2.tar.gz"],
    sha256 = "0e5801b1834a51c1308044e9abaeb6aaf945e4a4274866ea84fbccc50a292368",
    strip_prefix = "rules_cc-0.0.2",
)
load("@rules_cc//cc:repositories.bzl", "rules_cc_dependencies", "rules_cc_toolchains")
rules_cc_dependencies()
rules_cc_toolchains()

http_archive(
    name = "rules_foreign_cc",
    sha256 = "6041f1374ff32ba711564374ad8e007aef77f71561a7ce784123b9b4b88614fc",
    strip_prefix = "rules_foreign_cc-0.8.0",
    url = "https://github.com/bazelbuild/rules_foreign_cc/archive/0.8.0.tar.gz",
)
load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")
rules_foreign_cc_dependencies()

#third party libraries
http_archive(
    name = "fmt",
    build_file = "//build_files:fmt.BUILD",
    sha256 = "9a1e0e9e843a356d65c7604e2c8bf9402b50fe294c355de0095ebd42fb9bd2c5",
    strip_prefix = "fmt-9.0.0",
    urls = ["https://github.com/fmtlib/fmt/archive/9.0.0.tar.gz"],
) 

http_archive(
    name = "magic_enum",
    build_file = "//build_files:magic_enum.BUILD",
    strip_prefix = "magic_enum-0.8.0",
    sha256 = "5e7680e877dd4cf68d9d0c0e3c2a683b432a9ba84fc1993c4da3de70db894c3c",
    urls = ["https://github.com/Neargye/magic_enum/archive/v0.8.0.tar.gz"],
    
)

http_archive(
    name = "opencv",
    build_file = "//build_files:opencv.BUILD",
    strip_prefix = "opencv-4.6.0",
    sha256 = "1ec1cba65f9f20fe5a41fda1586e01c70ea0c9a6d7b67c9e13edf0cfe2239277",
    urls = ["https://github.com/opencv/opencv/archive/4.6.0.tar.gz"],
)
