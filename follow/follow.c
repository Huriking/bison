#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PRODUCTIONS 100
#define MAX_SYMBOLS 100
#define MAX_RHS_LENGTH 100
#define MAX_SYMBOL_LENGTH 10

// Structure to represent a grammar production
typedef struct {
    char lhs[MAX_SYMBOL_LENGTH];
    char rhs[MAX_RHS_LENGTH];
} Production;

// Structure to store sets of symbols
typedef struct {
    char symbols[MAX_SYMBOLS][MAX_SYMBOL_LENGTH];
    int count;
} SymbolSet;

// Global variables
Production productions[MAX_PRODUCTIONS];
int num_productions = 0;
SymbolSet first_sets[MAX_SYMBOLS];
SymbolSet follow_sets[MAX_SYMBOLS];
char terminals[MAX_SYMBOLS][MAX_SYMBOL_LENGTH];
char non_terminals[MAX_SYMBOLS][MAX_SYMBOL_LENGTH];
int num_terminals = 0;
int num_non_terminals = 0;
char start_symbol[MAX_SYMBOL_LENGTH];

// Function prototypes
void read_grammar();
void compute_first_sets();
void compute_follow_sets();
void add_to_set(SymbolSet *set, char *symbol);
void print_sets(SymbolSet *sets, char names[][MAX_SYMBOL_LENGTH], int count, const char *set_name);
int is_non_terminal(char *symbol);
int get_non_terminal_index(char *symbol);
int is_in_set(SymbolSet *set, char *symbol);
void find_first_of_string(char *string, SymbolSet *result);
int get_production_index(char *non_terminal);

int main() {
    printf("Enter the grammar productions (end with an empty line):\n");
    printf("Format: A -> B C | D\n");
    read_grammar();
    
    compute_first_sets();
    compute_follow_sets();
    
    // Print the FOLLOW sets
    print_sets(follow_sets, non_terminals, num_non_terminals, "FOLLOW");
    
    return 0;
}

// Read grammar from user input
void read_grammar() {
    char line[MAX_RHS_LENGTH + MAX_SYMBOL_LENGTH + 10];
    char *lhs, *rhs, *token;
    
    // First production is considered the start symbol
    int is_first = 1;
    
    while (fgets(line, sizeof(line), stdin)) {
        // Remove newline character
        line[strcspn(line, "\n")] = 0;
        
        // If empty line, stop reading
        if (strlen(line) == 0) {
            break;
        }
        
        // Split the line into LHS and RHS
        lhs = strtok(line, "->");
        if (lhs == NULL) {
            printf("Invalid input format. Please use 'A -> B C | D'\n");
            continue;
        }
        
        // Trim whitespace from LHS
        while (isspace((unsigned char)*lhs)) lhs++;
        char *end = lhs + strlen(lhs) - 1;
        while (end > lhs && isspace((unsigned char)*end)) end--;
        *(end + 1) = 0;
        
        if (is_first) {
            strcpy(start_symbol, lhs);
            is_first = 0;
        }
        
        // Check if the non-terminal is already in the list
        int found = 0;
        for (int i = 0; i < num_non_terminals; i++) {
            if (strcmp(non_terminals[i], lhs) == 0) {
                found = 1;
                break;
            }
        }
        
        if (!found) {
            strcpy(non_terminals[num_non_terminals], lhs);
            num_non_terminals++;
        }
        
        // Get the RHS
        rhs = strtok(NULL, "\n");
        if (rhs == NULL) {
            printf("Invalid input format. Please use 'A -> B C | D'\n");
            continue;
        }
        
        // Split the RHS by "|" to get multiple productions
        token = strtok(rhs, "|");
        while (token != NULL) {
            // Trim whitespace
            while (isspace((unsigned char)*token)) token++;
            end = token + strlen(token) - 1;
            while (end > token && isspace((unsigned char)*end)) end--;
            *(end + 1) = 0;
            
            strcpy(productions[num_productions].lhs, lhs);
            strcpy(productions[num_productions].rhs, token);
            num_productions++;
            
            // Identify terminals and non-terminals in RHS
            char *symbol = strtok(strdup(token), " ");
            while (symbol != NULL) {
                if (isupper((unsigned char)symbol[0])) {
                    // Non-terminal
                    int found = 0;
                    for (int i = 0; i < num_non_terminals; i++) {
                        if (strcmp(non_terminals[i], symbol) == 0) {
                            found = 1;
                            break;
                        }
                    }
                    
                    if (!found) {
                        strcpy(non_terminals[num_non_terminals], symbol);
                        num_non_terminals++;
                    }
                } else if (strcmp(symbol, "e") != 0) {  // Skip epsilon
                    // Terminal
                    int found = 0;
                    for (int i = 0; i < num_terminals; i++) {
                        if (strcmp(terminals[i], symbol) == 0) {
                            found = 1;
                            break;
                        }
                    }
                    
                    if (!found) {
                        strcpy(terminals[num_terminals], symbol);
                        num_terminals++;
                    }
                }
                
                symbol = strtok(NULL, " ");
            }
            
            token = strtok(NULL, "|");
        }
    }
    
    printf("Grammar read successfully.\n");
}

