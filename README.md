# SmartCalc

In this project, I wrote a calculator with the ability to substitute a value for the variable x when performing calculations. There is also the possibility of graphical representation of expressions and calculation of loan payments in the loan calculator. The application is written in C using GTK version 3.

## Features

The program offers two main modes of operation:

### Standard Calculator

- Supports the input of both integer and floating-point numbers, denoted by a dot.
- Calculations are performed after the complete expression has been entered and the [=] button is pressed.
- Supports the computation of arbitrary bracketed arithmetic expressions in infix notation.
- Capable of graphically representing functions defined by expressions in infix notation with the variable [x].

### Loan Calculator

- Calculates annuity and differentiated payments.
- Input data: total loan amount, term, interest rate, type of payment.
- Output data: monthly payment, total overpayment on the loan, total payment amount.

## Installation

GCC and GTK+3 are required for compiling the program. Install the necessary dependencies in your system using the package manager.

## Usage

Documentation on how to use SmartCalc is contained in the `src_program.texi` file, which can also be compiled into a PDF format for convenience.

---

Date: 06-10-2023
