### Introduction
SHOM is a stack-based esolang with minimalist syntax.

---

### Primitive Types
Integers: Positive and negative whole numbers (e.g. `1`)

Doubles: Real numbers (e.g. `0.2`)

Strings: Text (e.g.`hello world`)

---

### Basic Stack Manipulation
`$`: Prompts for input and stores it in the stack as a string

`~`: Prints the topmost value in the stack

`,`: Removes the topmost value in the stack

`_`: Flushes the stack

`\`: Swaps the topmost two values in the stack

';': Duplicates the topmost value in the stack

---

### Arithmetic Operations
The following symbols will remove the top two elements and perform the corresponding arithmetic operation, before storing the result in the stack.

`+`: Addition

`-`: Subtraction

`*`: Multiplication

`/`: Division

`^`: Exponent

`%`: Modulus

---

### Logical operations

`>`: Greater than

`<`: Less than

`&`: Logical and

`|`: Logical or

`=`: Equality

`!`: Not

---

### Type Casting

`I`: Converts the topmost value in the stack to an integer

`D`: Converts the topmost value in the stack to a double

`S`: Converts the topmost value in the stack to a string

---

### Control flow

Conditionals
- `{}?`
- If the topmost value in the stack is truthy, the first code block will be executed, otherwise, if there is a 2nd code block, it will be executed instead
- `0{"true"~}{"false"~}?`
    - In this example, since 0 is pushed onto the stack, the conditional evaluates to false, and "false" will be printed

Loops
-`{}:`
- The code block will be looped n times, where n is the topmost value in the stack
- If the topmost value is a string, the loop will be repeated for the number of characters
- `3{"this is a loop\n"~}:`
    - the string will be printed 3 times
- `-1{}:`
    - this simulates an infinite loop

`i`
- Used to access the current iteration of the loop
- `3{i~}:`
    - Output: `012`

`'`: break statement

---

### Arrays

Declaration
- Arbitrary number of values enclosed by square braces
- `[1 "hello world" 0.2]`

`@`
- Applies indexing to an array
- The topmost element in the stack has to be an integer, and the next element must be an array

`(`: Pushes a value into an array

`)`: Removes a value from an array at the specified index

`` ` ``: Gets the index of a value in an array