// Check if a symbol is a non-terminal
int is_non_terminal(char *symbol) {
    for (int i = 0; i < num_non_terminals; i++) {
        if (strcmp(non_terminals[i], symbol) == 0) {
            return 1;
        }
    }
    return 0;
}

// Get the index of a non-terminal in the non_terminals array
int get_non_terminal_index(char *symbol) {
    for (int i = 0; i < num_non_terminals; i++) {
        if (strcmp(non_terminals[i], symbol) == 0) {
            return i;
        }
    }
    return -1;  // Not found
}

// Check if a symbol is in a set
int is_in_set(SymbolSet *set, char *symbol) {
    for (int i = 0; i < set->count; i++) {
        if (strcmp(set->symbols[i], symbol) == 0) {
            return 1;
        }
    }
    return 0;
}

// Add a symbol to a set if it's not already there
void add_to_set(SymbolSet *set, char *symbol) {
    if (!is_in_set(set, symbol)) {
        strcpy(set->symbols[set->count], symbol);
        set->count++;
    }
}

// Add all symbols from one set to another
void add_set_to_set(SymbolSet *dest, SymbolSet *src) {
    for (int i = 0; i < src->count; i++) {
        add_to_set(dest, src->symbols[i]);
    }
}

// Print the sets (FIRST or FOLLOW)
void print_sets(SymbolSet *sets, char names[][MAX_SYMBOL_LENGTH], int count, const char *set_name) {
    printf("\n%s Sets:\n", set_name);
    for (int i = 0; i < count; i++) {
        printf("%s(%s) = { ", set_name, names[i]);
        for (int j = 0; j < sets[i].count; j++) {
            printf("%s", sets[i].symbols[j]);
            if (j < sets[i].count - 1) {
                printf(", ");
            }
        }
        printf(" }\n");
    }
}

// Find the FIRST set of a string (sequence of symbols)
void find_first_of_string(char *string, SymbolSet *result) {
    result->count = 0;
    
    // Copy the string since we'll modify it
    char str_copy[MAX_RHS_LENGTH];
    strcpy(str_copy, string);
    
    // Get the first symbol in the string
    char *symbol = strtok(str_copy, " ");
    if (symbol == NULL) {
        // Empty string, add epsilon
        add_to_set(result, "e");
        return;
    }
    
    if (!is_non_terminal(symbol)) {
        // Terminal symbol, just add it
        add_to_set(result, symbol);
    } else {
        // Non-terminal, add its FIRST set
        int idx = get_non_terminal_index(symbol);
        int has_epsilon = 0;
        
        // Add all non-epsilon symbols from the FIRST set
        for (int i = 0; i < first_sets[idx].count; i++) {
            if (strcmp(first_sets[idx].symbols[i], "e") == 0) {
                has_epsilon = 1;
            } else {
                add_to_set(result, first_sets[idx].symbols[i]);
            }
        }
        
        // If this non-terminal can derive epsilon, check the next symbol
        if (has_epsilon) {
            // Get the rest of the string
            char rest[MAX_RHS_LENGTH] = "";
            symbol = strtok(NULL, " ");
            while (symbol != NULL) {
                strcat(rest, symbol);
                strcat(rest, " ");
                symbol = strtok(NULL, " ");
            }
            
            // If there's more in the string, find its FIRST set
            if (strlen(rest) > 0) {
                SymbolSet rest_first;
                rest_first.count = 0;
                find_first_of_string(rest, &rest_first);
                add_set_to_set(result, &rest_first);
            } else {
                // End of string, add epsilon
                add_to_set(result, "e");
            }
        }
    }
}

