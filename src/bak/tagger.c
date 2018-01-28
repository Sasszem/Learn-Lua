// tagger.c
#include "includes.h"

#include "tagger.h"

#include "widgets.h"

GtkTextBuffer *program_buffer;
// temporarly var to store program

GtkTextTag *tags[9];
// TAGS:
// -flow
// -bool operators + values
// -funcs(return, break)
// -special(local, nil)
// -string
// -comments
// -escaped strings
// -io funcs(print)
// -error line(color is BG!)

// name groups for highlighting
char *FLOW = "do else elseif end for if repeat then until while function\0";
char *OPS = "and false not or true in\0";
char *FUNC = "break return\0";
char *IO = "print\0";
char *OTHER = "local nil\0";

// colors for the tags
char colors[9][8] = {"#CD9103\0", "#FFDA00\0", "#0079BD\0",
                     "#A40030\0", "#75CC2B\0", "#75CCEC\0",
                     "#929292\0", "#149292\0", "#FF0000\0"};

// buffer of the text currently processing
char *text;

// make all the tags
void make_tags() {
    // The first 4 are also bold
    for (int i = 0; i < 4; i++) {
        tags[i] = gtk_text_buffer_create_tag(program_buffer, NULL, "foreground",
                                             colors[i], "weight", 800, NULL);
    }
    // The second 4 are just regular
    for (int i = 0; i < 4; i++) {

        tags[i + 4] = gtk_text_buffer_create_tag(
            program_buffer, NULL, "foreground", colors[i + 4], NULL);
    }
    // the last is different
    tags[8] = gtk_text_buffer_create_tag(program_buffer, NULL, "background",
                                         colors[8], NULL);
    g_print("[Tagger]Tags created\n");
}

// tag a single keyword with the given tag
void tag_kw(char *kw, GtkTextTag *tag) {
    int l = strlen(kw);

    // offset holds our current position in the text
    int offset = -l;

    // tags and tage are the bounds of the current word we tag
    GtkTextIter tags, tage;

    while (1) {
        // Search kw in the text
        char *pos = g_strstr_len(text + offset + l, -1, kw);
        if (pos == NULL) {
            break; // If we at the end quit
        } else {
            offset = pos - text;               // recalc offset
            char b4 = *(text + offset - 1);    // the start of the word
            char after = *(text + offset + l); // the end of the word
            if (((after == ' ') || (after == '\n') || (after == '\0') ||
                 (after == ')') || (after == '(') || (after == '\t')) &&
                ((b4 == ' ') || (b4 == '\n') || (b4 == '\0') || (b4 == ')') ||
                 (b4 == '(') || (b4 == '\t'))) { // check if the word is a word

                // set iters
                gtk_text_buffer_get_iter_at_offset(program_buffer, &tags,
                                                   offset);
                gtk_text_buffer_get_iter_at_offset(program_buffer, &tage,
                                                   offset + l);

                // apply tag
                gtk_text_buffer_apply_tag(program_buffer, tag, &tags, &tage);
            }
        }
    }
    // free buffer
}

void tag_comments(GtkTextTag *tag) {

    int offset = 0;
    GtkTextIter coms, come;
    int state = 0; // 0 if looking for start of comment('--'), 1 if looking for
                   // end of comment('\n')
    while (*(text + offset) != '\0') { // iterate over text by chars

        if ((state == 0) &&
            (text[offset] ==
             '-') && // if looking for start of comment and found one
            (text[offset + 1] == '-')) {
            state = 1; // change state
            gtk_text_buffer_get_iter_at_offset(program_buffer, &coms,
                                               offset); // save start pos

        } else if (state == 1 && // if looking for end and found end of
                                 // line/buffer
                   (text[offset] == '\n' || text[offset] == '\0')) {
            state = 0; // change back state
            // get end pos
            gtk_text_buffer_get_iter_at_offset(program_buffer, &come, offset);

            // remove every tag from that area
            for (int i = 0; i < 4; i++) {
                gtk_text_buffer_remove_tag(program_buffer, tags[i], &coms,
                                           &come);
            }
            // apply new tag
            gtk_text_buffer_apply_tag(program_buffer, tag, &coms, &come);
        }
        offset++; // increment offset
    }
    // if left loop without closing the last comment
    // eg the last line was a comment
    // then close it as the others
    if (state == 1) {
        gtk_text_buffer_get_iter_at_offset(program_buffer, &come, offset);

        for (int i = 0; i < 4; i++) {
            gtk_text_buffer_remove_tag(program_buffer, tags[i], &coms, &come);
        }

        gtk_text_buffer_apply_tag(program_buffer, tag, &coms, &come);
    }
}

