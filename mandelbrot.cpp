#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

char * addExtension(const char *, const char *);
double mapToRe(int, int, double, double);
double mapToIm(int, int, double, double);
int getMandelbrotN(double, double, int);

int main(int argc, char ** argv) {

    // Make sure argv is present
    if (argc < 3) {
        cout << "Usage: ./mandelbrot [inputFileName] [outputFileName]" << endl;
        return 1;
    }

    // Open input file
    char * inputFileName = addExtension(argv[1], ".txt");
    fstream inputFile(inputFileName, ios::in);
    int imageWidth, imageHeight, maxN;
    double minRe, maxRe, minIm, maxIm;

    if (!inputFile) {
        cout << "Error: Could not open file." << endl;
        return 1;
    }

    // Get input values from the file
    inputFile >> imageWidth >> imageHeight >> maxN;
    inputFile >> minRe >> maxRe >> minIm >> maxIm;
    inputFile.close();

    // Open output file
    char * outputFileName = addExtension(argv[2], ".ppm");
    fstream outputFile(outputFileName, ios::out);

    // Begin PPM header
    outputFile << "P3" << endl;
    outputFile << imageWidth << " " << imageHeight << endl;
    outputFile << "256" << endl;

    // For every pixel:
    for (int y = 0; y < imageHeight; y++) {
        for (int x = 0; x < imageWidth; x++) {
            // Map to z = x + iy corrsponding to pixel
            double zRe = mapToRe(x, imageWidth, minRe, maxRe);
            double zIm = mapToIm(y, imageHeight, minIm, maxIm);
            // Get number of Mandelbrot iterations
            int iterations = getMandelbrotN(zRe, zIm, maxN);
            // Get RGB value based on iterations
            int r = iterations % 256;
            int g = iterations % 256;
            int b = iterations % 256;
            // Output to image
            outputFile << r << " " << g << " " << b << " ";
        }
        outputFile << endl;
    }

    // Close output file
    outputFile.close();

    return 0;
}

char * addExtension(const char * name, const char * ext) {
    char * output = strdup(name);
    strcat(output, ext);
    return output;
}

double mapToRe(int x, int imageWidth, double minRe, double maxRe) {
    return (x*(maxRe - minRe))/imageWidth + minRe;
}

double mapToIm(int y, int imageHeight, double minIm, double maxIm) {
    return (y*(maxIm - minIm))/imageHeight + minIm;
}

int getMandelbrotN(double zRe, double zIm, int maxN) {
    int i = 0;
    double cRe = 0.0, cIm = 0.0;
    while ((i++ < maxN) && (cRe * cRe + cIm * cIm < 4)) {
        double temp = cRe * cRe - cIm * cIm + zRe;
        cIm = 2 * cRe * cIm + zIm;
        cRe = temp;
    }
    return i;
}