// Compute the FIRST sets for all non-terminals
void compute_first_sets() {
    // Initialize all FIRST sets to empty
    for (int i = 0; i < num_non_terminals; i++) {
        first_sets[i].count = 0;
    }
    
    int change;
    do {
        change = 0;
        
        // For each production
        for (int i = 0; i < num_productions; i++) {
            int lhs_idx = get_non_terminal_index(productions[i].lhs);
            
            // If the RHS starts with a terminal or is epsilon
            char rhs_copy[MAX_RHS_LENGTH];
            strcpy(rhs_copy, productions[i].rhs);
            char *first_symbol = strtok(rhs_copy, " ");
            
            if (first_symbol == NULL || strcmp(first_symbol, "e") == 0) {
                // Epsilon production
                if (!is_in_set(&first_sets[lhs_idx], "e")) {
                    add_to_set(&first_sets[lhs_idx], "e");
                    change = 1;
                }
            } else if (!is_non_terminal(first_symbol)) {
                // Terminal symbol
                if (!is_in_set(&first_sets[lhs_idx], first_symbol)) {
                    add_to_set(&first_sets[lhs_idx], first_symbol);
                    change = 1;
                }
            } else {
                // Non-terminal symbol
                SymbolSet rhs_first;
                rhs_first.count = 0;
                find_first_of_string(productions[i].rhs, &rhs_first);
                
                // Add all from rhs_first to FIRST(lhs)
                for (int j = 0; j < rhs_first.count; j++) {
                    if (!is_in_set(&first_sets[lhs_idx], rhs_first.symbols[j])) {
                        add_to_set(&first_sets[lhs_idx], rhs_first.symbols[j]);
                        change = 1;
                    }
                }
            }
        }
    } while (change);
}

// Compute the FOLLOW sets for all non-terminals
void compute_follow_sets() {
    // Initialize all FOLLOW sets to empty
    for (int i = 0; i < num_non_terminals; i++) {
        follow_sets[i].count = 0;
    }
    
    // Add $ to FOLLOW(start_symbol)
    int start_idx = get_non_terminal_index(start_symbol);
    add_to_set(&follow_sets[start_idx], "$");
    
    int change;
    do {
        change = 0;
        
        // For each production
        for (int i = 0; i < num_productions; i++) {
            char *lhs = productions[i].lhs;
            char rhs_copy[MAX_RHS_LENGTH];
            strcpy(rhs_copy, productions[i].rhs);
            
            // Split the RHS into symbols
            char *symbols[MAX_RHS_LENGTH];
            int num_symbols = 0;
            
            char *symbol = strtok(rhs_copy, " ");
            while (symbol != NULL) {
                symbols[num_symbols++] = strdup(symbol);
                symbol = strtok(NULL, " ");
            }
            
            // For each symbol in the RHS
            for (int j = 0; j < num_symbols; j++) {
                if (is_non_terminal(symbols[j])) {
                    int B_idx = get_non_terminal_index(symbols[j]);
                    
                    // If B is the last symbol
                    if (j == num_symbols - 1) {
                        // FOLLOW(B) += FOLLOW(A)
                        int A_idx = get_non_terminal_index(lhs);
                        for (int k = 0; k < follow_sets[A_idx].count; k++) {
                            if (!is_in_set(&follow_sets[B_idx], follow_sets[A_idx].symbols[k])) {
                                add_to_set(&follow_sets[B_idx], follow_sets[A_idx].symbols[k]);
                                change = 1;
                            }
                        }
                    } else {
                        // Compute FIRST of everything after B
                        char beta[MAX_RHS_LENGTH] = "";
                        for (int k = j + 1; k < num_symbols; k++) {
                            strcat(beta, symbols[k]);
                            if (k < num_symbols - 1) {
                                strcat(beta, " ");
                            }
                        }
                        
                        SymbolSet first_beta;
                        first_beta.count = 0;
                        find_first_of_string(beta, &first_beta);
                        
                        int has_epsilon = 0;
                        // Add all non-epsilon symbols from FIRST(beta) to FOLLOW(B)
                        for (int k = 0; k < first_beta.count; k++) {
                            if (strcmp(first_beta.symbols[k], "e") == 0) {
                                has_epsilon = 1;
                            } else {
                                if (!is_in_set(&follow_sets[B_idx], first_beta.symbols[k])) {
                                    add_to_set(&follow_sets[B_idx], first_beta.symbols[k]);
                                    change = 1;
                                }
                            }
                        }
                        
                        // If beta can derive epsilon, FOLLOW(B) += FOLLOW(A)
                        if (has_epsilon) {
                            int A_idx = get_non_terminal_index(lhs);
                            for (int k = 0; k < follow_sets[A_idx].count; k++) {
                                if (!is_in_set(&follow_sets[B_idx], follow_sets[A_idx].symbols[k])) {
                                    add_to_set(&follow_sets[B_idx], follow_sets[A_idx].symbols[k]);
                                    change = 1;
                                }
                            }
                        }
                    }
                }
            }
            
            // Free memory
            for (int j = 0; j < num_symbols; j++) {
                free(symbols[j]);
            }
        }
    } while (change);
}