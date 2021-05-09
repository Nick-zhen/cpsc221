/**
 * @file anagram_dict.cpp
 * Implementation of the AnagramDict class.
 *
 * @author Matt Joras
 * @date Winter 2013
 */

#include "anagram_dict.h"
#include <algorithm>
#include <fstream>

using std::string;
using std::vector;
using std::ifstream;

/**
 * Constructs an AnagramDict from a filename with newline-separated
 * words.
 * @param filename The name of the word list file.
 */
AnagramDict::AnagramDict(const string& filename)
{
    ifstream words(filename);
    string word;
    if (words.is_open()) {
        while (getline(words, word)) {
            add_word_to_dict(word);
        }
    }
}

/**
 * Constructs an AnagramDict from a vector of words.
 * @param words The vector of strings to be used as source words.
 */
AnagramDict::AnagramDict(const vector<string>& words)
{
    for (const string& word : words) {
        add_word_to_dict(word);
    }
}

bool areAnagram(string str1, string str2) 
{ 
    // Get lengths of both strings 
    int n1 = str1.length(); 
    int n2 = str2.length(); 
  
    // If length of both strings is not same, then they 
    // cannot be anagram 
    if (n1 != n2) 
        return false; 
  
    // Sort both the strings 
    sort(str1.begin(), str1.end()); 
    sort(str2.begin(), str2.end()); 
  
    // Compare sorted strings 
    for (int i = 0; i < n1; i++) 
        if (str1[i] != str2[i]) 
            return false; 
  
    return true; 
} 

/**
 * @param word The word to add to the dictionary
 * Adds the word as a (key, value) pair to the dictionary.
 * NOTE: word is the value. You will need to decide an appropriate key
 * for the word.
 */
void AnagramDict::add_word_to_dict(const std::string& word)
{
    for (auto & key_val : dict) {
        if (areAnagram(key_val.first, word)) {
            key_val.second.push_back(word);
            return;
        }
    }
    vector<string> vec;
    vec.push_back(word);
    dict[word] = vec;
}

/**
 * @param word The word to find anagrams of.
 * @return A vector of strings of anagrams of the given word. Empty
 * vector returned if no anagrams are found or the word is not in the
 * word list.
 */
vector<string> AnagramDict::get_anagrams(const string& word) const
{
    
    auto lookup = dict.find(word);
    if (lookup != dict.end()) {
        return lookup->second;
    }
    
    return vector<string>();
}

/**
 * @return A vector of vectors of strings. Each inner vector contains
 * the "anagram siblings", i.e. words that are anagrams of one another.
 * NOTE: It is impossible to have one of these vectors have less than
 * two elements, i.e. words with no anagrams are ommitted.
 */
vector<vector<string>> AnagramDict::get_all_anagrams() const
{   
    vector<vector<string>> out;
    for (auto & key_val : dict) {
        if (key_val.second.size() >= 2) {
            out.push_back(key_val.second);
        }
        
    }
    return out;
}
