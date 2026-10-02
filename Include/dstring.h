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

typedef enum {
    DS_OK        =  0,  /* success                                    */
    DS_ERR_NOMEM = -1,  /* out of memory, or size would overflow      */
    DS_ERR_ARG   = -2   /* invalid argument (e.g. NULL String)        */
} Status;

/* Create a new String from a C string .. use NULL for empty String*/
String *ds_new(const char *newString);

/* Append a single character to s. return s .. possibly reallocated!! */
Status ds_appendChar(String *s, const char c);

/* Append a String suffix to s. return s .. possibly reallocated!! */
Status ds_appendStr(String *s, const char *suffix);

/* Append using printf-style formatting. Generally sprintf. Return s, possibly reallocated*/
Status ds_sprintf(String *s, const char *fmt, ...);

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
Status ds_shrinkToFit(String *s);

/* ------- Version 0.2 --------- */

/* Find the first occurence of the needle in the haystack, Returns the index if found or else -1 */
long ds_find(const String *s, const char *needle);

/* Return 1 if s contains needle or else 0 */
int ds_contains(const String *s, const char *needle);

/* Return 1 if s starts with prefix, 0 otherwise */
int ds_startsWith(const String *s, const char *prefix);

/* Return 1 if s ends with suffix, 0 otherwise */
int ds_endsWith(const String *s, const char *suffix);

/* Returning a brand NEW string containing len characters of s from start .. clamped to what's available */
String *ds_substr(const String *s, size_t start, size_t len);

/*Insert Str in s at position pos, shifting the rest of s to the right */
Status ds_insertStr(String *s, size_t pos, const char *str);

/* Replace the first occurrence of find with replace .. DS_ERR_ARG if find not found!! */
Status ds_replaceStr(String *s, const char *find, const char *replace);

#endif