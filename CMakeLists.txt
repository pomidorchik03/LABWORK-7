cmake_minimum_required(VERSION 3.10.0)
project(lab VERSION 0.1.0 LANGUAGES C CXX)

 
find_package(OpenSSL QUIET)
find_package(Boost QUIET COMPONENTS program_options system filesystem)

 
if (NOT OPENSSL_FOUND)
    include(ExternalProject)

    set(OPENSSL_INSTALL_DIR ${CMAKE_BINARY_DIR}/External/OpenSSL/Install)
    
     
    if (WIN32)
         
        set(OPENSSL_CONFIG_CMD perl Configure VC-WIN64A --prefix=${OPENSSL_INSTALL_DIR} --openssldir=${OPENSSL_INSTALL_DIR})
        set(OPENSSL_BUILD_CMD nmake)
        set(OPENSSL_INSTALL_CMD nmake install)
    else()
         
        set(OPENSSL_CONFIG_CMD ./config --prefix=${OPENSSL_INSTALL_DIR} --openssldir=${OPENSSL_INSTALL_DIR} no-shared -fPIC)
        set(OPENSSL_BUILD_CMD make -j)
        set(OPENSSL_INSTALL_CMD make install)
    endif()

    ExternalProject_Add(
        OpenSSL_Installer
        GIT_REPOSITORY https://github.com/openssl/openssl.git
        GIT_TAG openssl-3.0.7   
        SOURCE_DIR ${CMAKE_BINARY_DIR}/External/OpenSSL/Src
        BUILD_IN_SOURCE 1
        
        CONFIGURE_COMMAND ${OPENSSL_CONFIG_CMD}
        BUILD_COMMAND ${OPENSSL_BUILD_CMD}
        INSTALL_COMMAND ${OPENSSL_INSTALL_CMD}
        
         
        TEST_COMMAND ""
        LOG_DOWNLOAD 1
        LOG_CONFIGURE 1
        LOG_BUILD 1
    )
    
     
    set(OPENSSL_INCLUDE_DIR ${OPENSSL_INSTALL_DIR}/include)
    if (WIN32)
        set(OPENSSL_LIBRARIES 
            ${OPENSSL_INSTALL_DIR}/lib/libssl.lib
            ${OPENSSL_INSTALL_DIR}/lib/libcrypto.lib)
    else()
        set(OPENSSL_LIBRARIES 
            ${OPENSSL_INSTALL_DIR}/lib/libssl.a
            ${OPENSSL_INSTALL_DIR}/lib/libcrypto.a)
    endif()
    
     
    include_directories(${OPENSSL_INCLUDE_DIR})
endif()

 
if (NOT Boost_FOUND)
    message(STATUS "Boost not found. Downloading and building from source...")
    include(ExternalProject)

    ExternalProject_Add(
        Boost_Installer
        GIT_REPOSITORY https://github.com/boostorg/boost.git  
        GIT_TAG boost-1.84.0 
        SOURCE_DIR ${CMAKE_BINARY_DIR}/External/Boost/Src
        INSTALL_DIR ${CMAKE_BINARY_DIR}/External/Boost/Install
        
         
        CONFIGURE_COMMAND ""
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_BINARY_DIR}/External/Boost/Src/tools/build/src/bootstrap.sh
                ${CMAKE_BINARY_DIR}/External/Boost/Src/bootstrap.sh
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_BINARY_DIR}/External/Boost/Src/tools/build/src/bootstrap.bat
                ${CMAKE_BINARY_DIR}/External/Boost/Src/bootstrap.bat
        COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_BINARY_DIR}/External/Boost/Src
                ${CMAKE_COMMAND} -E env "NO_ZLIB=1" "NO_BZIP2=1"
                ${CMAKE_COMMAND} -E env "PATH=$ENV{PATH}" 
                ${CMAKE_COMMAND} -E env "CC=${CMAKE_C_COMPILER}" "CXX=${CMAKE_CXX_COMPILER}"
                ./bootstrap.sh --prefix=${CMAKE_BINARY_DIR}/External/Boost/Install
        COMMAND ${CMAKE_BINARY_DIR}/External/Boost/Src/b2 
                headers
                --prefix=${CMAKE_BINARY_DIR}/External/Boost/Install
        
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        
        LOG_DOWNLOAD 1
        LOG_CONFIGURE 1
    )
    
    set(BOOST_ROOT ${CMAKE_BINARY_DIR}/External/Boost/Src)
    add_subdirectory(${BOOST_ROOT} ${CMAKE_BINARY_DIR}/External/Boost/Build)
endif()

 
find_package(Boost REQUIRED COMPONENTS program_options system filesystem)
find_package(OpenSSL REQUIRED)

add_definitions(-DBOOST_ALL_DYN_LINK)

add_executable(lab 
    main.cpp 
    controller.cpp
    scanner.cpp 
    hash.cpp 
    reader.cpp
    mask.cpp
    )



target_link_libraries(lab PRIVATE 
    Boost::system 
    Boost::filesystem
    Boost::container_hash
    Boost::program_options
    Boost::headers
    OpenSSL::SSL
    OpenSSL::Crypto
)

 
if (TARGET OpenSSL_Installer)
    add_dependencies(lab OpenSSL_Installer)
endif()

if (TARGET Boost_Installer)
    add_dependencies(lab Boost_Installer)
endif()

 
include(CTest)
enable_testing()

 
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)