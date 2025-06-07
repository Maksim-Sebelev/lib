#include "lib/lib.hpp"
#include "fileread/fileread.hpp"

#ifdef _DEBUG
#include "logger/log.hpp"
#endif // _DEBUG

int main()
{
    ON_DEBUG(
    COLOR_PRINT(GREEN, "begin\n");
    LOG_OPEN();
    )


    const char* file1 = "tests/test1.txt";
    
    WordArray words = ReadBufferFromFile(file1);

    ON_DEBUG(
    LOG_PRINT(Red, "First Test Result\n\n");
    LOG_PRINT(Blue, "size = '%lu'\n", words.size);
    )

    OFF_DEBUG(
    COLOR_PRINT(RED, "First Test Result\n\n");
    COLOR_PRINT(BLUE, "size = '%lu'\n", words.size);
    )

    for (size_t i = 0; i < words.size; i++)
    {
        Word word = words.words[i];
        ON_DEBUG(
        LOG_PRINT(Yellow, "word[%1lu] = \n{", i);
        LOG_PRINT(Green, "\tint word = '%f'\n\tlen = %lu\n\t%s:%lu:%lu\n", WordToDouble(&word), word.len, file1, word.line, word.inLine);
        LOG_PRINT(Yellow, "}\n\n");
        )
        OFF_DEBUG(
        COLOR_PRINT(YELLOW, "word[%1lu] = \n{", i);
        COLOR_PRINT(GREEN, "\tint word = '%f'\n\tlen = %lu\n\t%s:%lu:%lu\n", WordToDouble(&word), word.len, file1, word.line, word.inLine);
        COLOR_PRINT(YELLOW, "}\n\n");
        )
    }

    BufferDtor(&words);
        
    //-----------------------------------------------------
    ON_DEBUG(
    LOG_NS(); LOG_NS(); LOG_NS();
    )
    OFF_DEBUG(
    printf("\n\n\n");
    )
    //-----------------------------------------------------

    const char* file2 = "tests/test2.txt";

    words = ReadBufferFromFile(file2);
    
    ON_DEBUG(
    LOG_PRINT(Red, "Second Test Result\n\n");
    LOG_PRINT(Blue, "size = '%lu'\n", words.size);
    )

    OFF_DEBUG(
    COLOR_PRINT(RED, "Second Test Result\n\n");
    COLOR_PRINT(BLUE, "size = '%lu'\n", words.size);
    )

    for (size_t i = 0; i < words.size; i++)
    {
        Word word = words.words[i];
        ON_DEBUG(
        LOG_PRINT(Yellow, "word[%1lu] = \n{", i);
        LOG_PRINT(Green, "\tword = '%s'\n\tlen = %lu\n\t%s:%lu:%lu\n", word.word, word.len, file1, word.line, word.inLine);
        LOG_PRINT(Yellow, "}\n\n");
        )
        OFF_DEBUG(
        COLOR_PRINT(YELLOW, "word[%1lu] = \n{", i);
        COLOR_PRINT(GREEN, "\tword = '%s'\n\tlen = %lu\n\t%s:%lu:%lu\n", word.word, word.len, file1, word.line, word.inLine);
        COLOR_PRINT(YELLOW, "}\n\n");
        )
    }

    BufferDtor(&words);

    ON_DEBUG(
    LOG_CLOSE();
    COLOR_PRINT(GREEN, "\nend\n");
    )

    return 0;
}
