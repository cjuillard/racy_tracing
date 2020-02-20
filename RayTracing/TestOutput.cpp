#include <iostream>
#include <fstream>
using namespace std;

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "InOneWeekend/vec3.h"

int main() {
    ofstream myfile;
    myfile.open("example.ppm");
    
    int nx = 200;
    int ny = 100;
    int comp = 3;   // number of components
    uint8_t* pixels = new uint8_t[nx * ny * comp];
    int index = 0;
    myfile << "P3\n" << nx << " " << ny << "\n255\n";
    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            vec3 col(float(i) / float(nx), float(j) / float(ny), 0.2);
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);
            myfile << ir << " " << ig << " " << ib << "\n";

            pixels[index++] = ir;
            pixels[index++] = ig;
            pixels[index++] = ib;
        }
    }

    int stride = nx * 3;
    stbi_write_png("stbpng.png", nx, ny, comp, pixels, stride);

    myfile.close();
}