#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int compare(const void *l, const void *r) {
 printf("compare: %d, left: %c, right: %c\n", ((*(char *)r) - (*(char *)l)), (*(char *)l), (*(char *)r));
 return (*(char *)r) - (*(char *)l);
}

void stringSortQS(char *s) {
 printf("length: %d\n", strlen(s));
 qsort(s, strlen(s), sizeof(char), compare);
}

void merge(char *l, const int ll, char *r, const int rl, char *s, const int sl) {
 int li = 0, ri = 0, si = 0;
 while(li < ll && ri < rl) {
  if(l[li] > r[ri]) s[si++] = l[li++];
  else s[si++] = r[ri++];
 }
 while(li < ll) s[si++] = l[li++];
 while(ri < rl) s[si++] = r[ri++];
}

void stringSortMS(char *s, const int len) {
 int i;
 const int m = len / 2, rl = len - m;
 char res[len + 1], l[m + 1], r[rl + 1]; 
 if(len <= 1) return;
 for(i = 0; i < len; ++i) {
  if(i < m) l[i] = s[i];
  else r[i - m] = s[i];
 }
 res[len] = l[m] = r[rl] = '\0';
 printf("pivot: %d, left: %s, right: %s\n", m, l, r);
 stringSortMS(l, m);
 stringSortMS(r, rl);
 merge(l, m, r, rl,  res, len);
 strncpy(s, res, len);
}

int main() {
 char s[] = "The quick brown fox jumps over the lazy dog.";
#if 0
 stringSortQS(s);
#else
 stringSortMS(s, strlen(s));
#endif
 printf("'%s'\n", s);
 return 0;
}
