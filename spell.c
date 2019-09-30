#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"



bool check_word(const char* word, hashmap_t hashtable[]){
    if(strlen(word) > LENGTH){return false;}


    //Check if word is number, if so return true
    int num_of_digits = 0;
    for(int i = 0; i < strlen(word); i++){
        if(isdigit(word[i])){num_of_digits++;}
    }
    if(num_of_digits == strlen(word)){
        return true;
    }


    //Set int bucket to the output of hash_function(word).
    int bucket = hash_function(word);
    //Set hashmap_t cursor equal to hashmap[bucket].
    hashmap_t cursor = hashtable[bucket];
    //While cursor is not NULL:
    while (cursor != NULL){
        //If word equals cursor->word: return True.
        if (!strcmp(word, cursor->word)){return true;}
        //Set cursor to cursor->next.
        cursor = cursor->next;
    }
    
    char lowercase_word[LENGTH];
    //Lowercase word
    for(int i = 0; i < strlen(word); i++){
        lowercase_word[i] = tolower(word[i]);
    }
    lowercase_word[strlen(word)] = '\0';
    //Set int bucket to the output of hash_function(word).
    bucket = hash_function(lowercase_word);
    //Set hashmap_t cursor equal to hashtable[bucket].
    cursor = hashtable[bucket];
    //While cursor is  not NULL:
    while (cursor != NULL){
        //If lower_case(word) equals cursor->word: return True.
        if (!strcmp(lowercase_word, cursor->word)){return true;}
        //Set cursor to cursor->next.
        cursor = cursor->next;
    }

    return false;
}



int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[]){
    //Set int num_misspelled to 0.
    int num_misspelled = 0;

    char c;
    char word[LENGTH];
    int index = 0;
    int num_of_non_alpha_char = 0;
    int num_of_digits = 0;
    //While line in fp is not EOF (end of file):
    while((c = fgetc(fp)) != EOF){
        if(strlen(word) == 0){
            if(!isspace(c)){
                word[0] = c;
                index = 1;
                if(!isalpha(c)){num_of_non_alpha_char++;}
                if(isdigit(c)){num_of_digits++;}
            }
        }else{
            if(isspace(c)){
                //if word is not a number
                if(strlen(word) != num_of_digits){
                    if(num_of_non_alpha_char > 0){
                        //Remove non alpha from beginning and end of word.
                        int begin_index = 0;
                        while(!isalpha(word[begin_index])){begin_index++;}
                        int end_index = strlen(word) - 1;
                        while(!isalpha(word[end_index])){end_index--;}
                        char filtered_word[LENGTH];
                        int j = 0;
                        for(int i = begin_index; i <= end_index; i++){
                            filtered_word[j] = word[i];
                            j++;
                        }
                        filtered_word[j] = '\0';
                        strncpy(word, filtered_word, LENGTH);
                        word[j] = '\0';
                    }else{
                        word[index] = '\0';
                    }
                    //If not check_word(word):
                    if(!check_word(word, hashtable)){
                        //Append word to misspelled.
                        misspelled[num_misspelled] = malloc(LENGTH);
                        strncpy(misspelled[num_misspelled], word, LENGTH);
                        //Increment num_misspelled.
                        num_misspelled++;
                    }
                }
                //clear string
                int len_of_word = strlen(word);
                for(int i = 0; i < len_of_word; i++){
                    word[i] = '\0';
                }
                index = 0;
                num_of_non_alpha_char = 0;
                num_of_digits = 0;
            }else{
                word[index] = c;
                index++;
                if(!isalpha(c)){num_of_non_alpha_char++;}
                if(isdigit(c)){num_of_digits++;}
            }  
        }
    }
    
    return num_misspelled;
}



bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]){
    // Initialize all values in hash table to NULL.
    for(int i = 0; i < HASH_SIZE; i++){
        hashtable[i] = NULL;
    }

    //Open dict_file from path stored in dictionary.
    FILE* dict_file = fopen(dictionary_file, "r");

    //If dict_file is NULL: return false.
    if (dict_file == NULL){
        return false;
    }

    char dict_word[LENGTH];
    // While word in dict_file is not EOF (end of file):
    while (fscanf(dict_file, "%45s", dict_word) == 1){
        //eliminate new line character
        //Set hashmap_t new_node to a new node.
        hashmap_t new_node = malloc(sizeof(node));
        //Set new_node->next to NULL.
        new_node->next = NULL;
        //Set new_node->word equal to word.
        strncpy(new_node->word, dict_word, strlen(dict_word));
        new_node->word[strlen(dict_word)] = '\0';
        //Set int bucket to hash_function(word).
        int bucket = hash_function(new_node->word);
        //if hashtable[bucket] is NULL: 
        if(hashtable[bucket] == NULL){
            //Set hashtable[bucket] to new_node.
            hashtable[bucket] = new_node;
        }
        else{
            //Set new_node->next to hashtable[bucket].
            new_node->next = hashtable[bucket];
            //Set hashtable[bucket] to new_node.
            hashtable[bucket] = new_node;
        } 
    }

    //Close dict_file.
    fclose(dict_file);

    return true;
}