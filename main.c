#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashTable.h"
#include "mapAdjList.h"
#include "mapReader.h"
#include "mapEditor.h"
#include "pathFinder.h"

/***********************************/
/* XJCO 1921M Programming Project  */
/* 2022-23 Spring Semester         */
/*                                 */
/* Version: Store data in adj list */
/* Yifan Xiang					   */
/***********************************/

/***********************************/
/* Main Routine                    */
/***********************************/

int main()
{
    AdjacencyList adj_list; // Define an adjacency list variable
    
    init_adjacency_list(&adj_list); // Initialize the adjacency list

    int result = read_map(&adj_list);
                                        
    result = ask_modify_map(&adj_list);    // modify_map
    if (result == 1)
    {
        printf("\nÐÞ¸Ä´ÎÊý %d", adj_list.num_edit);
    }

    double** p;
    p = (double**)malloc(sizeof(double*) * N);
    for (int i = 0; i < N; i++) {
        p[i] = (double*)malloc(sizeof(double) * N);
        for (int j = 0; j < N; j++) {
            p[i][j] = INF;
        }
    }
    int n = adj_list.num_points;
    int m = adj_list.num_edges;
    int a, b;
    double c;
    for (int i = 0; i < m; i++)
    {
        a = hashtable_lookup(adj_list.point_hash, adj_list.edges[i].from);
        b = hashtable_lookup(adj_list.point_hash, adj_list.edges[i].to);
        c = adj_list.edges[i].length;
        p[a][b] = min(p[a][b], c);
        p[b][a] = min(p[b][a], c);
    }
    for (int i = 0; i < m; i++)
    {
        p[i][i] = 0;
    }
    int start = 0;
    int end = 0;
    scanf("%d", &start);
    scanf("%d", &end);
    start = hashtable_lookup(adj_list.point_hash, start);
    end = hashtable_lookup(adj_list.point_hash, end);
    dijkstra(start, end, n, m, p);
    for (int i = 0; i < N; i++) {
        free(p[i]);
    }
    free(p);
    return 0;

    return 0;

}