#include <stdio.h>
#include <stdlib.h>

#include "readline.h"
#include "mpc.h"

int main(int argc, char** argv) {

    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* Operator = mpc_new("operator");
    mpc_parser_t* Expr = mpc_new("expr");
    mpc_parser_t* Cheemscript = mpc_new("cheemscript");

    mpca_lang(MPCA_LANG_DEFAULT,
        " \
         number : /-?[0-9]+/ ; \
         operator : '+' | '-' | '*' | '/' ; \
         expr : <number> | '(' <operator> <expr>+ ')' ; \
         cheemscript : /^/ <operator> <expr>+ /$/ ; \
        ",
        Number, Operator, Expr, Cheemscript);

    puts("Cheemscript v0.0.1");
    puts("Press Ctrl+c to exit\n");

    while (1) {
        char* input = readline("cheemscript> ");

        add_history(input);

        mpc_result_t r;
        if (mpc_parse("<stdin>", input, Cheemscript, &r)) {
            mpc_ast_print(r.output);
            mpc_ast_delete(r.output);
        } else {
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }

        free(input);
    }

    mpc_cleanup(4, Number, Operator, Expr, Cheemscript);
    return 0;
}
