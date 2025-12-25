project('Lab1_IntegerSet', 'cpp',
  version : '0.1',
  default_options : ['cpp_std=c++20', 'warning_level=3'])

# 1. Dependencies
# Meson will look for these on the system, if not found, it uses subprojects/
gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_main_dep')
gmock_dep = gtest_proj.get_variable('gmock_dep')

json_proj = subproject('nlohmann_json')
json_dep = dependency('nlohmann_json', fallback : ['nlohmann_json', 'nlohmann_json_dep'])

# Add PostgreSQL client library
pqxx_dep = dependency('libpqxx')

# Add threads for parallelization later and DB drivers
thread_dep = dependency('threads')
# For later: sqlite_dep = dependency('sqlite3')

# 2. Includes
inc = include_directories('include')

# 3. Sources
core_sources = files(
  'src/core/IntegerSet.cpp'
)
storage_sources = files(
  'src/storage/IntegerSetSerializer.cpp',
  'src/storage/PostgresRepository.cpp'
)
test_sources = files(
    'tests/test_main.cpp',
    'tests/set_tests.cpp',
    'tests/storage_tests.cpp'
)

# 4. Library (The "Engine" of your lab)
lib_set = static_library('integer_set',
  [core_sources, storage_sources],
  include_directories : inc,
  dependencies : [json_dep, pqxx_dep]
)

# 5. Executables (The "Wraps")
executable('lab1_console',
  'src/ui/console/main.cpp',
  link_with : lib_set,
  include_directories : inc
)

# 6. Tests (TDD focus)
test_exe = executable('run_tests',
    test_sources,
    link_with : lib_set,
    include_directories : inc,
    dependencies : [gtest_dep, gmock_dep, pqxx_dep]
)

test('Set logic tests', test_exe)
