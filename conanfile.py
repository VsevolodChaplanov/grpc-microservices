import os
from conan import ConanFile
from conan.tools.cmake import cmake_layout


class AsioGrpcRecipe(ConanFile):

    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
        self.requires("boost/[~1.81]")
        self.requires("fmt/[~10]")
        self.requires("range-v3/[~0.12]")
        self.requires("asio-grpc/2.4.0")
        self.requires("libpqxx/[>7.7.4]")
        self.requires("gtest/cci.20210126")
        self.requires("catch2/3.3.2")

        if self.settings.os == "Windows":
            self.requires("base64/[~0.4]")
    
    def build_requirements(self):
        if self.settings.os != "Windows":
            self.tool_requires("cmake/[>3.15]")

    def layout(self):
        cmake_layout(self)