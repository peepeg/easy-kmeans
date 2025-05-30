#include <stdio.h>
#include <stdlib.h>
#include "kmeans.h"

#define NUM_POINTS 10
#define DIMENSION 2

//
int main() {
    // Создание датасета
    Point points[NUM_POINTS];
    for (int i = 0; i < NUM_POINTS; i++) {
        points[i].coords = (double *) malloc(DIMENSION * sizeof(double));
        points[i].dimension = DIMENSION;
    }

    // Пример данных 
    double coords[NUM_POINTS][DIMENSION] = {
        {1.0, 2.0},
        {1.5, 1.8},
        {5.0, 8.0},
        {8.0, 8.0},
        {1.0, 0.6},
        {9.0, 11.0},
        {8.0, 2.0},
        {10.0, 2.0},
        {9.0, 3.0},
        {5.0, 4.0}
    };


    //Заполнение точек данными
    for (int i = 0; i < NUM_POINTS; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            points[i].coords[j] = coords[i][j];
        }
    }

    //Инициализация K-means
    int k = 3; // Количество кластеров
    KMeans *kmeans = kmeans_init(k, DIMENSION);

    //Обучение модели
    kmeans_train(kmeans, points, NUM_POINTS, 100);

    //Предсказание меток для точек
    int *labels = (int*) malloc(NUM_POINTS * sizeof(int));
    kmeans_predict(kmeans, points, NUM_POINTS, labels);


    //Вывод
    printf("Результаты кластеризации:\n");
    for (int i = 0; i < NUM_POINTS; i++) {
        printf("Точка (%.2f, %.2f) принадлежит кластеру %d\n", points[i].coords[0], points[i].coords[1], labels[i]);
    }


    //Освобождение памяти
    for (int i = 0; i < NUM_POINTS; i++) {
        free(points[i].coords);
    }
    free(labels);
    kmeans_free(kmeans);
    return 0;
}
