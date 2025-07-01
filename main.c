#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>  
#include <windows.h>
#include <ctype.h>

// Имена файлов, которые будем использовать по умолчанию. Удобно хранить их здесь.
const char* default_filename = "Array.txt";
const char* sorted_filename = "SortedArray.txt";

// Объявления функций
int OwnArray(const char* filename);
int MakeArrayFile(int min, int max, int size, const char* filename);
int readArrayFromFile(const char* filename, int** array, int* size);
void bubbleSort(int* arr, int size, int method);
void RecordSortedArray(const char* filename, int* array, int size);
int checkForExtraChars();


// Точка входа в программу. Здесь происходит все взаимодействие с пользователем 
int main() {
    // Чтобы кириллица в консоли не превращалась в кракозябры.
    system("chcp 1251");
    system("cls");
    // Инициализируем генератор случайных чисел, чтобы массивы каждый раз были разными.
    srand(time(NULL));

    // Тут будем хранить имя файла, с которым работаем сейчас.
    char current_filename[100] = "Array.txt";
    int size = 0;
    // Указатель на наш массив в памяти. Пока что он никуда не указывает.
    int* current_array = NULL;

    // Главный цикл программы. Он будет крутиться, пока пользователь не решит выйти.
    while (1) {
        printf("\nМеню:\n");
        printf("1. Ввести массив вручную\n");
        printf("2. Сгенерировать случайный массив\n");
        printf("3. Загрузить массив из файла\n");
        printf("4. Отсортировать текущий массив\n");
        printf("5. Выйти\n");
        printf("Выберите действие: ");

        int choice;
        // Проверяем, ввел ли пользователь вообще число. Эдакая защита от дурака.
        if (scanf_s("%d", &choice) != 1) {
            printf("Ошибка ввода. Пожалуйста, введите число от 1 до 5.\n");
            // Если в буфере осталось что-то лишнее (например, буквы), "съедаем" это,
            // чтобы оно не помешало следующему вводу.
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 1: { // Блок для ручного создания массива
            // Очищаем буфер после scanf_s, чтобы следующий fgets сработал как надо.
            while (getchar() != '\n');
            printf("Введите имя файла (или нажмите Enter, чтобы использовать '%s'): ", current_filename);

            char filename_buffer[100];
            fgets(filename_buffer, sizeof(filename_buffer), stdin);
            // fgets любит добавлять в конец строки символ новой строки ('\n'). Убираем его.
            filename_buffer[strcspn(filename_buffer, "\n")] = '\0';

            // Если пользователь ввел новое имя файла, а не просто нажал Enter, то используем его.
            if (strlen(filename_buffer) > 0) {
                strcpy_s(current_filename, sizeof(current_filename), filename_buffer);
            }

            if (OwnArray(current_filename)) {
                // Если в памяти уже был какой-то массив, освобождаем место перед загрузкой нового.
                if (current_array) free(current_array);
                // Сразу после создания файла пытаемся прочитать из него массив в память.
                if (readArrayFromFile(current_filename, &current_array, &size)) {
                    printf("Массив успешно загружен. Размер: %d\n", size);
                }
            }
            break;
        }

        case 2: { // Генерация массива со случайными числами
            printf("Введите размер массива: ");
            // Убедимся, что размер — это положительное число, и нет лишних символов после него.
            if (scanf_s("%d", &size) != 1 || size <= 0 || checkForExtraChars()) {
                printf("Неверный размер массива. Нужно ввести положительное число.\n");
                while (getchar() != '\n');
                continue;
            }

            int min, max;
            printf("Введите диапазон чисел (например: -100 100): ");
            // То же самое для диапазона: два числа, и min должен быть меньше max.
            if (scanf_s("%d %d", &min, &max) != 2 || min >= max || checkForExtraChars()) {
                printf("Неверный диапазон. Убедитесь, что min < max.\n");
                while (getchar() != '\n');
                continue;
            }

            if (MakeArrayFile(min, max, size, current_filename)) {
                if (current_array) free(current_array);
                if (readArrayFromFile(current_filename, &current_array, &size)) {
                    printf("Массив успешно создан и загружен. Размер: %d\n", size);
                }
            }
            getchar(); // Ждем нажатия Enter перед очисткой экрана.
            system("cls");
            break;
        }

        case 3: { // Загрузка массива из файла
            while (getchar() != '\n'); // Старая добрая очистка буфера.

            printf("Введите имя файла, из которого нужно загрузить массив: ");
            char filename_buffer[100];
            fgets(filename_buffer, sizeof(filename_buffer), stdin);
            filename_buffer[strcspn(filename_buffer, "\n")] = '\0';

            if (current_array) free(current_array);
            // Пытаемся прочитать массив. Если получилось, запоминаем его имя.
            if (readArrayFromFile(filename_buffer, &current_array, &size)) {
                strcpy_s(current_filename, sizeof(current_filename), filename_buffer);
                printf("Массив '%s' успешно загружен. Размер: %d\n", current_filename, size);
            }
            getchar();
            system("cls");
            break;
        }

        case 4: { // Сортировка! Самое интересное.
            // Проверяем, а есть ли вообще что сортировать?
            if (!current_array || size == 0) {
                printf("Сортировать нечего. Сначала загрузите или создайте массив.\n");
                break;
            }
            printf("Как сортируем?\n1. По убыванию\n2. По возрастанию\nВаш выбор: ");
            int method;
            if (scanf_s("%d", &method) != 1 || (method != 1 && method != 2) || checkForExtraChars()) {
                printf("Неверный выбор. Введите 1 или 2.\n");
                while (getchar() != '\n');
                continue;
            }

            bubbleSort(current_array, size, method);
            RecordSortedArray(sorted_filename, current_array, size);
            getchar();
            system("cls");
            break;
        }

        case 5: // Пора прощаться.
            if (current_array) free(current_array); // Наводим порядок перед выходом.
            return 0;

        default: // Если пользователь ввел что-то кроме чисел от 1 до 5.
            printf("Неизвестная команда. Попробуйте снова.\n");
        }
    }
}