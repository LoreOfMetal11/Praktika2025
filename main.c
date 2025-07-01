#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>  
#include <windows.h>
#include <ctype.h>

const char* default_filename = "Array.txt";
const char* sorted_filename = "SortedArray.txt";

int readArrayFromFile(const char* filename, int** array, int* size) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Îøèáêà: íå óäàëîñü îòêðûòü ôàéë %s\n", filename);
        return 0;
    }

    int count = 0;
    int temp;
    while (fscanf_s(file, "%d", &temp) == 1) count++;

    if (count == 0) {
        printf("Ôàéë ïóñò èëè ñîäåðæèò íå÷èñëîâûå äàííûå\n");
        fclose(file);
        return 0;
    }

    *array = (int*)malloc(count * sizeof(int));
    if (*array == NULL) {
        printf("Îøèáêà âûäåëåíèÿ ïàìÿòè\n");
        fclose(file);
        return 0;
    }

    rewind(file);
    for (int i = 0; i < count; i++) {
        if (fscanf_s(file, "%d", &(*array)[i]) != 1) {
            printf("Îøèáêà ÷òåíèÿ ÷èñëà èç ôàéëà\n");
            free(*array);
            fclose(file);
            return 0;
        }
    }

    fclose(file);
    *size = count;
    return 1;
}

void RecordSortedArray(const char* filename, int* array, int size) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Îøèáêà: íå óäàëîñü ñîçäàòü ôàéë %s\n", filename);
        return;
    }

    for (int i = 0; i < size; i++) {
        fprintf(file, "%d ", array[i]);
    }

    fclose(file);
    printf("Ìàññèâ óñïåøíî çàïèñàí â ôàéë %s\n", filename);
}

void bubbleSort(int* arr, int size, int method) {
    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (method == 2) {
                if (arr[j] > arr[j + 1]) {  
                    int temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
            else if (method == 1) {
                if (arr[j] < arr[j + 1]) {  
                    int temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }
    }

    QueryPerformanceCounter(&end);
    double time_spent = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

    printf("Îòñîðòèðîâàííûé ìàññèâ:\n");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\nÂðåìÿ âûïîëíåíèÿ: %.6f ñåêóíä\n", time_spent);
}

int MakeArrayFile(int min, int max, int size, const char* filename) {
    if (min >= max) {
        printf("Îøèáêà: íèæíèé ïðåäåë äîëæåí áûòü ìåíüøå âåðõíåãî.\n");
        return 0;
    }
    if (size <= 0) {
        printf("Îøèáêà: ðàçìåð ìàññèâà äîëæåí áûòü ïîëîæèòåëüíûì.\n");
        return 0;
    }

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Îøèáêà: íå óäàëîñü ñîçäàòü èëè îòêðûòü ôàéë %s.\n", filename);
        return 0;
    }

    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        int num = rand() % (max - min + 1) + min;
        printf("%d ", num);
        fprintf(file, "%d ", num);
    }

    fclose(file);
    printf("\n");
    printf("Ôàéë %s óñïåøíî ñîçäàí.\n", filename);
    return 1;
}

int OwnArray(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Îøèáêà îòêðûòèÿ ôàéëà!\n");
        return 0;
    }

    printf("Ââåäèòå öåëûå ÷èñëà ÷åðåç ïðîáåë èëè êàæäîå ñ íîâîé ñòðîêè.\n");
    printf("Äëÿ çàâåðøåíèÿ ââîäà ââåäèòå '!' íà íîâîé ñòðîêå:\n");
    char input[256];
    char* context = NULL;
    while (1) {
        fgets(input, sizeof(input), stdin);
        if (strchr(input, '!') != NULL) break;

        char* token = strtok_s(input, " \n", &context);
        while (token != NULL) {
            if (strcmp(token, "!") == 0) {
                fclose(file);
                printf("Ôàéë %s óñïåøíî ñîçäàí.\n", filename);
                getchar();
                system("cls");
                return 1;
            }

            int value;
            if (sscanf_s(token, "%d", &value) == 1) {
                fprintf(file, "%d ", value);
            }
            token = strtok_s(NULL, " \n", &context);
        }
    }

    fclose(file);
    printf("Ôàéë %s óñïåøíî ñîçäàí.\n", filename);
    return 1;
}

int checkForExtraChars() {
    int c = getchar();
    return (c != '\n' && c != EOF);
}

