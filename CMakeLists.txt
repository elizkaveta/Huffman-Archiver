enable_testing()
find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 17)

SET(SRC compare_codes.h priority_queue.cpp in_wrapper.cpp node.h out_wrapper.cpp to_trie.h trie.cpp
        unzipper.cpp zipper.cpp priority_queue.h in_wrapper.h out_wrapper.h trie.h
        unzipper.h zipper.h)

add_executable(archiver main.cpp ${SRC})

add_executable(archiver_tests ${SRC} tests/test.cpp)
target_link_libraries(archiver_tests GTest::GTest GTest::Main)

add_test(AllArchiverTests archiver_tests)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
