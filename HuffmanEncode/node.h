#ifndef NODE_H
#define NODE_H
#include <iostream>

using namespace std;

class node
{
private:
    node *right;
    node *left;
    string value;

public:
    node();
};

#endif // NODE_H
