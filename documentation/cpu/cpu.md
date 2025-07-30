# CPU Documentation

## Introduction 

The SNES uses the 65816 CPU, which is a 16-bit CPU. It's built off the 6502 CPU, even backwards compatible with the 6502. These notes are taken off [this source](https://wiki.superfamicom.org/uploads/assembly-programming-manual-for-w65c816.pdf).

## Registers

The 65816 is pretty similar to the 6502, but can use 16 bit registers and introduces two new registers.

### A Register

The Accumulator is meant for math arithmetic. It can be a 16 bit register, but it can also be two eight bit registers, where one register is accessible(A), and another one is hidden but exchangeable (B). The setting is toggled with the m status flag.

### X and Y registers

These registers are index registers. They can either be sixteen bits or 8 bit registers

### bank registers

The SNES introduces a concept known as banks. A bank of memory is similar to a page, but each bank is 64Kb while a page is 256 bytes. The SNES divides the memory range into these banks. You select this bank using the bank register

There are two of these: Program Bank Register and Data bank registers

#### program bank registers

The program bank register describes what bank the program counter should access. You can think of it as an attachment to the program counter, hence a 24 bit program counter. When an interrupt happens, both the program counter and bank are pushed onto the stack. **One important note**: when the program counter passes 0xFFFF, it wraps around **without incrementing the program counter bank**.

### Data bank register 

This register defines what bank to use for reading/writing data. Unlike the program bank register, the data bank register increments.

### Direct Page Register

The original Nintendo Entertainment System had a zero page. The SNES takes this and turns it into the direct page. When the register is 0, we point to page zero, and acts like a zero page.

### Stack pointer

The stack pointer is similar to the 6502, but is 16 bits, and can float anywhere (not confined to page 1)

## status register

The status register describes the result of an operation and the status of the CPU. There are three new flags.

| Flag | meaning |
| --- | --- |
| N | set if result of arithmetic operation is negative |
| V | set if arithmetic operation resulted in overflow |
| M | determines if accumulator + memory access is 16 or 8 bits |
| X | determines if index registers are 16 or 8 bits |
| D | decimal mode |
| I | disable IRQs |
| Z | arithmetic operation results in zero |
| C/E | Carry and Emulation mode |

- E is emulation mode, and is only accessible through the Carry flag. If Emulation mode is set, the CPU will run like the 6502. Native mode if set to 0.
- M will determine if the accumulator is 16 or 8 bits. It will also determine the size of the memory accesses.
- X is to indicate the size of the index registers, X and Y.

### Accumulator and Index Registers

Similar to the 6502, there's the A, X and Y registers. The sizes are toggled with the m and x status flags. The Accumulator is 16 bits if the m flag is clear. The index registers are 16 bits if the x flag is clear. Also, when you read/write to memory, you'll write/read 16 bits to/from memory if the m flag is clear.

#### Register size switching behaviors

For the index registers, when you switch from 16 bit to 8, the high byte becomes 0. If you switch from 8 to 16, the high byte will also become 0.

For the accumulator, when you convert from 16 to 8, the high byte stays the same, but you only read from the accumulator. When you convert from 8 to 16, you keep the high byte.

## Addressing modes

As mentioned before, there's no zero page addressing mode in the 65816 in native mode. Instead, you have the **Direct Page**

|  65816 Direct Page addressing | syntax |
| --- | --- |
| Direct Page | `LDA $55` |
| Direct Page indexed with X | `LDA $55, X` |
| Direct Page Indexed with Y | `LDA $55, Y` |
| Direct Page Indirect Indexed with Y | `LDA ($55), Y` |
| Direct Page Indexed Indirect with X | `LDA ($55,X)` |
| Direct Page Indirect | `LDA($55)` |


There are also new addressing modes introduced with the 65816

| addressing mode | syntax |
| --- | --- |
| Stack Relative | `LDA 3,S` |
| Stack Relative Indirect Indexed with Y | `LDA (5,S),Y` |
| Block Move | `MVP 0,0` |
| Absolute Long | `LDA $02F0000` |
| Absolute Long Indexed with X | `LDA $12D080,X` |
| Absolute Indirect Long | `jmp [$2000]` |
| Direct Page Indirect Long | `LDA [$55]` |
| Direct Page Indirect Long Indexed with Y | `LDA [$55],Y` |

### Stack relative

In the stack addressing modes, the stack pointer acts like a base pointer to the array, while the operand is the index to this array. 

### Stack relative indirect indexed

It's similar to stack relative, but you get the address of the base of array located somewhere else.


### Block move Addressing

Facilitates the moving of blocks of bytes. The parameters are stored in the accumulator (count), and the index registers store the source and destination.

### Absolute Long

This addressing mode will override the data bank, treating the address as 12 bits.

```bash
LDA $02F000
```

### Absolute Long Indexed with X

- this will add X to the 3 byte address:

```bash
LDA $12D080, X
```

### Absolute indirect long

- the 3 byte address is located in the address specified

```bash
JMP [$2000] # will read 3 bytes from address $2000
```

### Direct Page Indirect Long

- will read three bytes from address `DP + $address`

```bash
LDA [$55] # will read 3 bytes from address DP + $5
```

### Direct Page Indirect Long, Indexed with Y

- Will read 3 bytes `DP + $address` and add y to the address (not affecting bank byte)

```bash
LDA [$55], Y
```

## Interrupt vector

The Interrupt vector is located in bank zero. It's an array containing the location of the Interrupt Service Routines.

| Interrupt | emulation mode | native mode |
| --- | --- | --- |
| IRQ | 0xFFFE,0xFFFF | 0xFFEE,0xFFEF |
| RESET | 0xFFFC, 0xFFFD | - |
| NMI | 0xFFFA, 0xFFFB | 0xFFEA, 0xFFEB |
| ABORT | 0xFFF8, 0xFFF9 | 0xFFE8, 0xFFE9 |
| BRK | - | 0xFFE6, 0xFFE7 |
| COP | 0xFFF4, 0xFFF5 | 0xFFE4, 0xFFE5 |

You can see that there's no vector for native mode. This is because the CPU starts in emulation mode


## Emulation Mode

Under Emulation mode, the CPU acts exactly like the 6502. For example, the Accumulator register will always be limited to 8 bits. Despite this, there are certain instructions which will transfer the register values. The stack pointer is stuck at page 1, similar to the 6502. The status register in emulation mode is similar to the 6502, but the emulation register still exists.