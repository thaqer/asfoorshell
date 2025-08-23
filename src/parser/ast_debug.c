// #include <stdio.h>
// #include <string.h>
// #include <unistd.h>
// #include <stdlib.h>
// #include "parser.h"

// /* ---------- tiny utils ---------- */

// static const char *node_type_str(t_ast_node_type t) {
//     switch (t) {
//         case AST_SIMPLE_COMMAND: return "SIMPLE";
//         case AST_PIPELINE:       return "PIPE";
//         case AST_LOGICAL_AND:    return "AND";
//         case AST_LOGICAL_OR:     return "OR";
//         case AST_SUBSHELL:       return "SUBSHELL";
//         default:                 return "?";
//     }
// }

// const char *redir_type_str(t_redir_type t) {
//     switch (t) {
//         case REDIR_IN:     return "<";
//         case REDIR_OUT:    return ">";
//         case REDIR_APPEND: return ">>";
//         case REDIR_HEREDOC:return "<<";
//         default:           return "?";
//     }
// }

// static void fdputs(int fd, const char *s) { if (s) write(fd, s, strlen(s)); }
// static void fdputc(int fd, char c) { write(fd, &c, 1); }

// /* join argv into one line, best-effort (for display only) */
// static char *argv_summary(char *const *argv) {
//     if (!argv || !argv[0]) return strdup("(none)");
//     size_t cap = 64, len = 0;
//     char *buf = malloc(cap);
//     if (!buf) return NULL;
//     buf[0] = '\0';
//     for (int i = 0; argv[i]; ++i) {
//         const char *w = argv[i];
//         size_t wl = strlen(w);
//         if (len + wl + 2 >= cap) {
//             cap = (cap * 2) + wl + 16;
//             char *nb = realloc(buf, cap);
//             if (!nb) { free(buf); return NULL; }
//             buf = nb;
//         }
//         if (i) { buf[len++] = ' '; }
//         memcpy(buf + len, w, wl);
//         len += wl;
//         buf[len] = '\0';
//     }
//     return buf;
// }

// /* ---------- ASCII printer ---------- */
// /* ---------- ASCII printer ---------- */

// static void print_redirs_fd(const t_redir *r, int fd, const char *prefix)
// {
//     for ( ; r; r = r->next) {
//         fdputs(fd, prefix);
//         fdputs(fd, "  └─ redir ");
//         fdputs(fd, (char*)redir_type_str(r->type));
//         fdputs(fd, " ");
//         fdputs(fd, r->target ? r->target : "(null)");
//         fdputc(fd, '\n');
//     }
// }

// static char *make_child_prefix(const char *prefix, int is_last)
// {
//     size_t L = strlen(prefix);
//     char  *p = malloc(L + 3 + 1);          // "│  " or "   " + NUL
//     if (!p) return NULL;
//     memcpy(p, prefix, L);
//     memcpy(p + L, is_last ? "   " : "│  ", 3);
//     p[L + 3] = '\0';
//     return p;
// }

// /* prefix drawing: pass e.g. "" at root */
// static void ast_print_impl_fd(const t_ast *n, int fd, const char *prefix, int is_last)
// {
//     if (!n) return;

//     /* branch connector */
//     fdputs(fd, prefix);
//     fdputs(fd, is_last ? "└─ " : "├─ ");

//     switch (n->type) {
//         case AST_SIMPLE_COMMAND: {
//             char *sum = argv_summary(n->simple_command.argv);
//             fdputs(fd, "SIMPLE  ");
//             fdputs(fd, sum ? sum : "(oom)");
//             fdputc(fd, '\n');
//             free(sum);

//             char *childpref = make_child_prefix(prefix, is_last);
//             if (!childpref) return;
//             print_redirs_fd(n->simple_command.redirs, fd, childpref);
//             free(childpref);
//         } break;

//         case AST_PIPELINE:
//         case AST_LOGICAL_AND:
//         case AST_LOGICAL_OR: {
//             fdputs(fd, node_type_str(n->type));
//             fdputc(fd, '\n');

//             char *childpref = make_child_prefix(prefix, is_last);
//             if (!childpref) return;

//             /* left then right; right is last */
//             ast_print_impl_fd(n->binary.left,  fd, childpref, 0);
//             ast_print_impl_fd(n->binary.right, fd, childpref, 1);
//             free(childpref);
//         } break;

//         case AST_SUBSHELL: {
//             fdputs(fd, "SUBSHELL\n");

//             char *childpref = make_child_prefix(prefix, is_last);
//             if (!childpref) return;

