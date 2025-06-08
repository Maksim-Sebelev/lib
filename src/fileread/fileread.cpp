#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <assert.h>
#include "fileread/fileread.hpp"
#include "lib/lib.hpp"

#ifdef _DEBUG
#include "logger/log.hpp"
#endif // _DEBUG

#ifdef _WCHAR_T

    #include <wchar.h>
    #include <locale.h>
    #include <stdio.h>

    #define ON_WCHAR(...) __VA_ARGS__

    #define STR_TO_LONG(  buf, buf_end, base) wcstol(buf, buf_end, base)
    #define STR_TO_DOUBLE(buf, buf_end      ) wcstod(buf, buf_end      )

    #define SPECIFIER "%ls"
    

#else

    #define ON_WCHAR(...)


    #define STR_TO_LONG(  buf, buf_end, base) strtol(buf, buf_end, base)
    #define STR_TO_DOUBLE(buf, buf_end      ) strtod(buf, buf_end      )

    #define sp "%s"
#endif

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct Pointer
{
    size_t wp; // word pointer
    size_t lp; // line pointer
    size_t sp; // str pointer (char pos in line)
    size_t bp; // buffer pointer
    size_t wbp; // previous word begin pointer
};

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static size_t  GetBufferLenAndBufferAndWordsArray(const char* file, char_t** buffer, Word** words_array);
static size_t  CalcFileLen                       (const char* const fileName);
static size_t  GetPreviousWordLen                (const Pointer* pointer);

static Pointer PointersCtor                      ();

static void    ReadBufRealloc                    (WordArray* wordArray);
        
