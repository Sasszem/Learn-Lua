//tagger.h

void tag_keywords();

void make_tags();
void tag_kw_group(char *group, GtkTextTag *tag);

GtkTextTag *tags[7];
//FLOW, OPS, FUNC, OTHER, STR, COMMENT, ESCAPE
