//tagger.h

void _tag_keywords();
void _init_tagger();
void _tag_error_line(int line);

static struct Tagger
{
void (*init)(void);
void (*tag)(void);
void (*tag_error)(int);
} Tagger =
{
.init=&_init_tagger,
.tag=&_tag_keywords,
.tag_error=&_tag_error_line
};

