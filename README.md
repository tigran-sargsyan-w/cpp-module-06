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
