#ifndef KMEANS_H
#define KMEANS_H

#include <stdlib.h>


typedef struct {
    double *coords;
    int dimension; //Количество измерений/плоскостей, длина coords
} Point;


typedef struct {
    Point *centroids; //Центры кластеров
    int k; //Количество кластеров
} KMeans;


/**
 * @brief Инициализирует кластеры.
 * @param k>0 Количество кластеров.
 * @param dimension>0 Размерность кластера.
 * @return KMeans* Указатель на кластеры.
 * @warning Выделение памяти внутри функции.
 */
KMeans* kmeans_init(int k, int dimension);


/**
 * @brief Освобождает память, выделенную для кластеров.
 * @param kmeans Указатель на структуру с кластерами.
 */
void kmeans_free(KMeans *kmeans);


/**
 * @brief Функция обучения.
 * @param kmeans Указатель на структуру с кластерами.
 * @param points Указатель на массив точек.
 * @param num_points Количество точек, длина points 
 * @param max_iterations Максимальное количество итераций обучения.
 */
void kmeans_train(KMeans *kmeans, Point *points, int num_points, int max_iterations);


/**
 * @brief Присваивает метку кластера каждой точке.
 * @param kmeans Указатель на структуру с кластерами.
 * @param points Указатель на массив точек.
 * @param num_points Количество точек, длина points.
 * @param labels Указатель на массив с метками кластеров для каждой точки.
 */
void kmeans_predict(KMeans *kmeans, Point *points, int num_points, int *labels);


#endif //KMEANS_H
