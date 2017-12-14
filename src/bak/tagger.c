// tagger.c
#include "includes.h"

#include "tagger.h"


GtkTextBuffer *program_buffer;

GtkTextTag *tags[9];


char *FLOW = "do else elseif end for if repeat then until while function\0";
char *OPS = "and false not or true in\0";
char *FUNC = "break return\0";
char *IO = "print input\0";
char *OTHER = "local nil\0";

// char kw_types[4][7]= {"FLOW\0","OPS\0","FUNC\0","OTHER\0"};
char colors[9][8] = {"#CD9103\0", "#FFDA00\0", "#0079BD\0", "#A40030\0",
                     "#75CC2B\0", "#75CCEC\0", "#929292\0", "#149292\0","#FF0000\0"};


void init_tagger()
{
program_buffer=(GtkTextBuffer*)gtk_builder_get_object(builder,"program_buffer");
make_tags();
}

void make_tags() {



    for (int i = 0; i < 4; i++) {
        tags[i] = gtk_text_buffer_create_tag(program_buffer, NULL, "foreground",
                                             colors[i], "weight", 800, NULL);
    }
    tags[4] = gtk_text_buffer_create_tag(program_buffer, NULL, "foreground",
                                         colors[4], NULL);

    tags[5] = gtk_text_buffer_create_tag(program_buffer, NULL, "foreground",
                                         colors[5], NULL);
    tags[6] = gtk_text_buffer_create_tag(program_buffer, NULL, "foreground",
                                         colors[6], NULL);
    tags[7] = gtk_text_buffer_create_tag(program_buffer, NULL, "foreground",
                                         colors[7], NULL);
    tags[8] = gtk_text_buffer_create_tag(program_buffer, NULL, "background",
                                         colors[8], NULL);
    g_print("Tags created\n");
}

void tag_kw(char *kw, GtkTextTag *tag) {
    int l = strlen(kw);
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(program_buffer, &start, &end);
    char *text = gtk_text_buffer_get_text(program_buffer, &start, &end, FALSE);
    int offset = -l;
    GtkTextIter tags, tage;
    // gtk_text_buffer_remove_tag(buffer, tag, &start, &end);
    while (1) {
        char *pos = g_strstr_len(text + offset + l, -1, kw);
        if (pos == NULL) {
            break;
        } else {
            offset = pos - text;
            char b4 = *(text + offset - 1);
            char after = *(text + offset + l);
            if (((after == ' ') || (after == '\n') || (after == '\0') ||
                 (after==')') || (after == '(') || (after == '\t')) &&
                ((b4 == ' ') || (b4 == '\n') || (b4 == '\0') || (b4 == ')') || (b4 == '(') || (b4 == '\t'))) {
                gtk_text_buffer_get_iter_at_offset(program_buffer, &tags,
                                                   offset);
                gtk_text_buffer_get_iter_at_offset(program_buffer, &tage,
                                                   offset + l);
                gtk_text_buffer_apply_tag(program_buffer, tag, &tags, &tage);
            }
        }
    }
    g_free(text);
}

void tag_comments(GtkTextTag *tag) {

    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(program_buffer, &start, &end);
    char *text = gtk_text_buffer_get_text(program_buffer, &start, &end, FALSE);
    int offset = 0;
    GtkTextIter coms, come;
    int state = 0; // 0 if looking for start of comment('--'), 1 if looking for
                   // end of comment('\n')
    while (*(text + offset) != '\0') {

        if ((state == 0) && (text[offset] == '-') &&
            (text[offset + 1] == '-')) {
            state = 1;
            gtk_text_buffer_get_iter_at_offset(program_buffer, &coms, offset);

        } else if (state == 1 &&
                   (text[offset] == '\n' || text[offset] == '\0')) {
            state = 0;

            gtk_text_buffer_get_iter_at_offset(program_buffer, &come, offset);

            for (int i = 0; i < 4; i++) {
                gtk_text_buffer_remove_tag(program_buffer, tags[i], &coms,
                                           &come);
            }

            gtk_text_buffer_apply_tag(program_buffer, tag, &coms, &come);
        }
        offset++;
    }
    if (state == 1) {
        gtk_text_buffer_get_iter_at_offset(program_buffer, &come, offset);

        for (int i = 0; i < 4; i++) {
            gtk_text_buffer_remove_tag(program_buffer, tags[i], &coms, &come);
        }

        gtk_text_buffer_apply_tag(program_buffer, tag, &coms, &come);
    }
}