static void    SetNullWord                       (Word** split_buffer, const char_t* buffer);
static void    SetPreviousWordLen                (Word* split_buffer, const Pointer* pointer, size_t wordLen);
static void    SetWordAndFilePosition            (Word* split_buffer, const char_t* word, Pointer* pointer);
        
        
static bool    IsPassSymbolAndChangePointer      (const char_t c, Pointer* pointer);
static bool    IsPassSymbol                      (const char_t c);
static bool    IsSpace                           (const char_t c);
static bool    IsSlashN                          (const char_t c);
static bool    IsSlashR                          (const char_t c);
static bool    IsSlashT                          (const char_t c);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int WordToInt(const Word* word)
{
    assert(word);
    
    const char_t* str     = word->word;
    char_t*       str_end = nullptr;
    size_t        len     = word->len;
    
    long res = STR_TO_LONG(str, &str_end, 10);

    if ((str_end - str) != (int) len)
    {
        EXIT(EXIT_FAILURE, 
            "Trying to convert not 'int' str to 'int'\n"
            "str (word) = '"SPECIFIER"'\n"
            "file: %lu:%lu\n",
            str, word->line, word->inLine
        );
    }

    return (int) res;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

double WordToDouble(const Word* word)
{
    assert(word);

    const char_t* str     = word->word;
    char_t*       str_end = nullptr;
    size_t        len     = word->len;
    
    double res = STR_TO_DOUBLE(str, &str_end);

    if ((str_end - str) != (int) len)
    {
        EXIT(EXIT_FAILURE,
            "Trying to convert not 'double' str to 'double'\n"
            "str (word) = '"SPECIFIER"'\n"
            "file: %lu:%lu\n",
            str, word->line, word->inLine
        );
    }

    return res;
}

//============================ Read File ==============================================================================================================

WordArray ReadBufferFromFile(const char* file)
{
    assert(file);

    char_t* buffer = {};
    Word*   words  = {};
    
    size_t buffer_len = GetBufferLenAndBufferAndWordsArray(file, &buffer, &words);

    SetNullWord(&words, buffer);

    Pointer pointer = PointersCtor();

    if (!IsPassSymbol(buffer[0]))
        SetWordAndFilePosition(words, &buffer[0], &pointer);

    for (; pointer.bp < buffer_len; pointer.bp++)
    {
        if (!IsPassSymbolAndChangePointer(buffer[pointer.bp], &pointer))
        {
            pointer.sp++;
            continue;
        }

        size_t previousWordLen = GetPreviousWordLen(&pointer);

        do
        {
            buffer[pointer.bp] = '\0';  
            pointer.bp++;
        } while (IsPassSymbolAndChangePointer(buffer[pointer.bp], &pointer));
        
        SetPreviousWordLen(words, &pointer, previousWordLen);
        SetWordAndFilePosition(words, &buffer[pointer.bp], &pointer);

        pointer.sp++;
        pointer.wbp = pointer.bp;
    }

    size_t wordsQuant = pointer.wp - 1;

    WordArray wordArray = {words, wordsQuant};

    ReadBufRealloc(&wordArray);

    return wordArray;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void BufferDtor(WordArray* wordArray)
{
    assert(wordArray);
    assert(wordArray->words);

    wordArray->size = 0;

    wordArray->words--;
    assert(wordArray->words);

    FREE(wordArray->words[0].word);
    FREE(wordArray->words);

    return;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static size_t GetBufferLenAndBufferAndWordsArray(const char* file, char_t** buffer, Word** words_array)
{
    assert(file);
    assert(buffer);
    assert(words_array);

    FILE* file_ptr = fopen(file, "rb");

    if (!file_ptr)
        EXIT(EXIT_FAILURE, "failed open '%s'\n", file);

    size_t buffer_len      = CalcFileLen(file);
    size_t real_buffer_len = buffer_len + 2;

    *buffer       = (char_t*) calloc (real_buffer_len, sizeof(*buffer     ));
    *words_array  = (Word*)   calloc (real_buffer_len, sizeof(*words_array));

    if (!(*buffer))
        EXIT(EXIT_FAILURE, "failed allocate memory for reading '%s'\n", file);

    if(!(*words_array))
        EXIT(EXIT_FAILURE, "failed allocate memory, to parse '%s'\n", file);

    // if (buffer_len != fread(buffer, sizeof(char_t), buffer_len, file_ptr))
        // EXIT(EXIT_FAILURE, "failed read '%s'\n", file);

    if (fclose(file_ptr) != 0)
        EXIT(EXIT_FAILURE, "failed close '%s'\n", file);

    (*buffer)[buffer_len    ] = ' ';
    (*buffer)[buffer_len + 1] = '\0';

    return real_buffer_len;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static Pointer PointersCtor()
{
    Pointer pointer = 
    {
        .wp  = 0,
        .lp  = 1,
        .sp  = 1,
        .bp  = 0,
        .wbp = 0,
    };

    return pointer;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static size_t GetPreviousWordLen(const Pointer* pointer)
{
    assert(pointer);

    return pointer->bp - pointer->wbp;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void ReadBufRealloc(WordArray* wordArray)
{
    assert(wordArray);
    assert(wordArray->words);

    wordArray->words--;
    assert(wordArray->words);

    Word*  words = wordArray->words;
    size_t size  = wordArray->size + 1;

    wordArray->words = (Word*) realloc(words, size * sizeof(*words));
    assert(wordArray->words);

    wordArray->words++;

    assert(wordArray->words);
    return;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static size_t CalcFileLen(const char* const fileName)
{
    assert(fileName);
    struct stat buf = {};
    stat(fileName, &buf);
    return (size_t) buf.st_size;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void SetWordAndFilePosition(Word* split_buffer, const char_t* word, Pointer* pointer)
{
    assert(split_buffer);
    assert(pointer);

    size_t wordPointer = pointer->wp;

    split_buffer[wordPointer].word   = word;
    split_buffer[wordPointer].line   = pointer->lp;
    split_buffer[wordPointer].inLine = pointer->sp;

    pointer->wp++;

    return;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void SetPreviousWordLen(Word* split_buffer, const Pointer* pointer, size_t wordLen)
{
    assert(split_buffer);
    assert(pointer);

    size_t wordPointer = pointer->wp;

    wordPointer = (wordPointer == 0) ? 0 : wordPointer - 1;

    split_buffer[wordPointer].len = wordLen;

    return;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void SetNullWord(Word** split_buffer, const char_t* buffer)
{
    assert(split_buffer);
    assert(buffer);

    split_buffer[0]->word = buffer;
    (*split_buffer)++;

    return;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static bool IsPassSymbol(const char_t c)
{
    return (IsSpace (c) ||
            IsSlashN(c) ||
            IsSlashR(c) ||
            IsSlashT(c)
           );
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static bool IsPassSymbolAndChangePointer(const char_t c, Pointer* pointer)
{
    assert(pointer);

    if (IsSpace (c))
    {
        pointer->sp++;
        return true;
    }

    if (IsSlashN(c)) 
    {
        pointer->lp++;
        pointer->sp = 1;
        return true;
    }

    if (IsSlashT(c))
    {
        pointer->sp += 4;
        return true;
    }

    if (IsSlashR(c))
    {
        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static bool IsSpace(const char_t c)
{
    return (c == ' ');
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static bool IsSlashN(const char_t c)
{
    return (c == '\n');
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static bool IsSlashR(const char_t c)
{
    return (c == '\r');
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static bool IsSlashT(const char_t c)
{
    return (c == '\t');
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
