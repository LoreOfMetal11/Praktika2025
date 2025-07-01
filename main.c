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
        printf("Ошибка: не удалось открыть файл %s\n", filename);
        return 0;
    }
    *array = (int*)malloc(size * sizeof(int));
    if (*array == NULL) {
        printf("Ошибка выделения памяти\n");
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
        printf("Ошибка: не удалось создать файл %s\n", filename);
        return;
    }
    for (int i = 0; i < size; i++) {
        fprintf(file, "%d ", array[i]);
    }
    fclose(file);
    printf("Массив успешно записан в файл %s\n", filename);
}

void bubbleSort(int size, char* method) {
    int* arr;
    method[strcspn(method, "\n")] = 0;
    int actual_size = readArrayFromFile(filename, &arr, size);
    LARGE_INTEGER frequency, start, end;
    if (actual_size == 0) {
        printf("Не удалось прочитать массив для сортировки\n");
        return;
    }
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    for (int i = 0; i < actual_size - 1; i++) {
        for (int j = 0; j < actual_size - i - 1; j++) {
            if (_stricmp(method, "возрастание") == 0) {
                if (arr[j] > arr[j + 1]) {
                    int temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
            else if (_stricmp(method, "убывание") == 0) {
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
    printf("\nОтсортированный массив:\n");
    for (int i = 0; i < actual_size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    printf("Время выполнения: %.6f секунд\n", time_spent);
    RecordSortedArray(sorted, arr, actual_size);
    free(arr);
}

int MakeArrayFile(int min, int max, int size, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка: не удалось создать или открыть файл %s.\n", filename);
        return 0;
    }
    srand(time(NULL));
    printf("Сгенерированный массив:\n");
    for (int i = 0; i < size; i++) {
        int num = rand() % (max - min + 1) + min;
        printf("%d ", num);
        fprintf(file, "%d ", num);
    }
    printf("\n");
    fclose(file);
    printf("Файл %s успешно создан.\n", filename);
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
        printf("================== МЕНЮ ==================\n");
        printf("1. Создать массив вручную (числа от %d до %d)\n", GLOBAL_MIN, GLOBAL_MAX);
        printf("2. Сгенерировать случайный массив (в диапазоне от %d до %d)\n", GLOBAL_MIN, GLOBAL_MAX);
        printf("3. Открыть файл с отсортированным массивом\n");
        printf("4. Выход\n");
        printf("==========================================\n");

        while (1) {
            printf("Ваш выбор: ");
            if (scanf("%d", &choice) == 1 && choice >= 1 && choice <= 4) {
                clear_input_buffer();
                break;
            }
            else {
                printf("Ошибка: введите число от 1 до 4.\n");
                clear_input_buffer();
            }
        }

        array_is_ready = 0;

        switch (choice) {
        case 1: {
            while (1) {
                printf("Введите размер массива: ");
                if (scanf("%d", &size) == 1 && size > 0) { clear_input_buffer(); break; }
                printf("Ошибка: размер должен быть положительным числом.\n");
                clear_input_buffer();
            }

            FILE* f = fopen(filename, "w");
            if (f == NULL) { break; }
            printf("Введите %d элементов массива (каждый от %d до %d):\n", size, GLOBAL_MIN, GLOBAL_MAX);
            for (int i = 0; i < size; i++) {
                while (1) {
                    printf("Элемент %d: ", i + 1);
                    int element;
                    if (scanf("%d", &element) == 1) {
                        clear_input_buffer();
                        if (element >= GLOBAL_MIN && element <= GLOBAL_MAX) {
                            fprintf(f, "%d ", element);
                            break;
                        }
                        else {
                            printf("Ошибка: число должно быть в диапазоне от %d до %d.\n", GLOBAL_MIN, GLOBAL_MAX);
                        }
                    }
                    else {
                        printf("Ошибка: вводите только целые числа!\n");
                        clear_input_buffer();
                    }
                }
            }
            fclose(f);
            printf("Массив успешно записан в файл %s\n", filename);
            array_is_ready = 1;
            break;
        }
        case 2: {
            while (1) {
                printf("Введите размер массива: ");
                if (scanf("%d", &size) == 1 && size > 0) { clear_input_buffer(); break; }
                printf("Ошибка: размер должен быть положительным числом.\n");
                clear_input_buffer();
            }
            
            while (1) {
                printf("Введите нижний диапазон генерации (от %d до %d): ", GLOBAL_MIN, GLOBAL_MAX);
                if (scanf("%d", &min) == 1 && min >= GLOBAL_MIN && min <= GLOBAL_MAX) {
                    clear_input_buffer();
                    break;
                }
                printf("Ошибка: неверный диапазон или формат ввода.\n");
                clear_input_buffer();
            }
            while (1) {
                printf("Введите верхний диапазон генерации (от %d до %d): ", min, GLOBAL_MAX);
                if (scanf("%d", &max) == 1 && max >= min && max <= GLOBAL_MAX) {
                    clear_input_buffer();
                    break;
                }
                printf("Ошибка: неверный диапазон или формат ввода.\n");
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
                printf("Файл '%s' еще не создан.\n", sorted);
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
                printf("\nВведите тип сортировки: 'убывание' или 'возрастание'.\n> ");
                fgets(method, sizeof(method), stdin);
                method[strcspn(method, "\n")] = 0;
                if (_stricmp(method, "убывание") == 0 || _stricmp(method, "возрастание") == 0) {
                    break;
                }
                else {
                    printf("Ошибка: введите 'убывание' или 'возрастание'.\n");
                }
            }
            bubbleSort(size, method);
        }

        printf("\nНажмите Enter, чтобы вернуться в меню...");
        getchar();
    }

    return 0;
}
