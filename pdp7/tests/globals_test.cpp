#include <fstream>

#include "fixture.h"

TEST_F(pdp7, global_var)
{
    auto output = compile(R"(
        foo;
    )");
    EXPECT_EQ(output, ".foo:0\n");
}

TEST_F(pdp7, global_line_1)
{
    auto output = compile(R"(
        line 1;
    )");
    EXPECT_EQ(output, ".line:1\n");
}

TEST_F(pdp7, global_peeksym_minus_1)
{
    auto output = compile(R"(
        peeksym -1;
    )");
    EXPECT_EQ(output, ".peeksym:-1\n");
}

TEST_F(pdp7, global_scalars)
{
    auto output = compile(R"(
        peekc;
        eof;
        csym;
        ns;
        cval;
        isn;
        nerror;
        nauto;
    )");
    const std::string expect = R"(.peekc:0
.eof:0
.csym:0
.ns:0
.cval:0
.isn:0
.nerror:0
.nauto:0
)";
    EXPECT_EQ(output, expect);
}

TEST_F(pdp7, global_symbuf)
{
    auto output = compile(R"(
        symbuf[10];
    )");
    const std::string expect = R"(.symbuf:.+1
.=.+10
)";
    EXPECT_EQ(output, expect);
}

TEST_F(pdp7, global_ctab)
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
    const std::string expect = R"(.ctab:.+1
0
127
127
127
0
127
127
127
127
126
126
127
127
127
127
127
127
127
127
127
127
127
127
127
127
127
127
127
127
127
127
127
126
34
122
127
127
44
47
121
6
7
42
40
9
41
127
43
124
124
124
124
124
124
124
124
124
124
8
1
63
80
65
90
127
123
123
123
123
123
123
123
123
123
123
123
123
123
123
123
123
123
123
123
123
123
123
123
123
123
123
4
127
5
48
127
127
123
123
123
123
123
123
123
123
123
123
123
123
123
123
123
123
123
123
123
123
123
123
123
123
123
123
2
48
3
127
127
)";
    EXPECT_EQ(output, expect);
}

TEST_F(pdp7, global_symtab)
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
    const std::string expect = R"(.symtab:.+1
1
5
97
117
116
111
0
1
6
101
120
116
114
110
0
1
10
103
111
116
111
0
1
11
114
101
116
117
114
110
0
1
12
105
102
0
1
13
119
104
105
108
101
0
1
14
101
108
115
101
0
.=.+249
)";
    EXPECT_EQ(output, expect);
}
