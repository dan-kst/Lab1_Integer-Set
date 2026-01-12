# Integer Set Engine (Lab 1) 

A high-performance C++ engine for managing mathematical sets of integers. This project utilizes a Hexagonal Architecture, decoupling the core logic from the storage layer and multiple user interfaces.

## 🌟 Features

* Core Logic: Mathematical operations (Union, Intersection, Difference) on integer sets.
* Persistence: PostgreSQL backend with JSON serialization.
* Console UI: Fast CLI with basic and advanced (command-line) modes.
* Desktop GUI: Modern GTK 4 interface with modal dialogs and batch operations.
* Web Interface: REST API built with Crow and a responsive Tailwind CSS dashboard.
* Testing: Comprehensive suite using GoogleTest and GoogleMock.

## 🛠️ Prerequisites & Dependencies

### Linux (Debian 13 / Trixie)

Install the following via apt:
```bash
sudo apt update
sudo apt install build-essential meson ninja-build libgtkmm-4.0-dev libpqxx-dev docker-compose
```

### Windows 10 (via MSYS2)

Use the UCRT64 terminal:
```
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-meson mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-gtkmm-4.0 mingw-w64-ucrt-x86_64-libpqxx
```

## Automatic Dependencies (Meson Subprojects)

The following are handled automatically by Meson: 
* nlohmann_json
* gtest / gmock
* crow (Ensure you have the .wrap file in subprojects/)

## 🗄️ Database Setup

The application requires a PostgreSQL instance. The easiest way to run this is via Docker:

1. Navigate to the project root.
1. Start the container:
```docker
docker-compose up -d
```

_Note: Ensure the environment variables in docker-compose.yml match your PostgresRepository connection string._

## 🏗️ Build Instructions

1. Setup the build directory:
```meson
meson setup build
```
1. Compile everything:
```meson
meson compile -C build
```

## 🚀 Running the Application

This project provides three "Wraps" (interfaces). Run them from the project root to ensure file paths resolve correctly.

1. Console Interface
```
./build/lab1_console
```
2. Desktop GUI (GTK 4)
```
./build/lab1_gtkmm
```
3. Web Dashboard
```
./build/lab1_web
```
Once running, open your browser to: http://localhost:18080

## 🧪 Testing
To verify the core logic and storage integration:
```
./build/run_tests
```

_Tip: Use `--gtest_filter=Name.` to run specific test groups if you are focused on one module._

## 📂 Project Structure

* `include/core`: Mathematical logic.
* `include/storage`: Database and Serialization.
* `include/ui`: Interface-specific headers (Common, Console, GTK, Web).
* `src/`: Implementation files.
* `public/`: Web dashboard assets (HTML/JS).
* `tests/`: GTest suites.