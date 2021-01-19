SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_C_COMPILER clang)
SET(CMAKE_CXX_COMPILER clang++)

SET(CMAKE_C_FLAGS_DEBUG "")
SET(CMAKE_C_FLAGS_DEBUG "-O0 -g")
SET(CMAKE_CXX_FLAGS_DEBUG "")
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

SET(CMAKE_C_FLAGS_RELEASE "")
SET(CMAKE_C_FLAGS_RELEASE "-flto -Ofast -ffast-math -DNDEBUG -mllvm -polly -mllvm -polly-vectorizer=stripmine")
SET(CMAKE_CXX_FLAGS_RELEASE "")
SET(CMAKE_CXX_FLAGS_RELEASE "-flto -Ofast -ffast-math -DNDEBUG -mllvm -polly -mllvm -polly-vectorizer=stripmine")

SET(CMAKE_LINKER "lld")
