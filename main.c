#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <err.h>

#include "stack.h"
#include "syntax.h"
#include "assembly.h"

void print_help() {
    printf("Babyc is a very basic C compiler.\n\n");
    printf("To compile a file:\n");
    printf("    $ babyc foo.c\n");
    printf("To output the AST without compiling:\n");
    printf("    $ babyc --dump-ast foo.c\n");
    printf("To print this message:\n");
    printf("    $ babyc --help\n\n");
    printf("For more information, see https://github.com/Wilfred/babyc\n");
}

extern Stack *syntax_stack;

extern int yyparse(void);
extern FILE *yyin;

int main(int argc, char *argv[])
{
    ++argv, --argc;  /* Skip over program name. */

    bool dump_ast = false;
    char *file_name;
    if (argc == 1 && strcmp(argv[0], "--help") == 0) {
        print_help();
        return 0;
    } else if (argc == 1) {
        file_name = argv[0];
        yyin = fopen(argv[0], "r");
    } else if (argc == 2 && strcmp(argv[0], "--dump-ast") == 0) {
        dump_ast = true;
        file_name = argv[1];
    } else {
        print_help();
        return 1;
    }

    int result;

    yyin = fopen(file_name, "r");

    if (yyin == NULL) {
        // TODO: work out what the error was.
        // TODO: Unit test this.
        printf("Could not open file: '%s'\n", file_name);
        result = 2;
        goto cleanup_file;
    }

    syntax_stack = stack_new();

    result = yyparse();
    if (result != 0) {
        printf("\n");
        goto cleanup_syntax;
    }

    Syntax *complete_syntax = stack_pop(syntax_stack);
    if (syntax_stack->size > 0) {
        warnx("Did not consume the whole syntax stack during parsing! %d left over.",
              syntax_stack->size);
    }

    if (dump_ast) {
        print_syntax(complete_syntax);
    } else {
        write_assembly(complete_syntax);
        syntax_free(complete_syntax);

        printf("Written out.s.\n");
        printf("Build it with:\n");
        printf("    $ as out.s -o out.o\n");
        printf("    $ ld -s -o out out.o\n");
    }

cleanup_syntax:
    /* TODO: if we exit early from syntactically invalid code, we will
       need to free multiple Syntax structs on this stack.
     */
    stack_free(syntax_stack);
cleanup_file:
    if (yyin != NULL) {
        fclose(yyin);
    }

    return result;
}
