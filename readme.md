# base64_enc

Base64 encoding is primarily used to store binary data as an ASCII string for
use with ASCII-based applications, e.g. embedding images withing HTML pages.

Base64 encoding is also used for various puzzles and ARGs to encode plain text.

## How it works

The input string is split up into 24-bit chunks (three 8-bit characters) then
processed into indices (four 6-bit numbers). These indices are then referenced
to a charmap and converted back into 8-bit characters.

```
Input   - H          e          l
ASCII   - 01001000   01100101   01101100
Indices - 010010  000110  010101  101100
        - 18      6       21      44
Encoded - S       G       V       s
```

The output is padded (with `=`) to ensure each 24-bit chunk is full.
