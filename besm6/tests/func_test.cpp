#include <fstream>

#include "fixture.h"

TEST_F(besm6, function_definitions)
{
    auto output = compile_and_run(R"(
        a() {}
        b {}
        c { label:; }
        d { label: goto label; }
        e { return; }
        f(x) { return(x); }
        g(x) { x; }
        h(x) { if(x) 123; }
        i(x) { if(x) 123; else 456; }
        j(x) { while(x); }
        m { extrn x; }
        n { auto x; }
        x;

        main() {
            printf("before a()*n");
            a();    printf("after a(), before b()*n");
            b();    printf("after b(), before c()*n");
            c();    printf("after c(), before e()*n");
            e();    printf("after e(), before f()*n");
            f(42);  printf("after f(), before g()*n");
            g(42);  printf("after g(), before h()*n");
            h(42);  printf("after h(), before i()*n");
            i(42);  printf("after i(), before j()*n");
            j(0);   printf("after j(), before m()*n");
            m(42);  printf("after m(), before n()*n");
            n(42);  printf("after n()*n");
        }
    )");
    const std::string expect = R"(BEFORE A()
AFTER A(), BEFORE B()
AFTER B(), BEFORE C()
AFTER C(), BEFORE E()
AFTER E(), BEFORE F()
AFTER F(), BEFORE G()
AFTER G(), BEFORE H()
AFTER H(), BEFORE I()
AFTER I(), BEFORE J()
AFTER J(), BEFORE M()
AFTER M(), BEFORE N()
AFTER N()
)";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, function_arguments)
{
    auto output = compile_and_run(R"(
        func(a, b, c)
        {
            printf("a = %d, b = '%c', c = *"%s*"*n", a, b, c);
        }

        main() {
            func(123, 'foo', "bar");
        }
    )");
    const std::string expect = R"(A = 123, B = 'FOO', C = "BAR"
)";
    EXPECT_EQ(output, expect);
}

//
// Ternary operator ?: is not supported by PDP-7 version of B compiler.
//
TEST_F(besm6, ternary_operator)
{
    auto output = compile_and_run(R"(
        choose(a, b, c) {
            return (a ? b : c);
        }
        main() {
            printf("%d*n", choose(1, 123, 456));
            printf("%d*n", choose(0, 123, 456));
        }
    )");
    const std::string expect = R"(123
456
)";
    EXPECT_EQ(output, expect);
}
