#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*holds value of current coodinate and pointer to the next one*/
struct coordinate {
    double value;
    struct coordinate* next;
};

/*hold ponters tothe next datapoint and */
struct datapoint {
    struct datapoint* next;
    struct coordinate* pointer;
    int cluster;
};

typedef struct coordinate coordinate;
typedef struct datapoint datapoint;

int k;
int num_of_points;
int max_iter;
int dimention;
double **centroids;
double **new_centroids;
const double EPSILON = 0.001;

/*general error message*/
void general_error() {
    printf("An Error Has Occured\n");
    exit(1);
}

datapoint* readText() {
    datapoint* head_point;
    datapoint* curr_point;
    datapoint* prev_point;
    coordinate* head_cord;
    coordinate* curr_cord;
    double curr_value;
    char is_char;
    num_of_points = 0;
    dimention = 0;
    /*allocate memory for the first coordinate*/
    head_cord = malloc(sizeof(coordinate));
    if (head_cord == NULL) {
        general_error();
    }
    curr_cord = head_cord;
    curr_cord->next = NULL;
    /*allocate memory for the first datapoint*/
    head_point = malloc(sizeof(datapoint));
    if (head_point == NULL) {
        free(head_cord);
        general_error();
    }
    curr_point = head_point;
    curr_point->next = NULL;
    /*reat the input and creating linked lists that represent the datapoints and their coordinats*/
    while (scanf("%lf%c", &curr_value, &is_char) == 2) {
        /*in case we get to the end of the line - finished the current datapoint*/
        if (is_char == '\n') {
            num_of_points++;
            curr_cord->value = curr_value;
            curr_point->pointer = head_cord;
            head_cord = malloc(sizeof(coordinate));
            if (head_cord == NULL) {
                general_error();
            }
            prev_point = curr_point;
            curr_point = curr_point->next;
            curr_point->next = NULL;
            head_point = malloc(sizeof(datapoint));
            if (head_point == NULL) {
                free(head_cord); /*TODO memory free*/
                general_error();
            }
            curr_point = head_point;
            curr_point->next = NULL;
            continue;
        }
        /*new coordinate of the datapoint*/
        curr_cord->value = curr_value;
        curr_cord->next = malloc(sizeof(coordinate));
        if (head_cord == NULL) {
            general_error();   /*TODO memory free*/
        }
        curr_cord = curr_cord->next;
        curr_cord->next = NULL;
        dimention++;
    }
    prev_point->next = NULL;
    if (k <= 1 || k >= num_of_points)
        {
            printf("Invalid number of clusters!\n");
            exit(1);
        }
    dimention = dimention / num_of_points;
    return head_point;
}

/*creating 2 2D matrixes for the centroids, putting the first k datapionts in the centroid matrix*/
void initilize_original_centroids (datapoint* point_head) {
    int i;
    int j;
    coordinate* curr_coordinate = point_head->pointer;
    /*allocate memory for the centriods matrix*/
    centroids = (double**)malloc(num_of_points * sizeof(double*));
    if (centroids == NULL) {
        /*TODO memrory free*/
        general_error();
    }
    new_centroids = (double**)malloc(num_of_points * sizeof(double*));
    if (new_centroids == NULL) {
        /*TODO memrory free*/
        general_error();
    }
    /*allocate memory for each row in the centroids matrix (each datapoint)*/
    for (i = 0; i < k; i++) {
        centroids[i] = (double*)malloc(dimention * sizeof(double));
        if (centroids[i] == NULL) {
            /*TODO memrory free*/
            general_error();
        }
        new_centroids[i] = (double*)malloc(dimention * sizeof(double));
        if (new_centroids[i] == NULL) {
            /*TODO memrory free*/
            general_error();
        }
    }
    for (i = 0; i < k; i++) {
        for (j = 0; j < dimention; j++) {
            centroids[i][j] = curr_coordinate->value;
        }
        point_head = point_head->next;
        curr_coordinate = point_head->pointer;
    }
}

/*calculating eclidean distance between datapoint p1 and array of doubles p2*/
double distance(datapoint* p1, double* p2) {
    double sum = 0.0;
    int i = 0;
    coordinate* value1 = p1->pointer;
    while (value1 != NULL) {
        sum += (value1->value - p2[i]) * (value1->value - p2[i]);
        i++;
        value1 = value1->next;
    }
    return sqrt(sum);
}

/*calculating eclidean distance between array of doubles p1 and array of doubles p2*/
double distance_doubles(double* p1, double* p2) {
    double sum = 0.0;
    int i;
    for (i = 0; i < dimention; i++) {
        sum += (p1[i] - p2[i]) * (p1[i] - p2[i]);
    }
    return sqrt(sum);
}

/*find the closest centroid for datapoint and assiging the cluster*/
void closest_cluster(datapoint* point_head) {
    double min_distance = distance(point_head, centroids[0]);
    int min_index = 0;
    int i;
    for (i = 1; i < k; i++){
        double curr_distance = distance(point_head, centroids[i]);
        if (curr_distance < min_distance) {
            min_distance = curr_distance;
            min_index = i;
        }
    }
    point_head->cluster = min_index;
}

/*updatde the new centroids*/
void update_centroid(datapoint* point_head) {
    int i;
    int j; 
    int point_counter;
    datapoint* curr_point = point_head;
    coordinate* curr_coordinate;
    double* sums;
    sums = (double*)malloc(dimention * sizeof(double*));
    if (sums == NULL) {
        /*TODO memrory free*/
        general_error();
    }
    for (i = 0; i < k; i++) {
        while (curr_point != NULL) {
        if (curr_point->cluster == i) {
            curr_coordinate = curr_point->pointer;
            point_counter++;
            for (j = 0; j < dimention; j++) {
                sums[j] += curr_coordinate->value;
                curr_coordinate = curr_coordinate->next;
            }
        }
        curr_point = curr_point->next;
        }
        for (j = 0; j < dimention; j++) {
            new_centroids[i][j] = sums[j] / point_counter;
        }
        curr_point = point_head;
        point_counter = 0;
    }
    free(sums);
}

/*calculating the distance betwwen the old centroisd and the new*/
int delta_centroid() {
    int i;
    for (i = 0; i < k; i++) {

        if (distance_doubles(centroids[i], new_centroids[i]) > EPSILON) {
            return 0;
        }
    }
    return 1;
}

/*printing the outcome*/
void print_result()
{
    int i;
    int j;
    for (i = 0; i < k; i++) {
        for (j = 0; j < dimention; j++) {
            if (j == dimention - 1) {
                printf("%.4f\n", centroids[i][j]);
            }
            else {
                printf("%.4f,", centroids[i][j]);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    int num_iteration = 0;
    datapoint* head;
    datapoint* curr_point;
    if (argc < 2 || argc > 3) {
        general_error();
    }
    k = atoi(argv[1]);     /*האם צריך לבדוק שקיי הוא אכן אינט או ושונה מאפס*/
    /*if there is no argument passed for max number of iterations*/
    if (argc == 2) {     
        max_iter = 200;
    }
    if (argc == 3) {
        max_iter = atoi(argv[1]);
    }
    head = readText();
    curr_point = head;
    initilize_original_centroids(head);
    while (num_iteration < max_iter) {
        while (curr_point->next != NULL) {
            closest_cluster(curr_point);
            curr_point = curr_point->next;
        }
        update_centroid(head);
        if (delta_centroid() < EPSILON) {
            break;
        }
        centroids = new_centroids;
    }
    print_result();
    return 0;
}
