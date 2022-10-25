    /*Name: Idris Akala
      UID: 117000627
      Directory ID: iakala*/

#include <stdio.h>
#include <math.h>
#define ASSIGNMENT_MAX 50
double compute_score(int info[], int num_of_assignments, 
int penalty_points, int assignments_dropped);
double compute_mean(int info[], int penalty_points, int num_of_assignments);
double compute_standard_deviation(int info[], int penalty_points, 
int num_of_assignments, double mean);
int main() {
    int penalty_points, assignments_dropped, 
    num_of_assignments, assignment_info[ASSIGNMENT_MAX * 4] = {0}
    ,i, j, copy[ASSIGNMENT_MAX * 4] = {0};
    double score, mean, standard_dev;
    char stats;
    scanf("%d %d %c\n", &penalty_points, &assignments_dropped, &stats);
    scanf("%d\n", &num_of_assignments);
    for (i = 0;i < num_of_assignments * 4;i++) {
        scanf("%d, ", &assignment_info[i]);
    }
    /* Copy array for function computing*/
    for (i = 0;i < num_of_assignments * 4;i++) {
        copy[i] = assignment_info[i];
    }
    score = compute_score(copy, num_of_assignments, 
    penalty_points, assignments_dropped);
    if (score < 0) {
        return 0;
    }
    printf("Numeric Score: %5.4f\n", score);
    printf("Points Penalty Per Day Late: %d\n", penalty_points);
    printf("Number of Assignments Dropped: %d\n", assignments_dropped);
    printf("Values Provided:\nAssignment, Score, Weight, Days Late\n");
    for (i = 0;i <= num_of_assignments;i++) {
        for (j = 0;j < num_of_assignments * 4;j+=4) {
            if(i == assignment_info[j]) {
                printf("%d, %d, %d, %d\n", assignment_info[j], 
                assignment_info[j+1], 
                assignment_info[j+2], assignment_info[j+3]);
            }
        }
    }
    if (stats == 'Y' || stats == 'y') {
        mean = compute_mean(assignment_info, 
        penalty_points, num_of_assignments);
        standard_dev = compute_standard_deviation(assignment_info, 
        penalty_points, num_of_assignments, mean);
        printf("Mean: %5.4f, ", mean);
        printf("Standard Deviation: %5.4f\n", standard_dev);

    }
    return 0;
}
double compute_standard_deviation(int info[], int penalty_points, 
int num_of_assignments, double mean) {
    int i, penalty, length = num_of_assignments * 4;
    double calc, standard_deviation, variance;
    for (i = 0;i < length;i += 4) {
        penalty = info[i + 3] * penalty_points;
        calc += pow((mean - (info[i + 1] - penalty)), 2);
    }
    variance = calc / num_of_assignments;
    return (standard_deviation = sqrt(variance));
}
double compute_mean(int info[], int penalty_points, int num_of_assignments) {
    int i, penalty, length = num_of_assignments * 4;
    double calc = 0, mean = 0;
    for (i = 0;i < length;i+=4) {
        penalty = info[i + 3] * penalty_points;
        if ((info[i + 1] - penalty) < 0) {
            calc += 0;
        } else {
            calc += (info[i + 1] - penalty);
        }
    }
    return (mean = calc / num_of_assignments);
}
double compute_score(int info[], int num_of_assignments, 
int penalty_points, int assignments_dropped) {
    int i, j, assign_num, low, curr_low, weight = 0, penalty;
    double score = 0, calc = 0;
    for (j = 0;j < assignments_dropped;j++) {
        curr_low = info[1] * info[2];
        /*Process to drop assignments*/
        for(i = 0;i < num_of_assignments * 4;i += 4) {
            low = info[i+1] * info[i+2];
            if (low == 0) {
                continue;
            } else {
                if (low < curr_low) {
                assign_num = i;
                curr_low = low;
                } else if(low == curr_low && info[i] < info[assign_num]) {
                        assign_num = i;
                        curr_low = low;
                    }
                }
            }
        for(i = 0;i < 4;i++) {
            info[assign_num + i] = 0;
        }
    }
    /*Check for weight validity */
    for (i = 0;i < num_of_assignments * 4;i+=4) {
        weight += info[i+2];
    }
    if (weight > 100) {
        printf("ERROR: Invalid values provided\n");
        return -1;
    }
    /*Score computing */
    for(i = 0;i < num_of_assignments * 4;i += 4) {
        penalty = info[i + 3] * penalty_points;
        if (info[i + 1] - penalty < 0) {
            calc += 0 * info[i + 2];
        } else{
            calc += (info[i + 1] - penalty) * info[i + 2];
        }
    }
    score = calc / weight;
    return score;
}