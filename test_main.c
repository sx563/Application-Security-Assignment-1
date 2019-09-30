#include <check.h>
#include "dictionary.h"
#include <stdlib.h>

#define DICTIONARY "wordlist.txt"
#define TESTDICT "test_worlist.txt"

START_TEST(test_dictionary_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(TESTDICT, hashtable);
    // Here we can test if certain words ended up in certain buckets
    // to ensure that our load_dictionary works as intended. I leave
    // this as an exercise.
    const char* word1 = "first";
    int bucket1 = hash_function(word1);
    hashmap_t cursor1 = hashtable[bucket1];
    ck_assert(!strcmp(cursor1->word, word1));
    const char* word2 = "second";
    int bucket2 = hash_function(word2);
    hashmap_t cursor2 = hashtable[bucket2];
    ck_assert(!strcmp(cursor2->word, word2));
    const char* word3 = "third";
    int bucket3 = hash_function(word3);
    hashmap_t cursor3 = hashtable[bucket3];
    ck_assert(!strcmp(cursor3->word, word3));
    const char* word4 = "test";
    int bucket4 = hash_function(word4);
    hashmap_t cursor4 = hashtable[bucket4];
    ck_assert(!strcmp(cursor4->word, word4));
}
END_TEST

START_TEST(test_check_word_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    const char* correct_word = "Justice";
    const char* punctuation_word_2 = "pl.ace";
    ck_assert(check_word(correct_word, hashtable));
    ck_assert(!check_word(punctuation_word_2, hashtable));
    // Test here: What if a word begins and ends with "?
    const char* word_with_question_marks = "?Justice?";
    ck_assert(!check_word(word_with_question_marks, hashtable));
}
END_TEST

START_TEST(test_check_words_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    char* expected[3];
    expected[0] = "sogn";
    expected[1] = "skyn";
    expected[2] = "betta";
    char *misspelled[MAX_MISSPELLED];
    FILE *fp = fopen("test1.txt", "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 3);
    bool test = strlen(misspelled[0]) == strlen(expected[0]);
    int len1 = strlen(misspelled[0]);
    int len2 = strlen(expected[0]);
    ck_assert_msg(test, "%d!=%d", len1, len2);
    ck_assert_msg(strcmp(misspelled[0], expected[0]) == 0);
    ck_assert_msg(strcmp(misspelled[1], expected[1]) == 0);
    ck_assert_msg(strcmp(misspelled[2], expected[2]) == 0);
}
END_TEST

START_TEST(test_check_word_overflow)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    const char* long_word = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    ck_assert(!check_word(long_word, hashtable));
}
END_TEST

START_TEST(test_dictionary_file_does_not_exist)
{
    hashmap_t hashtable[HASH_SIZE];
    ck_assert(!load_dictionary("file_does_not_exit.txt", hashtable)); 
}
END_TEST

START_TEST(test_dictionary_file_does_exist)
{
    hashmap_t hashtable[HASH_SIZE];
    ck_assert(load_dictionary(TESTDICT, hashtable));   
}
END_TEST

START_TEST(test_check_word_number)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    const char* number = "101";
    ck_assert(check_word(number, hashtable));
}
END_TEST

START_TEST(test_check_words_numbers)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    char *misspelled[MAX_MISSPELLED];
    FILE *fp = fopen("numbers.txt","w+");
    for(int i = 0; i < 11; i++){
       fprintf(fp, "%d\n", i);
    }
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 0);
    fclose(fp);
}
END_TEST

START_TEST(test_check_words_mixed)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    char *misspelled[MAX_MISSPELLED];
    FILE *fp = fopen("mixed.txt","w+");
    for(int i = 0; i < 11; i++){
        hashmap_t cursor = hashtable[i];
        int j = i;
        while(cursor->word == NULL){
            j++;
            cursor = hashtable[j];
        }
        fprintf(fp, " %d %s ", i, cursor->word);
    }
    
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 0);
    fclose(fp);
}
END_TEST

Suite *
check_word_suite(void)
{
    Suite * suite;
    TCase * check_word_case;
    suite = suite_create("check_word");
    check_word_case = tcase_create("Core");
    tcase_add_test(check_word_case, test_check_word_normal);
    tcase_add_test(check_word_case, test_check_word_overflow);
    tcase_add_test(check_word_case, test_check_word_number);
    suite_add_tcase(suite, check_word_case);

    return suite;
}

Suite *
check_words_suite(void)
{
    Suite * suite;
    TCase * check_words_case;
    suite = suite_create("check_words");
    check_words_case = tcase_create("Core");
    tcase_add_test(check_words_case, test_check_words_normal);
    tcase_add_test(check_words_case, test_check_words_numbers);
    tcase_add_test(check_words_case, test_check_words_mixed);
    suite_add_tcase(suite, check_words_case);

    return suite;
}

Suite *
load_dictionary_suite(void)
{
    Suite * suite;
    TCase * load_dictionary_case;
    suite = suite_create("load_dictionary");
    load_dictionary_case = tcase_create("Core");
    tcase_add_test(load_dictionary_case, test_dictionary_normal);
    tcase_add_test(load_dictionary_case, test_dictionary_file_does_not_exist);
    tcase_add_test(load_dictionary_case, test_dictionary_file_does_exist);
    suite_add_tcase(suite, load_dictionary_case);

    return suite;
}

int
main(void)
{
    int failed;
    Suite *suite;
    SRunner *runner;
    
    suite = check_word_suite();
    runner = srunner_create(suite);

    suite = check_words_suite();
    srunner_add_suite(runner, suite);

    suite = load_dictionary_suite();
    srunner_add_suite(runner, suite);

    srunner_run_all(runner, CK_NORMAL);
    failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return (failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}

