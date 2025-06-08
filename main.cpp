#include <stdlib.h>
#include <assert.h>

#include "lib/lib.hpp"
#include "fileread/fileread.hpp"

#include <locale.h>

#ifdef _DEBUG
#include "logger/log.hpp"
#endif // _DEBUG


static void test_run(const char* test_file, size_t test_num, bool check_double);
static void after_test();

int main()
{

    if (!setlocale(LC_ALL, "C.UTF-8"))
        EXIT(EXIT_FAILURE, "failed setlocale.\n");

    COLOR_PRINT(GREEN, "\nBEGIN\n\n");
    ON_DEBUG(
        LOG_OPEN();
    )
    
    const char* file1 = "tests/test1.txt";
    const char* file2 = "tests/test2.txt";
    const char* file3 = "tests/test3.txt";
    const char* file4 = "tests/test4.jpg";

    // test_run(file1, 1, true);
    // test_run(file2, 2, false);
    test_run(file3, 3, false);
    // test_run(file4, 4, false);

    ON_DEBUG(
    LOG_CLOSE();
    )
    COLOR_PRINT(GREEN, "\nEND\n\n");

    return EXIT_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void test_run(const char* test_file, size_t test_num, bool check_double)
{
    WordArray words = ReadBufferFromFile(test_file);

    COLOR_PRINT(GREEN, "running test %lu\n", test_num);

    ON_DEBUG(
    LOG_TITLE(Red, "Test Result\n\n");
    LOG_PRINT(Blue, "size = '%lu'\n\n", words.size);
    )

    OFF_DEBUG(

    if (system("mkdir -p tests_result/") != 0)
        EXIT(EXIT_FAILURE, "failed make dir for tests.\n");

    const static size_t name_len = 256;
    char test_result_file[name_len] = {};
    snprintf(test_result_file, name_len, "tests_result/test_%lu.txt", test_num);

    FILE* test_result = fopen(test_result_file, "w");

    if (!test_result)
        EXIT(EXIT_FAILURE, "failed open '%s'\n", test_result_file);

    fprintf(test_result, "%lu Test Result\n\n", test_num);
    fprintf(test_result, "size = '%lu'\n\n", words.size);
    )

    for (size_t i = 0; i < words.size; i++)
    {
        Word word = words.words[i];
        ON_DEBUG
        (
        LOG_PRINT(Yellow, "word[%1lu] = \n{\n", i);
    
        if (check_double)
            LOG_PRINT(Green, "\tdouble word = '%f'\n\tlen = %lu\n\t%s:%lu:%lu\n", WordToDouble(&word), word.len, test_file, word.line, word.inLine);
        else
            LOG_PRINT(Green, "\tword = '%s'\n\tlen = %lu\n\t%s:%lu:%lu\n", word.word, word.len, test_file, word.line, word.inLine);
    
        LOG_PRINT(Yellow, "}\n\n");
        )

        OFF_DEBUG
        (
        fprintf(test_result, "word[%1lu] = \n{\n", i);
        
        if (check_double)
            fprintf(test_result, "\tdouble word = '%f'\n\tlen = %lu\n\t%s:%lu:%lu\n", WordToDouble(&word), word.len, test_file, word.line, word.inLine);
        else
            fprintf(test_result, "\tword = '%ls'\n\tlen = %lu\n\t%s:%lu:%lu\n", word.word, word.len, test_file, word.line, word.inLine);
            
        fprintf(test_result, "}\n\n");
        )
    }

    BufferDtor(&words);

    after_test();

    return;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void after_test()
{
    ON_DEBUG(
    LOG_NS(); LOG_NS(); LOG_NS();
    )
    OFF_DEBUG(
    // printf("\n\n\n");
    )
    
    return;
}