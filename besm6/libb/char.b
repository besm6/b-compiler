char(s, n) {
  if (n & 1) return(s[n/2] & 0777);
  return((s[n/2]/512) & 0777);
}
