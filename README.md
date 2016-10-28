Binary Structures
=================

This is yet another serialization library, built for efficiently serializing
C-style structures.

Given the following code:

```c
#define EI_NIDENT 16

typedef struct elf32_hdr{
  unsigned char e_ident[EI_NIDENT];
  Elf32_Half  e_type;
  Elf32_Half  e_machine;
  Elf32_Word  e_version;
  Elf32_Addr  e_entry;  /* Entry point */
  Elf32_Off e_phoff;
  Elf32_Off e_shoff;
  Elf32_Word  e_flags;
  Elf32_Half  e_ehsize;
  Elf32_Half  e_phentsize;
  Elf32_Half  e_phnum;
  Elf32_Half  e_shentsize;
  Elf32_Half  e_shnum;
  Elf32_Half  e_shstrndx;
} Elf32_Ehdr;
```

**bstruct** is able to generate a serializer which will serialize either
to the native platform conventions or to a user-specified foreign one.

## Other software

**bstruct** is similar to [Google's Protocol Buffers](http://github.com/google/protobuf),
but with one big difference: it doesn't require an additional protocol
specification, because it can directly parse C source code to generate a binary
serializer.

## Support

Like it? Fork it!

