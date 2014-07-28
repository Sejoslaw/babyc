%{
#include <stdio.h>
#include "syntax.h"
#include "assembly.h"
#include "stack.h"

int yyparse(void);

void yyerror(const char *str)
{
	fprintf(stderr,"error: %s\n",str);
}

int yywrap()
{
	return 1;
}

extern FILE *yyin;

Stack *syntax_stack;

int main(int argc, char *argv[])
{
    ++argv, --argc;  /* Skip over program name. */
    if (argc != 1) {
        printf("Please specify a file to compile.\n");
        printf("    $ babyc <your file here>\n");
        return 1;
    }

    yyin = fopen(argv[0], "r");

    if (yyin == NULL) {
        // TODO: work out what the error was.
        // TODO: Unit test this.
        printf("Failed to open file.\n");
        return 2;
    }

    syntax_stack = stack_new();
    yyparse();

    Syntax *complete_syntax = stack_pop(syntax_stack);
    write_assembly(complete_syntax);

    if (syntax_stack->size > 0) {
        printf("WARNING: Did not consume the whole syntax stack during parsing!");
    }

    stack_free(syntax_stack);
    syntax_free(complete_syntax);

    printf("Written out.s.\n");
    printf("Build it with:\n");
    printf("    $ as out.s -o out.o\n");
    printf("    $ ld -s -o out out.o\n");

    return 0;
}

%}

%token INCLUDE HEADER_NAME
%token TYPE IDENTIFIER RETURN NUMBER
%token OPEN_BRACE CLOSE_BRACE

%%

program:
	function
        ;

function:
	TYPE IDENTIFIER '(' ')' OPEN_BRACE statement CLOSE_BRACE
        ;

statement:
        RETURN expression ';'
        ;

expression:
	NUMBER
        {
            stack_push(syntax_stack, immediate_new($1));
        }
        |
        '~' expression
        {
            Syntax *current_syntax = stack_pop(syntax_stack);
            stack_push(syntax_stack, bitwise_negation_new(current_syntax));
        }
        |
        '!' expression
        {
            Syntax *current_syntax = stack_pop(syntax_stack);
            stack_push(syntax_stack, logical_negation_new(current_syntax));
        }
        |
        expression '+' expression
        {
            Syntax *left = stack_pop(syntax_stack);
            Syntax *right = stack_pop(syntax_stack);
            stack_push(syntax_stack, addition_new(left, right));
        }
        ;
