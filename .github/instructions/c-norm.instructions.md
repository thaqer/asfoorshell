---
applyTo: "**/*.{c,h}"
---
# C / 42 Norm constraints (no whitespace changes)

- Names are snake_case ASCII; explicit English.
- Functions: ≤25 lines (excluding braces), ≤4 params, ≤5 locals; declarations at top; one statement/line.
- Exactly one TAB between return type and function name; `return (expr);` or `return ;` for void.
- Braces on their own line (except inside struct/enum/union declarations).
- **Forbidden:** for, do…while, switch/case, goto, ternary `?:`, VLA, implicit int.
- Files: ≤5 function definitions per `.c`; never include `.c` into `.c`.
- Do **not** modify the 42 header; do **not** change indentation/whitespace.
