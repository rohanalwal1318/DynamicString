#ifndef DSTRING_H
#define DSTRING_H

#include<stdio.h>
#include<stddef.h>
#include<string.h>
#include<stdlib.h>
#include<stdarg.h>

typedef struct{

  char *data; /* heap allocated memory, always null terminated*/
  size_t length; /* number of characters in use, excluding '\0' character */
  size_t capacity; /* Total allocated size of data */

}String;

/* Note ---- size_t -> unsigned long long int has been used to hold larger integer data */

/* Create a new String from a C string .. use NULL for empty String*/
String *ds_new(const char *newString);

/* Append a single character to s. return s .. possibly reallocated!! */
String *ds_appendChar(String *s, const char c);

/* Append a String suffix to s. return s .. possibly reallocated!! */
String *ds_appendStr(String *s, const char *suffix);

/* Append using printf-style formatting. Generally sprintf. Return s, possibly reallocated*/
String *ds_sprintf(String *s, const char *fmt, ...);

/* Free a string and its buffer */
void ds_free(String *s);

/* Reset the length to 0, without freeing the buffer Keeps CAPACITY*/
void ds_clear(String *s);

/* Current Length in characters */
size_t ds_len(String *s);

/* Current Capacity */
size_t ds_cap(String *s);

/* Get the underlying null terminated string */
const char *ds_str(const String *s);

/* Compare two strings like strcmp */
int ds_strcmp(const String *a, const String *b);

/* Shrink the entire string to fit the exact length */
String *ds_shrinkToFit(String *s);

#endif