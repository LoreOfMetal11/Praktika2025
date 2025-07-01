#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>  
#include <windows.h>
#include <ctype.h>


const char* filename = "Array.txt";
const char* sorted = "SortedArray.txt";

const int GLOBAL_MIN = -1000;
const int GLOBAL_MAX = 1000;

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int readArrayFromFile(const char* filename, int** array, int size) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("������: �� ������� ������� ���� %s\n", filename);
        return 0;
    }
    *array = (int*)malloc(size * sizeof(int));
    if (*array == NULL) {
        printf("������ ��������� ������\n");
        fclose(file);
        return 0;
    }
    int count = 0;
    while (count < size && fscanf(file, "%d", &(*array)[count]) == 1) {
        count++;
    }
    fclose(file);
    return count;
}

void RecordSortedArray(const char* filename, int* array, int size) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("������: �� ������� ������� ���� %s\n", filename);
        return;
    }
    for (int i = 0; i < size; i++) {
        fprintf(file, "%d ", array[i]);
    }
    fclose(file);
    printf("������ ������� ������� � ���� %s\n", filename);
}

void bubbleSort(int size, char* method) {
    int* arr;
    method[strcspn(method, "\n")] = 0;
    int actual_size = readArrayFromFile(filename, &arr, size);
    LARGE_INTEGER frequency, start, end;
    if (actual_size == 0) {
        printf("�� ������� ��������� ������ ��� ����������\n");
        return;
    }
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    for (int i = 0; i < actual_size - 1; i++) {
        for (int j = 0; j < actual_size - i - 1; j++) {
            if (_stricmp(method, "�����������") == 0) {
                if (arr[j] > arr[j + 1]) {
                    int temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
            else if (_stricmp(method, "��������") == 0) {
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
    printf("\n��������������� ������:\n");
    for (int i = 0; i < actual_size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    printf("����� ����������: %.6f ������\n", time_spent);
    RecordSortedArray(sorted, arr, actual_size);
    free(arr);
}

int MakeArrayFile(int min, int max, int size, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("������: �� ������� ������� ��� ������� ���� %s.\n", filename);
        return 0;
    }
    srand(time(NULL));
    printf("��������������� ������:\n");
    for (int i = 0; i < size; i++) {
        int num = rand() % (max - min + 1) + min;
        printf("%d ", num);
        fprintf(file, "%d ", num);
    }
    printf("\n");
    fclose(file);
    printf("���� %s ������� ������.\n", filename);
    return 1;
}


int main() {
    system("chcp 1251");
    srand(time(NULL));

    int choice;
    int size = 0, max = 0, min = 0;
    int array_is_ready = 0;

    while (1) {
        system("cls");
        printf("================== ���� ==================\n");
        printf("1. ������� ������ ������� (����� �� %d �� %d)\n", GLOBAL_MIN, GLOBAL_MAX);
        printf("2. ������������� ��������� ������ (� ��������� �� %d �� %d)\n", GLOBAL_MIN, GLOBAL_MAX);
        printf("3. ������� ���� � ��������������� ��������\n");
        printf("4. �����\n");
        printf("==========================================\n");

        while (1) {
            printf("��� �����: ");
            if (scanf("%d", &choice) == 1 && choice >= 1 && choice <= 4) {
                clear_input_buffer();
                break;
            }
            else {
                printf("������: ������� ����� �� 1 �� 4.\n");
                clear_input_buffer();
            }
        }

        array_is_ready = 0;

        switch (choice) {
        case 1: {
            while (1) {
                printf("������� ������ �������: ");
                if (scanf("%d", &size) == 1 && size > 0) { clear_input_buffer(); break; }
                printf("������: ������ ������ ���� ������������� ������.\n");
                clear_input_buffer();
            }

            FILE* f = fopen(filename, "w");
            if (f == NULL) { break; }
            printf("������� %d ��������� ������� (������ �� %d �� %d):\n", size, GLOBAL_MIN, GLOBAL_MAX);
            for (int i = 0; i < size; i++) {
                while (1) {
                    printf("������� %d: ", i + 1);
                    int element;
                    if (scanf("%d", &element) == 1) {
                        clear_input_buffer();
                        if (element >= GLOBAL_MIN && element <= GLOBAL_MAX) {
                            fprintf(f, "%d ", element);
                            break;
                        }
                        else {
                            printf("������: ����� ������ ���� � ��������� �� %d �� %d.\n", GLOBAL_MIN, GLOBAL_MAX);
                        }
                    }
                    else {
                        printf("������: ������� ������ ����� �����!\n");
                        clear_input_buffer();
                    }
                }
            }
            fclose(f);
            printf("������ ������� ������� � ���� %s\n", filename);
            array_is_ready = 1;
            break;
        }
        case 2: {
            while (1) {
                printf("������� ������ �������: ");
                if (scanf("%d", &size) == 1 && size > 0) { clear_input_buffer(); break; }
                printf("������: ������ ������ ���� ������������� ������.\n");
                clear_input_buffer();
            }
            
            while (1) {
                printf("������� ������ �������� ��������� (�� %d �� %d): ", GLOBAL_MIN, GLOBAL_MAX);
                if (scanf("%d", &min) == 1 && min >= GLOBAL_MIN && min <= GLOBAL_MAX) {
                    clear_input_buffer();
                    break;
                }
                printf("������: �������� �������� ��� ������ �����.\n");
                clear_input_buffer();
            }
            while (1) {
                printf("������� ������� �������� ��������� (�� %d �� %d): ", min, GLOBAL_MAX);
                if (scanf("%d", &max) == 1 && max >= min && max <= GLOBAL_MAX) {
                    clear_input_buffer();
                    break;
                }
                printf("������: �������� �������� ��� ������ �����.\n");
                clear_input_buffer();
            }

            if (MakeArrayFile(min, max, size, filename)) {
                array_is_ready = 1;
            }
            break;
        }
        case 3: {
            FILE* sorted_file = fopen(sorted, "r");
            if (sorted_file == NULL) {
                printf("���� '%s' ��� �� ������.\n", sorted);
            }
            else {
                fclose(sorted_file);
                char command[100];
                sprintf(command, "notepad %s", sorted);
                system(command);
            }
            break;
        }
        case 4:
            return 0;
        }

        if (array_is_ready) {
            char method[30];
            while (1) {
                printf("\n������� ��� ����������: '��������' ��� '�����������'.\n> ");
                fgets(method, sizeof(method), stdin);
                method[strcspn(method, "\n")] = 0;
                if (_stricmp(method, "��������") == 0 || _stricmp(method, "�����������") == 0) {
                    break;
                }
                else {
                    printf("������: ������� '��������' ��� '�����������'.\n");
                }
            }
            bubbleSort(size, method);
        }

        printf("\n������� Enter, ����� ��������� � ����...");
        getchar();
    }

    return 0;
}
