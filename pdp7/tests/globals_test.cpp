#include <fstream>

#include "fixture.h"

TEST_F(pdp7, global_var)
{
    auto output = compile(R"(
        foo;
    )");
    EXPECT_EQ(output, ".foo:0\n");
}

TEST_F(pdp7, DISABLED_global_scalars)
{
    auto output = compile(R"(
        peeksym -1;
        peekc;
        eof;
        line 1;
        csym;
        ns;
        cval;
        isn;
        nerror;
        nauto;
    )");
    const std::string expect = R"(
TODO
)";
    EXPECT_EQ(output, expect);
}

TEST_F(pdp7, DISABLED_global_symbuf)
{
    auto output = compile(R"(
        symbuf[10];
    )");
    const std::string expect = R"(
TODO
)";
    EXPECT_EQ(output, expect);
}

TEST_F(pdp7, DISABLED_global_ctab)
{
    auto output = compile(R"(
        ctab[]
            0,127,127,127,  0,127,127,127,  /* NUL SOH STX ETX EOT ENQ ACK BEL */
          127,126,126,127,127,127,127,127,  /* BS  TAB LF  VT  FF  CR  SO  SI  */
          127,127,127,127,127,127,127,127,  /* DLE DC1 DC2 DC3 DC4 NAK SYN ETB */
          127,127,127,127,127,127,127,127,  /* CAN EM  SUB ESC FS  GS  RS  US  */
          126, 34,122,127,127, 44, 47,121,  /* SPC  !   "   #   $   %   &   '  */
            6,  7, 42, 40,  9, 41,127, 43,  /*  (   )   *   +   ,   -   .   /  */
          124,124,124,124,124,124,124,124,  /*  0   1   2   3   4   5   6   7  */
          124,124,  8,  1, 63, 80, 65, 90,  /*  8   9   :   ;   <   =   >   ?  */
          127,123,123,123,123,123,123,123,  /*  @   A   B   C   D   E   F   G  */
          123,123,123,123,123,123,123,123,  /*  H   I   J   K   L   M   N   O  */
          123,123,123,123,123,123,123,123,  /*  P   Q   R   S   T   U   V   W  */
          123,123,123,  4,127,  5, 48,127,  /*  X   Y   Z   [   \   ]   ^   _  */
          127,123,123,123,123,123,123,123,  /*  `   a   b   c   d   e   f   g  */
          123,123,123,123,123,123,123,123,  /*  h   i   j   k   l   m   n   o  */
          123,123,123,123,123,123,123,123,  /*  p   q   r   s   t   u   v   w  */
          123,123,123,  2, 48,  3,127,127;  /*  x   y   z   {   |   }   ~  DEL */
    )");
    const std::string expect = R"(
TODO
)";
    EXPECT_EQ(output, expect);
}

TEST_F(pdp7, DISABLED_global_symtab)
{
    auto output = compile(R"(
        symtab[300] /* class value name */
          1, 5,'a','u','t','o', 0 ,
          1, 6,'e','x','t','r','n', 0 ,
          1,10,'g','o','t','o', 0 ,
          1,11,'r','e','t','u','r','n', 0 ,
          1,12,'i','f', 0 ,
          1,13,'w','h','i','l','e', 0 ,
          1,14,'e','l','s','e', 0 ;
    )");
    const std::string expect = R"(
TODO
)";
    EXPECT_EQ(output, expect);
}
