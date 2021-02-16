# Bop - Representation of Expressions

## Overview

Bop represents mathematical expressions with _expression trees_.

To simplify things, the same expression tree is carried through the entire 
pipeline of rendering, editing, and evaluating an expression. To enable this
while avoiding an overly-strict editing workflow, an expression tree is a
mixture of _structured_ and _unstructured_ nodes.

A structured node contains a specific, constant number of _slots_ for other
nodes. For example, a division node is a structured node with a "top" slot and a
"bottom" slot. A square-root node is a structured node with an "inner" slot.
The expression root is also treated as a structured node with one slot.

Each slot contains one unstructured node. An unstructured node is a flat list
of child nodes, in order from left to right.

For any syntactically-valid expression tree, it will be possible to _upgrade_
it so that all unstructured nodes have exactly one child, and that child must
be a structured node. The tree can then simply be traversed depth-first to
evaluate it. For editing, the expression remains in its _downgraded_ state.
There are two nodes, _token_ and _placeholder_, which may only appear in a
downgraded tree.

## Input Example

The trees here are shown with `+` for structured nodes and `-` for unstructured
nodes.

| Input                     | Tree                                              |
|===========================|===================================================|
|                           | +root                                             | 
|---------------------------|---------------------------------------------------|
| 123                       | +root                                             |
|                           |   -top:                                           |
|                           |     +number(123)                                  |
|---------------------------|---------------------------------------------------|
| 123+456                   | +root                                             |
|                           |   -top:                                           |
|                           |     +number(123)                                  |
|                           |     +token(+)                                     |
|                           |     +number(456)                                  |
|---------------------------|---------------------------------------------------|
| 123+456-sqrt(_)           | +root                                             |
|                           |   -top:                                           |
|                           |     +number(123)                                  |
|                           |     +token(+)                                     |
|                           |     +number(456)                                  |
|                           |     +token(-)                                     |
|                           |     +sqrt                                         |
|                           |       -inner:                                     |
|                           |         +placeholder                              |
|---------------------------|---------------------------------------------------|
|                  2        | +root                                             |
| 123+456-sqrt(4)*---       |   -top:                                           |
|                  4        |     +number(123)                                  |
|                           |     +token(+)                                     |
|                           |     +number(456)                                  |
|                           |     +token(-)                                     |
|                           |     +sqrt                                         |
|                           |       -inner:                                     |
|                           |         +number(4)                                |
|                           |     +token(*)                                     |
|                           |     +divide                                       |
|                           |       -top:                                       |
|                           |         +number(2)                                |
|                           |       -bottom:                                    |
|                           |         +number(4)                                |
|---------------------------|---------------------------------------------------|

## Upgrading

As you can see from the diagram below, this process applies precedence and 
associativity, and ensures that all slots have only one child.

+root                           +root
  -top:                           -top:
    +number(123)  \     .---------> +subtract:            
    +token(+)      >---/--.           -left:
    +number(456)  /   /   |          /  +add:
    +token(-)  ------'    |          |   -left: 
    +sqrt            \    '---------<       +number(123)
      -inner:        |               |    -right:
        +number(4)   |               \      +number(456)
    +token(*)        |                -right:
    +divide           >------.       /  +multiply:
      -top:          |       |       |    -left:
        +number(2)   |       |       |      +sqrt
      -bottom:       |       |       |        -inner:
        +number(4)   /       |       |          +number(4)
                             '------<   +divide       
                                     |    -top:       
                                     |      +number(2)
                                     |    -bottom:    
                                     \      +number(4)