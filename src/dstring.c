#include "dstring.h"

#define DS_INITIAL_CAPACITY 16

/* Create a String, calculate length (Safer when user want's to create a null or empty string), define capacity, allocate memory for s->data using malloc, use memcpy to copy the provide *newStr to s->data, update null character, length and capacity of a String*/

String *ds_new(const char *newStr){

  String *s = malloc(sizeof(String)); /* allocate memory to create String  */

  if(!s) return NULL; /* Failed to allocate */

  size_t len = newStr ? strlen(newStr) : 0; /* On safe side to tackle Empty String*/
  
  size_t cap = len + 1 > DS_INITIAL_CAPACITY ? len + 1 : DS_INITIAL_CAPACITY; /* Safer side to calculate capacity when user wants to create a empty string*/

  s->data = malloc(cap); /* Enough to store all the Chars*/

  if(!s->data) return NULL; /* Failed to allocate */

  if(len > 0){
    memcpy(s->data, newStr, len); /* Copy the new string */
  }

  s->data[len] = '\0'; /* Always null terminated String */
  s->length = len; /* Update length */
  s->capacity = cap; /* Update Capacity */

  return s;

}

/* Ensure Capacity is already fits than needed or else reallocate generally Geometric Growth*/

static int ds_ensureCapacity(String *s, size_t needed){

  if(needed + 1 <= s->capacity) return 1; /* Already enough, Including null character */

  size_t newCap = s->capacity == 0 ? DS_INITIAL_CAPACITY : s->capacity; /* Define Capacity */

  while(newCap < needed + 1){
    newCap *= 2;  /* Geometric growth (Double) */
  }

  char *newData = realloc(s->data, newCap); /* New Capacity abled string */

  if(!newData) return 0; /* Reallocation Failed */

  s->data = newData; /* Update reallocated new string */
  s->capacity = newCap; /* Update new capacity */

  return 1;

}


/* Ensure the Capacity is enough, Eventually grow in to fit the character, append the character at the end of the day! */

String *ds_appendChar(String *s, const char c){

  if(!s) return s; /* Empty String */

  if(!ds_ensureCapacity(s, s->length + 1)){ /* Ensure capacity is enough for    the overall -> present str + 1 char */
    return s; /* Failed to Enusre capacity .. leave s untouched */
  }

  /* Append a single character at the end with a null character */
  s->data[s->length] = c;
  s->length += 1;
  s->data[s->length] = '\0';

  return s;

}

/* Append suffix string to current string .. first ensure capacity, Use memcpy to append suffix string to current string return string possibly reallocated */

String *ds_appendStr(String *s, const char *suffix){

  if(!s || !suffix) return s;

  size_t suffLen = strlen(suffix); /* Length of the suffix String */

  if(suffLen == 0) return s;

  if(!ds_ensureCapacity(s, s->length + suffLen)) /* Ensure the capacity length + suffixLength*/
  {
    return s; /* Failed to reallocate */
  }

  memcpy(s->data + s->length, suffix, suffLen); /* Copy the suffix String to og string */

  s->length += suffLen; /* Length = s->length + suffixLen */
  s->data[s->length] = '\0'; /* Always Null terminated String */

  return s; /* Remember s is reallocated to a new memory */

}

/* A variadic Function uses vsnprintf() to allow printf-style formatting to the string, create two argument pointers .. one for calculating size (needed) and another for actual decoding, use vsnprintf() to write to the string*/

String *ds_sprintf(String *s, const char *fmt, ...){

  if(!s || !fmt) return s;

  va_list ap;
  va_start(ap, fmt);
  va_list apCopy;
  va_copy(apCopy, ap);

  int needed = vsnprintf(NULL, 0, fmt, ap); /* Calculate total size needed */
  va_end(ap);

  if(needed < 0){
    va_end(apCopy);
    return s; /* Encoding error */
  }

  if(!ds_ensureCapacity(s, (size_t)needed)){
    va_end(apCopy);
    return s; /* Reallocation Failed !! */
  }

  vsnprintf(s->data + s->length, (size_t)needed + 1, fmt, apCopy); /* Write the formatted string to original string */
  va_end(apCopy);

  s->length += (size_t)needed; /* Update new length */

  return s;

}

/* Free's the entire string structure and its pointed buffer */

void ds_free(String *s){
  if(!s) return;
  free(s->data); /* Free buffer first */
  free(s);
}

/* Reset the length to 0 .. keep capacity and buffer as it is */

void ds_clear(String *s){
  if(!s) return;
  s->length = 0;
}

/* Return the length */
size_t ds_len(String *s){
  return s ? s->length : 0;
}

/* Return the current capacity */
size_t ds_cap(String *s){
  return s ? s->capacity : 0;
}

/* Return the string pointed to by data */
const char *ds_str(const String *s){
  return s ? s->data : "";
}

/* Compare two strings by using strcmp function */
int ds_strcmp(const String *a, const String *b){
  return strcmp(a->data, b->data);
}

/* Find the exact length, reallocate and return s, possibly reallocated */
String *ds_shrinkToFit(String *s){
  
  if(!s) return s;

  size_t exact = ds_len(s) + 1; /* Calculate the exact length */

  if(exact <= 0) return s; /* If it's a null string */

  char *newData = realloc(s->data, exact);

  if(!newData) return s; /* Reallocation Failed !! */

  s->data = newData;
  s->capacity = exact;

  return s;

}

