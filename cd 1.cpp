#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_ID_LENGTH 31

void skipWhitespaceAndComments(FILE *file) {
    int c;
    while ((c = fgetc(file)) != EOF) {
        if (isspace(c)) {
            continue;
        } else if (c == '/') {
            int next = fgetc(file);
            if (next == '/') {  // Single-line comment
                while ((c = fgetc(file)) != '\n' && c != EOF);
            } else if (next == '*') {  // Multi-line comment
                while ((c = fgetc(file)) != EOF) {
                    if (c == '*' && (c = fgetc(file)) == '/') {
                        break;
                    }
                }
            } else {
                ungetc(next, file);
                ungetc(c, file);
                break;
            }
        } else {
            ungetc(c, file);
            break;
        }
    }
}

void recognizeTokens(FILE *file) {
    int c;
    while ((c = fgetc(file)) != EOF) {
        skipWhitespaceAndComments(file);

        if (isalpha(c) || c == '_') {  // Identifier
            char identifier[MAX_ID_LENGTH + 1];
            int i = 0;
            do {
                if (i < MAX_ID_LENGTH) {
                    identifier[i++] = c;
                }
                c = fgetc(file);
            } while (isalnum(c) || c == '_');
            identifier[i] = '\0';
            ungetc(c, file);
            printf("Identifier: %s\n", identifier);
        } else if (isdigit(c)) {  // Constant
            char number[20];
            int i = 0;
            do {
                number[i++] = c;
                c = fgetc(file);
            } while (isdigit(c));
            number[i] = '\0';
            ungetc(c, file);
            printf("Constant: %s\n", number);
        } else if (strchr("+-*/=<>!&|", c)) {  // Operator
            char op[3] = {c, '\0', '\0'};
            int next = fgetc(file);
            if (strchr("=+-*/&|<>", next)) {
                op[1] = next;
            } else {
                ungetc(next, file);
            }
            printf("Operator: %s\n", op);
        }
    }
}

int main() {
    FILE *file = fopen("input.c", "r");
    if (!file) {
        printf("Error opening file!\n");
        return 1;
    }
    recognizeTokens(file);
    fclose(file);
    return 0;
}

