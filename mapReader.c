#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashTable.h"
#include "mapAdjMatrix.h"
#include "mapReader.h"
#include "mapError.h"

/*  the main function in read map module */
int read_map(AdjacencyMatrix* adj_matrix)
{
    printf("\nWelcome to the Map Navigator! Here is an overview of the entire process:\n");
    printf("1. Choose your map\n");
    printf("2. Modify map properties (this modification will be added to the map file)\n");
    printf("3. Enter location information\n");
    printf("This will allow you to see a visualization of the map that meets your requirements!\n\n");

    while (1)
    {
        // Prompt the user to enter the map filename and read from standard input
        printf("Please enter the map filename: ");
        char* filename = (char*)malloc(100 * sizeof(char));
        if (fgets(filename, 100, stdin) == NULL)
        {
            // If reading fails, continue to prompt the user
            printf("Failed to read user input, please try again\n");
            free(filename);
            continue;
        }

        int len = strlen(filename);
        if (len == 100)
        {
            printf("File name too long! Retype!\n");
            free(filename);
            continue;
        }
        // 去掉末尾的回车符
        if (filename[len - 1] == '\n')
        {
            filename[len - 1] = '\0';
        }

        /* Read the data file and transform it into an adjacency matrix */
        int result = read_points(filename, adj_matrix);
        if (result == ERROR_BAD_FILE)
        {
            // If opening the file fails, prompt the user to re-enter the filename
            printf("Failed to open file! Please try again\n");
            free(filename);
            continue;
        }
        result = read_edges(filename, adj_matrix);
        free(filename);
        printf("\nMap read successfully!\n");
        return FUNCTION_SUCCESS;
    }
}


int read_points(char* filename, AdjacencyMatrix* adj_matrix)
{
    /* Define variables for parsing the data file*/
    char line[512];
    int count = 0;

    /* Open the data file */
    FILE* fp = fopen(filename, "r");
    if (fp == NULL)
    {
        return ERROR_BAD_FILE;
    }

    /* Read each line from the data file */
    while (fgets(line, sizeof(line), fp) != NULL)
    { // Read the contents of the data file line by line
        if (line[0] == '<' && line[1] == 'n' && line[2] == 'o' && line[3] == 'd' && line[4] == 'e')
        {
            /* If the line contains information about a point */
            long long id = -1;
            double lat = -1;
            double lon = -1;

            /// Use sscanf() to parse the line string
            if (sscanf(line, "<node id=%lld lat=%lf lon=%lf /node>", &id, &lat, &lon) != 3)
            {
                free_adjacency_matrix(adj_matrix);
                fclose(fp);
                fprintf(stderr, "Error: Failed to parse node data in %d line\n", count);
                exit(ERROR_DATA_READ_FAILED);
            }

            // Add the point to the adjacency matrix
            add_point_to_adjacency_matrix(adj_matrix, id, lat, lon);
        }
        count++;
        memset(line, 0, sizeof(line));
    }
    fclose(fp); // Close the data file
    return FUNCTION_SUCCESS;
}


int read_edges(char* filename, AdjacencyMatrix* adj_matrix)
{
    /* Define variables for parsing the data file */
    char line[512];
    int count = 0;

    /* Open the data file */
    FILE* fp = fopen(filename, "r");
    if (fp == NULL)
    {
        return ERROR_BAD_FILE;
    }

    while (fgets(line, sizeof(line), fp))
    { // Read the contents of the data file line by line
        if (line[0] == '<' && line[1] == 'l' && line[2] == 'i' && line[3] == 'n' && line[4] == 'k')
        {
            /* If the line contains information about an edge */
            long long link_id = -1;
            long long node_id_from = -1;
            long long node_id_to = -1;
            int way = -1;
            double length = -1;
            if (sscanf(line, "<link id=%lld node=%lld node=%lld way=%d length=%lf", &link_id, &node_id_from, &node_id_to, &way, &length) != 5)
            {
                fprintf(stderr, "Error: Failed to parse link data in %d\n", count);
                exit(ERROR_DATA_READ_FAILED);
            }
            add_edge_to_adjacency_matrix(adj_matrix, link_id, node_id_from, node_id_to, length); // 将该link添加到邻接表中
        }
        count++;
    }
    fclose(fp); // Close the data file
    return FUNCTION_SUCCESS;
}
