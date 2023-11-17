#ifndef _FIBONACCI_HEAP_H_
#define _FIBONACCI_HEAP_H_

#include <stdio.h>
#include <vector> 
#include <string>
#include <utility>

enum State
{
    LABELED, UNLABELED, SCANNED
};

class Node;

class Edge
{
private:
public:
    Node* tail;
    Node* head;
    double length;

    Edge(Node* tail, Node* head, double length);
};

class Node
{
private:
public:

    
    Node* parent;
    Node* leftSibling, * rightSibling;
    Node* children;
    Node* next;

    std::pair<int, std::string> data;
    double key;
    int rank;

    std::vector<Edge*> incomingEdges;
    std::vector<Edge*> outgoingEdges;

    State state;

    Node(int number, const std::string& str, double key);
    Node();

    bool addChild(Node* node);
    bool addSibling(Node* node);
    bool remove();

    Node* leftMostSibling();
    Node* rightMostSibling();

    void addIncomingEdge(Edge* edge);
    void addOutgoingEdge(Edge* edge);
};

class FibonacciHeap
{
private:
    Node* rootListByRank[100];

    bool link(Node* root);
    Node* minRoot;

public:
    FibonacciHeap();
    FibonacciHeap(Node* root);
    ~FibonacciHeap();

    bool isEmpty();
    bool insertVertex(Node* node);
    void decreaseKey(double delta, Node* vertex);
    Node* findMin();
    Node* deleteMin();
};

#endif
