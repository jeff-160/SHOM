### Introduction
SHOM is a stack-based esolang with minimalist syntax.

### Primitive Types
Integers: Positive and negative whole numbers (e.g. `1`)

Doubles: Real numbers (e.g. `0.2`)

Strings: Text (e.g.`hello world`)

### Basic Stack Manipulation
`$`: Prompts for input and stores it in the stack as a string

`~`: Prints the topmost value in the stack

`,`: Removes the topmost value in the stack

`_`: Flushes the stack

`\`: Swaps the topmost two values in the stack

';': Duplicates the topmost value in the stack

### Arithmetic Operations
The following symbols will remove the top two elements and perform the corresponding arithmetic operation, before storing the result in the stack.

`+`: Addition

`-`: Subtraction

`*`: Multiplication

`/`: Division

`^`: Exponent

`%`: Modulus

### Logical operations

`>`: Greater than

`<`: Less than

`&`: Logical and

`|`: Logical or

`=`: Equality

`!`: Not

### Type Casting

`I`: Converts the topmost value in the stack to an integer

`D`: Converts the topmost value in the stack to a double

`S`: Converts the topmost value in the stack to a string

### Control flow

Conditionals
- `{}?`

### Arrays