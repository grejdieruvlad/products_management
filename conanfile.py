from conans import CMake, ConanFile

class executable_package(ConanFile):
  name = "project"
  settings = "os", "arch", "compiler", "build_type"
  generators = "cmake"
  requires = ("googletest/1.10.0@cvs/stable"),("mariadb-connector-c/3.1.3@cvs/stable"), ("cvs_query/1.7.0@cvs/stable"), ("cvs_util/1.13.0@cvs/stable"),("boost/1.72.0@cvs/stable", "override")
  default_options = ("cvs_util:build_network=True", "cvs_util:build_file=True",
  "cvs_util:build_time=True")

  def build(self):
    cmake = CMake(self)
    cmake.configure()
    cmake.build()

  def source(self):
    raise Exception("Only conan install and build are available")

  def package(self):
    raise Exception("Only conan install and build are available")

  def package_info(self):
    raise Exception("Only conan install and build are available")
