//3
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================
   ЗАДАНИЕ 2: Проверка и построение кучи для массива целых чисел
   ============================================================ */

typedef enum { MIN_HEAP, MAX_HEAP } HeapType;

// Проверка: является ли массив мин-кучей
int isMinHeap(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < n && arr[i] > arr[left]) return 0;
        if (right < n && arr[i] > arr[right]) return 0;
    }
    return 1;
}

// Проверка: является ли массив макс-кучей
int isMaxHeap(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < n && arr[i] < arr[left]) return 0;
        if (right < n && arr[i] < arr[right]) return 0;
    }
    return 1;
}

// Просеивание вниз для целочисленной кучи
// Правила: если потомки равны — выбираем левого;
//          если родитель равен выбранному потомку — обмена нет.
void siftDownInt(int arr[], int i, int n, HeapType type) {
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left >= n) break; // нет потомков

        int chosen = left;
        if (right < n) {
            if (type == MIN_HEAP) {
                if (arr[right] < arr[left]) chosen = right;
            } else {
                if (arr[right] > arr[left]) chosen = right;
            }
        }

        // Проверка: нужен ли обмен
        if (type == MIN_HEAP) {
            if (arr[i] <= arr[chosen]) break;
        } else {
            if (arr[i] >= arr[chosen]) break;
        }

        int tmp = arr[i];
        arr[i] = arr[chosen];
        arr[chosen] = tmp;
        i = chosen;
    }
}

// Построение кучи из массива (последовательное просеивание вниз)
void buildHeapInt(int arr[], int n, HeapType type) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        siftDownInt(arr, i, n, type);
    }
}

// Проверка корректности построенной кучи
int checkHeapInt(int arr[], int n, HeapType type) {
    if (type == MIN_HEAP) return isMinHeap(arr, n);
    else return isMaxHeap(arr, n);
}

void task2(void) {
    int n;
    printf("\n=== ЗАДАНИЕ 2 ===\n");
    printf("Введите N: ");
    scanf("%d", &n);

    int *arr = (int *)malloc(n * sizeof(int));
    printf("Введите %d целых чисел:\n", n);
    for (int i = 0; i < n; i++) scanf("%d", &arr[i]);

    // Определяем тип исходной кучи
    if (isMinHeap(arr, n)) {
        printf("YES min\n");
    } else if (isMaxHeap(arr, n)) {
        printf("YES max\n");
    } else {
        printf("NO\n");
    }

    // Выбор типа кучи для построения
    int choice;
    printf("Построить кучу: 1 - минимальная, 2 - максимальная: ");
    scanf("%d", &choice);
    HeapType type = (choice == 1) ? MIN_HEAP : MAX_HEAP;

    buildHeapInt(arr, n, type);

    printf("Построенная куча: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");

    if (checkHeapInt(arr, n, type))
        printf("Куча построена корректно.\n");
    else
        printf("ОШИБКА: куча построена некорректно!\n");

    free(arr);
}


/* ============================================================
   ЗАДАНИЕ 3: Удаление элемента из кучи (корня)
   ============================================================ */

// Извлечение корня: возвращает значение, размер уменьшается на 1
int extractRootInt(int arr[], int *n, HeapType type) {
    if (*n == 0) return -1;
    int root = arr[0];
    arr[0] = arr[*n - 1];
    (*n)--;
    if (*n > 0) siftDownInt(arr, 0, *n, type);
    return root;
}

void task3(void) {
    int n;
    printf("\n=== ЗАДАНИЕ 3 ===\n");
    printf("Введите N: ");
    scanf("%d", &n);

    int *arr = (int *)malloc(n * sizeof(int));
    printf("Введите %d целых чисел:\n", n);
    for (int i = 0; i < n; i++) scanf("%d", &arr[i]);

    int choice;
    printf("Тип кучи: 1 - мин, 2 - макс: ");
    scanf("%d", &choice);
    HeapType type = (choice == 1) ? MIN_HEAP : MAX_HEAP;

    buildHeapInt(arr, n, type);
    printf("Куча: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");

    int idx;
    printf("Введите индекс элемента для удаления (0..%d): ", n - 1);
    scanf("%d", &idx);
    if (idx < 0 || idx >= n) {
        printf("Неверный индекс.\n");
        free(arr);
        return;
    }

    // Стандартный способ: меняем с последним, уменьшаем размер,
    // затем восстанавливаем кучу (просеивание вверх или вниз).
    // Здесь используем простой подход через извлечение корня после замены.
    int removed = arr[idx];
    arr[idx] = arr[n - 1];
    n--;
    if (n > 0) {
        // Нужно восстановить: сначала sift-up от idx, потом sift-down
        // Для простоты — перестраиваем кучу заново
        buildHeapInt(arr, n, type);
    }
    printf("Удалён элемент: %d\n", removed);
    printf("Куча после удаления: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");

    free(arr);
}


/* ============================================================
   ЗАДАНИЕ 4: Сортировка последовательным удалением из кучи
   ============================================================ */

