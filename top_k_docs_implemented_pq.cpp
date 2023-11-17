/*
g++ top_k_docs_implemented_pq.cpp FibonacciHeap.cpp -o top_k
*/
#include "FibonacciHeap.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <cmath>
#include <dirent.h>
#include <cctype>
#include <chrono>
#include <bits/stdc++.h>

using namespace std;
using namespace std::chrono;
#define DT_REG _A_NORMAL

struct Document
{
    string name;
    double score;

    Document(string n, double s) : name(n), score(s) {}

    bool operator<(const Document &other) const
    {
        return score < other.score;
    }
};

map<string, int> documentFrequencies; // Stores DF (Document Frequency) for terms
int totalDocuments = 0;               // Total number of documents

// Function to tokenize a string and perform basic text preprocessing
vector<string> tokenizeAndPreprocess(const string &input)
{
    vector<string> tokens;
    istringstream iss(input);
    string word;

    while (iss >> word)
    {
        // Normalize and preprocess the word (convert to lowercase)
        string processedWord;
        for (char c : word)
        {
            processedWord += tolower(c); // Convert to lowercase
        }

        // Add all words to the tokens
        tokens.push_back(processedWord);
    }

    return tokens;
}

void processDirectory(const string &dirPath)
{
    DIR *directory;
    struct dirent *entry;

    if ((directory = opendir(dirPath.c_str())) != NULL)
    {
        while ((entry = readdir(directory)) != NULL)
        {
            if (entry->d_type == DT_REG)
            {
                string filePath = dirPath + "/" + entry->d_name;
                ifstream file(filePath);
                string documentText((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

                // Tokenize and preprocess the document text
                vector<string> terms = tokenizeAndPreprocess(documentText);

                set<string> uniqueTerms(terms.begin(), terms.end());

                totalDocuments++;

                // Update document frequencies for terms
                for (const string &term : uniqueTerms)
                {
                    documentFrequencies[term]++;
                }
            }
        }
        closedir(directory);
    }
}

double computeTFIDF(const string &term, int termFrequency)
{
    // Convert the term to lowercase for case-insensitive comparison
    string lowercaseTerm = term;
    transform(lowercaseTerm.begin(), lowercaseTerm.end(), lowercaseTerm.begin(), ::tolower);
    if (documentFrequencies.find(lowercaseTerm) != documentFrequencies.end())
    {

        int df = documentFrequencies[lowercaseTerm];
        double idf = log(static_cast<double>(totalDocuments) / (df + totalDocuments)); // Add 1 to avoid division by zero
        return termFrequency * idf;
    }
    return 0.0;
}

int main()
{
    auto start = high_resolution_clock::now();
    const string directoryPath = "docs/1000";
    const int K = 5; // Number of top documents to retrieve
    const string query = "for the music";
    // Process the documents in the directory
    processDirectory(directoryPath);

    // FibonacciHeap *heap = new FibonacciHeap();

    // Process the query
    istringstream iss(query);
    string word;
    map<string, int> queryTermFrequency;

    while (iss >> word)
    {
        // Tokenize and preprocess the query term
        vector<string> terms = tokenizeAndPreprocess(word);
        // Update queryTermFrequency as needed
        for (const string &term : terms)
        {
            queryTermFrequency[term]++;
        }
    }
    // Calculate scores for each document
    // vector<Document> allDocuments; // Store all documents with scores
    FibonacciHeap heap;

    // Iterate through the documents in the directory again and compute the score
    DIR *directory;
    struct dirent *entry;

    if ((directory = opendir(directoryPath.c_str())) != NULL)
    {
        while ((entry = readdir(directory)) != NULL)
        {
            if (entry->d_type == DT_REG)
            {
                string filePath = directoryPath + "/" + entry->d_name;
                ifstream file(filePath);
                string documentText((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

                // Tokenize and preprocess the document text
                vector<string> terms = tokenizeAndPreprocess(documentText);

                // Compute TF-IDF score for the terms in the document
                double documentScore = 0.0;
                for (const string &term : terms)
                {
                    int termFrequency = 0;
                    for (const auto &queryTerm : queryTermFrequency)
                    {
                        if (term == queryTerm.first)
                        {
                            termFrequency += queryTerm.second; // Add the term frequency from the map
                        }
                    }

                    double termScore = computeTFIDF(term, termFrequency);
                    documentScore += termScore;
                }

                // Store the document with its score
                // allDocuments.push_back(Document(entry->d_name, documentScore));
                Node *documentNode = new Node(0, entry->d_name, documentScore);
                heap.insertVertex(documentNode);

                // string s1 = entry->d_name;
                // int sc= documentScore;
                // pair<int,string>temp = {sc,s1};
                // heap->insertVertex(temp);
            }
        }
        closedir(directory);
    }
    int cnt = 0;
    while (cnt < K)
    {
        Node *docNode = heap.deleteMin();
        cout << docNode->data.second << " (Score: " << docNode->key << ")" << endl;
        cnt++;
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "\n\nTime taken by function: " << duration.count() << " microseconds" << endl;
    return 0;
}
