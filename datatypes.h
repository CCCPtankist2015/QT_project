#ifndef DATATYPES_H
#define DATATYPES_H
#include <vector>
#include <string>

struct DocumentNodeData {
    float x;
    float y;
    char* acessibleName;
};

std::vector<std::string>
document_node_to_string(DocumentNodeData&);

#endif // DATATYPES_H
