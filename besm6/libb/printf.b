printf(fmt,x1,x2,x3,x4,x5,x6,x7,x8,x9) {
  auto adx, a, c, i, n;
  
  i = 0;
  adx = &x1;
loop:
  while ((c = char(fmt, i)) != '%') {
    if (c == '*e')
      return;
    putchar(c);
    i = i+1;
  }
  i = i+1;
  a = *adx;
  c = char(fmt, i);
  if (c=='d')
    putnum(a, 10);
  else if (c=='o') 
    putnum(a, 8);
  else if (c=='c')
    putchar(a);
  else if (c=='s') {
    n = 0;
    while ((c = char(a, n)) != '*e') {
      putchar(c);
      n = n+1;
    }
  } else {
    putchar('%');
    goto loop;
  }
  i = i+1;
  adx = adx+1;
  goto loop;
}
