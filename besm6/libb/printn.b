putnum(n, b) {
  auto a, d;

  d = 0;
  if (n < 0) {
    n = -n;
    if (n < 0) {
      n = n-1;
      d = 1;
    } else
      putchar('-');
  }
  if (a = n/b)
    putnum(a, b);
  putchar(n%b + '0' + d);
}
