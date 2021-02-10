# Bop

Bop is a very work-in-progress C library for dealing with the entire pipeline
of performing maths. Once done, it'll include:

- A parser for mathematical expressions
- An evaluator for the parsed representation
- A system for editing and rendering expressions in a "natural"/"textbook"
  format

The design goals are for Bop to be:

- Light enough for the average embedded hobbyist use-case
- Easy to integrate into an existing project

This project is starting off by breaking out and generalising the
parser/evaluator from my
[Delta M0 scientific calculator](https://github.com/AaronC81/delta-m0). After
that, I'll look into expanding the functionality much further.
