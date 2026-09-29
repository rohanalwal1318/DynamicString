#include<assert.h>
#include "dstring.h"

static void test_new_cstr(){
  String *s = ds_new("Hello World!!");
  assert(strcmp(ds_str(s), "Hello World!!") == 0);
  ds_free(s);

  String *s1 = ds_new("abc");
  assert(strcmp(ds_str(s), "abc") == 0);
  assert(ds_len(s1) == 3);
  ds_free(s1);

  String *s2 = ds_new(NULL);
  assert(ds_len(s2) == 0);
  assert(strcmp(ds_str(s2), "") == 0);
  ds_free(s2);
}

static void test_append(){

  String *s = ds_new("x");
  for(int i=0; i<100; i++){
    ds_appendStr(s, "y");
  }

  assert(ds_len(s) == 101);
  assert(ds_cap(s) >= ds_len(s) + 1);

  ds_free(s);

}

static void test_appendChar(){

  String *s = ds_new("ab");
  ds_appendChar(s, 'c');
  assert(strcmp(ds_str(s), "abc") == 0);
  assert(ds_len(s) == 3);
  ds_free(s);

}

static void test_sprintf(){

  String *s = ds_new(NULL);
  ds_sprintf(s, "%d-%c", 10, 'A');
  assert(strcmp(ds_str(s), "10-A") == 0);
  assert(ds_len(s) == 4);
  assert(ds_cap(s) == 16);
  ds_free(s);

}

static void test_shrink_to_fit(){

  String *s = ds_new("Hello World!!");
  assert(ds_cap(s) == 16);
  ds_shrinkToFit(s);
  assert(ds_cap(s) == ds_len(s) + 1);
  ds_free(s);

}

int main(){
  test_new_cstr();
  test_append();
  test_appendChar();
  test_sprintf();
  test_shrink_to_fit();
  printf("Successfull !!\n");
}