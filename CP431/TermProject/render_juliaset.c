
#include <stdio.h>
#include <GL/glut.h> // Include OpenGL for Mac <GLUT/glut.h>
#include "lib/lodepng.h"
#include <time.h>

#define WIDTH 10000
#define HEIGHT 10000
#define MAX_ITERATIONS 100.0
#define NUM_COLORS 10
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define INPUT_FILE "./output/julia_set.txt"
#define OUTPUT_PNG_FILE "./output/julia_set.png"

// Color palette for Julia Set
float COLORS[NUM_COLORS][3] = {
    {0.0, 0.0, 0.0},
    {0.85, 0.85, 0.85},
    {1.0, 0.4, 0.7},
    {0.5, 0.0, 0.5},
    {0.0, 0.0, 1.0},
    {0.0, 0.75, 1.0},
    {0.0, 1.0, 0.0},
    {1.0, 1.0, 0.0},
    {1.0, 0.65, 0.0},
    {1.0, 0.0, 0.0},
};

unsigned char* julia_data;
float* calc_julia_data;

void save_png(char *file) {
    printf("Saving Julia set to PNG file...\n");
    // Start timer
    clock_t time = clock();

    size_t fileSize = (size_t) WIDTH * (size_t) HEIGHT * (size_t) 3;
    unsigned char* data = (unsigned char*) malloc(fileSize * sizeof(unsigned char));
    if (data == NULL) {
        printf("Error allocating memory to save file.\n");
        exit(1);
    }

    // Convert data to RGB format, keeping it as unsigned char
    for (size_t i = 0; i < WIDTH; i++) {
        for (size_t j = 0; j < HEIGHT; j++) {
            float intensity = ((float) julia_data[j * (size_t) WIDTH + (WIDTH - i)]) / MAX_ITERATIONS; // Adjust for proper coloring
            float* colors = COLORS[(int) (intensity * NUM_COLORS)];

            data[(i * HEIGHT + j) * 3] = (unsigned char) (colors[0] * intensity * 255);
            data[(i * HEIGHT + j) * 3 + 1] = (unsigned char) (colors[1] * intensity * 255);
            data[(i * HEIGHT + j) * 3 + 2] = (unsigned char) (colors[2] * intensity * 255);
        }
    }

    // Save data to PNG file
    lodepng_encode24_file(file, data, WIDTH, HEIGHT);
    free(data);

    // End timer
    time = clock() - time;
    printf("Saved Julia set to PNG file. Time taken: %f seconds.\n", ((float) time) / CLOCKS_PER_SEC);
}

void load_julia_set(char* file_path) {
    printf("Loading Julia set from file...\n");
    // Start timer
    clock_t time = clock();

    FILE *fp = fopen(file_path, "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Read data from file
    size_t fileSize = (size_t) WIDTH * (size_t) HEIGHT;
    julia_data = (unsigned char*) malloc(fileSize * sizeof(unsigned char));
    if (julia_data == NULL) {
        printf("Error allocating memory for Julia Set data.\n");
        exit(1);
    }
    fread(julia_data, sizeof(unsigned char), fileSize, fp);

    if (fclose(fp) != 0) {
        printf("Error closing file.\n");
    }

    // End timer
    time = clock() - time;
    printf("Loaded Julia set from file. Taking up %f MB in memory. ", (float) fileSize / 1024 / 1024);
    printf("Time taken: %f seconds.\n", ((float) time) / CLOCKS_PER_SEC);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    // Start timer
    clock_t time = clock();

    for (size_t i = 0; i < WIDTH; i++) {
        for (size_t j = 0; j < HEIGHT; j++) {
            // Scale iterations to determine color intensity
            float intensity = ((float) julia_data[i * (size_t) HEIGHT + j]) / MAX_ITERATIONS;
            // Map intensity to color
            float* colors = COLORS[(int) (intensity * NUM_COLORS)];

            // Draw pixel
            glBegin(GL_POINTS);
            glColor3f(
                colors[0] * intensity,
                colors[1] * intensity,
                colors[2] * intensity
            );
            glVertex2i(i, j);
            glEnd();
        }
    }

    // End timer
    time = clock() - time;
    printf("Rendered Julia set. Time taken: %f seconds.\n", ((float) time) / CLOCKS_PER_SEC);

    glFlush();
}

void onClose() {
    // Save image to file
    save_png(OUTPUT_PNG_FILE);
    free(julia_data);
}

int main(int argc, char** argv) {
    // Load Julia set from file
    load_julia_set(INPUT_FILE);

    glutInit(&argc, argv);

    // Initialize OpenGL context
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Julia Set");

    // Set background color
    glClearColor(0, 0, 0, 0.0);  // Set clear color to black
    glMatrixMode(GL_PROJECTION); // Set current matrix mode
    gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);

    glutCloseFunc(onClose);
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
