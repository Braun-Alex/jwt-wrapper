# JWT Wrapper

## Description

JWT Wrapper is a convenient and secure C++ library for handling JSON Web Tokens (JWTs). 
It provides a simple interface to generate and validate JWTs, with an underlying AES encryption layer 
provided by the [AES Wrapper library](https://github.com/Braun-Alex/aes-wrapper) for added security.

## Features

- Generation and validation of JWTs.
- AES encryption of sensitive token payload data for enhanced security.
- Cross-platform support.

## Getting started

### Prerequisites

Before you begin, ensure you have met the following requirements:

- A modern C++ compiler that supports C++23.
- CMake version 3.26 or higher.
- Git for cloning the repository.
- OpenSSL installed on your system for key generation.
- The [AES Wrapper library](https://github.com/Braun-Alex/aes-wrapper) installed on your system.

### Installation

To install JWT Wrapper, follow these steps:

1. Clone the repository:

```bash
git clone https://github.com/Braun-Alex/jwt-wrapper.git
```

2. Navigate to the cloned directory:
```bash
cd jwt-wrapper
```

3. Create a new directory for the build:
```bash
mkdir build
```

4. Enter it:
```bash
cd build
```

5. Generate the Makefile using CMake:
```bash
sudo cmake ..
```

6. Compile and install the library:
```bash
sudo make install
```

This process will compile and install the JWT Wrapper on your system, making it available for linking in your C++ projects.

### Using JWT Wrapper in your project

After installing JWT Wrapper, you can easily integrate it into your C++ project.
Here's how to set up your `CMakeLists.txt` to include and link against JWT Wrapper:

1. Specify the minimum required version of CMake and define your project name:
```cmake
cmake_minimum_required(VERSION 3.26)
project(ProjectName)
```

2. Set the C++ standard for your project:
```cmake
set(CMAKE_CXX_STANDARD 23)
```

3. List your project's files:
```cmake
set(HEADERS example.h)
set(SOURCES example.cpp main.cpp)
```

4. Fetch and make available the required libraries:
```cmake
include(FetchContent)
FetchContent_Declare(
        Poco
        URL https://github.com/pocoproject/poco/archive/refs/tags/poco-1.12.5-release.zip
)
FetchContent_MakeAvailable(Poco)
```

5. Create an executable target for your project:
```cmake
set(${PROJECT_NAME} ${HEADERS} ${SOURCES})
```

6. Link against JWT Wrapper and the required libraries:
```cmake
target_link_libraries(${PROJECT_NAME} PUBLIC jwt_wrapper aes_wrapper Poco::Crypto Poco::Util)
```

After installing and configuring the JWT Wrapper, you can use it in your C++ application to generate and validate JWTs. Here is an example:

```c++
#include "JWT.h"
#include <iostream>

int main() {
    const std::string data = "Alex Braun", passphrase = "AlAzazaAl123";
    JWT wrapper("publicKey.pem", "privateKey.pem", passphrase);
    std::cout << "Data: \"" << data << "\"\n"
              << "Passphrase to protect token payload data: \"" << passphrase << "\"\n";

    const std::string accessToken = wrapper.generateAccessToken(data);
    std::string protectedData;
    bool isValid = wrapper.verifyToken(accessToken, protectedData);
    std::cout << "Access token: \"" << accessToken << "\"\n"
    << std::boolalpha << "Is valid access token: " << isValid << "\n"
    << "Access token payload data: \"" << protectedData << "\"\n" ;

    const std::string refreshToken = wrapper.generateRefreshToken(data);
    isValid = wrapper.verifyToken(refreshToken, protectedData);
    std::cout << "Refresh token: \"" << refreshToken << "\"\n"
    << "Is valid refresh token: " << isValid << "\n"
    << "Refresh token payload data: \"" << protectedData << "\"";
}
```

Note that `privateKey.pem` and `publicKey.pem` should be a cryptographic pair of keys using the elliptic curve `secp256r1`.

You can a private key using:
```bash
openssl ecparam -genkey -name prime256v1 -noout -out privateKey.pem
```

To generate a corresponding public key from the private key:
```bash
openssl ec -in privateKey.pem -pubout -out publicKey.pem
```
