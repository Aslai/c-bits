C Bits
======

This is a basic public domain library intended to make the generation of large bitsets in C/C++ easier.

There are two headers, `c_bits.h` and `cpp_bits.hpp`. The interfaces are very simple.

## `c_bits.h`

```c
#include <stdio.h>
#include "c_bits.h"

int main(){
const char * data = b01001000 b01100101 b01101100 b01101100 b01101111 b00101110;
    // data now has a string literal that represents that bit pattern. In this case,
    // we have the ASCII/UTF-8 representation of "Hello."
    printf("%s", data);
    return 0;
}
```

If you look at the implementation, you will see that these are simply convenience macros which translate 
into string literals which contain hex character escape sequences. That way, you can string many of them
together to obtain a single large string literal.


The generated code makes this behavior plainly obvious:

```asm
;GCC 6.2
.LC0:
        .string "Hello."
.LC1:
        .string "%s"
main:
        sub     rsp, 8
        mov     esi, OFFSET FLAT:.LC0
        mov     edi, OFFSET FLAT:.LC1
        xor     eax, eax
        call    printf
        xor     eax, eax
        add     rsp, 8
        ret
```

## `cpp_bits.hpp`

```cpp
#include <cstdio>
#include "cpp_bits.hpp"

int main(){
    auto data = bin_str("010010000110010101101100011011000110111100101110");
    // data now has a std::array that represents that bit pattern. In this case,
    // we have the ASCII/UTF-8 representation of "Hello."
    printf("%s", data.data());
    return 0;
}
```

The implementation for `cpp_bits.hpp` involves C++11 (and later) template metaprogramming. It will take an arbitrarily long string of `1` and `0` characters, and generate an equivalent bit string. Note that any character which is not '1' will be interpreted as a zero.


The generated code is a bit less elegant than the C version:

```asm
;GCC 6.2
.LC0:
        .string "%s"
main:
        sub     rsp, 24
        mov     edi, OFFSET FLAT:.LC0
        xor     eax, eax
        mov     rsi, rsp
        mov     BYTE PTR [rsp], 72
        mov     BYTE PTR [rsp+1], 101
        mov     BYTE PTR [rsp+2], 108
        mov     BYTE PTR [rsp+3], 108
        mov     BYTE PTR [rsp+4], 111
        mov     BYTE PTR [rsp+5], 46
        call    printf
        xor     eax, eax
        add     rsp, 24
        ret
```

Overall, it still works fairly well. There is some overhead in the conversion to `std::array`, but not a huge amount.