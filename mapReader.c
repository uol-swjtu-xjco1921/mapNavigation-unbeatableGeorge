#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashTable.h"
#include "mapAdjList.h"
#include "mapReader.h"

/*  the main function in read map module */
int read_map(AdjacencyList* adj_list)
{
    const char filename[] = "leeds.map";
    /* Read the data file and transform it into an adjacency list */
    read_points(filename, adj_list);

    read_edges(filename, adj_list);

    /* Print the adjacency list */
    print_adjacency_list(adj_list);

    /* Free the memory used by the adjacency list */
    free_adjacency_list(adj_list);

    return 0;
}

void read_points(const char* filename, AdjacencyList* adj_list)
{
    /* Define variables for parsing the data file*/
    char line[512];
    int count = 0;

    /* Open the data file */
    FILE* fp = fopen(filename, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Error: Failed to open data file\n");
        exit(1);
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

            if (line[strlen(line) - 1] != '\n') {
                printf("Line too long. Exiting...\n");
                exit(1);
            }

            /// Use sscanf() to parse the line string
            if (sscanf(line, "<node id=%lld lat=%lf lon=%lf /node>", &id, &lat, &lon) != 3)
            {
                free_adjacency_list(adj_list);
                fclose(fp);
                fprintf(stderr, "Error: Failed to parse node data in %d line\n", count);
                exit(1);
            }
            add_point_to_adjacency_list(adj_list, id, lat, lon);  // Add the point to the adjacency list
        }
        count++;
        memset(line, 0, sizeof(line));
    }
    fclose(fp); // Close the data file
}


void read_edges(const char* filename, AdjacencyList* adj_list)
{
    /* Define variables for parsing the data file */
    char line[512];
    int count = 0;

    /* Open the data file */
    FILE* ffp = fopen(filename, "r");
    if (ffp == NULL)
    {
        fprintf(stderr, "Error: Failed to open data file\n");
        exit(1);
    }

    while (fgets(line, sizeof(line), ffp))
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
                exit(1);
            }
            add_edge_to_adjacency_list(adj_list, link_id, node_id_from, node_id_to, length);
        }
        count++;
    }
    fclose(ffp); // Close the data file
}