//             /* child subtree; if redirs exist, child is not last */
//             int child_is_last = (n->subshell.redirs == NULL);
//             ast_print_impl_fd(n->subshell.child, fd, childpref, child_is_last == 1);

//             /* redirs (if any) are printed after child */
//             if (n->subshell.redirs) {
//                 print_redirs_fd(n->subshell.redirs, fd, childpref);
//             }
//             free(childpref);
//         } break;

//         default:
//             fdputs(fd, "?\n");
//             break;
//     }
// }

// void ast_print_fd(const t_ast *node, int fd) { ast_print_impl_fd(node, fd, "", 1); }
// void ast_print(const t_ast *node) { ast_print_fd(node, 2); }  /* stderr by default */


// /* ---------- Graphviz DOT generator ---------- */

// static void dot_escape(FILE *f, const char *s) {
//     if (!s) { fputs("(null)", f); return; }
//     for (; *s; ++s) {
//         unsigned char c = (unsigned char)*s;
//         if (c == '"' || c == '\\') fputc('\\', f);
//         if (c == '\n') { fputs("\\n", f); continue; }
//         fputc(c, f);
//     }
// }

// static void dot_node_label(FILE *f, const t_ast *n)
// {
//     switch (n->type) {
//         case AST_SIMPLE_COMMAND: {
//             char *sum = argv_summary(n->simple_command.argv);
//             fputs("SIMPLE\\n", f);
//             dot_escape(f, sum ? sum : "(oom)");
//             free(sum);
//         } break;
//         case AST_PIPELINE:    fputs("PIPE", f); break;
//         case AST_LOGICAL_AND: fputs("AND", f); break;
//         case AST_LOGICAL_OR:  fputs("OR",  f); break;
//         case AST_SUBSHELL:    fputs("SUBSHELL", f); break;
//         default:              fputs("?", f); break;
//     }
// }

// static int dot_emit(FILE *f, const t_ast *n, int *next_id)
// {
//     if (!n) return -1;
//     int my = (*next_id)++;
//     fprintf(f, "  n%d [shape=box, style=rounded, label=\"", my);
//     dot_node_label(f, n);
//     fputs("\"];\n", f);

//     /* attach redirs as tiny diamond nodes */
//     if (n->type == AST_SIMPLE_COMMAND) {
//         for (const t_redir *r = n->simple_command.redirs; r; r = r->next) {
//             int rid = (*next_id)++;
//             fprintf(f, "  n%d [shape=diamond, label=\"%s ", rid, redir_type_str(r->type));
//             dot_escape(f, r->target ? r->target : "(null)");
//             fputs("\"];\n", f);
//             fprintf(f, "  n%d -> n%d;\n", my, rid);
//         }
//     } else if (n->type == AST_SUBSHELL) {
//         for (const t_redir *r = n->subshell.redirs; r; r = r->next) {
//             int rid = (*next_id)++;
//             fprintf(f, "  n%d [shape=diamond, label=\"%s ",
//                     rid, redir_type_str(r->type));
//             dot_escape(f, r->target ? r->target : "(null)");
//             fputs("\"];\n", f);
//             fprintf(f, "  n%d -> n%d;\n", my, rid);
//         }
//     }

//     /* recurse to children */
//     if (n->type == AST_PIPELINE || n->type == AST_LOGICAL_AND || n->type == AST_LOGICAL_OR) {
//         int left = dot_emit(f, n->binary.left, next_id);
//         int right = dot_emit(f, n->binary.right, next_id);
//         if (left >= 0)  fprintf(f, "  n%d -> n%d [label=\"L\"];\n", my, left);
//         if (right >= 0) fprintf(f, "  n%d -> n%d [label=\"R\"];\n", my, right);
//     } else if (n->type == AST_SUBSHELL) {
//         int c = dot_emit(f, n->subshell.child, next_id);
//         if (c >= 0) fprintf(f, "  n%d -> n%d;\n", my, c);
//     }
//     return my;
// }

// int ast_to_dot_file(const t_ast *node, const char *path)
// {
//     FILE *f = fopen(path, "w");
//     if (!f) return -1;

//     fputs("digraph AST {\n", f);
//     fputs("  rankdir=TB;\n  node [fontname=\"Menlo,Consolas,monospace\"];\n", f);

//     int next_id = 0;
//     if (!node) {
//         fputs("  empty [label=\"(empty)\"];\n", f);
//     } else {
//         dot_emit(f, node, &next_id);
//     }
//     fputs("}\n", f);
//     fclose(f);
//     return 0;
// }

