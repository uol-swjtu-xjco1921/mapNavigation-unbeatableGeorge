#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashTable.h"
#include "mapAdjMatrix.h"
#include "mapError.h"

/* Initialize adjacency and hash tables */
void init_adjacency_matrix(AdjacencyMatrix* adj_matrix)
{
    adj_matrix->num_points = 0;
    adj_matrix->num_edges = 0;
    adj_matrix->num_edit = 0;
    adj_matrix->points = NULL;
    adj_matrix->adj = NULL;
    adj_matrix->point_hash = (HashTable*)calloc(1, sizeof(HashTable));
    if (adj_matrix->point_hash == NULL)
        exit(ERROR_MALLOC_FAILED);
    init_hashtable(adj_matrix->point_hash);   // Initialize the hash table
}


/* Free adjacency matrix memory   */
void free_adjacency_matrix(AdjacencyMatrix* adj_matrix)
{
    if (adj_matrix->points != NULL)
        free(adj_matrix->points);

    if (adj_matrix->adj != NULL) {
        for (int i = 0; i < adj_matrix->num_points; i++) {
            if (adj_matrix->adj[i] != NULL) {
                free(adj_matrix->adj[i]);
            }
        }
        free(adj_matrix->adj);
    }

    if (adj_matrix->point_hash != NULL)
        free_hashtable(adj_matrix->point_hash);
}


/* Add a point to the adjacency matrix    */
void add_point_to_adjacency_matrix(AdjacencyMatrix* adj_matrix, long long id, double lat, double lon)
{
    // Look up in Hash table to prevent the same point id
    if (hashtable_lookup(adj_matrix->point_hash, id) != -1)
    {
        printf("Duplicated point ID %lld!\n", id);
        free_adjacency_matrix(adj_matrix);
        exit(ERROR_HASH_LOOK_UP);
    }

    // Add point in Hash table
    if (hashtable_add(adj_matrix->point_hash, id, adj_matrix->num_points) == ERROR_HASH_ADD)
    {
        printf("Too much points! hash add failed!\n");
        free_adjacency_matrix(adj_matrix);
        exit(ERROR_HASH_ADD);
    }

    if (adj_matrix->num_points == 0)
    { // If it's the first point, allocate memory dynamically
        adj_matrix->points = (Point*)malloc(sizeof(Point));
        adj_matrix->adj = (double**)malloc(sizeof(double*));
        adj_matrix->adj[0] = (double*)malloc(sizeof(double));
        adj_matrix->adj[0][0] = 0;  // 初始化为 0
        if (adj_matrix->points == NULL || adj_matrix->adj == NULL)
        {
            printf("malloc failed in point add process!\n");
            free_adjacency_matrix(adj_matrix);
            exit(ERROR_MALLOC_FAILED);
        }
    }
    else
    { // Otherwise, memory needs to be reallocated
        adj_matrix->points = (Point*)realloc(adj_matrix->points, (unsigned long long)(adj_matrix->num_points + 1) * sizeof(Point));
        adj_matrix->adj = (double**)realloc(adj_matrix->adj, (unsigned long long)(adj_matrix->num_points + 1) * sizeof(double*));
        if (adj_matrix->points == NULL || adj_matrix->adj == NULL)
        {
            printf("malloc failed in point add process!\n");
            free_adjacency_matrix(adj_matrix);
            exit(ERROR_MALLOC_FAILED);
        }

        // initial the new pointer to NULL
        adj_matrix->adj[adj_matrix->num_points] = NULL;

        for (int i = 0; i < adj_matrix->num_points + 1; i++) 
        {
            adj_matrix->adj[i] = (double*)realloc(adj_matrix->adj[i], (unsigned long long)(adj_matrix->num_points + 1) * sizeof(double));
            if (i == adj_matrix->num_points && adj_matrix->adj[i] == NULL) 
            {
                printf("malloc failed in point add process!\n");
                free_adjacency_matrix(adj_matrix);
                exit(ERROR_MALLOC_FAILED);
            }
        }

        // initial the new row and col to INF
        for (int i = 0; i < adj_matrix->num_points + 1; i++) 
        {
            adj_matrix->adj[i][adj_matrix->num_points] = INF;
            adj_matrix->adj[adj_matrix->num_points][i] = INF;
        }
        // The point on the diagonal is 0
        adj_matrix->adj[adj_matrix->num_points][adj_matrix->num_points] = 0;
    }

    adj_matrix->points[adj_matrix->num_points].id = id;
    adj_matrix->points[adj_matrix->num_points].lat = lat;
    adj_matrix->points[adj_matrix->num_points].lon = lon;
    adj_matrix->num_points++;
}


/* Add an edge to adjacency matrix */
void add_edge_to_adjacency_matrix(AdjacencyMatrix* adj_matrix, long long id, long long from, long long to, double length)
{
    if (hashtable_lookup(adj_matrix->point_hash, from) == -1 || hashtable_lookup(adj_matrix->point_hash, to) == -1)
    {
        printf("Error: Invalid edge (%lld, %lld)\n", from, to);
        free_adjacency_matrix(adj_matrix);
        exit(ERROR_FIND_PATH_FAILED);
    }

    if (adj_matrix->num_edges == 0)
    { // If it's the first edge, allocate dynamically
        adj_matrix->edges = (Edge*)malloc(sizeof(Edge));
        if (adj_matrix->edges == NULL)
        {
            printf("malloc failed");
            free_adjacency_matrix(adj_matrix);
            exit(ERROR_MALLOC_FAILED);
        }
    }
    else
    { // Otherwise, memory needs to be reallocated
        adj_matrix->edges = (Edge*)realloc(adj_matrix->edges, (unsigned long long)(adj_matrix->num_edges + 1) * sizeof(Edge));
        if (adj_matrix->edges == NULL)
        {
            printf("malloc failed");
            free_adjacency_matrix(adj_matrix);
            exit(ERROR_MALLOC_FAILED);
        }
    }

    /* 存入边的信息到edges中 */
    adj_matrix->edges[adj_matrix->num_edges].id = id;
    adj_matrix->edges[adj_matrix->num_edges].from = from;
    adj_matrix->edges[adj_matrix->num_edges].to = to;
    adj_matrix->edges[adj_matrix->num_edges].length = length;

    /* Find information about the points from and to from the hash table    */
    int from_index = hashtable_lookup(adj_matrix->point_hash, from);
    int to_index = hashtable_lookup(adj_matrix->point_hash, to);
    adj_matrix->adj[from_index][to_index] = length;
    adj_matrix->adj[to_index][from_index] = length;

    adj_matrix->num_edges++;
}