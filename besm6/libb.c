//
// Implement read(), write(c), printf().
//
#include <stdarg.h>
#ifndef B_TYPE
    /* type representing B's single data type (64-bit int on x86_64) */
    #include <stdint.h>
    #define B_TYPE intptr_t
#endif
#ifndef B_FN
    /* this macro allows to give each B std function a pre- or postfix
       to avoid issues with common names
       e.g.: #define B_FN(name) __b##name <-- gives each B funcion the prefix "__b" */
    #define B_FN(name) name
#endif

//
// Convert Unicode character to KOI7 encoding.
// For details, see:
// https://ru.wikipedia.org/wiki/%D0%9A%D0%9E%D0%98-7#%D0%9A%D0%9E%D0%98-7_%D0%9D2
//
static unsigned char unicode_to_koi7(unsigned short val)
{
    static const unsigned char tab0[256] = {
        // clang-format off
        /* 00 - 07 */  0,    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        /* 08 - 0f */  0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
        /* 10 - 17 */  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        /* 18 - 1f */  0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
        /*  !"#$%&' */ 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
        /* ()*+,-./ */ 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
        /* 01234567 */ 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
        /* 89:;<=>? */ 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
        /* @ABCDEFG */ 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
        /* HIJKLMNO */ 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
        /* PQRSTUVW */ 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
        /* XYZ[\]^_ */ 0x58, 0x59, 0x5a, 0x5b, 0x1d, 0x5d, 0x5c, 0x5f,
        /* `abcdefg */ 0,    0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, //  ABCDEFG
        /* hijklmno */ 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, // HIJKLMNO
        /* pqrstuvw */ 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, // PQRSTUVW
        /* xyz{|}~  */ 0x58, 0x59, 0x5a, 0x0e, 0x5e, 0x0f, 0x1f, 0,    // XYZ≤|≥¬
        /* 80 - 87 */  0,    0,    0,    0,    0,    0,    0,    0,
        /* 88 - 8f */  0,    0,    0,    0,    0,    0,    0,    0,
        /* 90 - 97 */  0,    0,    0,    0,    0,    0,    0,    0,
        /* 98 - 9f */  0,    0,    0,    0,    0,    0,    0,    0,
        /* a0 - a7 */  0,    0,    0,    0,    0,    0,    0,    0,
        /* a8 - af */  0,    0,    0,    0,    0x1f, 0,    0,    0, // ¬
        /* b0 - b7 */  0x19, 0,    0,    0,    0,    0,    0,    0, // °
        /* b8 - bf */  0,    0,    0,    0,    0,    0,    0,    0,
        /* c0 - c7 */  0,    0,    0,    0,    0,    0,    0,    0,
        /* c8 - cf */  0,    0,    0,    0,    0,    0,    0,    0,
        /* d0 - d7 */  0,    0,    0,    0,    0,    0,    0,    0x06, // ×
        /* d8 - df */  0,    0,    0,    0,    0,    0,    0,    0,
        /* e0 - e7 */  0,    0,    0,    0,    0,    0,    0,    0,
        /* e8 - ef */  0,    0,    0,    0,    0,    0,    0,    0,
        /* f0 - f7 */  0,    0,    0,    0,    0,    0,    0,    0x1a, // ÷
        /* f8 - ff */  0,    0,    0,    0,    0,    0,    0,    0,
        // clang-format on
    };
    switch (val >> 8) {
    case 0x00:
        return tab0[val];
    case 0x04:
        switch ((unsigned char)val) {
        case 0x01:
            return 'E'; // Ë - 0x65
        case 0x04:
            return 'E'; // Ukrainian Є -> E - 0x65
        case 0x06:
            return 'I'; // Ukrainian І -> I - 0x73
        case 0x07:
            return 'I'; // Ukrainian Ї -> I - 0x73
        case 0x10:
            return 'A'; // А - 0x61
        case 0x11:
            return 0x62; // Б
        case 0x12:
            return 'B'; // В - 0x77
        case 0x13:
            return 0x67; // Г
        case 0x14:
            return 0x64; // Д
        case 0x15:
            return 'E'; // Е - 0x65
        case 0x16:
            return 0x76; // Ж
        case 0x17:
            return 0x7a; // З
        case 0x18:
            return 0x69; // И
        case 0x19:
            return 0x6a; // Й
        case 0x1a:
            return 'K'; // К - 0x6b
        case 0x1b:
            return 0x6c; // Л
        case 0x1c:
            return 'M'; // М - 0x6d
        case 0x1d:
            return 'H'; // Н - 0x6e
        case 0x1e:
            return 'O'; // О - 0x6f
        case 0x1f:
            return 0x70; // П
        case 0x20:
            return 'P'; // Р - 0x72
        case 0x21:
            return 'C'; // С - 0x73
        case 0x22:
            return 'T'; // Т - 0x74
        case 0x23:
            return 'Y'; // У - 0x75
        case 0x24:
            return 0x66; // Ф
        case 0x25:
            return 'X'; // Х - 0x68
        case 0x26:
            return 0x63; // Ц
        case 0x27:
            return 0x7e; // Ч
        case 0x28:
            return 0x7b; // Ш
        case 0x29:
            return 0x7d; // Щ
        case 0x2a:
            return 0x05; // Ъ
        case 0x2b:
            return 0x79; // Ы
        case 0x2c:
            return 0x78; // Ь
        case 0x2d:
            return 0x7c; // Э
        case 0x2e:
            return 0x60; // Ю
        case 0x2f:
            return 0x71; // Я
        case 0x30:
            return 'A'; // а - 0x61
        case 0x31:
            return 0x62; // б
        case 0x32:
            return 'B'; // в - 0x77
        case 0x33:
            return 0x67; // г
        case 0x34:
            return 0x64; // д
        case 0x35:
            return 'E'; // е - 0x65
        case 0x36:
            return 0x76; // ж
        case 0x37:
            return 0x7a; // з
        case 0x38:
            return 0x69; // и
        case 0x39:
            return 0x6a; // й
        case 0x3a:
            return 'K'; // к - 0x6b
        case 0x3b:
            return 0x6c; // л
        case 0x3c:
            return 'M'; // м - 0x6d
        case 0x3d:
            return 'H'; // н - 0x6e
        case 0x3e:
            return 'O'; // о - 0x6f
        case 0x3f:
            return 0x70; // п
        case 0x40:
            return 'P'; // р - 0x72
        case 0x41:
            return 'C'; // с - 0x73
        case 0x42:
            return 'T'; // т - 0x74
        case 0x43:
            return 'Y'; // у - 0x75
        case 0x44:
            return 0x66; // ф
        case 0x45:
            return 'X'; // х - 0x68
        case 0x46:
            return 0x63; // ц
        case 0x47:
            return 0x7e; // ч
        case 0x48:
            return 0x7b; // ш
        case 0x49:
            return 0x7d; // щ
        case 0x4a:
            return 0x05; // ъ
        case 0x4b:
            return 0x79; // ы
        case 0x4c:
            return 0x78; // ь
        case 0x4d:
            return 0x7c; // э
        case 0x4e:
            return 0x60; // ю
        case 0x4f:
            return 0x71; // я
        case 0x51:
            return 'E'; // ё - 0x65
        case 0x54:
            return 'E'; // Ukrainian є -> е - 0x65
        case 0x56:
            return 'I'; // Ukrainian і -> i - 0x73
        case 0x57:
            return 'I'; // Ukrainian ї -> i - 0x73
        case 0x90:
            return 0x67; // Ukrainian Ґ -> Г
        case 0x91:
            return 0x67; // Ukrainian ґ -> г
        }
        break;
    case 0x20:
        switch ((unsigned char)val) {
        case 0x15:
            return '\25'; // ―
        case 0x18:
            return '\20'; // ‘
        case 0x19:
            return '\33'; // ’
        case 0x28:
            return 0x0a;
        case 0x32:
            return '\'';
        case 0x3e:
            return '\\';
        }
        break;
    case 0x21:
        switch ((unsigned char)val) {
        case 0x2f:
            return 'E';
        case 0x91:
            return '\26'; // ↑ (was '@')
        }
        break;
    case 0x22:
        switch ((unsigned char)val) {
        case 0x27:
            return '&'; // ∧
        case 0x28:
            return '\36'; // ∨
        case 0x60:
            return '\30'; // ≠ (was '#')
        case 0x61:
            return '\35'; // ≡
        case 0x64:
            return '\16'; // ≤
        case 0x65:
            return '\17'; // ≥
        case 0x83:
            return '\34'; // ⊃
        }
        break;
    case 0x23:
        switch ((unsigned char)val) {
        case 0xe8:
            return '\27'; // ⏨
        }
        break;
    case 0x25:
        switch ((unsigned char)val) {
        case 0xc7:
            return '$';
        }
        break;
    }
    return 0;
}

