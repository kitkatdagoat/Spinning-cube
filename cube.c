#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


float A, B ,C;
float cubeWidth = 10; 
int width = 160, height = 44;
float zBuffer [160 * 44];
char buffer [160 * 44];
int background = ' ';
int distance = 60;
float K1 = 40;

float incrementSpeed = 0.6;

float x, y, z;
float ooz;
int xp, yp;
int idx;



float coordX(int i, int j, int k){
    return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) + j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}

float coordY(int i, int j, int k){
    return j * cos(A) * cos(C) + k * sin(A) * cos(C) - j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) - i * cos(B) * sin(C);
}

float coordZ(int i, int j, int k){
    return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}

void calculateForSurface(float cubeX, float cubeY, float cubeZ, char ch){
    x = coordX(cubeX, cubeY, cubeZ);
    y = coordY(cubeX, cubeY, cubeZ);
    z = coordZ(cubeX, cubeY, cubeZ) + distance; //distance to adjust the size of object


    //the 2d coord of (x,y,z) is (x/z, y/z)
    ooz = 1/z; //Objects further away (larger z values) will appear smaller
    xp = (int)(width / 2 + K1 * ooz * x * 2); // without mutiple by 2 the object will be stretched vertically because the aspect ratio of termincal character
    yp = (int)(height / 2 + K1 * ooz * y);

    idx = xp + yp * width;
    if (idx >= 0 && idx < width * height) {
        if (ooz > zBuffer[idx]) {
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

int main(){
    printf("\x1b[2J");
    while(1){
        memset(buffer, background, width * height);
        memset(zBuffer, 0, width * height * 4);
        for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed){
            for (float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += incrementSpeed){
                calculateForSurface(cubeX, cubeY, cubeWidth, '@'); //back face
                calculateForSurface(cubeX, cubeY, -cubeWidth, '#'); //front face
                calculateForSurface(-cubeWidth, cubeY, cubeX, '$'); //left face
                calculateForSurface(cubeWidth, cubeY, cubeX, '.'); //right face
                calculateForSurface(cubeX, cubeWidth, cubeY, '*'); //bottom face
                calculateForSurface(cubeX, -cubeWidth, cubeY, '&'); //top face
            }
        }
        printf("\x1b[H");
        for (int k = 0; k < width * height; k++) {
            putchar(k % width ? buffer[k] : 10);
        }
        A += 0.05;
        B += 0.05;
        usleep(18000);
    }
    return 0;
}