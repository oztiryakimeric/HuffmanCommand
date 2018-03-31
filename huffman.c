#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define True 1

typedef struct HuffmanNode {
    int frequency;
    char character;
    HuffmanNode *leftChild;
    HuffmanNode *rightChild;
} HuffmanNode;

typedef struct ListNode {
    HuffmanNode *data;
    ListNode *next;
    ListNode *previous;
} ListNode;

typedef struct LinkedList {
    ListNode *head;
    ListNode *tail;
} LinkedList;

HuffmanNode *newHuffmanNode(char ch, int frequency) {
    HuffmanNode *node = (HuffmanNode*) malloc(sizeof(HuffmanNode));
    node->frequency = frequency;
    node->character = ch;
    node->leftChild = NULL;
    node->rightChild = NULL;
    return node;
}

HuffmanNode *newHuffmanNode(HuffmanNode *first, HuffmanNode *second) {
    HuffmanNode *node = (HuffmanNode*) malloc(sizeof(HuffmanNode));
    node->frequency = first->frequency + second->frequency;
    node->leftChild = first;
    node->rightChild = second;
    return node;
}

void printHuffmanNode(HuffmanNode *node) {
    printf("(HuffmanNode) -> Frequency: %d, Character: %c\n", node->frequency, node->character);
}

LinkedList *newLinkedList() {
    LinkedList *list = (LinkedList*) malloc(sizeof(LinkedList));        // Allocate memory for linked list
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void addItem(LinkedList *list, HuffmanNode *data) {                      
    ListNode *newNode = (ListNode*) malloc(sizeof(ListNode));               // Allocate memory for new linked list node
    newNode->data = data;   
    newNode->next = NULL;
    newNode->previous = NULL;

    if(list->head == NULL && list->tail == NULL) {                          // This means list is empty
        list->head = newNode;                                               // New item should be referenced by head
        list->tail = newNode;                                               // New item should be referenced by head
    }
    else {                                                                  // This means list not empty
        list->tail->next = newNode;                                         // New item should be referenced by last 
        newNode->previous = list->tail;                                           
        list->tail = newNode;                                               // node's next and also tail
    }
}

HuffmanNode *popMin(LinkedList *list) {
    ListNode *min = NULL;
    ListNode *current = list->head;
    
    while(current != NULL) {
        if(min == NULL || current->data->frequency < min->data->frequency) {
            min = current;
        }
        current = current->next;
    }
    //Remove
    min->previous->next = min->next;
    min->next->previous = min->previous;

    return min->data;
}

void printLinkedList(LinkedList *list) {
    ListNode *current = list->head;
    
    printf("Linked list:\n");
    int count = 0;
    while(current != NULL) {
        printf("\t%d) ", count++);
        printHuffmanNode(current->data);
        current = current->next;
    }
}

char *readFile(char fileName[]) {
    char *source = NULL;
    FILE *fp = fopen(fileName, "r");
    if (fp != NULL) {
        if (fseek(fp, 0L, SEEK_END) == 0) {                                 // Go to the end of the file
            long bufferSize = ftell(fp);                                    // Get the buffer size 
            if (bufferSize == -1) { /* Error */ }

            source = (char*) malloc(sizeof(char) * (bufferSize + 1));       // Allocate memory for buffer

            if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */ }               // Go to the start of the file

            size_t newLen = fread(source, sizeof(char), bufferSize, fp);    // Read the whole file to buffer
        }
        fclose(fp);                                                         
    }
    return source;
}

int *generateFrequencyArray(char *file){
    int *frequencyArr = (int*) malloc(sizeof(int) * 127);      // Allocate memory for frequency map
    
    for(int i=0; file[i] != '\0'; i++)                         // Iterate all characters
        frequencyArr[file[i]] += 1;                            // Increase frequency of each character

    return frequencyArr;
}

int main(void) {
    char fileName[] = "test.txt";
    char *file = readFile(fileName);                            
    int *frequencyArray = generateFrequencyArray(file);

    LinkedList *huffmanList = newLinkedList();

    for(int i=0; i<127; i++) {
        if(frequencyArray[i] != 0) {
            HuffmanNode *newNode = newHuffmanNode((char) i, frequencyArray[i]);
            addItem(huffmanList, newNode);
        }
    }
    printLinkedList(huffmanList);

    HuffmanNode *node = popMin(huffmanList);
    printHuffmanNode(node);

    printLinkedList(huffmanList);

    HuffmanNode *node2 = popMin(huffmanList);
    printHuffmanNode(node2);
}