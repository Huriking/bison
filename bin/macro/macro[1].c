#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LENGTH 100

char sub[20];
char argz1[20], argz2[20];
int pos = 0;
char expanded[MAX_LENGTH][MAX_LENGTH];

int substring(char *source, int start, int length, char *dest) {
    int i;
    for (i = 0; i < length && source[start + i] != '\0' && source[start + i] != ','; i++) {
        dest[i] = source[start + i];
    }
    dest[i] = '\0';
    return start + i + 2;
}

int checkMacro(char macroName[]) {
    substring(macroName, 0, 6, sub);
    if (strcmp(sub, "ADDVAL") == 0) return 1;
    if (strcmp(sub, "SUBVAL") == 0) return 2;
    if (strcmp(sub, "MULVAL") == 0) return 3;
    if (strcmp(sub, "DIVVAL") == 0) return 4;
    return -1;
}

int getArg1(char macroName[], int start) {
    if (start == 7)
        return substring(macroName, start, 20, argz1);
    else
        return substring(macroName, start, 20, argz2);
}

void expandMacro(char macroName[]) {
    int macroType = checkMacro(macroName);
    int nextStart = getArg1(macroName, 7);
    int arg1 = atoi(argz1);
    getArg1(macroName, nextStart);
    int arg2 = atoi(argz2);
    int A, B;

    if (macroType == 1) {
        sprintf(expanded[pos++], "MOV A, %d\n", arg1);
        sprintf(expanded[pos++], "ADD A, %d\n", arg2);
    } else if (macroType == 2) {
        sprintf(expanded[pos++], "MOV A, %d\n", arg1);
        sprintf(expanded[pos++], "SUB A, %d\n", arg2);
    } else if (macroType == 3) {
        sprintf(expanded[pos++], "MOV A, %d\n", arg1);
        sprintf(expanded[pos++], "MUL A, %d\n", arg2);
    } else if (macroType == 4) {
        if (arg2 == 0) {
            printf("Error: Division by zero is not allowed!\n");
            return;
        }
        sprintf(expanded[pos++], "MOV A, %d\n", arg1);
        sprintf(expanded[pos++], "DIV A, %d\n", arg2);
    } else {
        printf("ERROR: Invalid macro\n");
        return;
    }

    sprintf(expanded[pos++], "MOV B, A\n");
    A = (macroType == 1) ? arg1 + arg2 :
        (macroType == 2) ? arg1 - arg2 :
        (macroType == 3) ? arg1 * arg2 :
        (arg2 != 0)      ? arg1 / arg2 : 0;

    sprintf(expanded[pos++], "Result (B) = %d\n", A);
}

int main() {
    char sentence[MAX_LENGTH];

    while (1) {
        fgets(sentence, sizeof(sentence), stdin);

        // Remove trailing newline
        sentence[strcspn(sentence, "\n")] = 0;

        if (strcmp(sentence, "MAIN ENDP") == 0)
            break;

        if (checkMacro(sentence) != -1) {
            expandMacro(sentence);
        } else {
            strcpy(expanded[pos++], sentence);
            strcat(expanded[pos - 1], "\n");  // To ensure line endings are preserved
        }
    }

    // Print final expanded code
    printf("\n--- Expanded Code ---\n");
    for (int i = 0; i < pos; i++) {
        printf("%s", expanded[i]);
    }

    // Print "MAIN ENDP" after all macros have been expanded
    printf("MAIN ENDP\n");

    return 0;
}

