/*
 * B compiler for BESM-6.
 *
 * Based on B compiler for PDP-7 Unix by Robert Swierczek.
 *
 * (C) 2016 Robert Swierczek, GPL3
 * (C) 2018, 2025 Serge Vakulenko
 */

main() {
  extrn symtab, eof, ns;

  while (!eof) {
    ns = &symtab[51];
    extdef();
    blkend();
  }
}

lookup() {
  extrn symtab, symbuf, eof, ns;
  auto np, sp, rp;

  rp = symtab;
  while (rp < ns) {
    np = &rp[2];
    sp = symbuf;
    while (*np == *sp) {
      if (!*np)
        return (rp);
      np = &np[1];
      sp = &sp[1];
    }
    while (*np)
      np = &np[1];
    rp = &np[1];
  }
  sp = symbuf;
  if (ns >= &symtab[590]) {
    error("Symbol table overflow");
    eof = 1;
    return (rp);
  }
  *ns = 0;
  ns[1] = 0;
  ns = &ns[2];
  while (*ns = *sp) {
    ns = &ns[1];
    sp = &sp[1];
  }
  ns = &ns[1];
  return (rp);
}

symbol() {
  extrn symbuf, ctab, peeksym, peekc, eof, line, csym, cval;
  auto b, c, ct, sp;

  if (peeksym >= 0) {
    c = peeksym;
    peeksym = -1;
    return (c);
  }
  if (peekc) {
    c = peekc;
    peekc = 0;
  } else {
    if (eof)
      return (0);
    c = read();
  }
loop:
  ct = ctab[c];

  if (ct == 0) { /* eof */
    eof = 1;
    return (0);
  }

  if (ct == 126) { /* white space */
    if (c == '*n')
      line = line+1;
    c = read();
    goto loop;
  }

  if (c == '=')
    return (subseq('=', 80, 60));

  if (c == '<') {
    if (subseq('=', 0, 1))
      return (62); /* <= */
    if (subseq('<', 0, 1))
      return (66); /* << */
    return (63); /* < */
  }
  if (c == '>') {
    if (subseq('=', 0, 1))
      return (64); /* >= */
    if (subseq('>', 0, 1))
      return (67); /* >> */
    return (65); /* > */
  }
  if (c == '!')
    return (subseq('=', 34, 61));

  if (c == '$') {
    if (subseq('(', 0, 1))
      return (2);
    if (subseq(')', 0, 1))
      return (3);
  }
  if (c == '/') {
    if (subseq('**', 1, 0))
      return (43);
com:
    c = read();
com1:
    if (c == 4) {
      eof = 1;
      error("Unexpected end of file");
      return (0);
    }
    if (c == '*n')
      line = line+1;
    if (c != '**')
      goto com;
    c = read();
    if (c != '/')
      goto com1;
    c = read();
    goto loop;
  }
  if (ct == 124) { /* number */
    cval = 0;
    if (c == '0')
      b = 8;
    else
      b = 10;
    while (c >= '0' & c <= '9') {
      cval = cval*b + c -'0';
      c = read();
    }
    peekc = c;
    return (21);
  }
  if (c == '*'') { /* ' */
    getcc();
    return (21);
  }
  if (ct == 123) { /* letter */
    sp = symbuf;
    while (ct == 123 | ct == 124) {
      if (sp < &symbuf[9]) {
        *sp = c;
        sp = &sp[1];
      }
      ct = ctab[c = read()];
    }
    *sp = 0;
    peekc = c;
    csym = lookup();
    if (csym[0] == 1) {
      cval = csym[1];
      return (19); /* keyword */
    }
    return (20); /* name */
  }
  if (ct == 127) { /* unknown */
    error("Invalid character");
    c = read();
    goto loop;
  }
  return (ctab[c]);
}

subseq(c, a, b) {
  extrn peekc;

  if (!peekc)
    peekc = read();
  if (peekc != c)
    return (a);
  peekc = 0;
  return (b);
}

