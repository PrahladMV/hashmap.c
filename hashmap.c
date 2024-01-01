/*This is a C program that I made that uses a hash map to locate a single house in a city.
In this example, the houses are represented by their street addresses,
and the hash map is used to store the mapping between addresses and the corresponding houses. */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a structure to represent a house
typedef struct {
    char address[100];
    int houseNumber;
} House;

// Define a structure for the hash map node
typedef struct HashNode {
    char key[100];
    House value;
    struct HashNode* next;
} HashNode;

// Define a structure for the hash map
typedef struct {
    HashNode** buckets;
    int size;
    int count; // Number of elements in the hash map
} HashMap;

// Hash function to convert a string key to an index
int hashFunction(const char* key, int size) {
    int hash = 0;
    while (*key) {
        hash = (hash * 31) + (*key++);
    }
    return hash % size;
}

// Initialize a new hash map
HashMap* initializeHashMap(int size) {
    HashMap* map = (HashMap*)malloc(sizeof(HashMap));
    map->size = size;
    map->count = 0;
    map->buckets = (HashNode**)calloc(size, sizeof(HashNode*));
    return map;
}

// Resize the hash map to double its current size
void resizeHashMap(HashMap* map) {
    int newSize = map->size * 2;

    // Create a new hash map with the doubled size
    HashMap* newMap = initializeHashMap(newSize);

    // Rehash and insert all existing elements into the new map
    for (int i = 0; i < map->size; i++) {
        HashNode* current = map->buckets[i];
        while (current != NULL) {
            insertHouse(newMap, current->key, current->value.houseNumber);
            current = current->next;
        }
    }

    // Update the current map with the new map's data
    free(map->buckets);
    map->size = newMap->size;
    map->count = newMap->count;
    map->buckets = newMap->buckets;

    // Free the memory allocated for the new map
    free(newMap);
}

// Insert a house into the hash map
void insertHouse(HashMap* map, const char* address, int houseNumber) {
    // Check if resizing is needed
    if (map->count >= map->size) {
        resizeHashMap(map);
    }

    int index = hashFunction(address, map->size);

    // Create a new house node
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    strcpy(newNode->key, address);
    newNode->value.houseNumber = houseNumber;
    strcpy(newNode->value.address, address);
    newNode->next = NULL;

    // Insert the node into the hash map
    if (map->buckets[index] == NULL) {
        map->buckets[index] = newNode;
    } else {
        newNode->next = map->buckets[index];
        map->buckets[index] = newNode;
    }

    map->count++;
}

// Search for a house in the hash map
House* findHouse(HashMap* map, const char* address) {
    int index = hashFunction(address, map->size);

    // Traverse the linked list at the given index
    HashNode* current = map->buckets[index];
    while (current != NULL) {
        if (strcmp(current->key, address) == 0) {
            return &current->value;
        }
        current = current->next;
    }

    // House not found
    return NULL;
}

// Main function
int main() {
    // Initialize the hash map with a size of 10
    HashMap* cityMap = initializeHashMap(10);

    // Insert some houses into the hash map
    insertHouse(cityMap, "123 Main St", 1);
    insertHouse(cityMap, "456 Oak Ave", 2);
    insertHouse(cityMap, "789 Pine Rd", 3);

    // Search for a house
    const char* searchAddress = "Prahlad's Avenue";
    House* foundHouse = findHouse(cityMap, searchAddress);

    // Display the result
    if (foundHouse != NULL) {
        printf("House found at %s, House Number: %d\n", foundHouse->address, foundHouse->houseNumber);
    } else {
        printf("House not found at %s\n", searchAddress);
    }

    // Free the memory allocated for the hash map
    for (int i = 0; i < cityMap->size; i++) {
        HashNode* current = cityMap->buckets[i];
        while (current != NULL) {
            HashNode* next = current->next;
            free(current);
            current = next;
        }
    }
    free(cityMap->buckets);
    free(cityMap);

    return 0;
}
