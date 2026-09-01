add_executable(
  test_args

  test_args.c
  ${CMAKE_SOURCE_DIR}/src/args.c
)

target_include_directories(
  test_args
  PRIVATE ${CMAKE_SOURCE_DIR}/src
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

# 5. Non-positive size: --size -5. A VLA can't have a zero/negative
#    dimension, so this must be rejected explicitly rather than left to
#    misbehave.
add_test(NAME args_invalid_size COMMAND test_args --size -5)
set_tests_properties(
  args_invalid_size PROPERTIES
  PASS_REGULAR_EXPRESSION "Invalid size: -5 \\(must be positive\\)"
)

# 6. Negative mine count: -m -3. Parses fine via strtol, but doesn't make
#    sense as a mine count and must be rejected.
add_test(NAME args_negative_mines COMMAND test_args -m -3)
set_tests_properties(
  args_negative_mines PROPERTIES
  PASS_REGULAR_EXPRESSION "Invalid mines: -3 \\(must be non-negative\\)"
)

# 7. Capacity check: --size 3 -m 9 asks for as many mines as cells (9), so
#    place_mine's reroll loop could never find a free cell. Must be rejected
#    before it ever reaches place_mine.
add_test(NAME args_too_many_mines COMMAND test_args --size 3 -m 9)
set_tests_properties(
  args_too_many_mines PROPERTIES
  PASS_REGULAR_EXPRESSION "Too many mines: 9 for a 3x3 board \\(must be less than 9\\)"
)

# 8. Capacity boundary: --size 3 -m 8 is exactly one below the 3x3 cell
#    count (9), so it's the largest mine count that should still succeed.
add_test(NAME args_mines_at_capacity_boundary COMMAND test_args --size 3 -m 8)
set_tests_properties(
  args_mines_at_capacity_boundary PROPERTIES
  PASS_REGULAR_EXPRESSION "size=3 mines=8"
)


# Test board.c
add_executable(
  test_board

  test_board.c
  ${CMAKE_SOURCE_DIR}/src/board.c
)

target_include_directories(
  test_board
  PRIVATE ${CMAKE_SOURCE_DIR}/src
)

add_test(NAME board_init_board COMMAND test_board init_board)
add_test(NAME board_place_mine COMMAND test_board place_mine)
add_test(NAME board_place_mine_excludes_safe_cell COMMAND test_board place_mine_excludes_safe_cell)
add_test(NAME board_compute_counts COMMAND test_board compute_counts)
add_test(NAME board_flood_fill COMMAND test_board flood_fill)
add_test(NAME board_is_win COMMAND test_board is_win)
add_test(NAME board_is_lose COMMAND test_board is_lose)
