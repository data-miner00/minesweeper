add_executable(
  test_args

  test_args.c
  ${CMAKE_SOURCE_DIR}/args.c
)

target_include_directories(
  test_args
  PRIVATE ${CMAKE_SOURCE_DIR}
)

# 1. Missing value: --size with nothing after it. The argc guard in
#    parse_minesweeper_args should skip parsing entirely and keep defaults,
#    rather than reading past the end of argv (the old crash).
add_test(NAME args_missing_value COMMAND test_args --size)
set_tests_properties(
  args_missing_value PROPERTIES
  PASS_REGULAR_EXPRESSION "size=9 mines=10"
)

# 2. Garbage value: --size abc. atoi used to silently parse this as 0;
#    strtol's endptr check should reject it and exit(1) with a clear error.
add_test(NAME args_garbage_value COMMAND test_args --size abc)
set_tests_properties(
  args_garbage_value PROPERTIES
  PASS_REGULAR_EXPRESSION "Invalid value for --size: abc"
)

# 3. Trailing junk: --size 12abc. strtol parses the leading "12" but leaves
#    "abc" unconsumed; the endptr != '\0' check should catch that too.
add_test(NAME args_trailing_junk COMMAND test_args --size 12abc)
set_tests_properties(
  args_trailing_junk PROPERTIES
  PASS_REGULAR_EXPRESSION "Invalid value for --size: 12abc"
)

# 4. Valid input: both flags, well-formed values, should parse cleanly.
add_test(NAME args_valid_value COMMAND test_args --size 12 -m 5)
set_tests_properties(
  args_valid_value PROPERTIES
  PASS_REGULAR_EXPRESSION "size=12 mines=5"
)
