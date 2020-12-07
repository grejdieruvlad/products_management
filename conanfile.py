import os, shutil, sys
if not os.path.isfile("conan/helper.py"):
    sys.stdout.write("The helper script was not found. "
        "Bringing it from remote..." + os.linesep)
    if os.path.isdir("conan"):
        shutil.rmtree("conan")
    os.system("git clone "
        "git@gitlab.computervoice.com:cvs/conan_helper.git -b v1.0.0 conan")
import conan.helper

conan_basic = conan.helper.conan_basic()

from conans import CMake, ConanFile

class project_recipe(ConanFile):
  name = "vgrejdieru_project"
  exports = "conan/*"
  settings = conan_basic.settings
  options = conan_basic.options
  generators = "cmake"
  default_options = conan_basic.default_options
  requires = (("googletest/1.10.0@cvs/stable"),
  ("mariadb-connector-c/3.1.3@cvs/stable"),("cvs_query/1.7.0@cvs/stable"),("cvs_util/1.13.0@cvs/stable"),("boost/1.72.0@cvs/stable", "override"))
  default_options += tuple (["cvs_util:build_network=True", "cvs_util:build_file=True", "cvs_util:build_time=True"])

  def build(self):
    cmake = CMake(self)
    self.set_cmake_options(cmake)
    cmake_extra_args = conan_basic.get_cmake_extra_args(self)

    if self.env.get('generate_eclipse_project', None) == "True":
      conan_basic.generate_eclipse_project(self, cmake=cmake, cmake_extra_args=cmake_extra_args)
      return

    cmake.configure(args=cmake_extra_args)
    cmake.build()

  def set_cmake_options(self, cmake):
    conan_basic.set_cmake_options(self, cmake)  

  #def package(self):
    #conan_helper.package(self, pkg = "project_package")

"""
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
"""