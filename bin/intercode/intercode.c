#include <stdio.h>
#include <string.h>
#include <ctype.h>

int tempVarCount = 1;

char *newTemp()
{
    static char temp[10][5];
    snprintf(temp[tempVarCount - 1], 5, "t%d", tempVarCount);
    return temp[tempVarCount++ - 1];
}

void generateTAC(char expr[])
{
    char lhs[10], rhs[50];
    if (sscanf(expr, "%s = %[^\n]", lhs, rhs) < 2)
    {
        printf("; Invalid expression\n");
        return;
    }

    char tokens[5][10];
    int tokIndex = 0;

    for (int i = 0; rhs[i] && tokIndex < 5;)
    {
        if (isspace(rhs[i]))
            i++;
        else if (isalnum(rhs[i]))
        {
            int j = 0;
            while (isalnum(rhs[i]) && j < 9)
                tokens[tokIndex][j++] = rhs[i++];
            tokens[tokIndex][j] = '\0';
            tokIndex++;
        }
        else
        {
            tokens[tokIndex][0] = rhs[i++];
            tokens[tokIndex][1] = '\0';
            tokIndex++;
        }
    }

    if (tokIndex == 0)
    {
        printf("; Empty RHS\n");
        return;
    }

    for (int i = 0; i < tokIndex; i++)
    {
        if (strcmp(tokens[i], "*") == 0 || strcmp(tokens[i], "/") == 0)
        {
            if (i < 1 || i + 1 >= tokIndex)
                continue;
            char *temp = newTemp();
            printf("%s = %s %s %s\n", temp, tokens[i - 1], tokens[i], tokens[i + 1]);
            strcpy(tokens[i - 1], temp);
            for (int j = i; j < tokIndex - 2; j++)
                strcpy(tokens[j], tokens[j + 2]);
            tokIndex -= 2;
            i = -1;
        }
    }

    for (int i = 0; i < tokIndex; i++)
    {
        if (strcmp(tokens[i], "+") == 0 || strcmp(tokens[i], "-") == 0)
        {
            if (i < 1 || i + 1 >= tokIndex)
                continue;
            char *temp = newTemp();
            printf("%s = %s %s %s\n", temp, tokens[i - 1], tokens[i], tokens[i + 1]);
            strcpy(tokens[i - 1], temp);
            for (int j = i; j < tokIndex - 2; j++)
                strcpy(tokens[j], tokens[j + 2]);
            tokIndex -= 2;
            i = -1;
        }
    }

    if (tokIndex > 0)
        printf("%s = %s\n", lhs, tokens[0]);
}

int main()
{
    char expr[50];
    printf("Enter expression: ");
    if (!fgets(expr, sizeof(expr), stdin))
        return 1;
    expr[strcspn(expr, "\n")] = 0;

    printf("\nTAC:\n");
    generateTAC(expr);
    return 0;
}