void task4(void) {
    int n;
    printf("\n=== ЗАДАНИЕ 4 ===\n");
    printf("Введите N: ");
    scanf("%d", &n);

    int *arr = (int *)malloc(n * sizeof(int));
    printf("Введите %d целых чисел:\n", n);
    for (int i = 0; i < n; i++) scanf("%d", &arr[i]);

    int choice;
    printf("Сортировка: 1 - по возрастанию (через мин-кучу), 2 - по убыванию (через макс-кучу): ");
    scanf("%d", &choice);
    HeapType type = (choice == 1) ? MIN_HEAP : MAX_HEAP;

    buildHeapInt(arr, n, type);

    int *sorted = (int *)malloc(n * sizeof(int));
    int size = n;
    for (int i = 0; i < n; i++) {
        sorted[i] = extractRootInt(arr, &size, type);
    }

    printf("Отсортированный массив: ");
    for (int i = 0; i < n; i++) printf("%d ", sorted[i]);
    printf("\n");

    free(arr);
    free(sorted);
}


/* ============================================================
   ЗАДАНИЕ 5: Классическая пирамидальная сортировка (Heap Sort)
   ============================================================ */

void siftDownSort(int arr[], int i, int n) {
    // Строим макс-кучу
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left >= n) break;

        int largest = left;
        if (right < n && arr[right] > arr[left]) largest = right;

        if (arr[i] >= arr[largest]) break;

        int tmp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = tmp;
        i = largest;
    }
}

void heapSort(int arr[], int n) {
    // Построение макс-кучи
    for (int i = n / 2 - 1; i >= 0; i--)
        siftDownSort(arr, i, n);

    // Поочерёдное извлечение максимума в конец
    for (int i = n - 1; i > 0; i--) {
        int tmp = arr[0];
        arr[0] = arr[i];
        arr[i] = tmp;
        siftDownSort(arr, 0, i);
    }
}

void task5(void) {
    int n;
    printf("\n=== ЗАДАНИЕ 5 ===\n");
    printf("Введите N: ");
    scanf("%d", &n);

    int *arr = (int *)malloc(n * sizeof(int));
    printf("Введите %d целых чисел:\n", n);
    for (int i = 0; i < n; i++) scanf("%d", &arr[i]);

    printf("Исходный массив: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");

    heapSort(arr, n);

    printf("Отсортированный массив: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");

    free(arr);
}


/* ============================================================
   ЗАДАНИЕ 6: Сортировка заявок пирамидальной сортировкой
   ============================================================ */

typedef struct {
    int id;
    int priority;
    int time;
} Task;

// Сравнение для кучи:
// - по убыванию priority
// - при равенстве — по возрастанию time
// - при равенстве — по возрастанию id
// Возвращает <0, если a имеет БОЛЬШИЙ приоритет (должен быть выше в куче)
int taskCmp(const Task *a, const Task *b) {
    if (a->priority != b->priority)
        return b->priority - a->priority; // DESC priority
    if (a->time != b->time)
        return a->time - b->time;         // ASC time
    return a->id - b->id;                 // ASC id
}

void siftDownTask(Task arr[], int i, int n) {
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left >= n) break;

        int best = left;
        if (right < n && taskCmp(&arr[right], &arr[left]) < 0)
            best = right;

        if (taskCmp(&arr[i], &arr[best]) <= 0) break;

        Task tmp = arr[i];
        arr[i] = arr[best];
        arr[best] = tmp;
        i = best;
    }
}

void heapSortTasks(Task arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        siftDownTask(arr, i, n);

    for (int i = n - 1; i > 0; i--) {
        Task tmp = arr[0];
        arr[0] = arr[i];
        arr[i] = tmp;
        siftDownTask(arr, 0, i);
    }
}

void task6(void) {
    int n;
    printf("\n=== ЗАДАНИЕ 6 ===\n");
    printf("Введите N (1..100000): ");
    if (scanf("%d", &n) != 1 || n < 1 || n > 100000) {
        printf("Неверный N\n");
        return;
    }

    Task *tasks = (Task *)malloc(n * sizeof(Task));
    for (int i = 0; i < n; i++) {
        scanf("%d %d %d", &tasks[i].id, &tasks[i].priority, &tasks[i].time);
    }

    heapSortTasks(tasks, n);

    for (int i = 0; i < n; i++) {
        printf("%d %d %d\n", tasks[i].id, tasks[i].priority, tasks[i].time);
    }

    free(tasks);
}


/* ============================================================
   МЕНЮ
   ============================================================ */

int main(void) {
    setbuf(stdout, NULL);
    int choice;
    do {
        printf("\n--- МЕНЮ ---\n");
        printf("2. Проверка и построение кучи (целые числа)\n");
        printf("3. Удаление элемента из кучи\n");
        printf("4. Сортировка последовательным удалением из кучи\n");
        printf("5. Пирамидальная сортировка (Heap Sort)\n");
        printf("6. Сортировка заявок (структуры)\n");
        printf("0. Выход\n");
        printf("Выберите пункт: ");
        scanf("%d", &choice);

        switch (choice) {
            case 2: task2(); break;
            case 3: task3(); break;
            case 4: task4(); break;
            case 5: task5(); break;
            case 6: task6(); break;
            case 0: return 0;
            default: printf("Нет такого пункта\n");
        }
    } while (1);
}