void tag_strings(GtkTextTag *tag, GtkTextTag *esc) {

    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(program_buffer, &start, &end);
    char *text = gtk_text_buffer_get_text(program_buffer, &start, &end, FALSE);
    int offset = 0;
    GtkTextIter strs, stre;
    GtkTextIter escs, esce;
    int state = 0; // 0 if looking for start of string("), 1 if looking for
                   // end of comment(", but not \")
    int escape = 0;
    while (*(text + offset) != '\0') {

        if ((state == 0) && (text[offset] == '"')) {
            state = 1;
            gtk_text_buffer_get_iter_at_offset(program_buffer, &strs, offset);

        } else if (state == 1 && (text[offset] == '"' & escape == 0)) {

            state = 0;
            gtk_text_buffer_get_iter_at_offset(program_buffer, &stre,
                                               offset + 1);
            for (int i = 0; i < 4; i++) {
                gtk_text_buffer_remove_tag(program_buffer, tags[i], &strs,
                                           &stre);
            }

            gtk_text_buffer_apply_tag(program_buffer, tag, &strs, &stre);
        }

        if (state == 1) {
            if (text[offset] == '\\' && escape == 0) {
                escape = 1;
                gtk_text_buffer_get_iter_at_offset(program_buffer, &escs,
                                                   offset);
            } else if (escape == 1) {

                escape = 0;
                gtk_text_buffer_get_iter_at_offset(program_buffer, &esce,
                                                   offset + 1);
                gtk_text_buffer_apply_tag(program_buffer, esc, &escs, &esce);
            }
        }

        offset++;
    }
    if (state == 1) {
        gtk_text_buffer_get_iter_at_offset(program_buffer, &stre, offset);

        for (int i = 0; i < 4; i++) {
            gtk_text_buffer_remove_tag(program_buffer, tags[i], &strs, &stre);
        }
        gtk_text_buffer_apply_tag(program_buffer, tag, &strs, &stre);
    }
}

void tag_kw_group(char *group, GtkTextTag *gtag) {
    char *items = g_strdup(group);

    char *token;
    while ((token = strsep(&items, " "))) {

        tag_kw(token, gtag);
    }

    g_free(items);
}


void tag_keywords() {
    g_print("Begin tagging...\n");
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(program_buffer, &start, &end);
    for (int i = 0; i < 9; i++) {
        gtk_text_buffer_remove_tag(program_buffer, tags[i], &start, &end);
    }
    tag_kw_group(FLOW, tags[0]);
    tag_kw_group(OPS, tags[1]);
    tag_kw_group(FUNC, tags[2]);
    tag_kw_group(OTHER, tags[3]);
    tag_strings(tags[4], tags[6]);
    tag_comments(tags[5]);
    tag_kw_group(IO, tags[7]);
    g_print("Tagging finished\n");
}

void tag_error_line(int line)
{
g_print("Begin tagging error at line %d\n",line);
GtkTextIter start,end;
gtk_text_buffer_get_iter_at_line (program_buffer,&start,line);
gtk_text_buffer_get_iter_at_line (program_buffer,&end,line);
gtk_text_iter_forward_line(&end);
gtk_text_iter_forward_to_line_end (&end);
gtk_text_buffer_apply_tag(program_buffer,tags[8],&start,&end);
g_print("Error tagged...\n");
}


