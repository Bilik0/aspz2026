#include <stdio.h>
#include <stdlib.h>
#include "erf_module.h"

void print_fixed_intervals() {
    printf("\n--- Standard Confidence Intervals (Normal Distribution) ---\n");
    double z_vals[] = {1.645, 1.960, 2.576}; 
    double target_percents[] = {90.0, 95.0, 99.0};

    for (int i = 0; i < 3; i++) {
        double prob = calculate_normal_probability(z_vals[i]);
        printf("Z = %.3f -> Calculated CI: %.4f%% (Target: %.0f%%)\n", 
               z_vals[i], prob * 100.0, target_percents[i]);
    }
    printf("---------------------------------------------------------\n");
}

int main(void) {
    int choice;
    double value;
    int df;

    print_fixed_intervals();

    printf("\nSelect distribution:\n");
    printf("1. Normal Distribution (Z-score)\n");
    printf("2. Student's t-distribution (t-score)\n");
    printf("Enter choice: ");
    
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid input for menu!\n");
        return 1;
    }

    if (choice == 1) {
        printf("Enter Z-score for custom interval: ");
        if (scanf("%lf", &value) != 1) {
            fprintf(stderr, "Invalid input!\n");
            return 1;
        }

        double prob = calculate_normal_probability(value);
        
        if (is_valid_result(prob)) {
            printf("The probability (CI) for Z=%.4f is: %.4f (%.2f%%)\n", 
                   value, prob, prob * 100.0);
        } else {
            fprintf(stderr, "Calculation error occurred.\n");
        }

    } else if (choice == 2) {
        printf("Enter t-score: ");
        if (scanf("%lf", &value) != 1) return 1;
        
        printf("Enter degrees of freedom (df): ");
        if (scanf("%d", &df) != 1) return 1;

        double prob = calculate_student_probability(value, df);
        
        if (is_valid_result(prob)) {
            printf("The probability for t=%.4f with df=%d is: %.4f (%.2f%%)\n", 
                   value, df, prob, prob * 100.0);
        } else {
            fprintf(stderr, "Calculation error.\n");
        }
    } else {
        printf("Unknown choice.\n");
    }

    return 0;
}