//
// Syscall wrapper implementation
//

/* type used for syscalls */
#define SYSCALL_TYPE long

static inline SYSCALL_TYPE syscall1(SYSCALL_TYPE n, SYSCALL_TYPE a1)
{
	unsigned SYSCALL_TYPE ret;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1) : "rcx", "r11", "memory");
	return ret;
}

static inline SYSCALL_TYPE syscall3(SYSCALL_TYPE n, SYSCALL_TYPE a1, SYSCALL_TYPE a2, SYSCALL_TYPE a3)
{
	unsigned SYSCALL_TYPE ret;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3) : "rcx", "r11", "memory");
	return ret;
}

/* syscall ids */
#define SYS_read 0
#define SYS_write 1
#define SYS_exit 60

//
// B standard library implementation
//

/* The `main` function must be declared in any B program */
extern B_TYPE B_FN(main)(void);

/* entry point of any B program */
void _start(void) __asm__ ("_start"); /* assure, that _start is really named _start in asm */
void _start(void)
{
    B_TYPE code = B_FN(main)();
    syscall1(SYS_exit, code);
}

/* The current process is terminated. */
void B_FN(exit)(void)
{
    syscall1(SYS_exit, 0);
}

/* The i-th character of the string is returned */
B_TYPE B_FN(_char)(B_TYPE string, B_TYPE i) __asm__ ("char"); /* alias name */
B_TYPE B_FN(_char)(B_TYPE string, B_TYPE i)
{
    return ((char*) string)[i];
}