/* ast_print.c
 *
 * Human-readable AST printer + Graphviz .dot exporter for Minishell.
 * - Prints argv[i] flags and redirection target_qflags using enum names:
 *   QF_NONE, QF_HAS_SQ, QF_HAS_DQ, QF_ALL_SQ, QF_ALL_DQ, QF_MIXED
 * - Fixes DOT syntax by escaping all embedded quotes inside label strings.
 *
 * Public API:
 *   void ast_print(const t_ast *node);
 *   void ast_print_fd(const t_ast *node, int fd);
 *   int  ast_to_dot_file(const t_ast *node, const char *path);
 */

/* ast_print.c
 *
 * Human-readable AST printer + Graphviz .dot exporter for Minishell.
 * - Prints argv[i] flags and redirection target_qflags using enum names:
 *   QF_NONE, QF_HAS_SQ, QF_HAS_DQ, QF_ALL_SQ, QF_ALL_DQ, QF_MIXED
 * - Fixes DOT syntax by escaping all embedded quotes inside label strings.
 *
 * Public API:
 *   void ast_print(const t_ast *node);
 *   void ast_print_fd(const t_ast *node, int fd);
 *   int  ast_to_dot_file(const t_ast *node, const char *path);
 */

#include "minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

/* ---------- local utils ---------- */

#ifndef UNUSED
# define UNUSED(x) ((void)(x))
#endif

static void fd_puts(int fd, const char *s) {
    if (!s) return;
    write(fd, s, (size_t)strlen(s));
}

static void fd_printf(int fd, const char *fmt, ...) {
    char buf[4096];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    fd_puts(fd, buf);
}

static void indent_fd(int fd, int depth) {
    for (int i = 0; i < depth; ++i) fd_puts(fd, "  ");
}

/* append helper that keeps buf NUL-terminated */
static void sappend(char *dst, size_t cap, const char *src) {
    size_t len = strlen(dst);
    if (len >= cap) return;
    snprintf(dst + len, (cap > len ? cap - len : 0), "%s", src ? src : "");
}

/* ---------- enum → string helpers ---------- */

static const char *ast_type_str(t_ast_node_type t) {
    switch (t) {
        case AST_SIMPLE_COMMAND: return "SIMPLE_COMMAND";
        case AST_PIPELINE:       return "PIPELINE";
        case AST_LOGICAL_AND:    return "LOGICAL_AND";
        case AST_LOGICAL_OR:     return "LOGICAL_OR";
        case AST_SUBSHELL:       return "SUBSHELL";
        default:                 return "UNKNOWN";
    }
}

const char *redir_type_str(t_redir_type t) {
    switch (t) {
        case REDIR_IN:      return "<";
        case REDIR_OUT:     return ">";
        case REDIR_APPEND:  return ">>";
        case REDIR_HEREDOC: return "<<";
        default:            return "?";
    }
}

/* ---- QFLAGS printers (argv[i].flags and redir.target_qflags) ---- */
static void qflags_to_str(unsigned int f, char *buf, size_t n) {
    if (!buf || n == 0) return;
    buf[0] = '\0';

    if (f == QF_NONE) {
        snprintf(buf, n, "QF_NONE");
        return;
    }
    if (f & QF_HAS_SQ) sappend(buf, n, "QF_HAS_SQ|");
    if (f & QF_HAS_DQ) sappend(buf, n, "QF_HAS_DQ|");
    if (f & QF_ALL_SQ) sappend(buf, n, "QF_ALL_SQ|");
    if (f & QF_ALL_DQ) sappend(buf, n, "QF_ALL_DQ|");
    if (f & QF_MIXED)  sappend(buf, n, "QF_MIXED|");

    size_t len = strlen(buf);
    if (len && buf[len - 1] == '|') buf[len - 1] = '\0';
}

/* ---------- pretty-printer (human-readable) ---------- */

static void ast_print_fd_depth(const t_ast *node, int fd, int depth);

static void print_redirs_fd(int fd, const t_redir *r, int depth) {
    char fbuf[128];

    for (const t_redir *p = r; p; p = p->next) {
        indent_fd(fd, depth);
        fd_printf(fd, "redir %s target=\"%s\"",
                  redir_type_str(p->type), p->target ? p->target : "(null)");

        fd_printf(fd, " delim_quoted=%d", (int)p->s_delim_quoted);

        qflags_to_str((unsigned int)p->s_delim_quoted, fbuf, sizeof(fbuf));
        fd_printf(fd, " target_qflags=%s", fbuf);

        fd_puts(fd, "\n");
    }
}

