#include <string.h>
#include <stdlib.h>

typedef struct _node node_t;
struct _node {
    char* data;
    node_t* next;
};

int main() {
    node_t* head = NULL;
    add_to_front(&head, "abc");  
}

void add_to_front(node_t** head_ptr, char* data) {
    // 创建一个 node，位于原始 head node 的前方
    node_t* node = (node_t*) malloc(sizeof(node_t));
    node->data = (char*) malloc(strlen(data) + 1);
    strcpy(node->data, data);
    node->next = *head_ptr; // 将新建的 head node 指向原来的 head node
    *head_ptr = node; // 修改头指针指向
}