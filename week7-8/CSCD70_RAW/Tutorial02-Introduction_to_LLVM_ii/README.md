# [Tutorial 2 Introduction to LLVM (ii)](https://v2.overleaf.com/read/vdwnnwdcshyx)

*Please click on the title for the tutorial slides.*

## Abstract

- How to write an LLVM **Transform** pass?
  - Basic Instruction Manipulation
  - User-Use-Value Relationship
- How to build up the *connection* between Analysis and Transform passes?
  - LLVM Pass Manager: **Require** and **Preserve**

## Changlog

- Added the `addPreserved < A >` to the list of common **Pass Manager** function calls.
