set(LAB1_GTEST_VERSION 2.3.4)
set(LAB1_GTEST_REPOSITORY https://github.com/google/GTest.git)

find_package(GTest ${LAB1_GTEST_VERSION})

if (GTest_FOUND)
    message(STATUS "Найден GTest ${GTest_VERSION}: ${GTest_DIR}")
else()
    message(STATUS
        "GTest ${MYLIB_GTEST_VERSION} будет взят с гитхаба: ${MYLIB_GTEST_REPOSITORY}")

    include(FetchContent)
    FetchContent_Declare(GTest
        GIT_REPOSITORY
            ${MYLIB_GTEST_REPOSITORY}
        GIT_TAG
            ${MYLIB_GTEST_VERSION}
    )
    FetchContent_MakeAvailable(GTest)
endif()
