# cpp-module-06
This module is designed to help you understand the different types of casting in C++.

## 🔥 Important Edge Case: float rounding near INT limits

Sometimes you’ll see:

```bash
./convert 2147483647
```

and the output includes:

```
int: 2147483647
float: 2147483648.0f
```

This is **expected**.

### Why it happens

`float` has only **24 significant bits**, so near `2^31` it cannot represent every integer.

At around `2^31`:

* `2147483648 = 2^31`
* spacing between neighboring float values becomes:

**ULP = 2^(31 − 23) = 2^8 = 256**

So floats there are effectively “grid points” spaced by **256**.

Closest representable values around `2^31`:

* `2147483392 = 2147483648 − 256`
* `2147483648`

Midpoint:

* `2147483392 + 128 = 2147483520`

So:

* `2147483392 ... 2147483519` → rounds to `2147483392`
* `2147483520 ... 2147483647` → rounds to `2147483648`

Negative values behave symmetrically.

---

# 🧩 Clarification: Why `char: Non displayable` for 128 and not always `impossible`

You noticed:

```
Input: 128.0
char: Non displayable
```

and you’re asking: should it be `impossible`? what about encodings?

## 1) What a `char` really is in C++

In C/C++, **`char` is just a 1-byte integer type**.

Two important properties:

1. `sizeof(char) == 1` always.
2. Whether `char` is **signed** or **unsigned** is **implementation-defined**.

   * On some systems: `char` behaves like `signed char` (range typically `-128..127`)
   * On others: it behaves like `unsigned char` (range typically `0..255`)

So when we ask: “can we convert to `char`?” — the answer depends on **which rule we choose**.

## 2) The subject usually wants *ASCII-like* printing

Most 42 solutions treat the `char` output as:

* Valid range: **0..127** (ASCII)
* Printable: **32..126**
* Control / DEL: `0..31` and `127` → `Non displayable`
* Everything outside `0..127` → `impossible`

Under that rule:

* `128` would be **`impossible`**.

✅ This is the safest choice if your evaluator/tester expects strict ASCII.

## 3) Why your current code prints `Non displayable` for 128

Your implementation checks the range using `unsigned char` limits (`0..255`).

That means:

* `128` is considered “representable as a byte” → so it’s **not impossible**
* but it’s not in printable ASCII (`32..126`) → so you print **Non displayable**

This is consistent with the logic:

> “If it fits into a byte, conversion is possible; if it’s not printable, show Non displayable.”

So from a *C++ type* point of view, this is not wrong.

## 4) Where encodings come into play (and why it’s tricky)

The reason `128` is a headache is not C++ casting — it’s **text encoding**:

* ASCII defines only **0..127**.
* Values **128..255** are **not ASCII**.
* Historically there were many “extended ASCII” code pages:

  * Windows-1252
  * ISO-8859-1
  * KOI8-R
  * and many others

In those encodings, byte `0x80` (128) can mean different things.

But today most terminals use **UTF-8**, where bytes `0x80..0xFF` may be part of multi-byte sequences.

### Practical consequence

If you print a single byte with value 128:

* it may appear as a strange symbol
* or as “replacement character”
* or break the output visually

That’s why many evaluators prefer the rule:

✅ “Only ASCII 0..127 is considered meaningful for `char` output.”

## 5) So… should 128 be `impossible` or `Non displayable`?

Both interpretations can be defended, but for 42 **tests**:

* **Most common expected behavior**: treat `char` output as ASCII → `128` is `impossible`.
* Your current behavior: treat `char` as a byte (`unsigned char`) → `128` is `Non displayable`.

### Recommended for maximum safety

Use ASCII range:

* if value < 0 or value > 127 → `impossible`
* else if value < 32 or value == 127 → `Non displayable`
* else → print `'c'`

This avoids encoding/terminal issues and matches typical subject expectations.


# 🔥 Important Concept: Pointer Size & `uintptr_t`

In **ex01 (Serialization)** we convert a pointer into an integer and back:

```cpp
uintptr_t raw = Serializer::serialize(ptr);
Data *again = Serializer::deserialize(raw);
```

At first glance this looks trivial — but there is an important architectural detail behind it.