static void print_simple_cmd_fd(int fd, const t_ast *n, int depth) {
    indent_fd(fd, depth);
    fd_puts(fd, "SIMPLE_COMMAND\n");

    if (n->s_simple_command.argv) {
        for (int i = 0; n->s_simple_command.argv[i]; ++i) {
            indent_fd(fd, depth + 1);
            fd_printf(fd, "argv[%d] = \"%s\"", i, n->s_simple_command.argv[i]);
            fd_puts(fd, "\n");
        }
    } else {
        indent_fd(fd, depth + 1);
        fd_puts(fd, "(argv=NULL)\n");
    }

    if (n->s_simple_command.redirs) {
        indent_fd(fd, depth + 1);
        fd_puts(fd, "redirections:\n");
        print_redirs_fd(fd, n->s_simple_command.redirs, depth + 2);
    }
}

static void print_binary_fd(int fd, const t_ast *n, int depth) {
    indent_fd(fd, depth);
    fd_printf(fd, "%s\n", ast_type_str(n->type));
    if (n->s_binary.left) {
        indent_fd(fd, depth + 1); fd_puts(fd, "left:\n");
        ast_print_fd_depth(n->s_binary.left, fd, depth + 2);
    }
    if (n->s_binary.right) {
        indent_fd(fd, depth + 1); fd_puts(fd, "right:\n");
        ast_print_fd_depth(n->s_binary.right, fd, depth + 2);
    }
}

static void print_subshell_fd(int fd, const t_ast *n, int depth) {
    indent_fd(fd, depth);
    fd_puts(fd, "SUBSHELL\n");
    if (n->s_subshell.child) {
        indent_fd(fd, depth + 1); fd_puts(fd, "child:\n");
        ast_print_fd_depth(n->s_subshell.child, fd, depth + 2);
    }
    if (n->s_subshell.redirs) {
        indent_fd(fd, depth + 1); fd_puts(fd, "redirections:\n");
        print_redirs_fd(fd, n->s_subshell.redirs, depth + 2);
    }
}

static void ast_print_fd_depth(const t_ast *node, int fd, int depth) {
    if (!node) {
        indent_fd(fd, depth);
        fd_puts(fd, "(null AST)\n");
        return;
    }
    switch (node->type) {
        case AST_SIMPLE_COMMAND: print_simple_cmd_fd(fd, node, depth); break;
        case AST_PIPELINE:
        case AST_LOGICAL_AND:
        case AST_LOGICAL_OR:     print_binary_fd(fd, node, depth);     break;
        case AST_SUBSHELL:       print_subshell_fd(fd, node, depth);   break;
        default:
            indent_fd(fd, depth);
            fd_puts(fd, "UNKNOWN NODE\n");
            break;
    }
}

void ast_print_fd(const t_ast *node, int fd) { ast_print_fd_depth(node, fd, 0); }
void ast_print(const t_ast *node) { ast_print_fd(node, STDERR_FILENO); }

/* ---------- Graphviz DOT exporter ---------- */

typedef struct s_idmap {
    const t_ast      *node;
    int               id;
    struct s_idmap   *next;
} t_idmap;

static int map_get_or_add(t_idmap **map, const t_ast *n, int *autoid) {
    for (t_idmap *p = *map; p; p = p->next)
        if (p->node == n) return p->id;
    t_idmap *nw = (t_idmap*)calloc(1, sizeof(*nw));
    if (!nw) return (*autoid)++; /* fallback if OOM */
    nw->node = n;
    nw->id   = (*autoid)++;
    nw->next = *map;
    *map = nw;
    return nw->id;
}

static void free_map(t_idmap *map) {
    while (map) { t_idmap *n = map->next; free(map); map = n; }
}

/* Escape for DOT-quoted labels: ", \, and \n */
static void dot_escape(const char *s, FILE *f) {
    if (!s) return;
    for (const unsigned char *p = (const unsigned char *)s; *p; ++p) {
        unsigned char c = *p;
        if (c == '"' || c == '\\') { fputc('\\', f); fputc(c, f); }
        else if (c == '\n') fputs("\\n", f);
        else fputc(c, f);
    }
}

/* NOTE: emit_* functions write INSIDE an already-open label=" ... " string.
 * Any embedded double quotes MUST be emitted as \" (i.e., write "\\\"" in C).
 */
