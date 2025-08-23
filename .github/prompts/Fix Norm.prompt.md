---
mode: "agent"
description: "Fix 42 Norm issues in selected C files (no indentation edits)."
---
**Task**: Fix 42 Norm issues in the attached files while preserving behavior. **Do not** change indentation or whitespace. **Ignore** the 42 header.

**Steps**
1) Identify violations (naming, function length, params, locals, declarations at top, forbidden constructs, brace style, return style).
2) Propose minimal changes to fix them (avoid churn).
3) Return unified diffs in ```diff fences (or full files if shorter).
4) Add **Applied checks** and list any local docs used (e.g., `04_NORM_QUICKSTART.md`).

**If more context is needed**, ask for ≤3 files with:
```
Needed now: <file1>, <file2> (optional <file3>)
Why:
- <file1> → <specific reason>
- <file2> → <specific reason>
Steer me:
- go: parser
- go: exec
- skip: signals
```
