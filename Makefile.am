COMMON_SOURCES = \
src/ast/and_or_ast.c \
src/ast/ast.h \
src/ast/case_command_ast.c \
src/ast/command_ast.c \
src/ast/compound_list_ast.c \
src/ast/for_command_ast.c \
src/ast/if_command_ast.c \
src/ast/item_case.c \
src/ast/item_case.h \
src/ast/list_ast.c \
src/ast/pipeline_ast.c \
src/ast/shell_command_ast.c \
src/ast/simple_command_ast.c \
src/ast/until_command_ast.c \
src/ast/while_command_ast.c \
src/builtins/alias.c \
src/builtins/builtins.h \
src/builtins/break.c \
src/builtins/cd.c \
src/builtins/continue.c \
src/builtins/echo.c \
src/builtins/exit.c \
src/builtins/export.c \
src/builtins/global.c \
src/builtins/pwd.c \
src/builtins/source.c \
src/builtins/unalias.c \
src/debug/and_or_ast_to_dot.c \
src/debug/ast_to_dot.c \
src/debug/case_command_ast_to_dot.c \
src/debug/command_ast_to_dot.c \
src/debug/compound_list_to_dot.c \
src/debug/compound_list_to_dot.h \
src/debug/for_command_ast_to_dot.c \
src/debug/if_command_ast_to_dot.c \
src/debug/list_ast_to_dot.c \
src/debug/pipeline_ast_to_dot.c \
src/debug/shell_command_ast_to_dot.c \
src/debug/simple_command_ast_to_dot.c \
src/debug/until_command_ast_to_dot.c \
src/debug/while_command_ast_to_dot.c \
src/exec/and_or_exec.c \
src/exec/case_command_exec.c \
src/exec/command_exec.c \
src/exec/compound_list_exec.c \
src/exec/exec.c \
src/exec/exec.h \
src/exec/for_command_exec.c \
src/exec/if_command_exec.c \
src/exec/list_exec.c \
src/exec/loop.c \
src/exec/loop.h \
src/exec/pipeline_exec.c \
src/exec/redir_exec.c \
src/exec/shell_command_exec.c \
src/exec/simple_command_exec.c \
src/exec/until_command_exec.c \
src/exec/while_command_exec.c \
src/expansion/command_substitution.c \
src/expansion/expand_special.c \
src/expansion/expansion.h \
src/expansion/tilde_expansion.c \
src/expansion/variable_expansion.c \
src/lexer/lexer.c \
src/lexer/lexer.h \
src/lexer/token.c \
src/lexer/token.h \
src/option/option.c \
src/option/option.h \
src/option/option_struct.c \
src/parser/and_or_parser.c \
src/parser/case_command_parser.c \
src/parser/command_parser.c \
src/parser/compound_list_parser.c \
src/parser/destroy_and_nullify.c \
src/parser/destroy_and_nullify.h \
src/parser/for_command_parser.c \
src/parser/if_command_parser.c \
src/parser/item_case_parser.c \
src/parser/list_parser.c \
src/parser/next_is_keyword.c \
src/parser/next_is_keyword.h \
src/parser/parser.c \
src/parser/parser.h \
src/parser/pipeline_parser.c \
src/parser/redir.c \
src/parser/redir.h \
src/parser/shell_command_parser.c \
src/parser/simple_command_parser.c \
src/parser/skip_newlines.c \
src/parser/skip_newlines.h \
src/parser/until_command_parser.c \
src/parser/while_command_parser.c \
src/utils/dstring.c \
src/utils/dstring.h \
src/utils/file_handler.c \
src/utils/file_handler.h \
src/utils/hash.c \
src/utils/hash_map.c \
src/utils/hash_map.h \
src/utils/hash_map2.c \
src/utils/is_number.c \
src/utils/is_number.h \
src/utils/name.c \
src/utils/name.h \
src/utils/queue.c \
src/utils/queue.h \
src/utils/run_command.c \
src/utils/run_command.h \
src/utils/string_vector.c \
src/utils/string_vector.h \
src/utils/strstrip.c \
src/utils/strstrip.h \
src/utils/vector.c \
src/utils/vector.h \
src/utils/xfcntl.c \
src/utils/xfcntl.h \
src/utils/xfnmatch.c \
src/utils/xfnmatch.h \
src/utils/xio.c \
src/utils/xio2.c \
src/utils/xio.h \
src/utils/xmalloc.c \
src/utils/xmalloc.h \
src/utils/xstring.c \
src/utils/xstring.h \
src/utils/xunistd.c \
src/utils/xunistd2.c \
src/utils/xunistd.h

# Flags
REQUIRED_FLAGS = -Wall -Wextra -Werror -std=c99 -pedantic
COMMON_INCLUDE_FLAGS = \
-Isrc/ast \
-Isrc/builtins \
-Isrc/debug \
-Isrc/exec \
-Isrc/expansion \
-Isrc/lexer \
-Isrc/option \
-Isrc/parser \
-Isrc/utils
DEBUG_FLAGS = -g -fsanitize=address

# 42sh #########################################################################
bin_PROGRAMS = 42sh
42sh_SOURCES = $(COMMON_SOURCES) src/main.c
42sh_CFLAGS = $(REQUIRED_FLAGS) $(COMMON_INCLUDE_FLAGS) $(DEBUG_FLAGS)
42sh_CPPFLAGS = -D_POSIX_C_SOURCE=200809L

# Documentation ################################################################
DOXYFILE = doc/Doxyfile
.PHONY: doc view
doc: $(DOXYFILE)
	doxygen $^

# Ast Printer ##################################################################
DOTFILE = ast.dot
CLEANFILES = ast.dot ast.png
view: $(DOTFILE)
	cat ast.dot | gvpack -u | dot -Tpng > ast.png
	feh -Z -. ast.png
viewreset:
	$(RM) ast.dot ast.png

# Tests ########################################################################

# Configure TAP driver
TEST_LOG_DRIVER = \
    env AM_TAP_AWK='$(AWK)' $(SHELL) $(top_srcdir)/build-aux/tap-driver.sh
TESTS = tests/functional_tests.sh tests/redir_test.sh

if WITH_CHECK
check_PROGRAMS = unit_tests

# Setup Check TAP output
AM_TESTS_ENVIRONMENT = \
    CHECK_OUTPUTS='tap:-'; \
    export CHECK_OUTPUTS;

# Add the testsuite to the TAP test harness
TESTS += unit_tests

unit_tests_SOURCES = \
$(COMMON_SOURCES) \
tests/check_assignment.c \
tests/check_lexer.c \
tests/check_option.c \
tests/check_parser.c \
tests/check_quoting.c \
tests/check_tests.h \
tests/unit_tests.c

# Use check library flags
unit_tests_CFLAGS = $(COMMON_INCLUDE_FLAGS) $(CHECK_CFLAGS)
unit_tests_LDFLAGS = $(CHECK_LIBS)
endif
