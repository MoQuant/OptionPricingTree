#include <iostream>
#include <math.h>
#include <algorithm>
#include <string>

// BINOMIAL TREE OPTION PRICING ALGORITHM

double rn(double x){
    return round(x*100)/100;
}

int main()
{
    // Initial Inputs
    double S = 100;
    double K = 105;
    double r = 0.05;
    double v = 0.20;
    double T = 30.0/365.0;
    int nodes = 4;
    double dt = T / (double) nodes;
    std::string opType = "call";

    // Formulas
    double Up = exp(v*sqrt(dt));
    double Down = exp(-v*sqrt(dt));

    double P_up = (exp(r*dt) - Down)/(Up - Down);
    double P_down = 1.0 - P_up;

    int Row = 4*nodes + 2;
    int Col = nodes + 1;

    int split = Row / 2 - 1;

    // Define array which holds tree
    double ** X = new double*[Row];
    for(int i = 0; i < Row; ++i){
        X[i] = new double[Col];
    }


    // Feed Forward Tree Calculations
    int upx = 2;
    int cs = 0;

    while(cs <= Col){
        X[split][cs] = S;
        for(int i = cs + 1; i < Col; ++i){
            X[split - (i - cs)*upx][i] = X[split - (i - cs - 1)*upx][i - 1]*Up;
            X[split + (i - cs)*upx][i] = X[split + (i - cs - 1)*upx][i - 1]*Down;
        }
        cs += 2;
    }

    // Payoff Calculations
    int b = 0;

    while(b <= Row){
        if(opType == "call"){
            X[b + 1][Col - 1] = std::max(X[b][Col - 1] - K, 0.0);
        } else {
            X[b + 1][Col - 1] = std::max(K - X[b][Col - 1], 0.0);
        }
        b += 4;
    }

    // Discount
    int cx = 0;
    int ls = 0;
    int osx = 0;

    for(int i = 1; i < Col; ++i){
        cx = Col - (i + 1);
        ls = 4 + osx;
        while(ls < Row - osx){
            if(opType == "call"){
                X[ls - 1][cx] = std::max(exp(-r*dt)*(P_up*X[ls - 3][cx + 1] + P_down*X[ls + 1][cx + 1]), 0.0);
            } else {
                X[ls - 1][cx] = std::max(exp(-r*dt)*(P_down*X[ls - 3][cx + 1] + P_up*X[ls + 1][cx + 1]), 0.0);
            }
            ls += 4;
        }
        osx += 2;
    }

    // Print option tree
    for(int i = 0; i < Row; ++i){
        for(int j = 0; j < Col; ++j){
            if(i == 0){
                std::cout << "\t";
            } else {
                std::cout << rn(X[i][j]) << "\t";
            }
        }
        std::cout << std::endl;
    }


    return 0;
}