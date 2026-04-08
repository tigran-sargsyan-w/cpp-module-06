# C++ Module 06 – C++ Casts 🔁🧠

✅ **Status**: Completed – all mandatory exercises  
🏫 **School**: 42 – C++ Modules (Module 06)  
🏅 **Score**: 90/100 (One of the reviewers did not agree with the scalar converter solution🤔)

> *Scalar conversion, pointer serialization via `reinterpret_cast`, and runtime type identification via `dynamic_cast` (without `<typeinfo>`).*

---

## 📚 Table of Contents

* [Description](#-description)
* [Goals of the Module](#-goals-of-the-module)
* [Exercises Overview](#-exercises-overview)

  * [ex00 – ScalarConverter](#ex00--scalarconverter)
  * [ex01 – Serializer](#ex01--serializer)
  * [ex02 – Identify real type](#ex02--identify-real-type)
* [Requirements](#-requirements)
* [Build & Run](#-build--run)
* [Repository Layout](#-repository-layout)
* [ex00 Notes](#ex00-notes)

  * [🔥 Important Edge Case: float rounding near INT limits](#-important-edge-case-float-rounding-near-int-limits)
  * [🧩 Clarification: Why `char: Non displayable` for 128 and not always `impossible`](#-clarification-why-char-non-displayable-for-128-and-not-always-impossible)
* [ex01 Notes](#ex01-notes)

  * [🔥 Important Concept: Pointer Size & `uintptr_t`](#-important-concept-pointer-size--uintptr_t)
  * [🔍 Why `reinterpret_cast` is required here](#-why-reinterpret_cast-is-required-here)
  * [⚠️ Lifetime vs Address](#️-lifetime-vs-address)
* [ex02 Notes](#ex02-notes)

  * [🧠 Identify Real Type – Critical Nuances](#-identify-real-type--critical-nuances)
* [🔍 Testing Tips](#-testing-tips)
* [🧾 42 Notes](#-42-notes)

---

## 📝 Description

This repository contains my solutions to **42’s C++ Module 06 (C++98)**.
The module is a deep dive into **casting and type conversions**:

* converting strings into scalars (`ex00`)
* converting pointers into integers and back (`ex01`)
* identifying the real dynamic type behind a base pointer/reference (`ex02`)

A big part of this module is not only making it work, but also understanding **why a specific cast is the correct tool**.

---

## 🎯 Goals of the Module

Concepts practiced:

* Correct use of C++ casts: `static_cast`, `reinterpret_cast`, `dynamic_cast`
* Handling pseudo literals (`nan`, `inf`, with and without `f`) in scalar conversions
* Edge cases: overflow, precision loss, non-displayable characters
* Why `uintptr_t` exists (portable “integer big enough to hold an address”)
* RTTI-style detection with `dynamic_cast` without using `<typeinfo>`

---

## 📦 Exercises Overview

### ex00 – ScalarConverter

**Goal:** Implement a non-instantiable class `ScalarConverter` with:

* `static void convert(std::string const& literal);`

It detects the literal type, converts it to the “real” type first, then prints:

* `char`
* `int`
* `float`
* `double`

Must handle pseudo literals:

* `nan`, `+inf`, `-inf`
* `nanf`, `+inff`, `-inff`

---

### ex01 – Serializer

**Goal:** Implement a non-instantiable class `Serializer` with:

* `static uintptr_t serialize(Data* ptr);`
* `static Data* deserialize(uintptr_t raw);`

Create a non-empty `Data` struct and verify that:

* serializing and deserializing gives the **same address** back
* pointer equality is preserved

---

### ex02 – Identify real type

**Goal:** Create:

* `Base` (only a public virtual destructor)
* `A`, `B`, `C` inheriting publicly from `Base`

Implement:

* `Base* generate();` (randomly returns new `A/B/C`)
* `void identify(Base* p);`
* `void identify(Base& p);` (**no pointer usage inside this one**)

Forbidden:

* `<typeinfo>` / `typeid`

---

## 🛠 Requirements

* **Compiler**: `c++`
* **Flags**: `-Wall -Wextra -Werror -std=c++98`
* **No external libraries** (no C++11+)
* **No** `printf`, `malloc`, `free` (and friends)

---

## ▶️ Build & Run

```bash
git clone <this-repo-url>
cd cpp-module-06
```

### ex00

```bash
cd ex00
make
./convert 0
./convert 42.0f
./convert nan
```

### ex01

```bash
cd ex01
make
./serializer
```

### ex02

```bash
cd ex02
make
./identify
```

---

## 📂 Repository Layout

```text
cpp-module-06/
├── ex00/
│   ├── Makefile
│   ├── ScalarConverter.hpp
│   ├── ScalarConverter.cpp
│   └── main.cpp
│
├── ex01/
│   ├── Makefile
│   ├── Serializer.hpp
│   ├── Serializer.cpp
│   ├── Data.hpp
│   └── main.cpp
│
└── ex02/
    ├── Makefile
    ├── Base.hpp
    ├── Base.cpp
    ├── A.hpp / A.cpp
    ├── B.hpp / B.cpp
    ├── C.hpp / C.cpp
    └── main.cpp
```

---

# ex00 Notes

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

## 🧩 Clarification: Why `char: Non displayable` for 128 and not always `impossible`

You might notice:

```
Input: 128.0
char: Non displayable
```

and wonder if it should be `impossible`.

### 1) What a `char` really is in C++

In C/C++, **`char` is just a 1-byte integer type**.

Two important properties:

1. `sizeof(char) == 1` always.
2. Whether `char` is **signed** or **unsigned** is **implementation-defined**.

* On some systems: `char` behaves like `signed char` (range typically `-128..127`)
* On others: it behaves like `unsigned char` (range typically `0..255`)

So when we ask: “can we convert to `char`?” — the answer depends on **which rule we choose**.

### 2) The subject usually wants *ASCII-like* printing

Most 42 solutions treat the `char` output as:

* Valid range: **0..127** (ASCII)
* Printable: **32..126**
* Control / DEL: `0..31` and `127` → `Non displayable`
* Everything outside `0..127` → `impossible`

Under that rule:

* `128` would be **`impossible`**

✅ This is the safest choice if your evaluator/tester expects strict ASCII.

### 3) Why some implementations print `Non displayable` for 128

If your implementation checks the range using `unsigned char` limits (`0..255`), then:

* `128` is considered “representable as a byte” → so it’s **not impossible**
* but it’s not in printable ASCII (`32..126`) → so you print **Non displayable**

That logic is consistent from a *C++ type system* point of view.

### 4) Where encodings come into play (and why it’s tricky)

The reason `128` is a headache is not C++ casting — it’s **text encoding**:

* ASCII defines only **0..127**.
* Values **128..255** are **not ASCII**.
* Historically there were many “extended ASCII” code pages (Windows-1252, ISO-8859-1, KOI8-R, ...).
* Modern terminals often use **UTF-8**, where a single byte `0x80` is not a valid standalone character.

So printing a raw byte with value 128 can:

* show a weird symbol
* show a replacement character
* visually break output

✅ That’s why the most evaluator-safe choice is usually: treat `char` output as ASCII-only.

### Recommended rule for maximum safety

* if value < 0 or value > 127 → `impossible`
* else if value < 32 or value == 127 → `Non displayable`
* else → print `'c'`

---

# ex01 Notes

## 🔥 Important Concept: Pointer Size & `uintptr_t`

In **ex01 (Serialization)** we convert a pointer into an integer and back:

```cpp
uintptr_t raw = Serializer::serialize(ptr);
Data* again = Serializer::deserialize(raw);
```

At first glance this looks trivial — but there is an important architectural detail behind it.

### 🧠 Why pointer size matters

A pointer is just an address in memory.
But the **size of that address depends on the machine architecture**.

* **32-bit systems**: pointer size is 32 bits (4 GB address space)
* **16-bit systems** (very old): pointer size is 16 bits (64 KB address space)
* **64-bit systems** (modern standard): pointer size is 64 bits
* **specialized / experimental** architectures may even use **128-bit** addressing

Hardcoding a type like `unsigned long` is not portable: on some platforms it might be too small.

### ✅ Why we use `uintptr_t`

`uintptr_t` (from `<stdint.h>`, C++98 compatible) is:

> an unsigned integer type capable of storing a pointer value without loss.

This means:

* On 16-bit systems → `uintptr_t` is 16 bits
* On 32-bit systems → `uintptr_t` is 32 bits
* On 64-bit systems → `uintptr_t` is 64 bits
* On 128-bit systems → it would match 128 bits

The compiler chooses the correct size automatically for the target architecture.

So instead of guessing pointer size, we rely on a **portable, architecture-safe type**.

### ⚠️ Important distinction

Using `uintptr_t`:

* ✔ guarantees we do not lose address bits
* ✔ makes the conversion reversible
* ✔ keeps the code portable

But it does **not**:

* ❌ extend object lifetime
* ❌ make a destroyed object valid again
* ❌ protect against dangling pointers

`uintptr_t` safely stores an address.
It does not guarantee that an object still exists at that address.
---

## 🔍 Why `reinterpret_cast` is required here

In this exercise we need to convert between:

* a **pointer type** (`Data*`)
* an **integer type** (`uintptr_t`)

This is a low-level reinterpretation of the same bit pattern.

### Why not `static_cast`?

`static_cast` is for well-defined conversions like numeric conversions and safe up/down casts in inheritance.
It’s **not meant** for arbitrary pointer ↔ integer conversions.

### Why `reinterpret_cast`?

`reinterpret_cast` is the cast designed for:

* treating an address value as an integer
* treating an integer as an address value

So in ex01 we do:

```cpp
return reinterpret_cast<uintptr_t>(ptr);
```

and the reverse:

```cpp
return reinterpret_cast<Data*>(raw);
```

Important: this cast does not validate anything — it only reinterprets.

---

## ⚠️ Lifetime vs Address

A core nuance of ex01 is understanding the difference between:

* **An address** (just a number)
* **A live object** (valid memory with a valid lifetime)

### ✅ The address can be stored safely

`uintptr_t` can store the pointer value without losing bits.
So the conversion is reversible:

```cpp
Data* again = Serializer::deserialize(Serializer::serialize(ptr));
```

### ❌ But the object may no longer exist

If the original object’s lifetime ends, the address still exists as a number,
but dereferencing it becomes **Undefined Behavior**.

Typical UB situations:

* stack object leaves scope → dangling pointer
* heap object is deleted → use-after-free

> `uintptr_t` preserves the address correctly, but it cannot preserve the object.

---

# ex02 Notes

## 🧠 Identify Real Type – Critical Nuances

This exercise is about **polymorphism**, **RTTI behavior**, and correct use of `dynamic_cast`.

---

### 🔥 Base MUST Be Polymorphic

```cpp
class Base
{
public:
    virtual ~Base();
};
```

Why this is mandatory:

* `dynamic_cast` works correctly **only with polymorphic types**
* a class becomes polymorphic if it has at least one `virtual` function

Also, deleting through base pointer must be safe:

```cpp
Base* ptr = new A();
delete ptr;
```

Without a virtual destructor:

* ❌ only `Base` destructor runs
* ❌ derived destructor does not run
* ❌ UB / partial destruction

So the virtual destructor guarantees:

* ✔ RTTI works
* ✔ proper destruction through base pointer

---

### 🔄 Implicit Upcasting in `generate()`

```cpp
return new A();
```

The function returns `Base*`, so this triggers an **implicit upcast**:

* `A* → Base*`

Equivalent to:

```cpp
A* ptrA = new A();
Base* ptrBase = ptrA;
return ptrBase;
```

This is safe because inheritance is `public`.

✅ Not slicing: there is no slicing because we use pointers.

---

### ⚠️ Pointer Adjustment (Subtle but Important)

In simple single inheritance, `A*` and `Base*` often have the same address.
But with multiple inheritance:

* `Base*` may require an internal offset
* the compiler automatically adjusts the pointer

Upcasting may internally modify the address to point to the correct base subobject.

---

### 🔎 `dynamic_cast` behavior

**Pointer version**:

```cpp
dynamic_cast<A*>(p);
```

* ✔ on failure: returns `NULL`
* ✔ on success: returns valid pointer

**Reference version**:

```cpp
dynamic_cast<A&>(p);
```

* ❗ on failure: throws `std::bad_cast`

So the reference-based identify must use `try/catch`.

**Critical difference**:

| Cast form          | On failure       |
| ------------------ | ---------------- |
| `dynamic_cast<T*>` | returns `NULL`   |
| `dynamic_cast<T&>` | throws exception |

Reason:

* a pointer may legally be `NULL`
* a reference must always refer to a valid object

---

### 🚫 Why `static_cast` is wrong here (downcasting)

```cpp
A* a = static_cast<A*>(basePtr);
```

If `basePtr` actually points to `B`:

* ❌ Undefined Behavior
* ❌ no runtime check is performed

Only `dynamic_cast` verifies the real runtime type.

---
### 🔬 What Happens "Under the Hood" (Upcast Internals)

When we write:

```cpp
return new A();
```

but the function returns `Base*`, an **implicit upcast** happens:

```
A* → Base*
```

This is NOT packaging or wrapping.

It is a compile-time conversion allowed because of `public` inheritance.

Conceptually equivalent to:

```cpp
A* ptrA = new A();
Base* ptrBase = ptrA;
return ptrBase;
```

### ⚠️ Important detail

In simple single inheritance the addresses are usually identical.

With multiple inheritance, the compiler may apply an **internal pointer offset adjustment** so that the `Base*` points to the correct base subobject.

No object slicing occurs because we are using pointers.

---

## 🚫 Why Not `static_cast`?

```cpp
A* a = static_cast<A*>(basePtr);
```

If `basePtr` actually points to `B`, this results in:

❌ Undefined Behavior
❌ No runtime type verification

`static_cast` performs no dynamic check.

Only `dynamic_cast` verifies the real runtime type using RTTI.

---

## ⚠️ Where Undefined Behavior Appears

UB can happen if:

• You downcast using `static_cast` incorrectly
• Base is not polymorphic (no virtual function)
• You delete through a base pointer without virtual destructor

This exercise forces correct design to avoid UB.

### 🛠 Viewing what the compiler generates (optional curiosity)

To see what happens under the hood, compile without optimizations:

```bash
c++ -std=c++98 -O0 -g3 *.cpp -o identify
```

Then inspect:

```bash
objdump -d -C identify | less
```

* `-C` demangles C++ names
* `-O0` prevents optimizations from hiding steps

You can also generate assembly directly:

```bash
c++ -std=c++98 -O0 -S main.cpp -o main.s
```

---

## 🔍 Testing Tips

### ex00

```bash
./convert a
./convert 'a'
./convert 0
./convert 127
./convert 128
./convert -1
./convert 2147483647
./convert -2147483649
./convert nan
./convert +inf
./convert -inff
```

Things to verify:

* pseudo-literals output
* overflow handling (`impossible`)
* float rounding near `INT_MAX`
* consistent `char` rules (ASCII safety)

### ex01

* check pointer equality after round-trip
* print the `uintptr_t` value and compare with the original address formatting
* ensure `Data` is non-empty

### ex02

* call `generate()` multiple times
* test both `identify(ptr)` and `identify(ref)`
* verify no `<typeinfo>` usage

---

## 🧾 42 Notes

* C++ modules do not use Norminette, but clean and readable code still matters.
* During evaluation you’ll likely be asked to justify why you used a specific cast.
* The “real learning” here is understanding precision, undefined behavior, and runtime type checks — not just passing the tests.

---

If you’re a 42 student working on the same module: feel free to explore the code, get inspired, but **write your own implementation** — that’s where the learning happens. 🚀
