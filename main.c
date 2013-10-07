#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"

#define FR

int getNum(int);
bool isConsonant(int);
int toUpper(int);
int toLower(int);
char *zSoundexWord(const char *, unsigned, char *);
List *zSoundex(const char *);

const char *separators = " ,?;.:!()'[]";

int NumFr[26] = {0, 1, 2, 3, 0, 9, 7, 0, 0, 7, 2, 4, 5, 5, 0, 1, 2, 6, 8, 3, 0, 9, 0, 8, 0, 8};
int NumEn[26] = {0, 1, 2, 3, 0, 1, 2, 0, 0, 2, 2, 4, 5, 5, 0, 1, 2, 6, 2, 3, 0, 1, 0, 2, 0, 2};
#if defined (FR)
    int *ptr = NumFr;
#elif defined (EN)
    int *ptr = NumEn;
#endif

int getNum(int letter) {
    letter = toUpper(letter);
    return ptr[letter - 'A'];
}

int toLower(int c) { return c >= 'A' && c <= 'Z' ? c - 'A' + 'a' : c; }
int toUpper(int c) { return c >= 'a' && c <= 'z' ? c - 'a' + 'A' : c; }

bool isConsonant(int c) {
    if(c <= 'Z' && c > 'A' && c != 'E' && c != 'I' && c != 'O' && c != 'U' && c != 'Y') return true;
    return false;
}

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

List *zSoundex(const char *str) {
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
    return ret;
}

bool isEqualSoundex(const char *str1, const char *str2) {
    List *l1 = zSoundex(str1),
         *l2 = zSoundex(str2);
    bool ret = List_equal(l1, l2);
    List_free(l1);
    List_free(l2);
    return ret;
}

int main(void) {
    return isEqualSoundex("je suis un chat", "Ji souis une catte");
}