---

## 🧠 Why pointer size matters

A pointer is just an address in memory.

However, the **size of that address depends on the machine architecture**.

### 🖥 32-bit systems

On a 32-bit system:

* Pointer size = **32 bits**
* Address space = up to 2^32 bytes (4 GB)

So a pointer can be safely stored inside a 32-bit integer type.

---

### 🖥 16-bit systems (very old architectures)

On old 16-bit systems:

* Pointer size = **16 bits**
* Address space = up to 2^16 bytes (64 KB)

Addresses were much smaller.

---

### 🖥 64-bit systems (modern standard)

On modern 64-bit systems:

* Pointer size = **64 bits**
* Address space is vastly larger

If you tried to store such a pointer inside a 32-bit integer,
**you would lose the upper 32 bits of the address**, and the program would break.

---

### 🚀 Even newer / experimental architectures

There are also experimental or specialized architectures
that may use:

* 128-bit addresses

In such cases, pointer size would be **128 bits**.

Hardcoding an integer type (like `unsigned long`) would become incorrect again.

---

## ✅ Why we use `uintptr_t`

`uintptr_t` is defined in `<stdint.h>` (C++98 compatible).

It is:

> An unsigned integer type capable of storing a pointer value without loss.

This means:

* On 16-bit systems → `uintptr_t` is 16 bits
* On 32-bit systems → `uintptr_t` is 32 bits
* On 64-bit systems → `uintptr_t` is 64 bits
* On 128-bit systems → it would match 128 bits

The compiler chooses the correct size automatically for the target architecture.

So instead of guessing pointer size,
we rely on a **portable, architecture-safe type**.

---

## ⚠️ Important Distinction

Using `uintptr_t`:

✔ Guarantees we do not lose address bits
✔ Makes the code portable
✔ Makes the conversion reversible

But it does NOT:

❌ Extend object lifetime
❌ Make a destroyed object valid again
❌ Protect against dangling pointers

`uintptr_t` safely stores an address.
It does not guarantee that an object still exists at that address.

---

## 🔍 Why `reinterpret_cast` is required here

In C++ there are different kinds of casts, each with a specific purpose.

In this exercise we need to convert between:

* a **pointer type** (`Data*`)
* an **integer type** (`uintptr_t`)

This is a **low-level reinterpretation** of the same bit-pattern.

### Why not `static_cast`?

`static_cast` is for safe, well-defined conversions (numeric conversions, up/down casts in inheritance, etc.).

It is **not meant** for arbitrary pointer ↔ integer conversions.

### Why `reinterpret_cast`?

`reinterpret_cast` is the C++ tool designed for:

* treating an address value as an integer
* treating an integer as an address value

So in ex01 we do:

```cpp
return reinterpret_cast<uintptr_t>(ptr);
```

and the reverse:

```cpp
return reinterpret_cast<Data *>(raw);
```

Important: this cast does not validate anything — it only reinterprets.

---

## ⚠️ Lifetime vs Address

A core nuance of ex01 is understanding the difference between:

* **An address** (a number)
* **A live object** (valid memory with a valid lifetime)

### ✅ The address can be stored safely

`uintptr_t` can store the pointer value without losing bits.

So the conversion is reversible:

```cpp
Data *again = Serializer::deserialize(Serializer::serialize(ptr));
```

### ❌ But the object may no longer exist

If the original object’s lifetime ends, the address might still exist as a number,
but dereferencing that address becomes **Undefined Behavior**.

Typical UB situations:

* stack object leaves scope → dangling pointer (stack-use-after-scope)
* heap object is deleted → use-after-free

This is why the exercise is subtle:

> `uintptr_t` preserves the address correctly, but it cannot preserve the object.

---

## 🧩 Final Takeaway

This exercise is not about complex serialization.
It is about understanding:

* What a pointer really is (an address)
* That pointer size depends on architecture
* Why portable code must not assume pointer width
* Why `uintptr_t` exists
* Why `reinterpret_cast` is the right tool here
* Why object lifetime is still your responsibility

> Address size is architecture-dependent.
> `uintptr_t` adapts automatically.
> Safety still depends on object lifetime management.
