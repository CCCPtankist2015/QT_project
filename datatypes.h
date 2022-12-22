#ifndef DATATYPES_H
#define DATATYPES_H
#include <vector>
#include <string>

struct DocumentNodeData {
    float x;
    float y;
    char* acessibleName;
};

struct DocumentTextData {
    float x;
    float y;
    char* text;
};

std::vector<std::string>
document_node_to_string(DocumentNodeData&);

#endif // DATATYPES_H
