#include <math.h>
#include <stdio.h>
#include <errno.h>
#include "erf_module.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double calculate_normal_probability(double z) {
    errno = 0;
    
    double result = erf(z * sqrt(0.5));
    
    if (errno != 0) {
        perror("Error in erf calculation");
        return -1.0; 
    }
    return result;
}

double student_pdf(double x, int df) {
    double numerator = tgamma((df + 1.0) / 2.0);
    double denominator = sqrt(df * M_PI) * tgamma(df / 2.0);
    double base = 1.0 + (x * x) / df;
    double exponent = -(df + 1.0) / 2.0;
    
    return (numerator / denominator) * pow(base, exponent);
}

double calculate_student_probability(double t, int df) {
    if (t < 0) t = -t;
    if (df < 1) return 0.0;

    int steps = 10000;
    double step_size = t / steps;
    double area = 0.0;

    for (int i = 0; i < steps; i++) {
        double x1 = i * step_size;
        double x2 = (i + 1) * step_size;
        area += (student_pdf(x1, df) + student_pdf(x2, df)) / 2.0 * step_size;
    }

    return area * 2.0;
}

int is_valid_result(double result) {
    if (isnan(result) || isinf(result) || result < 0.0 || result > 1.0) {
        return 0;
    }
    return 1;
}