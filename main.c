    // Интерфейс (менб) 
    int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    srand(time(NULL));

    char current_filename[100] = "Array.txt";
    int size = 0;
    int* current_array = NULL;

    while (1) {
        printf("\nМеню:\n");
        printf("1. Создать массив с клавиатуры\n");
        printf("2. Сгенерировать случайный массив\n");
        printf("3. Загрузить массив из файла\n");
        printf("4. Отсортировать текущий массив\n");
        printf("5. Выйти\n");
        printf("Выберите действие: ");

        int choice;
        if (scanf_s("%d", &choice) != 1) {
            printf("Ошибка ввода. Пожалуйста, введите число.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 1: {
            <<<<<< < HEAD
                while (getchar() != '\n');
            ====== =
                while (getchar() != '\n');
            >>>>>> > 4260603299ed186b4dbedd09ae4ae78f2c9b7d74
                printf("Введите имя файла (по умолчанию %s): ", current_filename);
            char filename_buffer[100];
            fgets(filename_buffer, sizeof(filename_buffer), stdin);
            filename_buffer[strcspn(filename_buffer, "\n")] = '\0';

            if (strlen(filename_buffer) > 0) {
                strcpy_s(current_filename, sizeof(current_filename), filename_buffer);
            }

            if (OwnArray(current_filename)) {
                if (current_array) free(current_array);
                if (readArrayFromFile(current_filename, &current_array, &size)) {
                    printf("Массив успешно загружен. Размер: %d\n", size);
                }
            }

            printf("Нажмите Enter чтобы продолжить.\n");
            getchar();
            system("cls");
            break;
        }

        case 2: {
            printf("Введите размер массива: ");
            if (scanf_s("%d", &size) != 1 || size <= 0 || checkForExtraChars()) {
                printf("Неверный размер массива.\n");
                while (getchar() != '\n');
                continue;
            }

            int min, max;
            printf("Введите диапазон (min max): ");
            if (scanf_s("%d %d", &min, &max) != 2 || min >= max || checkForExtraChars()) {
                printf("Неверный диапазон.\n");
                while (getchar() != '\n');
                continue;
            }

            if (MakeArrayFile(min, max, size, current_filename)) {
                if (current_array) free(current_array);
                if (readArrayFromFile(current_filename, &current_array, &size)) {
                    printf("Массив успешно создан и загружен. Размер: %d\n", size);
                }
            }
            printf("Нажмите Enter чтобы продолжить.\n");
            getchar();
            system("cls");
            break;
        }

        case 3: {
            <<<<<< < HEAD
                while (getchar() != '\n');
            ====== =
                while (getchar() != '\n');
            >>>>>> > 4260603299ed186b4dbedd09ae4ae78f2c9b7d74

                printf("Введите имя файла: ");
            char filename_buffer[100];
            fgets(filename_buffer, sizeof(filename_buffer), stdin);
            filename_buffer[strcspn(filename_buffer, "\n")] = '\0';

            if (current_array) free(current_array);
            if (readArrayFromFile(filename_buffer, &current_array, &size)) {
                strcpy_s(current_filename, sizeof(current_filename), filename_buffer);
                printf("Массив успешно загружен. Размер: %d\n", size);
            }
            printf("Нажмите Enter чтобы продолжить.\n");
            getchar();
            system("cls");
            break;
        }

        case 4: {
            if (!current_array || size == 0) {
                printf("Нет загруженного массива для сортировки.\n");
                break;
            }

            printf("Тип сортировки:\n1. По убыванию\n2. По возрастанию\nВыберите: ");
            int method;
            if (scanf_s("%d", &method) != 1 || (method != 1 && method != 2) || checkForExtraChars()) {
                printf("Неверный выбор.\n");
                while (getchar() != '\n');
                continue;
            }

            SortResult sortResult = bubbleSort(current_array, size, method);

            printf("Отсортированный массив:\n");
            for (int i = 0; i < size; i++) {
                printf("%d ", current_array[i]);
            }
            printf("\nВремя выполнения: %.6f секунд\n", sortResult.time_spent);
            printf("Количество перестановок: %d\n", sortResult.swapCount);

            RecordSortedArray(sorted_filename, current_array, size);
            printf("Нажмите Enter чтобы продолжить.\n");
            getchar();
            system("cls");
            break;
        }

        case 5:
            system("cls");
            printf("\n\n\n\n==========================\nРабота программы завершена.\nЖелаем хорошего дня.\n==========================\n\n\n\n");
            if (current_array) free(current_array);
            return 0;

        default:
            printf("Неверный выбор. Попробуйте снова.\n");
        }
    }
}
// работа с файлами (а именно чтение массива из файла)
int readArrayFromFile(const char* filename, int** array, int* size) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ошибка: не удалось открыть файл %s\n", filename);
        return 0;
    }

    int count = 0;
    int temp;
    while (fscanf_s(file, "%d", &temp) == 1) count++;

    if (count == 0) {
        printf("Файл пуст или содержит нечисловые данные\n");
        fclose(file);
        return 0;
    }

    *array = (int*)malloc(count * sizeof(int));
    if (*array == NULL) {
        printf("Ошибка выделения памяти\n");
        fclose(file);
        return 0;
    }

    rewind(file);
    for (int i = 0; i < count; i++) {
        if (fscanf_s(file, "%d", &(*array)[i]) != 1) {
            printf("Ошибка чтения числа из файла\n");
            free(*array);
            fclose(file);
            return 0;
        }
    }

    fclose(file);
    *size = count;
    return 1;
}
// запись отсортированного массива в файл
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
// рандом
int MakeArrayFile(int min, int max, int size, const char* filename) {
    if (min >= max) {
        printf("Ошибка: нижний предел должен быть меньше верхнего.\n");
        return 0;
    }
    if (size <= 0) {
        printf("Ошибка: размер массива должен быть положительным.\n");
        return 0;
    }

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка: не удалось создать или открыть файл %s.\n", filename);
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
    printf("Файл %s успешно создан.\n", filename);
    return 1;
}
// ввод массива с клавы
int OwnArray(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка открытия файла!\n");
        return 0;
    }

    printf("Введите целые числа через пробел или каждое с новой строки.\n");
    printf("Для завершения ввода введите '!' на новой строке:\n");
    char input[256];
    char* context = NULL;
    while (1) {
        fgets(input, sizeof(input), stdin);
        if (strchr(input, '!') != NULL) break;

        char* token = strtok_s(input, " \n", &context);
        while (token != NULL) {
            if (strcmp(token, "!") == 0) {
                fclose(file);
                printf("Файл %s успешно создан.\n", filename);
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
    printf("Файл %s успешно создан.\n", filename);
    return 1;
    <<<<<< < HEAD
}
====== =
}
>>>>>> > 4260603299ed186b4dbedd09ae4ae78f2c9b7d74
