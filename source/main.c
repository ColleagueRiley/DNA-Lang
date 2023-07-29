#include <stdio.h>

#define SI_IMPLEMENTATION
#include "siString.h"

inline char basePairMatch(char ch, int rna);
char basePairMatch(char ch, int rna) {
    switch (ch) {
        case'A': return rna ? 'U' : 'T';
        case 'G': return 'C';
        case 'T': case 'U': return 'A';
        case 'C': return 'G'; 
    }
}

int main(int argc, char** argv) {
    if (argc <= 1)
        return 1;
    
    char compile = 0;
    siString c_code; 

    if (argc >= 3 && argv[2][1] == 'c') {
        compile = 1;
    
        c_code = si_string_make_len("#include <stdio.h>\nint input() {\n   int num;\n   scanf(\"%i\", &num);\n   return num;\n}\n\nint main() {", 97);
    }

    FILE* f = fopen(argv[1], "r");
    
    if (f == NULL) {
        printf("Failed to open file %s\n", argv[1]);
        return 1;
    }

    /* get length of file */
    fseek(f, 0U, SEEK_END);
    size_t file_len = ftell(f);
    fseek(f, 0U, SEEK_SET);

    /* read file into string on RAM */
    char* file = malloc(sizeof(char) * file_len);
    fread(file, 1, file_len, f);

    fclose(f);

    char str[4];
    int i, j = 0;

    int logic[3];
    size_t logicStage = 0;

    size_t line = 0;

    for (i = 0; i < file_len; i += 4) {
        if (file[i] == 'S') {
            for (i; i < file_len && file[i] != '\n'; i++);
            i++;
            line++;

            if (i >= file_len)
               break;
        }

        while (file[i] != 'A' && file[i] != 'G' && file[i] != 'T' && file[i] != 'C' && i < file_len) {
            if (file[i] == '\n')
                line++;

            i++;
        }

        if (file[i + 3] != basePairMatch(file[i], 0))
            printf("Incorrect second base pair at line %i\n", line);

        str[j] = file[i];
        j++;
    
        if (j != 3) 
            continue;

        j = 0;
        str[3] = '\0';

        /* convert DND to RNA */
        int x;
        for (x = 0; x < 3; x++)
            str[x] = basePairMatch(str[x], 1);

        if (!compile) {
            if (si_cstr_equal(str, "UAC")) { /* print */
                for (i += 9; i < file_len; i++) {
                    putchar(file[i]);

                    if (file[i] == '\n')
                        break;
                }

                i -= 4;
                continue;
            }

            if (si_cstr_equal(str, "ACG")) { /* == */
                logic[1] = 1;
                logicStage++;

                continue;
            }

            else if (si_cstr_equal(str, "UUU")) { /* 0 */
                logic[logicStage] = 0;
                logicStage++;
            }
            
            else if (si_cstr_equal(str, "CCC")) { /* 1 */
                logic[logicStage] = 1;
                logicStage++;
            }

            else if (si_cstr_equal(str, "UAG")) { /* input() */
                scanf("%i", &logic[logicStage]);
                logicStage++;
            }

            if (logicStage == 3 && logic[1] == 1) {
                if (logic[0] != logic[2])
                    i += (4 * 4) + 1;

                logicStage = 0;
                logic[0] = 0;
                logic[1] = 0;
                logic[2] = 0;
            }
        
            continue;
        }

        f = fopen("source.c", "w+");

        if (si_cstr_equal(str, "UAC")) { /* print */ 
            char pstr[27];
            size_t pstr_size = 0;
            
            for (i += 9; i < file_len && file[i] != '\n'; i++) {
                pstr[pstr_size] = file[i];
                pstr_size++;
            }

            si_string_append_len(&c_code, "   printf(\"", 11);
            si_string_append_len(&c_code, pstr, pstr_size);
            si_string_append_len(&c_code, "\\n\");", 5);

            i -= 4;
        } 

        if (si_cstr_equal(str, "CGU")) {
            si_string_append_len(&c_code, "\n   if (", 8);
        }

        if (si_cstr_equal(str, "ACG")) { /* == */
            si_string_append_len(&c_code, " == ", 4);
            
            logicStage++;
            continue;
        }

        else if (si_cstr_equal(str, "UUU"))
            si_string_append_len(&c_code, " 0 ", 3);
        
        else if (si_cstr_equal(str, "CCC")) /* 1 */
            si_string_append_len(&c_code, " 1 ", 3);

        else if (si_cstr_equal(str, "UAG")) {/* input() */
            si_string_append_len(&c_code, " input() ", 9);
        }
        
        if (logicStage) {
            logicStage = 0;
            si_string_append_len(&c_code, ")\n   ", 5);
        }
    }

    if (compile) {
        si_string_append_len(&c_code, "\n}", 2);

        fwrite(c_code, 1, si_string_len(c_code), f);
        fclose(f);

        si_string_free(c_code);
        system("gcc source.c -o output && rm source.c");
    }
}