# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(cpp-order-bookTests LANGUAGES CXX)

# ---- Dependencies ----

find_package(GTest REQUIRED)
include(GoogleTest)

# ---- Tests ----

add_executable(
        cpp-order-book_test
        matching_engine_test.cc
        per_instrument_matching_engine_test.cc
)
target_link_libraries(
    cpp-order-book_test PRIVATE
    cpp-order-book_lib
    GTest::gtest_main
)
target_compile_features(cpp-order-book_test PRIVATE cxx_std_20)

gtest_discover_tests(cpp-order-book_test)

# ---- End-of-file commands ----

add_folders(Test)
