#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#incldue <ctype.h>

#define BUF_SIZE 28

#ifdef LINKED_LIST
#include "List.h"
#else
#include <stdbool.h>
#endif

enum Language {FR, EN};
const char *path[] = {"Fr.txt", "En.txt"};
const char *separators = " ,?;.:!()'[]";
char toN[26] = {};

int getNum(int);
bool isConsonant(int);
int toUpper(int);
int toLower(int);
char *zSoundexWord(const char *, unsigned, char *);

#ifdef LINKED_LIST
List *
#elif defined (STRING)
char *
#endif
zSoundex(const char *, enum Language);

int init_Tab_Soundex(const char *path) {
    char buf[BUF_SIZE];
    FILE *f = fopen(path, "r");
    if(f == NULL)
        return 0;
    while(fgets(buf, BUF_SIZE, f) != NULL) {
        int n = buf[0] - '0';
        for(int i = 1; buf[i] != '\n' && buf[i] != '\0'; ++i)
            toN[buf[i] - 'A'] = n;
    }
    return 1;
}

int getNum(int letter) {
    letter = toUpper(letter);
    return toN[letter - 'A'];
}

bool isConsonant(int c) { if(c <= 'Z' && c > 'A' && c != 'E' && c != 'I' && c != 'O' && c != 'U' && c != 'Y') return true; return false; }

char *zSoundexWord(const char *word, unsigned size, char *ret) { //ret must be a 5-cell tab
    int index = 1;
    if(word == NULL || ret == NULL) return NULL;
    for(unsigned i = 0; word[i] != '\0' && i < size; ++i)
        if((toLower(word[i]) < 'a' || toLower(word[i]) > 'z') && strchr(separators, word[i]) == NULL)
            return NULL;
    ret[0] = toUpper(word[0]);

    for(unsigned i = 1; word[i] != '\0' && i < size && index < 4; ++i) {
        if(isConsonant(toUpper(word[i]))
        && toUpper(word[i]) != 'H' && toUpper(word[i]) != 'W'
        && (getNum(word[i]) != getNum(word[i-1])))
            ret[index++] = getNum(word[i]) + '0';
    }

    for(; index < 4; ++index)
        ret[index] = '0';
    ret[index] = '\0';
    return ret;
}

#ifdef LINKED_LIST
List *
#elif defined (STRING)
char *
#endif
zSoundex(const char *str, enum Language lgg) {
    if(init_Tab_Soundex(path[lgg]) == 0) return NULL;
#ifdef LINKED_LIST
    List *ret = List_init();
    size_t  index = 0,
            length = strlen(str);
    bool inword = false;
    char *add;

    for(size_t i = 0; i <= length; ++i) {
        if(strchr(separators, str[i]) != NULL || i == length) {
            if(inword == true) {
                add = malloc(5);
                List_add(zSoundexWord(&str[index], i-index, add), ret);
                index = i;
                inword = false;
                free(add);
            }
            ++index;
        } else
            inword = true;
    }
#elif defined (STRING)
    char *ret,
         tmp[5];
    size_t length = strlen(str),
            nWords = 0,
            index = 0;
    bool inword = false;

    for(size_t i = 0; i <= length; ++i) {
        if(strchr(separators, str[i]) != NULL || i == length) {
            if(inword == true) {
                inword = false;
                ++nWords;
            }
        } else
            inword = true;
    }
    ret = malloc(nWords*5); //nWords * 4 (4 chars/word) + nWords-1 (space chars) + 1 (\0) -> nWords*4 + nWords - 1 + 1 = nWords*5
    ret[0] = '\0';
    if(ret != NULL) {
        for(size_t i = 0; i <= length; ++i) {
            if(strchr(separators, str[i]) != NULL || i == length) {
                if(inword == true) {
                    zSoundexWord(&str[index], i-index, tmp);
                    strcat(ret, tmp);
                    strcat(ret, " ");
                    index = i;
                    inword = false;
                }
                ++index;
            } else
                inword = true;
        }
        ret[5*nWords - 1] = '\0';
    }
#endif
    return ret;
}

bool isEqualSoundex(const char *str1, const char *str2, enum Language lgg) {
#ifdef LINKED_LIST
    List *l1 = zSoundex(str1, lgg),
         *l2 = zSoundex(str2, lgg);
    bool ret = List_equal(l1, l2);
    List_free(l1);
    List_free(l2);
    return ret;
#elif defined (STRING)
    char *s1 = zSoundex(str1, lgg),
         *s2 = zSoundex(str2, lgg);
    bool ret = !strcmp(s1, s2);
    free(s1); s1 = NULL;
    free(s2); s2 = NULL;
#endif
    return ret;
}

int main(void) {
    return isEqualSoundex("Je suis un chat.", "Ji souys une cat...", FR);
}