int main() {
    system("chcp 1251");
    system("cls");
    srand(time(NULL));

    char current_filename[100] = "Array.txt";
    int size = 0;
    int* current_array = NULL;

    while (1) {
        printf("\nÌåíþ:\n");
        printf("1. Ñîçäàòü ìàññèâ ñ êëàâèàòóðû\n");
        printf("2. Ñãåíåðèðîâàòü ñëó÷àéíûé ìàññèâ\n");
        printf("3. Çàãðóçèòü ìàññèâ èç ôàéëà\n");
        printf("4. Îòñîðòèðîâàòü òåêóùèé ìàññèâ\n");
        printf("5. Âûéòè\n");
        printf("Âûáåðèòå äåéñòâèå: ");

        int choice;
        if (scanf_s("%d", &choice) != 1) {
            printf("Îøèáêà ââîäà. Ïîæàëóéñòà, ââåäèòå ÷èñëî.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 1: {
            while (getchar() != '\n'); 
            printf("Ââåäèòå èìÿ ôàéëà (ïî óìîë÷àíèþ %s): ", current_filename);
            char filename_buffer[100];
            fgets(filename_buffer, sizeof(filename_buffer), stdin);
            filename_buffer[strcspn(filename_buffer, "\n")] = '\0';

            if (strlen(filename_buffer) > 0) {
                strcpy_s(current_filename, sizeof(current_filename), filename_buffer);
            }

            if (OwnArray(current_filename)) {
                if (current_array) free(current_array);
                if (readArrayFromFile(current_filename, &current_array, &size)) {
                    printf("Ìàññèâ óñïåøíî çàãðóæåí. Ðàçìåð: %d\n", size);
                }
            }
            break;
        }

        case 2: {
            printf("Ââåäèòå ðàçìåð ìàññèâà: ");
            if (scanf_s("%d", &size) != 1 || size <= 0 || checkForExtraChars()) {
                printf("Íåâåðíûé ðàçìåð ìàññèâà.\n");
                while (getchar() != '\n');
                continue;
            }

            int min, max;
            printf("Ââåäèòå äèàïàçîí (min max): ");
            if (scanf_s("%d %d", &min, &max) != 2 || min >= max || checkForExtraChars()) {
                printf("Íåâåðíûé äèàïàçîí.\n");
                while (getchar() != '\n');
                continue;
            }

            if (MakeArrayFile(min, max, size, current_filename)) {
                if (current_array) free(current_array);
                if (readArrayFromFile(current_filename, &current_array, &size)) {
                    printf("Ìàññèâ óñïåøíî ñîçäàí è çàãðóæåí. Ðàçìåð: %d\n", size);
                }
            }
            printf("Íàæìèòå Enter ÷òîáû ïðîäîëæèòü.\n");
            getchar();
            system("cls");
            break;
        }

        case 3: {
            while (getchar() != '\n');  

            printf("Ââåäèòå èìÿ ôàéëà: ");
            char filename_buffer[100];
            fgets(filename_buffer, sizeof(filename_buffer), stdin);
            filename_buffer[strcspn(filename_buffer, "\n")] = '\0';

            if (current_array) free(current_array);
            if (readArrayFromFile(filename_buffer, &current_array, &size)) {
                strcpy_s(current_filename, sizeof(current_filename), filename_buffer);
                printf("Ìàññèâ óñïåøíî çàãðóæåí. Ðàçìåð: %d\n", size);
            }
            printf("Íàæìèòå Enter ÷òîáû ïðîäîëæèòü.\n");
            getchar();
            system("cls");
            break;
        }

        case 4: {
            if (!current_array || size == 0) {
                printf("Íåò çàãðóæåííîãî ìàññèâà äëÿ ñîðòèðîâêè.\n");
                break;
            }

            printf("Òèï ñîðòèðîâêè:\n1. Ïî óáûâàíèþ\n2. Ïî âîçðàñòàíèþ\nÂûáåðèòå: ");
            int method;
            if (scanf_s("%d", &method) != 1 || (method != 1 && method != 2) || checkForExtraChars()) {
                printf("Íåâåðíûé âûáîð.\n");
                while (getchar() != '\n');
                continue;
            }

            bubbleSort(current_array, size, method);
            RecordSortedArray(sorted_filename, current_array, size);
            printf("Íàæìèòå Enter ÷òîáû ïðîäîëæèòü.\n");
            getchar();
            system("cls");
            break;
        }

        case 5:
            system("cls");
            printf("\n\n\n\n==========================\nÐàáîòà ïðîãðàììû çàâåðøåíà.\nÆåëàåì õîðîøåãî äíÿ.\n==========================\n\n\n\n");
            if (current_array) free(current_array);
            return 0;

        default:
            printf("Íåâåðíûé âûáîð. Ïîïðîáóéòå ñíîâà.\n");
        }
    }
}
