# DynamicString

A dynamic, growable string library for C — built from scratch to behave like
C++'s `std::string`, because plain C strings can't resize themselves.

## Why this exists

In C, a string is just a pointer into a block of memory you sized once, up
front:

```c
char buf[64];
strcpy(buf, "hello");
strcat(buf, " world"); // fine, as long as it still fits in 64 bytes
```

The moment the content outgrows that block, you're in trouble. There is no
built-in way to say "grow this string" — you either over-allocate a buffer
"just in case" and waste memory, or you hand-roll `realloc()` and manual
length tracking every single time you need to build a string incrementally.
Get the size wrong and you get a buffer overflow — one of the most common
sources of memory corruption and security vulnerabilities in C's history.

DynamicString exists to do that bookkeeping once, correctly, so nothing that
uses it has to reinvent it.

## What it actually is

A `String` struct that owns its own buffer and tracks its own length and
capacity:

```c
typedef struct {
  char   *data;      // heap-allocated, always null-terminated
  size_t  length;     // characters in use, excluding '\0'
  size_t  capacity;   // total allocated size of data
} String;
```

Every function that modifies a `String` grows the buffer automatically when
needed, using **geometric (doubling) growth** — the same strategy
`std::string` and `std::vector` use internally, so repeated appends are
amortized O(1) instead of degrading into O(n²) from naive exact-size
reallocation on every call.

Functions that can fail (out of memory, bad argument) return a `Status` code
(`DS_OK`, `DS_ERR_NOMEM`, `DS_ERR_ARG`) instead of failing silently.

## How it solves real problems

| Problem with plain C strings | How DynamicString solves it |
|---|---|
| Fixed-size buffers overflow when content grows | Buffer grows automatically; you never pre-guess a size |
| Building a string piece-by-piece means manual `realloc` + offset tracking everywhere | One call (`ds_appendStr`, `ds_appendChar`, `ds_sprintf`) handles it |
| No way to know your own length without `strlen()` (O(n), every time) | `ds_len()` is O(1) |
| Exact-size `realloc` on every append is O(n²) over many appends | Geometric growth makes it amortized O(1) |
| `fgets()` needs a fixed buffer and truncates long lines | `ds_readLine()` reads a line of **any length** safely |
| No built-in substring, find, split, replace, trim | All added in v0.3 |

This isn't a hypothetical problem — Redis's core string type (**SDS**) and
GLib's `GString` exist for exactly these reasons in production C codebases.

## Features

**Core (v0.1–v0.2)**
- `ds_new` / `ds_free` — create and destroy
- `ds_appendStr` / `ds_appendChar` — append a string or single char
- `ds_sprintf` — printf-style formatted append
- `ds_len` / `ds_cap` / `ds_str` — inspect length, capacity, and the raw C string
- `ds_strcmp` — compare two Strings
- `ds_clear` — reset length, keep the buffer
- `ds_shrinkToFit` — release unused capacity once you're done growing

**New in v0.3**
- `ds_find` / `ds_contains` — locate a substring
- `ds_startsWith` / `ds_endsWith`
- `ds_substr` — pull out a slice as a new String
- `ds_insertStr` — insert text at any position
- `ds_replaceStr` — replace the first occurrence of a substring
- `ds_trim` — strip leading/trailing whitespace in place
- `ds_splitStr` / `ds_freeSplitStr` — split on a delimiter into an array of Strings
- `ds_joinStr` — glue an array of Strings back together with a delimiter
- `ds_readLine(FILE*)` — read a line of any length from a file; the safe
  replacement for `fgets()` + a fixed buffer

## Usage

```c
#include "dstring.h"

int main(void) {
    String *s = ds_new("Hello");
    ds_appendStr(s, ", world");
    ds_appendChar(s, '!');
    printf("%s\n", ds_str(s));            // Hello, world!

    ds_trim(s);
    if (ds_contains(s, "world")) {
        ds_replaceStr(s, "world", "there");
    }

    size_t count;
    String **parts = ds_splitStr(s, ' ', &count);
    String *rejoined = ds_joinStr(parts, count, "_");
    printf("%s\n", ds_str(rejoined));     // Hello,_there!

    ds_freeSplitStr(parts, count);
    ds_free(rejoined);
    ds_free(s);
    return 0;
}
```

Reading a file line by line, with no line-length limit:

```c
String *line;
while ((line = ds_readLine(fp)) != NULL) {
    printf("%s\n", ds_str(line));
    ds_free(line);
}
```

## Build

```
make          # builds demo and test_dstring
make run      # runs the demo
make check    # runs the test suite
make clean
```

## Roadmap

- v1.0: switch to the SDS-style layout (metadata stored immediately before
  the character buffer in one allocation), so a `String` can be passed
  directly to any libc function expecting `const char*`
- Small String Optimization (store short strings inline, no heap allocation)
- Fuzz testing
