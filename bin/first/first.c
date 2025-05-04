#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NT 26    // Maximum non-terminals (A-Z)
#define MAX_TERM 128 // ASCII characters
#define MAX_RULES 50 // Maximum production rules

// Simple representation of a grammar rule
typedef struct {
    char lhs;         // Left-hand side (a non-terminal)
    char rhs[20];     // Right-hand side of the production
} Rule;

// Function to check if a character is a non-terminal (A-Z)
bool isNonTerminal(char c) {
    return (c >= 'A' && c <= 'Z');
}

// Simple implementation to find FIRST sets
void findFirst(Rule rules[], int ruleCount, bool first[MAX_NT][MAX_TERM]) {
    bool changed = true;
    
    // Keep iterating until no changes are made
    while (changed) {
        changed = false;
        
        // Process each rule
        for (int i = 0; i < ruleCount; i++) {
            char lhs = rules[i].lhs;
            char *rhs = rules[i].rhs;
            int lhsIndex = lhs - 'A';
            
            // Case 1: X -> ε (epsilon)
            if (strcmp(rhs, "e") == 0) {
                if (!first[lhsIndex]['e']) {
                    first[lhsIndex]['e'] = true;
                    changed = true;
                }
                continue;
            }
            
            // Case 2: X -> a... (terminal)
            if (!isNonTerminal(rhs[0])) {
                if (!first[lhsIndex][rhs[0]]) {
                    first[lhsIndex][rhs[0]] = true;
                    changed = true;
                }
                continue;
            }
            
            // Case 3: X -> Y... (non-terminal)
            int k = 0;
            bool canBeEmpty = true;
            
            while (canBeEmpty && rhs[k] != '\0') {
                canBeEmpty = false;
                
                if (isNonTerminal(rhs[k])) {
                    int ntIndex = rhs[k] - 'A';
                    
                    // Add all terminals from FIRST(Y) to FIRST(X)
                    for (int t = 0; t < MAX_TERM; t++) {
                        if (t != 'e' && first[ntIndex][t] && !first[lhsIndex][t]) {
                            first[lhsIndex][t] = true;
                            changed = true;
                        }
                    }
                    
                    // Check if Y can derive epsilon
                    if (first[ntIndex]['e']) {
                        canBeEmpty = true;
                    }
                } else {
                    // Terminal symbol - add to FIRST set
                    if (!first[lhsIndex][rhs[k]]) {
                        first[lhsIndex][rhs[k]] = true;
                        changed = true;
                    }
                    break;
                }
                
                k++;
            }
            
            // If the entire right side can be empty, add epsilon to FIRST(X)
            if (canBeEmpty && rhs[0] != '\0') {
                if (!first[lhsIndex]['e']) {
                    first[lhsIndex]['e'] = true;
                    changed = true;
                }
            }
        }
    }
}

int main() {
    Rule rules[MAX_RULES];
    int ruleCount = 0;
    bool first[MAX_NT][MAX_TERM] = {false};
    
    printf("Simple FIRST Set Calculator\n");
    printf("---------------------------\n");
    printf("Enter grammar rules (one per line), use 'e' for epsilon\n");
    printf("Format: A->BC or A->a or A->e\n");
    printf("Enter 'done' when finished.\n\n");
    
    // Input grammar rules
    char input[50];
    while (1) {
        printf("Rule: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';  // Remove newline
        
        if (strcmp(input, "done") == 0) {
            break;
        }
        
        // Simple parsing
        if (strlen(input) >= 4 && isNonTerminal(input[0]) && 
            input[1] == '-' && input[2] == '>') {
            
            rules[ruleCount].lhs = input[0];
            strcpy(rules[ruleCount].rhs, &input[3]);
            ruleCount++;
            
            printf("Added: %c -> %s\n", input[0], &input[3]);
        } else {
            printf("Invalid format. Use A->BC or A->a\n");
        }
    }
    
    // Calculate FIRST sets
    findFirst(rules, ruleCount, first);
    
    // Print FIRST sets
    printf("\nFIRST Sets:\n");
    for (int i = 0; i < MAX_NT; i++) {
        bool hasElements = false;
        
        // Check if this non-terminal has any entries
        for (int j = 0; j < MAX_TERM; j++) {
            if (first[i][j]) {
                hasElements = true;
                break;
            }
        }
        
        if (hasElements) {
            printf("FIRST(%c) = { ", 'A' + i);
            bool firstElement = true;
            
            // Print terminals
            for (int j = 0; j < MAX_TERM; j++) {
                if (first[i][j]) {
                    if (!firstElement) {
                        printf(", ");
                    }
                    
                    if (j == 'e') {
                        printf("epsilon");  // Print epsilon as ε
                    } else {
                        printf("%c", j);
                    }
                    
                    firstElement = false;
                }
            }
            
            printf(" }\n");
        }
    }
    
    return 0;
}