/* The character char is stored in the i-th character of the string. */
void B_FN(lchar)(B_TYPE string, B_TYPE i, B_TYPE chr)
{
    ((char*) string)[i] = chr;
}

//
// Select output stream for writeb().
// In this version of libb this feature is ignored.
// All output is sent to standard output, regardless of the value of fout.
//
B_TYPE fout = 0;

//
// One byte is written to the standard output.
//
void B_FN(writeb)(B_TYPE k)
{
    /* note: fout is ignored */
    syscall3(SYS_write, 1, (B_TYPE)&k, 1);
}

//
// One or more characters are written to the standard output.
//
void B_FN(write)(B_TYPE c)
{
    B_TYPE a;

    if ((a = (uintptr_t)c >> 8))
        B_FN(write)(a);

    writeb(c);
}

/* The following function will print a decimal number, possibly negative.
   This routine uses the fact that in the ANSCII character set,
   the digits O to 9 have sequential code values. */
void B_FN(printd)(B_TYPE n)
{
    B_TYPE a;

    if(n < 0) {
        B_FN(writeb)('-');
        n = -n;
    }

    if((a = n / 10))
        B_FN(printd)(a);
    B_FN(writeb)(n % 10 + '0');
}

/* The following function will print an unsigned number, n,
   to the base 8. */
void B_FN(printo)(B_TYPE n)
{
    B_TYPE a;

    if((a = (uintptr_t)n >> 3))
        B_FN(printo)(a);
    B_FN(writeb)((n & 7) + '0');
}

/* The following function is a general formatting, printing, and
   conversion subroutine. The first argument is a format string.
   Character sequences,of the form ‘%x’ are interpreted and cause
   conversion of type x’ of the next argument, other character
   sequences are printed verbatim. */
void B_FN(printf)(B_TYPE fmt, ...)
{
    B_TYPE x, c, i = 0, j;

    va_list ap;
    va_start(ap, fmt);
loop:
    while((c = B_FN(_char)(fmt, i++)) != '%') {
        if(c == '\0')
            goto end;
        B_FN(writeb)(c);
    }
    switch(c = B_FN(_char)(fmt, i++)) {
        case 'd': /* decimal */
        case 'D':
            x = va_arg(ap, B_TYPE);
            if(x < 0) {
                x = -x;
                B_FN(writeb)('-');
            }
            B_FN(printd)(x);
            goto loop;

        case 'o': /* octal */
        case 'O':
            x = va_arg(ap, B_TYPE);
            if(x < 0) {
                x = -x;
                B_FN(writeb)('-');
            }
            B_FN(printo)(x);
            goto loop;

        case 'c':
        case 'C':
            x = va_arg(ap, B_TYPE);
            B_FN(writeb)(x);
            goto loop;

        case 's':
        case 'S':
            x = va_arg(ap, B_TYPE);
            j = 0;
            while((c = B_FN(_char)(x, j++)) != '\0')
                B_FN(writeb)(c);
            goto loop;
        case '%':
            B_FN(writeb)('%');
            goto loop;
    }
    /* Unknown format. */
    B_FN(writeb)('%');
    i--;
    goto loop;

end:
    va_end(ap);
}

//
// The next character form the standard input file is returned.
// The character ‘*e’ is returned for an end-of-file.
//
B_TYPE B_FN(read)(void)
{
//printf((B_TYPE)"---read()\n");
    unsigned a = 0;
    if (syscall3(SYS_read, 0, (B_TYPE)&a, 1) != 1) {
        // End of file or i/o error.
        return 0;
    }
    if (!(a & 0x80)) {
        // Valid ASCII character.
//printf((B_TYPE)"<%c-%c>\n", a, unicode_to_koi7(a));
        return unicode_to_koi7(a);
    }

    unsigned b = 0;
    if (syscall3(SYS_read, 0, (B_TYPE)&b, 1) != 1) {
        // End of file or i/o error.
        return 0;
    }
    if (!(a & 0x20)) {
        // 10-bit character.
        a = (a & 0x1f) << 6 | (b & 0x3f);
//printf((B_TYPE)"<%c-%c>\n", a, unicode_to_koi7(a));
        return unicode_to_koi7(a);
    }

    unsigned c = 0;
    if (syscall3(SYS_read, 0, (B_TYPE)&c, 1) != 1) {
        // End of file or i/o error.
        return 0;
    }
    // 16-bit character.
    a = (a & 0x0f) << 12 | (b & 0x3f) << 6 | (c & 0x3f);
//printf((B_TYPE)"<%c-%c>\n", a, unicode_to_koi7(a));
    return unicode_to_koi7(a);
}

void B_FN(initdrum)(void)
{
    // Empty.
}

void B_FN(readdrum)(void)
{
    // Empty.
}
