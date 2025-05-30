#include "kmeans.h"
#include <math.h>
#include <stdio.h>
#include <string.h>


/**
 * @brief Safe malloc.
 * @param size Размер для выделения памяти в байтах.
 * @param error Уточнение места возникновения ошибки.
 */
void *smalloc(size_t size, char *error) {
    void *addr = malloc(size);
    if (addr == NULL) {
        fprintf(stderr, "Ошибка выделения памяти. %s", error);
        exit(1);
    }
    return addr;
}


KMeans* kmeans_init(int k, int dimension) {
    //Проверка правильности введёных аргументов
    if (k <= 0) {
        fprintf(stderr, "Неправильно введён аргумент k для kmeans_init. k > 0");
        exit(1);
    }
    if (dimension <= 0) {
        fprintf(stderr, "Неправильно введён аргумент dimension для kmeans_init. dimension > 0");
        exit(1);
    }

    KMeans *kmeans = (KMeans *) smalloc(sizeof(kmeans), "kmeans в kmeans_init");
    kmeans->k = k;
    kmeans->centroids = (Point *) smalloc(k * sizeof(Point), "kmeans->centroids в kmeans_init");

    for (int i = 0; i < k; i++) {
        kmeans->centroids[i].coords = (double *) smalloc(dimension * sizeof(double), "kmeans->centroids[i].coords в kmeans_init");
        kmeans->centroids[i].dimension = dimension;
    }
    return kmeans;
}


void kmeans_free(KMeans *kmeans) {
    for (int i = 0; i < kmeans->k; i++) {
        free(kmeans->centroids[i].coords);
    }
    free(kmeans->centroids);
    free(kmeans);
}


/**
 * @brief Вычисляет расстояние между двумя точками.
 * @param a Первая точка.
 * @param b Вторая точка.
 * @return double 
 */
double point_distance(Point *a, Point *b) {
    double sum = 0.0;
    for (int i = 0; i < a->dimension; i++) {
        sum += (a->coords[i] - b->coords[i]) * (a->coords[i] - b->coords[i]);
    }
    return sqrt(sum);
}


/**
 * @brief Обновление координат центроида.
 * @param kmeans Указатель на структуру с кластерами.
 * @param points Указатель на массив точек.
 * @param labels Указатель на массив с метками кластеров для каждой точки.
 * @param num_points Количество точек, длина points.
 */
int points_in_cluster_count(KMeans *kmeans, Point *points, int *labels, int num_points, int j) {
    int count = 0;
    for (int i = 0; i < num_points; i++) {
        if (labels[i] == j) {
            for (int d = 0; d < kmeans->centroids[j].dimension; d++) {
                kmeans->centroids[j].coords[d] += points[i].coords[d];
            }
            count++;
        }
    }
    return count;
}


/**
 * @brief Обновление координат центроида.
 * @param kmeans Указатель на структуру с кластерами.
 * @param points Указатель на массив точек.
 * @param labels Указатель на массив с метками кластеров для каждой точки.
 * @param num_points Количество точек, длина points.
 */
void centroid_update(KMeans *kmeans, Point *points, int *labels, int num_points) {
    for (int j = 0; j < kmeans->k; j++) {
        //Обнуление координат центроида
        memset(kmeans->centroids[j].coords, 0, kmeans->centroids[j].dimension * sizeof(double));
        int count = points_in_cluster_count(kmeans, points, labels, num_points, j);

        if (count > 0) {
            for (int d = 0; d < kmeans->centroids[j].dimension; d++) {
                kmeans->centroids[j].coords[d] /= count;
            }
        }
    }
}


/**
 * @brief Функция с циклом итераций обучения.
 * @param kmeans Указатель на структуру с кластерами.
 * @param points Указатель на массив точек.
 * @param num_points Количество точек, длина points.
 * @param max_iterations Максимальное количество итераций обучения.
 */
void training_iterations(KMeans *kmeans, Point *points, int num_points, int max_iterations) {
    // Массив для хранения меток кластеров для каждой точки
    int *labels = (int *) smalloc(num_points * sizeof(int), "labels в training_iterations");

    for (int iter = 0; iter < max_iterations; iter++) {
        kmeans_predict(kmeans, points, num_points, labels);
        centroid_update(kmeans, points, labels, num_points);
    }
    free(labels);
}


/**
 * @brief Инициализирует рандомные центроиды.
 * @param kmeans Указатель на структуру с кластерами.
 * @param points Указатель на массив точек.
 * @note На самом деле выбирает первые k точек для центроидов.
 */
void centroid_random_init(KMeans *kmeans, Point *points) {
    for (int i = 0; i < kmeans->k; i++) {
        memcpy(kmeans->centroids[i].coords, points[i].coords, points[i].dimension * sizeof(double));
    }
}


void kmeans_train(KMeans *kmeans, Point *points, int num_points, int max_iterations) {
    centroid_random_init(kmeans, points);
    training_iterations(kmeans, points, num_points, max_iterations);
}


void kmeans_predict(KMeans *kmeans, Point *points, int num_points, int *labels) {
    for (int i = 0; i < num_points; i++) {
        double min_distance = INFINITY;
        for (int j = 0; j < kmeans->k; j++) {
            //Вычисление дистанции до центра кластера
            double distance = point_distance(&points[i], &kmeans->centroids[j]);
            if (distance < min_distance) {
                //Обновление дистанции до ближайшего кластера
                min_distance = distance;
                //Присвоение метки ближайшего кластера
                labels[i] = j;
            }
        }
    }
}
