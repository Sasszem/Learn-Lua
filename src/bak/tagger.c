// tagger.c
#include "includes.h"

#include "tagger.h"

char *FLOW = "do else elseif end for if repeat then until while\0";
char *OPS = "and false not or true in\0";
char *FUNC = "break function return\0";
char *OTHER = "local nil\0";

// char kw_types[4][7]= {"FLOW\0","OPS\0","FUNC\0","OTHER\0"};
char colors[7][8] = {"#CD7714\0", "#FFDA00\0", "#0079BD\0", "#A40030\0",
                     "#45B430\0", "#3D3890\0", "#929292\0"};

void make_tags() {
    for (int i = 0; i < 4; i++) {
        tags[i] = gtk_text_buffer_create_tag(buffer, NULL, "foreground",
                                             colors[i], "weight", 800, NULL);
    }
    tags[4] =
        gtk_text_buffer_create_tag(buffer, NULL, "foreground", colors[4], NULL);

    tags[5] =
        gtk_text_buffer_create_tag(buffer, NULL, "foreground", colors[5], NULL);
    tags[6] =
        gtk_text_buffer_create_tag(buffer, NULL, "foreground", colors[6], NULL);
    g_print("Tags created\n");
}

void tag_kw(char *kw, GtkTextTag *tag) {
    int l = strlen(kw);
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    char *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
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
            if (((after == ' ') || (after == '\n') || (after == '\0')) &&
                ((b4 == ' ') || (b4 == '\n') || (b4 == '\0'))) {
                gtk_text_buffer_get_iter_at_offset(buffer, &tags, offset);
                gtk_text_buffer_get_iter_at_offset(buffer, &tage, offset + l);
                gtk_text_buffer_apply_tag(buffer, tag, &tags, &tage);
            }
        }
    }
    g_free(text);
}

void tag_comments(GtkTextTag *tag) {

    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    char *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    int offset = 0;
    GtkTextIter tags, tage;
    int state = 0; // 0 if looking for start of comment('--'), 1 if looking for
                   // end of comment('\n')
    while (*(text + offset) != '\0') {

        if ((state == 0) && (text[offset] == '-') &&
            (text[offset + 1] == '-')) {
            state = 1;
            gtk_text_buffer_get_iter_at_offset(buffer, &tags, offset);

        } else if (state == 1 &&
                   (text[offset] == '\n' || text[offset] == '\0')) {
            state = 0;

            gtk_text_buffer_get_iter_at_offset(buffer, &tage, offset);
            gtk_text_buffer_apply_tag(buffer, tag, &tags, &tage);
        }
        offset++;
    }
    if (state == 1) {
        gtk_text_buffer_get_iter_at_offset(buffer, &tage, offset);
        gtk_text_buffer_apply_tag(buffer, tag, &tags, &tage);
    }
}

void tag_strings(GtkTextTag *tag, GtkTextTag *esc) {

    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    char *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    int offset = 0;
    GtkTextIter strs, stre;
    GtkTextIter escs, esce;
    int state = 0; // 0 if looking for start of string("), 1 if looking for
                   // end of comment(", but not \")
    int escape = 0;
    while (*(text + offset) != '\0') {

        if ((state == 0) && (text[offset] == '"')) {
            state = 1;
            gtk_text_buffer_get_iter_at_offset(buffer, &strs, offset);

        } else if (state == 1 &&
                   (text[offset] == '"' & text[offset - 1] != '\\')) {

            state = 0;
            for (int i = 0; i<4; i++)
    {
            gtk_text_buffer_remove_tag(buffer, tags[i], &start, &end);
    }
            gtk_text_buffer_get_iter_at_offset(buffer, &stre, offset + 1);
            gtk_text_buffer_apply_tag(buffer, tag, &strs, &stre);
            
        }

        if (state == 1) {
            if (text[offset] == '\\' && escape == 0) {
                escape = 1;
                gtk_text_buffer_get_iter_at_offset(buffer, &escs, offset);
            } else if (escape == 1) {

                escape = 0;
                gtk_text_buffer_get_iter_at_offset(buffer, &esce, offset+1);
                gtk_text_buffer_apply_tag(buffer, esc, &escs, &esce);
            }
        }

        offset++;
    }
    if (state == 1) {
        gtk_text_buffer_get_iter_at_offset(buffer, &stre, offset);
        gtk_text_buffer_apply_tag(buffer, tag, &strs, &stre);
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
GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    for (int i = 0; i<6; i++)
    {
            gtk_text_buffer_remove_tag(buffer, tags[i], &start, &end);
    }
    tag_kw_group(FLOW, tags[0]);
    tag_kw_group(OPS, tags[1]);
    tag_kw_group(FUNC, tags[2]);
    tag_kw_group(OTHER, tags[3]);
    tag_strings(tags[4], tags[6]);
    tag_comments(tags[5]);
}