static void emit_redirs_inline(FILE *f, const t_redir *r) {
    char fbuf[128];
    for (const t_redir *p = r; p; p = p->next) {
        fputs("\\n", f);
        fputs(redir_type_str(p->type), f);
        fputs(" ", f);

        /* target wrapped in escaped quotes because we are inside a label */
        fputs("\\\"", f);
        if (p->target) dot_escape(p->target, f);
        else fputs("(null)", f);
        fputs("\\\"", f);

        /* heredoc + qflags */
        fputs(" [delim_quoted=", f);
        fprintf(f, "%d", (int)p->s_delim_quoted);

        qflags_to_str((unsigned int)p->s_delim_quoted, fbuf, sizeof(fbuf));
        fputs(", qflags=", f);
        dot_escape(fbuf, f);
        fputc(']', f);
    }
}

static void emit_node_label(FILE *f, const t_ast *n) {
    switch (n->type) {
        case AST_SIMPLE_COMMAND:
            fputs("SIMPLE", f);
            if (n->s_simple_command.argv && n->s_simple_command.argv[0]) {
                fputs("\\n", f);
                fputs("argv[0]=\\\"", f);               /* escaped opening quote */
                dot_escape(n->s_simple_command.argv[0], f);
                fputs("\\\"", f);                       /* escaped closing quote */
            }
            if (n->s_simple_command.redirs)
                emit_redirs_inline(f, n->s_simple_command.redirs);
            break;

        case AST_PIPELINE:     fputs("PIPELINE |", f); break;
        case AST_LOGICAL_AND:  fputs("LOGICAL &&", f); break;
        case AST_LOGICAL_OR:   fputs("LOGICAL ||", f); break;

        case AST_SUBSHELL:
            fputs("SUBSHELL", f);
            if (n->s_subshell.redirs)
                emit_redirs_inline(f, n->s_subshell.redirs);
            break;

        default:
            fputs("UNKNOWN", f); break;
    }
}

static void dot_emit(FILE *out, t_idmap **map, const t_ast *n, int *autoid) {
    if (!n) return;

    int id = map_get_or_add(map, n, autoid);

    /* node shape + label */
    fprintf(out, "  n%d [shape=box, style=rounded, fontname=\"Courier New\", label=\"", id);
    emit_node_label(out, n);
    fputs("\"];\n", out);

    /* edges + children */
    if (n->type == AST_SIMPLE_COMMAND) {
        /* attach argv nodes for clarity */
        if (n->s_simple_command.argv) {
            for (int i = 0; n->s_simple_command.argv[i]; ++i) {
                int child_id = (*autoid)++;
                fprintf(out, "  n%d [shape=note, fontname=\"Courier New\", label=\"argv[%d] = \\\"",
                        child_id, i);
                dot_escape(n->s_simple_command.argv[i], out);
                fputs("\\\"", out);
                fputs("\"];\n", out);
                fprintf(out, "  n%d -> n%d;\n", id, child_id);
            }
        }
    } else if (n->type == AST_SUBSHELL) {
        if (n->s_subshell.child) {
            int child_id = map_get_or_add(map, n->s_subshell.child, autoid);
            fprintf(out, "  n%d -> n%d [label=\"child\"];\n", id, child_id);
            dot_emit(out, map, n->s_subshell.child, autoid);
        }
    } else if (n->type == AST_PIPELINE || n->type == AST_LOGICAL_AND || n->type == AST_LOGICAL_OR) {
        if (n->s_binary.left) {
            int L = map_get_or_add(map, n->s_binary.left, autoid);
            fprintf(out, "  n%d -> n%d [label=\"left\"];\n", id, L);
            dot_emit(out, map, n->s_binary.left, autoid);
        }
        if (n->s_binary.right) {
            int R = map_get_or_add(map, n->s_binary.right, autoid);
            fprintf(out, "  n%d -> n%d [label=\"right\"];\n", id, R);
            dot_emit(out, map, n->s_binary.right, autoid);
        }
    }
}

int ast_to_dot_file(const t_ast *root, const char *path) {
    if (!path) return -1;
    FILE *out = fopen(path, "w");
    if (!out) return -1;

    fprintf(out, "digraph AST {\n");
    fprintf(out, "  rankdir=TB;\n");
    fprintf(out, "  node [shape=box];\n");

    t_idmap *map = NULL;
    int autoid = 1;

    if (root)
        dot_emit(out, &map, root, &autoid);
    else
        fprintf(out, "  null [label=\"(null AST)\"];\n");

    fprintf(out, "}\n");
    fclose(out);
    free_map(map);
    return 0;
}
