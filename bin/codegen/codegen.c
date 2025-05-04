#include <stdio.h>
#include <string.h>

void generateAssembly(char *tacLine) {
    char result[10], op1[10], op2[10], op;

    // Check for binary operation: a = b + c
    if (sscanf(tacLine, "%s = %s %c %s", result, op1, &op, op2) == 4) {
        printf("MOV R1, %s\n", op1);
        if (op == '+')
            printf("ADD R1, %s\n", op2);
        else if (op == '-')
            printf("SUB R1, %s\n", op2);
        else if (op == '*')
            printf("MUL R1, %s\n", op2);
        else if (op == '/')
            printf("DIV R1, %s\n", op2);
        printf("MOV %s, R1\n", result);
    }
    // Simple assignment: a = b
    else if (sscanf(tacLine, "%s = %s", result, op1) == 2) {
        printf("MOV R1, %s\n", op1);
        printf("MOV %s, R1\n", result);
    } else {
        printf("Invalid TAC: %s\n", tacLine);
    }
}

int main() {
    char tac[10][50];
    int n;

    printf("Enter number of TAC instructions: ");
    scanf("%d", &n);
    getchar(); // consume newline

    printf("Enter TAC instructions:\n");
    for (int i = 0; i < n; i++) {
        fgets(tac[i], sizeof(tac[i]), stdin);
        tac[i][strcspn(tac[i], "\n")] = 0; // Remove newline
    }

    printf("\nAssembly Code:\n");
    for (int i = 0; i < n; i++) {
        generateAssembly(tac[i]);
    }

    return 0;
}
