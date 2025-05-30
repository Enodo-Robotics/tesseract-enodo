find_gtest()

add_executable(${PROJECT_NAME}_unit tesseract_scene_graph_unit.cpp)
target_link_libraries(
  ${PROJECT_NAME}_unit
  PRIVATE GTest::GTest
          GTest::Main
          ${PROJECT_NAME}
          tesseract::tesseract_geometry)
target_compile_options(${PROJECT_NAME}_unit PRIVATE ${TESSERACT_COMPILE_OPTIONS_PRIVATE}
                                                    ${TESSERACT_COMPILE_OPTIONS_PUBLIC})
target_compile_definitions(${PROJECT_NAME}_unit PRIVATE ${TESSERACT_COMPILE_DEFINITIONS})
target_clang_tidy(${PROJECT_NAME}_unit ENABLE ${TESSERACT_ENABLE_CLANG_TIDY})
target_cxx_version(${PROJECT_NAME}_unit PRIVATE VERSION ${TESSERACT_CXX_VERSION})
target_code_coverage(
  ${PROJECT_NAME}_unit
  PRIVATE
  ALL
  EXCLUDE ${COVERAGE_EXCLUDE}
  ENABLE ${TESSERACT_ENABLE_CODE_COVERAGE})
add_gtest_discover_tests(${PROJECT_NAME}_unit)
add_dependencies(${PROJECT_NAME}_unit ${PROJECT_NAME})
add_dependencies(run_tests ${PROJECT_NAME}_unit)

add_executable(${PROJECT_NAME}_joint_unit tesseract_scene_graph_joint_unit.cpp)
target_link_libraries(
  ${PROJECT_NAME}_joint_unit
  PRIVATE GTest::GTest
          GTest::Main
          ${PROJECT_NAME}
          tesseract::tesseract_geometry)
target_compile_options(${PROJECT_NAME}_joint_unit PRIVATE ${TESSERACT_COMPILE_OPTIONS_PRIVATE}
                                                          ${TESSERACT_COMPILE_OPTIONS_PUBLIC})
target_clang_tidy(${PROJECT_NAME}_joint_unit ENABLE ${TESSERACT_ENABLE_CLANG_TIDY})
target_cxx_version(${PROJECT_NAME}_joint_unit PRIVATE VERSION ${TESSERACT_CXX_VERSION})
target_code_coverage(
  ${PROJECT_NAME}_joint_unit
  PRIVATE
  ALL
  EXCLUDE ${COVERAGE_EXCLUDE}
  ENABLE ${TESSERACT_ENABLE_CODE_COVERAGE})
add_gtest_discover_tests(${PROJECT_NAME}_joint_unit)
add_dependencies(${PROJECT_NAME}_joint_unit ${PROJECT_NAME})
add_dependencies(run_tests ${PROJECT_NAME}_joint_unit)

add_executable(${PROJECT_NAME}_link_unit tesseract_scene_graph_link_unit.cpp)
target_link_libraries(
  ${PROJECT_NAME}_link_unit
  PRIVATE GTest::GTest
          GTest::Main
          ${PROJECT_NAME}
          tesseract::tesseract_geometry)
target_compile_options(${PROJECT_NAME}_link_unit PRIVATE ${TESSERACT_COMPILE_OPTIONS_PRIVATE}
                                                         ${TESSERACT_COMPILE_OPTIONS_PUBLIC})
target_clang_tidy(${PROJECT_NAME}_link_unit ENABLE ${TESSERACT_ENABLE_CLANG_TIDY})
target_cxx_version(${PROJECT_NAME}_link_unit PRIVATE VERSION ${TESSERACT_CXX_VERSION})
target_code_coverage(
  ${PROJECT_NAME}_link_unit
  PRIVATE
  ALL
  EXCLUDE ${COVERAGE_EXCLUDE}
  ENABLE ${TESSERACT_ENABLE_CODE_COVERAGE})
add_gtest_discover_tests(${PROJECT_NAME}_link_unit)
add_dependencies(${PROJECT_NAME}_link_unit ${PROJECT_NAME})
add_dependencies(run_tests ${PROJECT_NAME}_link_unit)

add_executable(${PROJECT_NAME}_serialization_unit tesseract_scene_graph_serialization_unit.cpp)
target_link_libraries(
  ${PROJECT_NAME}_serialization_unit
  PRIVATE GTest::GTest
          GTest::Main
          ${PROJECT_NAME}
          tesseract::tesseract_geometry)
target_compile_options(${PROJECT_NAME}_serialization_unit PRIVATE ${TESSERACT_COMPILE_OPTIONS_PRIVATE}
                                                                  ${TESSERACT_COMPILE_OPTIONS_PUBLIC})
target_clang_tidy(${PROJECT_NAME}_serialization_unit ENABLE ${TESSERACT_ENABLE_CLANG_TIDY})
target_cxx_version(${PROJECT_NAME}_serialization_unit PRIVATE VERSION ${TESSERACT_CXX_VERSION})
target_code_coverage(
  ${PROJECT_NAME}_serialization_unit
  PRIVATE
  ALL
  EXCLUDE ${COVERAGE_EXCLUDE}
  ENABLE ${TESSERACT_ENABLE_CODE_COVERAGE})
add_gtest_discover_tests(${PROJECT_NAME}_serialization_unit)
add_dependencies(${PROJECT_NAME}_serialization_unit ${PROJECT_NAME})
add_dependencies(run_tests ${PROJECT_NAME}_serialization_unit)
