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

Status ds_appendChar(String *s, const char c){

  if(!s) return DS_ERR_ARG; /* Empty String */

  if(!ds_ensureCapacity(s, s->length + 1)){ /* Ensure capacity is enough for    the overall -> present str + 1 char */
    return DS_ERR_NOMEM; /* Failed to Enusre capacity .. leave s untouched */
  }

  /* Append a single character at the end with a null character */
  s->data[s->length] = c;
  s->length += 1;
  s->data[s->length] = '\0';

  return DS_OK;

}

/* Append suffix string to current string .. first ensure capacity, Use memcpy to append suffix string to current string return string possibly reallocated */

Status ds_appendStr(String *s, const char *suffix){

  if(!s || !suffix) return DS_ERR_ARG;

  size_t suffLen = strlen(suffix); /* Length of the suffix String */

  if(suffLen == 0) return DS_ERR_ARG;

  if(!ds_ensureCapacity(s, s->length + suffLen)) /* Ensure the capacity length + suffixLength*/
  {
    return DS_ERR_NOMEM; /* Failed to reallocate */
  }

  memcpy(s->data + s->length, suffix, suffLen); /* Copy the suffix String to og string */

  s->length += suffLen; /* Length = s->length + suffixLen */
  s->data[s->length] = '\0'; /* Always Null terminated String */

  return DS_OK;

}

/* A variadic Function uses vsnprintf() to allow printf-style formatting to the string, create two argument pointers .. one for calculating size (needed) and another for actual decoding, use vsnprintf() to write to the string*/

Status ds_sprintf(String *s, const char *fmt, ...){

  if(!s || !fmt) return DS_ERR_ARG;

  va_list ap;
  va_start(ap, fmt);
  va_list apCopy;
  va_copy(apCopy, ap);

  int needed = vsnprintf(NULL, 0, fmt, ap); /* Calculate total size needed */
  va_end(ap);

  if(needed < 0){
    va_end(apCopy);
    return DS_ERR_NOMEM; /* Encoding error */
  }

  if(!ds_ensureCapacity(s, (size_t)needed)){
    va_end(apCopy);
    return DS_ERR_NOMEM; /* Reallocation Failed !! */
  }

  vsnprintf(s->data + s->length, (size_t)needed + 1, fmt, apCopy); /* Write the formatted string to original string */
  va_end(apCopy);

  s->length += (size_t)needed; /* Update new length */

  return DS_OK;

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
Status ds_shrinkToFit(String *s){
  
  if(!s) return DS_ERR_ARG;

  size_t exact = ds_len(s) + 1; /* Calculate the exact length */

  if(exact <= 0) return DS_ERR_NOMEM; /* If it's a null string */

  char *newData = realloc(s->data, exact);

  if(!newData) return DS_ERR_NOMEM; /* Reallocation Failed !! */

  s->data = newData;
  s->capacity = exact;

  return DS_OK;

}

/* Version 0.2*/

/* Uses strstr to find needle in the haystack, return -1 if not found! */
long ds_find(const String *haystack, const char *needle){

  if(!haystack || !needle) return -1; /* if any one is NULL*/

  char *found = strstr(haystack->data, needle); /* Use strstr */

  if(!found) return -1; /* NOt found */

  return (found - haystack->data); /* Return the diff nothing but an index */

}

/* Just a wrap around of ds_find */
int ds_contains(const String *s, const char *needle){
  return ds_find(s, needle) >= 0; /* Check if present */
}

/* Uses strncmp to compare the first n characters of the prefix with s starting*/
int ds_startsWith(const String *s, const char *prefix){

  if(!s || !prefix) return 0; /* If any one is Empty */

  size_t prefixLen = strlen(prefix); /* Calculate the length of the prefix */

  if(prefixLen > s->length) return 0; /* Can't compare greater than the data */

  return strncmp(s->data, prefix, prefixLen) == 0; /* return 1 if found */

}
  
/* Uses strcmp to compare the last suffixlen with suffix */
int ds_endsWith(const String *s, const char *suffix){

  if(!s || !suffix) return 0; /* If any one is empty */

  size_t suffixLen = strlen(suffix); /* length of the suffix string */

  if(suffixLen > s->length) return 0; /* Can't compare which is greater!! */

  return strcmp(s->data + (s->length - suffixLen), suffix) == 0;
}

/* Returns the len characters of s from the start .. clamped to what's available */
String *ds_substr(const String *s, size_t start, size_t len){

  if(!s || start > s->length) return NULL;

  size_t available = s->length - start;

  if(len > available) len = available;

  char *buffer = malloc(len + 1);

  if(!buffer) return NULL;

  memcpy(buffer, s->data+start, len);
  buffer[len] = '\0';

  String *res = ds_new(buffer);
  free(buffer);

  return res;

}

/* Insert the str in s at the position pos, tail of the s is appended later, insert in between, use memmove to avoid unwanted overlap collisions will cause by memcpy*/
Status ds_insertStr(String *s, size_t pos, const char *str){

  if(!s || !str || s->length > pos) return DS_ERR_ARG;

  size_t insertLen = strlen(str);

  if(!ds_ensureCapacity(s, s->length + insertLen)) return DS_ERR_NOMEM;

  /* Shift the tail including '\0' character after the position pos to the right leaving the gap for str */
  memmove(s->data + pos + insertLen, s->data + pos, s->length - pos + 1);
  memcpy(s->data + pos, str, insertLen);

  s->length += insertLen;
  return DS_OK;

}

/* Find the first occurrence of find string in s->data using strstr, if replace string is larger than find string .. the memory must need to be reallocate .. capacity must vary, move the entire tailLen string along with '\0' character copy the replace to the position and update the length */

Status ds_replaceStr(String *s, const char *find, const char *replace){

  if(!s || !find || !replace) return DS_ERR_ARG;

  char *pos = strstr(s->data, find);

  if(!pos) return DS_ERR_ARG;

  size_t idx = (size_t)(pos - s->data);
  size_t findLen = strlen(find);
  size_t replaceLen = strlen(replace);
  size_t tailLen = s->length - idx - findLen;

  if(replaceLen > findLen){
    if(!ds_ensureCapacity(s, s->length - findLen + replaceLen))
      return DS_ERR_NOMEM;
  }

  pos = s->data + idx;

  memmove(pos+replaceLen, pos+findLen, tailLen+1);
  memcpy(pos, replace, replaceLen);

  s->length = s->length - findLen + replaceLen;

  return DS_OK;

}