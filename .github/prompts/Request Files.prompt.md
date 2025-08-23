---
mode: "ask"
description: "Politely request 2–3 specific files with rationale and steering options."
---
Based on the user's goal, request at most **2–3** files. Use this format:

```
Needed now: main.c, signals.c
Why:
- main.c → check prompt loop & parent/child split
- signals.c → Ctrl-C/D/\ handling & single-global rule
Steer me:
- go: parser (lexer.c + expand.c)
- go: exec (exec.c + exec_pipeline.c)
- skip: signals (I’ll proceed with parsing)
```
