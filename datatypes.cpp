#include "datatypes.h"

std::vector<std::string>
document_node_to_string(DocumentNodeData& n) {
    std::vector<std::string> r;
    r.push_back(std::to_string(n.x));
    r.push_back(std::to_string(n.y));
    r.push_back(std::string(n.acessibleName));
    return r;
}
