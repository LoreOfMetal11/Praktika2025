#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>  
#include <windows.h>
#include <ctype.h>

// ����� ������, ������� ����� ������������ �� ���������. ������ ������� �� �����.
const char* default_filename = "Array.txt";
const char* sorted_filename = "SortedArray.txt";

// ���������� �������
int OwnArray(const char* filename);
int MakeArrayFile(int min, int max, int size, const char* filename);
int readArrayFromFile(const char* filename, int** array, int* size);
void bubbleSort(int* arr, int size, int method);
void RecordSortedArray(const char* filename, int* array, int size);
int checkForExtraChars();


// ����� ����� � ���������. ����� ���������� ��� �������������� � ������������� 
int main() {
    // ����� ��������� � ������� �� ������������ � ����������.
    system("chcp 1251");
    system("cls");
    // �������������� ��������� ��������� �����, ����� ������� ������ ��� ���� �������.
    srand(time(NULL));

    // ��� ����� ������� ��� �����, � ������� �������� ������.
    char current_filename[100] = "Array.txt";
    int size = 0;
    // ��������� �� ��� ������ � ������. ���� ��� �� ������ �� ���������.
    int* current_array = NULL;

    // ������� ���� ���������. �� ����� ���������, ���� ������������ �� ����� �����.
    while (1) {
        printf("\n����:\n");
        printf("1. ������ ������ �������\n");
        printf("2. ������������� ��������� ������\n");
        printf("3. ��������� ������ �� �����\n");
        printf("4. ������������� ������� ������\n");
        printf("5. �����\n");
        printf("�������� ��������: ");

        int choice;
        // ���������, ���� �� ������������ ������ �����. ������ ������ �� ������.
        if (scanf_s("%d", &choice) != 1) {
            printf("������ �����. ����������, ������� ����� �� 1 �� 5.\n");
            // ���� � ������ �������� ���-�� ������ (��������, �����), "�������" ���,
            // ����� ��� �� �������� ���������� �����.
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 1: { // ���� ��� ������� �������� �������
            // ������� ����� ����� scanf_s, ����� ��������� fgets �������� ��� ����.
            while (getchar() != '\n');
            printf("������� ��� ����� (��� ������� Enter, ����� ������������ '%s'): ", current_filename);

            char filename_buffer[100];
            fgets(filename_buffer, sizeof(filename_buffer), stdin);
            // fgets ����� ��������� � ����� ������ ������ ����� ������ ('\n'). ������� ���.
            filename_buffer[strcspn(filename_buffer, "\n")] = '\0';

            // ���� ������������ ���� ����� ��� �����, � �� ������ ����� Enter, �� ���������� ���.
            if (strlen(filename_buffer) > 0) {
                strcpy_s(current_filename, sizeof(current_filename), filename_buffer);
            }

            if (OwnArray(current_filename)) {
                // ���� � ������ ��� ��� �����-�� ������, ����������� ����� ����� ��������� ������.
                if (current_array) free(current_array);
                // ����� ����� �������� ����� �������� ��������� �� ���� ������ � ������.
                if (readArrayFromFile(current_filename, &current_array, &size)) {
                    printf("������ ������� ��������. ������: %d\n", size);
                }
            }
            break;
        }

        case 2: { // ��������� ������� �� ���������� �������
            printf("������� ������ �������: ");
            // ��������, ��� ������ � ��� ������������� �����, � ��� ������ �������� ����� ����.
            if (scanf_s("%d", &size) != 1 || size <= 0 || checkForExtraChars()) {
                printf("�������� ������ �������. ����� ������ ������������� �����.\n");
                while (getchar() != '\n');
                continue;
            }

            int min, max;
            printf("������� �������� ����� (��������: -100 100): ");
            // �� �� ����� ��� ���������: ��� �����, � min ������ ���� ������ max.
            if (scanf_s("%d %d", &min, &max) != 2 || min >= max || checkForExtraChars()) {
                printf("�������� ��������. ���������, ��� min < max.\n");
                while (getchar() != '\n');
                continue;
            }

            if (MakeArrayFile(min, max, size, current_filename)) {
                if (current_array) free(current_array);
                if (readArrayFromFile(current_filename, &current_array, &size)) {
                    printf("������ ������� ������ � ��������. ������: %d\n", size);
                }
            }
            getchar(); // ���� ������� Enter ����� �������� ������.
            system("cls");
            break;
        }

        case 3: { // �������� ������� �� �����
            while (getchar() != '\n'); // ������ ������ ������� ������.

            printf("������� ��� �����, �� �������� ����� ��������� ������: ");
            char filename_buffer[100];
            fgets(filename_buffer, sizeof(filename_buffer), stdin);
            filename_buffer[strcspn(filename_buffer, "\n")] = '\0';

            if (current_array) free(current_array);
            // �������� ��������� ������. ���� ����������, ���������� ��� ���.
            if (readArrayFromFile(filename_buffer, &current_array, &size)) {
                strcpy_s(current_filename, sizeof(current_filename), filename_buffer);
                printf("������ '%s' ������� ��������. ������: %d\n", current_filename, size);
            }
            getchar();
            system("cls");
            break;
        }

        case 4: { // ����������! ����� ����������.
            // ���������, � ���� �� ������ ��� �����������?
            if (!current_array || size == 0) {
                printf("����������� ������. ������� ��������� ��� �������� ������.\n");
                break;
            }
            printf("��� ���������?\n1. �� ��������\n2. �� �����������\n��� �����: ");
            int method;
            if (scanf_s("%d", &method) != 1 || (method != 1 && method != 2) || checkForExtraChars()) {
                printf("�������� �����. ������� 1 ��� 2.\n");
                while (getchar() != '\n');
                continue;
            }

            bubbleSort(current_array, size, method);
            RecordSortedArray(sorted_filename, current_array, size);
            getchar();
            system("cls");
            break;
        }

        case 5: // ���� ���������.
            if (current_array) free(current_array); // ������� ������� ����� �������.
            return 0;

        default: // ���� ������������ ���� ���-�� ����� ����� �� 1 �� 5.
            printf("����������� �������. ���������� �����.\n");
        }
    }
}