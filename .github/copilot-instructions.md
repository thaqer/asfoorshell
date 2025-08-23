# Copilot Repo Instructions — Minishell (42)

- **Ignore the 42 file header** at the top of files. Do not comment on it or modify it.
- **Do not change indentation/whitespace or line wrapping.** If indentation violates the Norm, mention briefly but do **not** fix unless explicitly asked.
- Prefer **minimal, behavior-preserving** edits. No mass renames/reorders unless required for correctness.
- When asked to **fix the Norm**, enforce: naming (snake_case, English), ≤25 lines/function (excl. braces), ≤4 params, ≤5 locals, declarations at top, one statement/line, brace on its own line, `return (expr);`, no `for/do…while/switch/goto/?:/VLAs`.
- For **Minishell** specifics: one global (signal number only); heredoc text **not** in history; built-ins that mutate shell state run in the **parent**; `$?` reflects the last **foreground pipeline**.
- **Requesting files:** Ask for at most **2–3** files/functions at a time and include:
  - **Why** each file is needed (2–4 bullets).
  - **Steer me** options (e.g., `go: parser`, `go: exec`, `skip: signals`).
- **Output format for edits:** Use unified diffs in ```diff fences (or full files if shorter). Then add a short **Applied checks** list and which docs were used, e.g., `Used: 04_NORM_QUICKSTART.md, 02_PARSE_EXECUTION.md`.
- Prefer to **reference local docs** if available (e.g., `01_SPEC_MINISHELL.md` … `05_BUILD_SUBMISSION_QA.md`), but proceed even if absent.
