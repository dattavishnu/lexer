#include "parser.h"

int main() {
    Context ctx;
    ctx.vars["x"] = 42;
    ctx.vars["y"] = 42;
    Parser p("(x + y + 40 + (50 / 70))");
    Expr ast = p.parse();

    std::cout << ast->interpret(ctx) << std::endl; // 42

}