void tag_strings(GtkTextTag *tag, GtkTextTag *esc) {

    int offset = 0;

    GtkTextIter strs, stre; // iters
    GtkTextIter escs, esce;

    int state = 0;  // 0 if looking for start of string("), 1 if looking for
                    // end of comment(", but not \")
    int escape = 0; // if escape next char
    while (*(text + offset) != '\0') { // looping until eos

        if ((state == 0) &&
            (text[offset] ==
             '"')) {   // if looking for start of a string and found one
            state = 1; // change state
            gtk_text_buffer_get_iter_at_offset(program_buffer, &strs,
                                               offset); // save pos

        } else if (state == 1 && (text[offset] == '"' &
                                  escape == 0)) { // if looking for end of
                                                  // string and found one, but
                                                  // not escaping

            state = 0; // reset state
            gtk_text_buffer_get_iter_at_offset(program_buffer, &stre,
                                               offset + 1); // get pos
            // swipe out tags from string, EXCEPT escaping
            for (int i = 0; i < 4; i++) {
                gtk_text_buffer_remove_tag(program_buffer, tags[i], &strs,
                                           &stre);
            } // apply new tag
            gtk_text_buffer_apply_tag(program_buffer, tag, &strs, &stre);
        }

        if (state == 1) { // if still inside a string
            if (text[offset] == '\\' && escape == 0) { // if found a '\'
                escape = 1;                            // start of escape
                gtk_text_buffer_get_iter_at_offset(program_buffer, &escs,
                                                   offset); // save pos
            } else if (escape ==
                       1) { // if escaping(e.g the prev. char was a '\')
                escape = 0; // reset flag
                gtk_text_buffer_get_iter_at_offset(program_buffer, &esce,
                                                   offset + 1); // get pos
                gtk_text_buffer_apply_tag(program_buffer, esc, &escs,
                                          &esce); // apply tag
            }
        }

        offset++;
    }
    // if left the loop without closing last string
    // eg if we forgot a "
    // then close the string and apply tag
    if (state == 1) {
        gtk_text_buffer_get_iter_at_offset(program_buffer, &stre, offset);

        for (int i = 0; i < 4; i++) {
            gtk_text_buffer_remove_tag(program_buffer, tags[i], &strs, &stre);
        }
        gtk_text_buffer_apply_tag(program_buffer, tag, &strs, &stre);
    }
}

// apply a tag to a group of keyword defined above
void tag_kw_group(char *group, GtkTextTag *gtag) {
    char *items = g_strdup(group);

    char *token;
    while ((token = strsep(&items, " "))) {

        tag_kw(token, gtag);
    }

    g_free(items);
}

// tag everythingŰ(not just keywords)
// I know the name is misleading :(
void _tag_keywords() {
    // g_print("[Tagger]Begin tagging...\n");

    // get the text
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(program_buffer, &start, &end);
    text = gtk_text_buffer_get_text(program_buffer, &start, &end, FALSE);

    // remove all the previous tags
    for (int i = 0; i < 9; i++) {
        gtk_text_buffer_remove_tag(program_buffer, tags[i], &start, &end);
    }

    // tag eveything
    tag_kw_group(FLOW, tags[0]);
    tag_kw_group(OPS, tags[1]);
    tag_kw_group(FUNC, tags[2]);
    tag_kw_group(OTHER, tags[3]);
    tag_strings(tags[4], tags[6]);
    tag_comments(tags[5]);
    tag_kw_group(IO, tags[7]);

    // free memory
    g_free(text);
    // g_print("[Tagger]Tagging finished\n");
}

// Tag a line with red where an error happened
void _tag_error_line(int line) {
    g_print("[Tagger]Begin tagging error at line %d\n", line);
    GtkTextIter start, end;
    gtk_text_buffer_get_iter_at_line(program_buffer, &start, line);
    gtk_text_buffer_get_iter_at_line(program_buffer, &end, line);
    gtk_text_iter_forward_line(&end);
    gtk_text_iter_forward_to_line_end(&end);
    gtk_text_buffer_apply_tag(program_buffer, tags[8], &start, &end);
    gtk_text_view_scroll_to_iter(
        GTK_TEXT_VIEW(Widgets.get_object("program_view")), &start, 0.15, TRUE,
        0.5, 0.5);
    g_print("[Tagger]Error tagged...\n");
}

// init the tagger
void _init_tagger() {
    program_buffer = (GtkTextBuffer *)Widgets.get_object("program_buffer");
    make_tags();
}
