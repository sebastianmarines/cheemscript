#include <stdio.h>
#include <stdlib.h>

#include "readline.h"
#include "mpc.h"

long eval(mpc_ast_t *t);
long eval_op(long x, char *op, long y);

int main(int argc, char **argv)
{

    mpc_parser_t *Number = mpc_new("number");
    mpc_parser_t *Operator = mpc_new("operator");
    mpc_parser_t *Expr = mpc_new("expr");
    mpc_parser_t *Cheemscript = mpc_new("cheemscript");

    mpca_lang(MPCA_LANG_DEFAULT,
              " \
         number : /-?[0-9]+/ ; \
         operator : '+' | '-' | '*' | '/' | '%' ; \
         expr : <number> | '(' <operator> <expr>+ ')' ; \
         cheemscript : /^/ <operator> <expr>+ /$/ ; \
        ",
              Number, Operator, Expr, Cheemscript);

    puts("Cheemscript v0.0.1");
    puts("Press Ctrl+c to exit\n");

    while (1)
    {
        char *input = readline("cheemscript> ");

        add_history(input);

        mpc_result_t r;
        if (mpc_parse("<stdin>", input, Cheemscript, &r))
        {
            long result = eval(r.output);
            printf("%li\n", result);
            mpc_ast_delete(r.output);
        }
        else
        {
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }

        free(input);
    }

    mpc_cleanup(4, Number, Operator, Expr, Cheemscript);
    return 0;
}

long eval(mpc_ast_t *t)
{

    // If node is tagged as a number return it directly
    if (strstr(t->tag, "number"))
    {
        return atoi(t->contents);
    }

    // First child is a parentheses, operator is the second child
    char *op = t->children[1]->contents;

    long x = eval(t->children[2]);

    int i = 3;
    while (strstr(t->children[i]->tag, "expr"))
    {
        x = eval_op(x, op, eval(t->children[i]));
        i++;
    }
    return x;
}

long eval_op(long x, char *op, long y)
{
    if (strcmp(op, "+") == 0)
    {
        return x + y;
    }
    if (strcmp(op, "-") == 0)
    {
        return x - y;
    }
    if (strcmp(op, "*") == 0)
    {
        return x * y;
    }
    if (strcmp(op, "/") == 0)
    {
        return x / y;
    }
    if (strcmp(op, "%") == 0)
    {
        return x % y;
    }
    return 0;
}