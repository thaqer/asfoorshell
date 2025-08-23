# Copilot tuning (VS Code)

Drop this folder at the root of your minishell repo (merge with existing `.github/` and `.vscode/` if present).

## What you get
- `.github/copilot-instructions.md` (repo-wide guidance)
- `.github/instructions/*.instructions.md` (scoped rules; auto-applied via `applyTo`)
- `.github/prompts/*.prompt.md` (reusable prompts for Chat)
- `.vscode/settings.json` (enable instruction & prompt files; codebase search)

## Enable in VS Code
1. Install **GitHub Copilot Chat**.
2. Settings → search **Instruction Files** → ensure “Use Instruction Files” is **on**.
3. Settings → search **promptFiles** → ensure `chat.promptFiles` is **true**.
4. Settings → **Codesearch** → enable `github.copilot.chat.codesearch.enabled`.

## Use
- Open Chat → **Attach context → Prompt…** → choose **Fix Norm** or **Request Files**.
- Or type prompts directly and attach files/folders or use `#codebase` / `@workspace`.

Notes:
- Prompt files are preview/experimental in VS Code today.
- Org-level custom instructions exist for Copilot Enterprise (set on github.com) and combine with repo/user instructions.
