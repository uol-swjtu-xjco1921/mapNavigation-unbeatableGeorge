#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hashTable.h"
#include "mapAdjList.h"
#include "pathFinder.h"


void dijkstra(int start, int end, int n, int m, double** p)
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            printf("%lf ", p[i][j]);
        }
        printf("\n");
    }
    double* dist;
    dist = (double*)malloc(sizeof(double) * N);
    for (int i = 0; i < N; i++)
    {
        dist[i] = INF;
    }

    int* st;
    st = (int*)malloc(sizeof(int) * N); // 分配内存空间
    for (int i = 0; i < N; i++)
    {
        st[i] = 0;
    }

    int* pre;
    pre = (int*)malloc(sizeof(int) * N); // 分配内存空间
    for (int i = 0; i < N; i++)
    {
        pre[i] = -1;
    }

    dist[start] = 0;

    for (int i = 0; i < n; i++) {
        int k = -1;
        for (int j = 0; j < n; j++) {
            if (!st[j] && (k == -1 || dist[k] > dist[j])) {
                k = j;
            }
        }
        st[k] = 1;

        for (int j = 0; j < n; j++) {
            if (!st[j] && dist[j] > dist[k] + p[k][j]) {
                dist[j] = dist[k] + p[k][j];
                pre[j] = k;
            }
        }
    }

    // 输出最短路径
    printf("The shortest distance from node %d to node %d is %lf.\n", start, end, dist[end]);
    printf("The path is: ");
    int temp = end;
    while (temp != start) {
        printf("%d<-", temp);
        temp = pre[temp];
    }

    printf("%d\n", start);
    free(dist);
    free(st);
    free(pre);
}