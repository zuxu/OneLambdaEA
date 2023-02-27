#include <ea.hpp>
#include <vector>
#include <algorithm>
#include <random>
#include <iostream>
#include <fstream>

using namespace std;

int n = 1000;
int step = 20;
float c = 1;
int numPoints = 1000;
int numTrials = 100;
int seed = 2022;


int main(){
    srand(seed);
    default_random_engine gen;
    binomial_distribution<int> dis(n, c/n);
    
    // Hottopic
    int L = 100;
    double alpha = 0.25;
    double beta = 0.05;
    double eps = 0.05;
    Hotopic hotopic(n, L, alpha, beta, eps);
    
    // Binary
    Binary binary(n);
    
    // Dynamic Binval
    DynamicBinval dbv(n);
    
    // Binval
    Binval binval(n);
    
    // Onemax
    Bitcount onemax(n);
        
    vector<int> index(n);
    for(int i=0; i<n; ++i)
        index[i] = i;
    
    ofstream output("improvement.csv");

    for(int i=step; i<n; i+=step){
        if(i%200 == 0)
            cout << i << endl;
        float count[5] = {0, 0, 0, 0, 0};
        for(int j=0; j<numPoints; ++j){
            Point parent(n), child(n);
            std::random_shuffle(index.begin(), index.end());
            parent.setzero();
            parent.setones(i, index);
            for(int k=0; k<numTrials; ++k){
                child.mutate(parent, gen, dis);
                if(onemax.smaller(parent, child))
                    count[0] += 1;
                if(binary.smaller(parent, child, true))
                    count[1] += 1;
                if(hotopic.smaller(parent, child, true))
                    count[2] += 1;
                if(binval.smaller(parent, child))
                    count[3] += 1;
                if(dbv.smaller(parent, child))
                    count[4] += 1;
            }
            dbv.update();
        }
        
        for(int k=0; k<5; ++k){
            count[k] /= (numPoints*numTrials);
            output << count[k] << ',';
            //cout << count[k] << '\t';
        }
        //cout << endl;
    }
    output.close();
    return 0;
}