getcc() {
  extrn cval;
  auto c;

  cval = 0;
  if ((c = mapch('*'')) < 0)
    return;
  cval = c;                     /* character #1 */
  if ((c = mapch('*'')) < 0)
    return;
  cval = (cval << 8) | c;       /* character #2 */
  if ((c = mapch('*'')) < 0)
    return;
  cval = (cval << 8) | c;       /* character #3 */
  if ((c = mapch('*'')) < 0)
    return;
  cval = (cval << 8) | c;       /* character #4 */
  if ((c = mapch('*'')) < 0)
    return;
  cval = (cval << 8) | c;       /* character #5 */
  if ((c = mapch('*'')) < 0)
    return;
  cval = (cval << 8) | c;       /* character #6 */
  if (mapch('*'') >= 0)
    error("Too long character literal");
}

getstr() {
  auto i, c;
  auto word;

  i = 1;
loop:
  if ((c = mapch('"')) < 0) {
    write('    ');
    write(',log,');
    printo(0);
    write('*n');
    return (i);
  }
  word = c;                     /* character #1 */
  if ((c = mapch('"')) < 0) {
    write('    ');
    write(',log,');
    printo(word << 40);
    write('*n');
    return (i);
  }
  word = (word << 8) | c;       /* character #2 */
  if ((c = mapch('"')) < 0) {
    write('    ');
    write(',log,');
    printo(word << 32);
    write('*n');
    return (i);
  }
  word = (word << 8) | c;       /* character #3 */
  if ((c = mapch('"')) < 0) {
    write('    ');
    write(',log,');
    printo(word << 24);
    write('*n');
    return (i);
  }
  word = (word << 8) | c;       /* character #4 */
  if ((c = mapch('"')) < 0) {
    write('    ');
    write(',log,');
    printo(word << 16);
    write('*n');
    return (i);
  }
  word = (word << 8) | c;       /* character #5 */
  if ((c = mapch('"')) < 0) {
    write('    ');
    write(',log,');
    printo(word << 8);
    write('*n');
    return (i);
  }
  write('    ');
  write(',log,');
  printo((word << 8) | c);      /* character #6 */
  write('*n');
  i = i+1;
  goto loop;
}

mapch(c) {
  extrn peekc;
  auto a;

  if ((a = read()) == c)
    return (-1);

  if (a == '*n' | a == 0 | a == 4) {
    error("Unterminated string");
    peekc = a;
    return (-1);
  }

  if (a == '**') {
    a = read();

    if (a == '0')
      return (0);

    if (a == 'e')
      return (4);

    if (a == '(')
      return ('{');

    if (a == ')')
      return ('}');

    if (a == 't')
      return ('*t');

    if (a == 'r')
      return ('*r');

    if (a == 'n')
      return ('*n');
  }
  return (a);
}

expr(lev) {
  extrn peeksym, csym, cval, isn, retflag, is_lvalue;
  auto o, narg;

  retflag = 0;
  o = symbol();

  if (o == 21) { /* number */
case21:
    gen_const(cval);
    goto loop;
  }

  if (o == 122) { /* string */
    gen_string();
    goto loop;
  }

  if (o == 20) { /* name */
    if (*csym == 0) { /* not seen */
      if ((peeksym = symbol()) == 6) { /* ( */
        *csym = 6; /* extrn */
        gen_subp();
      } else {
        *csym = 2; /* internal label */
        csym[1] = isn;
        isn = isn+1;
      }
    }
    if (*csym == 5) { /* auto */
      gen_auto(csym[1]);
    } else if (*csym == 8) { /* param */
      gen_param(csym[1]);
    } else if (*csym == 6) { /* extrn */
      gen_extrn();
    } else { /* internal label */
      gen_intern(csym[1]);
    }
    goto loop;
  }

  if (o == 34) { /* ! */
    expr(1);
    gen_rvalue();
    gen_helper('not'); /* logical not */
    goto loop;
  }

  if (o == 41) { /* - */
    peeksym = symbol();
    if (peeksym == 21) { /* number */
      peeksym = -1;
      cval = negate(cval);
      goto case21;
    }
    expr(1);
    gen_rvalue();
    write('    ');
    write(',x-a,*n');
    goto loop;
  }

  if (o == 47) { /* & */
    expr(1);
    assert_lvalue();
    is_lvalue = 0;
    goto loop;
  }

  if (o == 42) { /* * */
    expr(1);
    gen_rvalue();
    is_lvalue = 1;
    goto loop;
  }

  if (o == 6) { /* ( */
    peeksym = o;
    pexpr();
    goto loop;
  }
  error("Bad expression");

loop:
  o = symbol();

  if (lev >= 14 & o == 80) { /* = */
    assert_lvalue();
    expr(14);
    gen_rvalue();
    gen_assign();
    goto loop;
  }
  if (lev >= 10 & o == 48) { /* | ^ */
    gen_rvalue();
    expr(9);
    gen_rvalue();
    write('  15');
    write(',aox,');
    write('*n');
    goto loop;
  }
  if (lev >= 8 & o == 47) { /* & */
    gen_rvalue();
    expr(7);
    gen_rvalue();
    write('  15');
    write(',aax,');
    write('*n');
    goto loop;
  }
  if (lev >= 7 & o == 60) { /* == */
    gen_rvalue();
    expr(6);
    gen_rvalue();
    gen_helper('eq'); /* relational equal */
    goto loop;
  }
  if (lev >= 7 & o == 61) { /* != */
    gen_rvalue();
    expr(6);
    gen_rvalue();
    gen_helper('ne'); /* relational not equal */
    goto loop;
  }
  if (lev >= 6 & o == 62) { /* <= */
    gen_rvalue();
    expr(5);
    gen_rvalue();
    gen_helper('le'); /* relational less or equal */
    goto loop;
  }
  if (lev >= 6 & o == 63) { /* < */
    gen_rvalue();
    expr(5);
    gen_rvalue();
    gen_helper('lt'); /* relational less than */
    goto loop;
  }
  if (lev >= 6 & o == 64) { /* >= */
    gen_rvalue();
    expr(5);
    gen_rvalue();
    gen_helper('ge'); /* relational greater or equal */
    goto loop;
  }
  if (lev >= 6 & o == 65) { /* > */
    gen_rvalue();
    expr(5);
    gen_rvalue();
    gen_helper('gt'); /* relational greater than */
    goto loop;
  }
  if (lev >= 5 & o == 66) { /* << */
    gen_rvalue();
    expr(4);
    gen_rvalue();
    gen_lshacc(); /* shift left by accumulator */
    goto loop;
  }
  if (lev >= 5 & o == 67) { /* >> */
    gen_rvalue();
    expr(4);
    gen_rvalue();
    gen_rshacc(); /* shift right by accumulator */
    goto loop;
  }
  if (lev >= 4 & o == 40) { /* + */
    gen_rvalue();
    expr(3);
    gen_rvalue();
    write('  15');
    write(',a+x,');
    write('*n');
    goto loop;
  }
  if (lev >= 4 & o == 41) { /* - */
    gen_rvalue();
    expr(3);
    gen_rvalue();
    write('  15');
    write(',x-a,');
    write('*n');
    goto loop;
  }
  if (lev >= 3 & o == 42) { /* * */
    gen_rvalue();
    expr(2);
    gen_rvalue();
    gen_helper('mul'); /* multiply */
    goto loop;
  }
  if (lev >= 3 & o == 43) { /* / */
    gen_rvalue();
    expr(2);
    gen_rvalue();
    gen_helper('div'); /* divide */
    goto loop;
  }
  if (lev >= 3 & o == 44) { /* % */
    gen_rvalue();
    expr(2);
    gen_rvalue();
    gen_helper('mod'); /* modulo */
    goto loop;
  }
  if (o == 4) { /* [ */
    gen_rvalue();
    expr(15);
    gen_rvalue();
    if (symbol() != 5)
      error("Bad array index");
    write('  15');
    write(',arx,');
    write('*n');
    is_lvalue = 1;
    goto loop;
  }
  if (o == 6) { /* ( */
    assert_lvalue();
    o = symbol();
    if (o == 7) /* ) */
      gen_mcall();
    else {
      narg = 0;
      peeksym = o;
      while (o != 7) { /* not ) */
        expr(15);
        gen_rvalue();
        o = symbol();
        if (o != 7 & o != 9) { /* not ) or , */
          error("Bad function argument");
          return;
        }
        narg = narg + 1;
      }
      gen_call(narg);
    }
    is_lvalue = 0;
    goto loop;
  }

  peeksym = o;
}

pexpr() {
  if (symbol() == 6) { /* ( */
    expr(15);
    if (symbol() == 7) /* ) */
      return;
  }
  error("Unbalanced parentheses in expression");
}

declare(kw) {
  extrn csym, cval, nauto, nparam;
  auto o;

  while ((o = symbol()) == 20) { /* name */
    if (kw == 6) { /* extrn */
      *csym = 6;
      gen_subp();
      o = symbol();
    } else if (kw == 8) { /* param */
      *csym = 8; /* param */
      csym[1] = nparam;
      o = symbol();
      nparam = nparam+1;
    } else { /* auto */
      *csym = 5; /* auto */
      csym[1] = nauto;
      o = symbol();
      if (o == 21) { /* number */
        nauto = nauto + cval;
        o = symbol();
      }
      nauto = nauto+1;
    }
    if (o != 9) /* , */
      goto done;
  }
done:
  if (o == 1 & kw != 8 | o == 7 & kw == 8) /* auto/extrn ;  param ')' */
    return;

  error("Bad declaration syntax");
}

extdef() {
  extrn peeksym, csym, cval, nauto, nparam, retflag;
  auto o, c;

  o = symbol();
  if (o == 0 | o == 1) /* eof ; */
    return;

  if (o != 20) /* name */
    goto syntax;

  csym[0] = 6; /* extrn */
  o = symbol();

  if (o == 2 | o == 6) { /* { ( */
    gen_prolog(1);
    gen_entry();
    nauto = 0;
    nparam = 0;
    if (o == 6) { /* ( */
      declare(8); /* param */
      if ((o = symbol()) != 2) /* { */
        goto syntax;
    }
    gen_bsave();
    while ((o = symbol()) == 19 & cval < 10) /* auto extrn */
      declare(cval);
    peeksym = o;
    if (nauto > 0)
        gen_stackp(nauto); /* setop */
    stmtlist();
    if (! retflag)
        gen_bret(); /* return */
    gen_epilog();
    return;
  }

  if (o == 21 | o == 41) { /* number or - */
    if (o == 41) { /* - */
      if (symbol() != 21) { /* number */
        goto syntax;
      }
      cval = negate(cval);
    }
    gen_prolog(0);
    write('    ');
    write(',log,');
    printo(cval);
    write('*n');
    gen_epilog();
    return;
  }

  if (o == 1) { /* ; */
    gen_prolog(0);
    write('    ');
    write(',bss,1');
    write('*n');
    gen_epilog();
    return;
  }

  if (o == 4) { /* [ */
    c = 0;
    if ((o = symbol()) == 21) { /* number */
      c = cval;
      o = symbol();
    }
    if (o != 5) /* ] */
      goto syntax;
    gen_prolog(0);
    write('    ');
    write(',z00,*n');
    write('    ');
    write(',z00,');
    write('**+1*n');
    if ((o = symbol()) == 1) /* ; */
      goto done;
    while (o == 21 | o == 41) { /* number - */
      if (o == 41) { /* - */
        if (symbol() != 21) {
          goto syntax;
        }
        cval = negate(cval);
      }
      write('    ');
      write(',log,');
      printo(cval);
      write('*n');
      c = c-1;
      if ((o = symbol()) == 1) /* ; */
        goto done;
      if (o != 9) /* , */
        goto syntax;
      else
        o = symbol();
    }
    goto syntax;
done:
    if (c > 0) {
      write('    ');
      write(',bss,');
      number(c);
      write('*n');
    }
    gen_epilog();
    return;
  }

  if (o == 0) /* eof */
    return;

syntax:
  error("Bad external declaration");
  stmt();
}

stmtlist() {
  extrn peeksym, eof;
  auto o;

  while (!eof) {
    if ((o = symbol()) == 3) /* } */
      return;
    peeksym = o;
    stmt();
  }
  error("Missing closing brace"); /* missing } */
}

stmt() {
  extrn peeksym, peekc, csym, cval, isn, nauto, retflag, acc_active;
  auto o, o1, o2;

next:
  acc_active = 0;
  o = symbol();

  if (o == 0) { /* eof */
    error("Unexpected end of file");
    return;
  }

  if (o == 1 | o == 3) /* ; } */
    return;

  if (o == 2) { /* { */
    stmtlist();
    return;
  }

  if (o == 19) { /* keyword */

    if (cval == 10) { /* goto */
      expr(15);
      gen_goto(); /* goto */
      goto semi;
    }

    if (cval == 11) { /* return */
      if ((peeksym = symbol()) == 6) /* ( */
        pexpr();
      gen_bret(); /* return */
      retflag = 1;
      goto semi;
    }

    if (cval == 12) { /* if */
      pexpr();
      gen_rvalue();
      o1 = isn;
      isn = isn+1;
      jumpc(o1);
      stmt();
      o = symbol();
      if (o == 19 & cval == 14) { /* else */
        o2 = isn;
        isn = isn+1;
        jump(o2);
        label(o1);
        stmt();
        label(o2);
        return;
      }
      peeksym = o;
      label(o1);
      return;
    }

    if (cval == 13) { /* while */
      o1 = isn;
      isn = isn+1;
      label(o1);
      pexpr();
      gen_rvalue();
      o2 = isn;
      isn = isn+1;
      jumpc(o2);
      stmt();
      jump(o1);
      label(o2);
      return;
    }

    error("Unrecognized keyword");
    goto syntax;
  }

  if (o == 20 & peekc == ':') { /* name : */
    peekc = 0;
    if (!*csym) {
      *csym = 2; /* internal label */
      csym[1] = isn;
      isn = isn+1;
    } else if (*csym != 2) {
      error("Label redefined");
      printf("At name: ");
      name(&csym[2]);
      goto next;
    }
    label(csym[1]);
    goto next;
  }

  peeksym = o;
  expr(15);
  gen_rvalue();

semi:
  o = symbol();
  if (o == 1) /* ; */
    return;

syntax:
  error("Bad statement syntax");
  goto next;
}

negate(n) {
  /* mantissa and sign, 41 bits */
  return (-n & 037777777777777);
}

blkend() {
  extrn isn;
  /* auto i; */

  if (!isn)
    return;
  /* Table of labels is not needed for BESM-6.
  write('1:');
  i = 0;
  while (i < isn) {
    write('l');
    number(i);
    write('*n');
    i = i+1;
  } */
  isn = 0;
}

assert_lvalue() {
  /* make sure accumulator has lvalue */
  extrn acc_active, is_lvalue;
  if (!acc_active) {
    error("Inactive accumulator");
  } else if (!is_lvalue) {
    error("Lvalue needed");
  }
}

gen_prolog(based) {
  /* start Madlen module */
  extrn csym;

  write(' ');
  name(&csym[2]);
  write(':');
  if (based) {
    /* globally based by registers 8,9,10 */
    write('8');
  }
  write(',name,');
  write('*n');
}

gen_entry() {
  extrn csym;

  if (csym[2] == 'm' & csym[3] == 'a' & csym[4] == 'i' & csym[5] == 'n' & csym[6] == 0) {
    write(' prog');
    write('ram:');
    write(',entry');
    write(',*n');

    /* global basing */
    write('   8');
    write(',vtm,2');
    write('0000b*n');
    write('   9');
    write(',vtm,4');
    write('0000b*n');
    write('  10');
    write(',vtm,6');
    write('0000b*n');
  }

  /* for recursive calls */
  write(' ');
  name(&csym[2]);
  write(':,bss,');
  write('*n');
}

gen_epilog() {
  /* finish Madlen module */
  write('    ');
  write(',end,');
  write('*n');
}

gen_subp() {
  extrn csym;

  write(' ');
  name(&csym[2]);
  write(':,subp');
  write(',*n');
}

/* Put address of external name on accumulator. */
gen_extrn() {
  extrn csym, acc_active, is_lvalue;

  write('  14');
  write(',vtm,');
  name(&csym[2]);
  write('*n');

  write('    ');
  if (acc_active)
    write(',its,');
  else
    write(',ita,');
  write('14*n');

  acc_active = 1;
  is_lvalue = 1;
}

gen_intern(n) {
  extrn acc_active, is_lvalue;

  write('  14');
  write(',vtm,/');
  number(n);
  write('*n');

  write('    ');
  if (acc_active)
    write(',its,');
  else
    write(',ita,');
  write('14*n');

  acc_active = 1;

  /* treat address of label or string as a regular number */
  is_lvalue = 0;
}

gen_const(n) {
  extrn acc_active, is_lvalue;

  write('    ');
  if (acc_active)
    write(',xts,=');
  else
    write(',xta,=');
  printo(n);
  write('*n');

  acc_active = 1;
  is_lvalue = 0;
}

gen_bsave() {
  /* call b/save or b/save0 */
  extrn nparam;

  write(' b/sav');
  if (nparam == 0)
    write('e0:');
  else
    write('e:');
  write(',subp,');
  write('*n');
  write(' b/ret');
  write(':,subp');
  write(',*n');

  write('    ');
  write(',its,');
  write('13*n');
  write('    ');
  write(',call,');
  write('b/');
  if (nparam == 0)
    write('save0*n');
  else
    write('save*n');
}

gen_bret() {
  /* call b/ret */
  gen_rvalue();
  write('    ');
  write(',uj, ');
  write('b/ret*n');
}

gen_stackp(n) {
  /* increment stack pointer, allocate space for auto variables */
  write('  15');
  write(',utm,');
  number(n);
  write('*n');
}

gen_mcall() {
  /* call without parameters */
  extrn acc_active;

  assert_lvalue();
  write('    ');
  write(',ati,');
  write('14*n');

  write('  14');
  write(',utc,*n');

  write('  13');
  write(',vjm,*n');
  acc_active = 1;
}

gen_call(narg) {
  /* call with parameters */
  extrn acc_active;

  write('  14');
  write(',vtm,');
  number(-narg);
  write('*n');

  if (narg > 1) {
    write('  15');
    write(',wtc,');
    number(-narg);
    write('*n');
  } else {
    write('  15');
    write(',wtc,*n');
  }
  write('  13');
  write(',vjm,*n');
  if (narg > 1) {
    write('  15');
    write(',utm,');
    write('-1*n');
  }
  acc_active = 1;
}

gen_goto() {
  /* jump to address */
  extrn acc_active;

  write('    ');
  write(',ati,');
  write('14*n');

  write('  14');
  write(',uj,');
  write('*n');

  acc_active = 0;
}

gen_helper(name) {
  /* call helper routine */
  write('    ');
  write(',call,');
  write('b/');
  write(name);
  write('*n');
}

gen_auto(offset) {
  /* get address of auto variable */
  extrn acc_active, is_lvalue;

  write('   7');
  write(',utc,');
  number(offset);
  write('*n');

  write('  14');
  write(',vtm,*n');

  write('    ');
  if (acc_active)
    write(',its,');
  else
    write(',ita,');
  write('14*n');

  acc_active = 1;
  is_lvalue = 1;
}

gen_param(offset) {
  /* get address of function parameter */
  extrn acc_active, is_lvalue;

  write('   6');
  write(',utc,');
  number(offset);
  write('*n');

  write('  14');
  write(',vtm,*n');

  write('    ');
  if (acc_active)
    write(',its,');
  else
    write(',ita,');
  write('14*n');

  acc_active = 1;
  is_lvalue = 1;
}

gen_assign() {
  write('  15');
  write(',wtc,*n');
  write('    ');
  write(',atx,*n');
}

gen_rvalue() {
  /* when accumulator contains lvalue - fetch rvalue */
  extrn is_lvalue;

  if (is_lvalue) {
    write('    ');
    write(',ati,');
    write('14*n');
    write('  14');
    write(',xta,*n');
    is_lvalue = 0;
  }
}

gen_string() {
  extrn isn;
  auto str_id, skip_id;

  str_id = isn;
  isn = isn + 1;
  skip_id = isn;
  isn = isn + 1;
  jump(skip_id);
  label(str_id);
  getstr();
  label(skip_id);
  gen_intern(str_id);
}

gen_lshift(n) {
  /* shift left by constant */
  write('    ');
  write(',asn,');
  write('64-');
  number(n & 077);
  write('*n');
}

gen_rshift(n) {
  /* shift right by constant */
  write('    ');
  write(',asn,');
  write('64+');
  number(n & 077);
  write('*n');
}

gen_lshacc() {
  /* shift left by accumulator */
  write('    ');
  write(',aex,');
  write('=177*n');
  write('    ');
  write(',sti,');
  write('14*n');
  write('  14');
  write(',asn,');
  write('64+1*n');
}

gen_rshacc() {
  /* shift right by accumulator */
  write('    ');
  write(',sti,');
  write('14*n');
  write('  14');
  write(',asn,');
  write('64*n');
}

jumpc(n) {
  write('    '); /* ifop */
  write(',uza,/');
  number(n);
  write('*n');
}

jump(n) {
  write('    ');
  write(',uj,/');
  number(n);
  write('*n');
}

label(n) {
  write(' /');
  number(n);
  write(':,bss,');
  write('*n');
}

number(x) {
  printd(x);
}

name(s) {
  auto c;
  while ((c = *s)) {
    /* replace _ with * for Madlen */
    if (c == '_')
      c = '**';
    write(c);
    s = &s[1];
  }
}

error(msg) {
  extrn line, eof, nerror;

  if (eof | nerror == 20) {
    eof = 1;
    return;
  }
  nerror = nerror + 1;
  printf("Error at line %d: %s*n", line, msg);
}

/* storage */

symtab[600] /* class value name */
  1, 5,'a','u','t','o', 0 ,
  1, 6,'e','x','t','r','n', 0 ,
  1,10,'g','o','t','o', 0 ,
  1,11,'r','e','t','u','r','n', 0 ,
  1,12,'i','f', 0 ,
  1,13,'w','h','i','l','e', 0 ,
  1,14,'e','l','s','e', 0 ;

ctab[]
    0,127,127,127,  0,127,127,127,  /* NUL SOH STX ETX EOT ENQ ACK BEL */
  127,126,126,127,127,127,  2,  3,  /* BS  TAB LF  VT  FF  CR  SO  SI  */
  127,127,127,127,127,127,127,127,  /* DLE DC1 DC2 DC3 DC4 NAK SYN ETB */
  127,127,127,127,127,127,127,127,  /* CAN EM  SUB ESC FS  GS  RS  US  */
  126, 34,122,127,127, 44, 47,121,  /* SPC  !   "   #   $   %   &   '  */
    6,  7, 42, 40,  9, 41,127, 43,  /*  (   )   *   +   ,   -   .   /  */
  124,124,124,124,124,124,124,124,  /*  0   1   2   3   4   5   6   7  */
  124,124,  8,  1, 63, 80, 65, 90,  /*  8   9   :   ;   <   =   >   ?  */
  127,123,123,123,123,123,123,123,  /*  @   A   B   C   D   E   F   G  */
  123,123,123,123,123,123,123,123,  /*  H   I   J   K   L   M   N   O  */
  123,123,123,123,123,123,123,123,  /*  P   Q   R   S   T   U   V   W  */
  123,123,123,  4,127,  5, 48,123,  /*  X   Y   Z   [   \   ]   ^   _  */
  127,123,123,123,123,123,123,123,  /*  `   a   b   c   d   e   f   g  */
  123,123,123,123,123,123,123,123,  /*  h   i   j   k   l   m   n   o  */
  123,123,123,123,123,123,123,123,  /*  p   q   r   s   t   u   v   w  */
  123,123,123,  2, 48,  3,127,127;  /*  x   y   z   {   |   }   ~  DEL */

symbuf[10]; /* The name currently being parsed */
peeksym -1; /* A token returned 'back' into input stream */
peekc;      /* Next character of input after the last parsed token */
eof;        /* Flag: on end of input file */
line 1;     /* Current line number in the source file */
csym;       /* Current symbol: pointer into symtab[] for name or keyword */
ns;         /* Next symbol: pointer to the free space of symtab[] */
cval;       /* Numeric value of last parsed literal or keyword */
isn;        /* Unique internal symbol number */
nerror;     /* Count of errors in the program */
nauto;      /* How many auto variables allocated by current function */
nparam;     /* How many parameters in the current function */
retflag;    /* Flag: last statement was 'return' */
acc_active; /* Flag: accumulator is currently in use */
is_lvalue;  /* Flag: accumulator holds address, not actual value */
