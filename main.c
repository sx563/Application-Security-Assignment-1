#include "dictionary.h"

int main(int argc, char *argv[]){
    //Load dictionary
    char* dictionary_file = argv[2];
    hashmap_t hashtable[HASH_SIZE];
    printf("Loading dictionary...\n");
    if(!load_dictionary(dictionary_file, hashtable)){
        printf("Error: can't load dictionary\n");
        return -1;
    }
    printf("Dictionary loaded successfully from %s\n", dictionary_file);

    //Check words
    char* test_input_file = argv[1];
    FILE* fp = fopen(test_input_file, "r");
    char* misspelled[MAX_MISSPELLED];
    printf("Checking words for misspellings...\n");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    printf("There are %d misspelled word(s) in %s:\n", num_misspelled, test_input_file);
    for(int i = 0; i < num_misspelled; i++){
        printf("%s\n", misspelled[i]);
    }

    fclose(fp);
    
    return 0;
}