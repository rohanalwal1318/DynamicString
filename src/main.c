#include "dstring.h"

int main(){

  /* Start with a new string .. to see how it behaves */
  String *s = ds_new("Hello");

  printf("Initial:- %s\nLength:- %zu\nCapacity: %zu\n", ds_str(s), ds_len(s), ds_cap(s));

  /* Try to append Strings and char */
  ds_appendStr(s, ", ");
  ds_appendStr(s, "World");
  ds_appendChar(s, '!');

  printf("Appended:- %s\nLength:- %zu\nCapacity:- %zu\n", ds_str(s), ds_len(s), ds_cap(s));

  /* Append more string so that to check Capacity is increasing or not */
  for(int i=0; i<5; i++){
    ds_appendStr(s, " more-text");
  }

  printf("Grown:- %s\nLength:- %zu\nCapacity:- %zu\n", ds_str(s), ds_len(s), ds_cap(s));

  /* Printf-style formatting the string .. Variadic Function */
  String *format = ds_new(NULL); /* Empty String should be sent by NULL */
  ds_sprintf(format, "%d %c %d -> %d\n", 10, '+', 20, 30);
  printf("Formatted:- %s\n", ds_str(format));

  /* Once you are done doing everything, Shrink to fit the string, trim wasted memory */
  ds_shrinkToFit(s);
  printf("Capacity:- %zu\n", ds_cap(s));

  ds_free(s);
  ds_free(format);

}