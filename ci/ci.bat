:build_cpp
  IF NOT EXIST %build_folder% (
    md %build_folder%
  )
  cd %build_folder%
  conan install .. --build=missing
&&conan build..
goto done

