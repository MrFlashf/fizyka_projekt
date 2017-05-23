#include <iostream>
#include <random>
//#include <stdlib.h>
//#include <time.h>
//#include <cstdlib>
#include <tgmath.h>
#include<graphics.h>

using namespace std;
const int SIZE          = 50;
const int START_X       = 20;
const int START_Y       = 20;
const int PIXEL_SIZE    = 5;

void randomizeArray(double array[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            double r = rand() % 2;
            array[i][j] = r == 0 ? (double)-1 : r;
        }
    }
}

class Model {
private:
    int steps;
    int stepsDone = 0;
    double array[SIZE][SIZE];
    double temperature;
public:
    Model(int steps, double temperature) {
        srand(time(0));
        this->steps = steps;
        this->temperature = temperature;
        randomizeArray(array);
        run();
    };

    void invert(int x, int y) {
        array[y][x] *= -1.0;
    }

    void touch(int x,int y) {

        double neighbours[4] = {
            array[y][x + 1 == SIZE ? 0 : x + 1], //right
            array[y + 1 == SIZE ? 0 : y + 1][x], //bottom
            array[y][x - 1 == -1 ? SIZE-1 : x - 1],//left
            array[y - 1 == -1 ? SIZE-1 : y - 1][x] //top
        };

        double state = array[y][x];

        double neighboursSum = 0;
        for (int i = 0; i < 4; i++) {
            neighboursSum += neighbours[i];
        }

        double ek = state * neighboursSum;
        double ep = -1 * ek;

        double deltaE   = ek - ep;
        double t        = -1 * (deltaE / temperature);
        double exp1     = exp(t);
        double r        = ((double)rand() / (double)RAND_MAX);

        if ((deltaE <= 0) || r <= exp1) {
            invert(x,y);
        }
    }

    void draw() {
        int start = 100;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                array[i][j] == -1 ? setcolor(15) : setcolor(3);
                bar(START_X+j*PIXEL_SIZE,START_Y+i*PIXEL_SIZE,START_X+(j+1)*PIXEL_SIZE,START_Y+(i+1)*PIXEL_SIZE);
            }
        }
    }

    void mcStep() {
        for (int i = 0; i < SIZE*SIZE; i++) {
            int x = rand() % SIZE;
            int y = rand() % SIZE;
            touch(x,y);
        }
    }

    bool sum() {
        double m = 0.0;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
               m += array[i][j];
            }
        }
        m = m / (SIZE*SIZE);
        m < 0 ? m = -1*m : m;
        cout << m << endl;
        return m == 1;
    }

    void run() {
        for (int i = 0; i < steps; i++) {
            mcStep();
            stepsDone++;
            if (i % 100==0)
                draw();
            if (sum()) {
                cout << stepsDone;
                return;
            }
        }
    }
};

int main(int argc, char** argv) {
    int gd = DETECT,gm;
    initgraph(&gd,&gm,NULL);
    srand(time(0));

    int mcsteps = argc > 2 ? stoi(argv[1]) : 50000;
    int temp    = argc > 2 ? stoi(argv[2]) : 1;
//    cout << mcsteps << ", " << temp << endl;
    new Model(mcsteps, temp);
//    lineto(100,200);
//    new Model(1, 1);
//    delay(5000);
//    closegraph();
    return 0;
//g++ -o program main.cpp -lgraph -std=gnu++11

}



