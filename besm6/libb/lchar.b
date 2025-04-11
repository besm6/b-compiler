lchar(s, n, c) {
  if (n & 1) s[n/2] = (s[n/2] & 0777000) | c;
  else s[n/2] = (s[n/2] & 0777) | (c*512);
  return(c);
}
