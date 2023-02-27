#include <cstring>
#include <string>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

#define bit(x) ((unsigned char)(1<<(x)))

// implementation of a search point
// n: number of bits, should be multiples of 8
// onemax: number of one-bits, '-1' indicates not evaluated yet
// fitness: fitness of the last evaluation, '-1' indicates not evaluated yet

class Point{
public:
    int n;
    int onemax;
    double fitness;
    unsigned char * bits;
    
    Point(int numBits){
        if(numBits%8 != 0)
            cout << "number of bits must be multiples of 8" << endl;
        n = numBits;
        onemax = -1;
        fitness = -1;
        bits = new unsigned char[n>>3];
        for(int i=0; i<(n>>3); ++i){
            bits[i] = (unsigned char)(rand() % 256);
        }
    }
    
    ~Point(){
        if(bits != NULL)
            delete bits;
    }
    
    // access the (index)-th bit
    int at(int index){
        int ind1 = index/8;
        int ind2 = index%8;
        return (bits[ind1] & bit(ind2)) > 0;
    }
    
    // set all bits to zero
    void setzero(){
        for(int i=0; i<(n>>3); ++i){
            bits[i] = 0;
        }
    }
    
    // set the first (numOnes) many bits to one
    void setones(int numOnes){
        if(numOnes > n)
            cout << "not enought number of bits" << endl;
        for(int i=0; i<(numOnes>>3); ++i)
            bits[i] = 0xff;
        int remaining = numOnes % 8;
        unsigned char mask = 0;
        while(remaining --){
            mask = (mask << 1);
            mask = (mask & 0x01);
        }
        bits[numOnes>>3] = mask;
    }
    
    // set (numOnes) many bits to one, with index of onebits specified
    void setones(int numOnes, vector<int>& index){
        for(int i=0; i<numOnes; ++i){
            int ind1 = index[i]/8;
            int ind2 = index[i]%8;
            bits[ind1] |= bit(ind2);
        }
    }
    
    // copy from another search point
    void copy(Point& p){
        if(n != p.n)
            cout << "invalid copying" << endl;
        std::copy(p.bits, p.bits+(n>>3), bits);
        onemax = p.onemax;
        fitness = p.fitness;
    }
    
    // mutate from another search point
    void mutate(Point& p, default_random_engine& gen, binomial_distribution<int>& dis){
        
        if(n != p.n)
            cout << "invalid mutation" << endl;
        
        int num = dis(gen);
        if(num == 0){ // duplication
            copy(p);
            return;
        }
        
        std::copy(p.bits, p.bits+(n>>3), bits);
        map<int, bool> mutated;
        
        while(num){
            int Ind = rand() % (n >> 3);
            int ind = rand() % 8;
            if(mutated[Ind*8+ind])
                continue;
            unsigned char x = bit(ind);
            bits[Ind] ^= x;
            mutated[Ind*8+ind] = true;
            -- num;
        }
        onemax = -1;
        fitness = -1;
    }
    
    // test whether onemax was evaluated before
    bool onebitsAvailable(){
        return (onemax > -0.9);
    }
    
    // test whether any monotone function was evaluated before
    // when the point needs to be evaluated for multiple functions
    // set (multiFunc) to ture 
    bool fitnessAvailable(bool multiFunc=false){
        return (fitness > -0.9) && (!multiFunc);
    }
};

// the function of onemax
// n: number of bits
// dynamic: whether the function is dynamic
class Bitcount{
public:
    int n;
    int count[256] = {
        0, 1, 1, 2, 1, 2, 2, 3, 
        1, 2, 2, 3, 2, 3, 3, 4
    }; // array of precomputed bits count
    const bool dynamic = false;
    
    Bitcount(int numBits=10000){
        n = numBits;
        for(int i=16; i<256; ++i){
            count[i] = count[i&0xf] + count[i>>4];
        }
    }
    
    // evaluate onemax on p, and update p.onemax
    int evaluate(Point& p){
        if(p.onebitsAvailable() == false){
            p.onemax = 0;
            for(int i=0; i<(p.n>>3); ++i)
                p.onemax += count[p.bits[i]];           
        }
        return p.onemax;
    }
    
    // return whether p1 has strictly less one-bits than p2
    bool smaller(Point& p1, Point& p2){
        return (evaluate(p1) < evaluate(p2));
    }
    
    // name of the function
    string name(){
        string function = string("Onemax-");
        return function + to_string(n);
    }
    
    // only for dynamic functions
    void update(){}
};

// implementation of binary value
class Binval{
public:
    int n;
    const bool dynamic = false;

    Binval(int numBits){
        n = numBits;
    }
    
    // evaluate binary value on p, and update p.fitness
    // note that when n>300, the value can hardly be fit to a double,
    // so p.fitness will be set to -0.5 in this case
    double evaluate(Point& p, bool multiFunc=false){
        if(p.fitnessAvailable(multiFunc) == false){
            if(n > 300){
                p.fitness = -0.5;
            }
            else{
                p.fitness = 0;
                for(int i=0; i<(n>>3); ++i){
                    p.fitness = p.fitness*256 + p.bits[i];
                }
            }
        }
        return p.fitness;
    }

    bool smaller(Point& p1, Point& p2){
        for(int i=0; i<(n>>3); ++i){
            if(p1.bits[i] < p2.bits[i])
                return true;
            else if(p1.bits[i] > p2.bits[i])
                return false;
        }
        return false;
    }

    string name(){
        string function = string("Binval-");
        return function + to_string(n);
    }
    
    void update(){}
};

// implementation of dynamic binval
// order: permuatation of the bits, earlier ones are more important
class DynamicBinval{
public:
    int n;
    vector<int> * order;
    const bool dynamic = true;

    DynamicBinval(int numBits){
        n = numBits;
        order = new vector<int>(n);
        for(int i=0; i<n; ++i)
            (*order)[i] = i;
    }

    ~DynamicBinval(){
        delete order;
    }

    bool smaller(Point& p1, Point& p2){
        for(int i=0; i<n; ++i){
            int bit1 = p1.at((*order)[i]);
            int bit2 = p2.at((*order)[i]);
            if(bit1 < bit2)
                return true;
            else if(bit1 > bit2)
                return false;
        }
        return false;
    }
    
    // similar to binary value, no evaluation for n>300
    double evaluate(Point& p){
        if(n > 300){
            p.fitness = -0.5;
        }
        else{
            p.fitness = 0;
            for(int i=0; i<n; ++i)
                p.fitness = p.fitness*2 + p.at((*order)[i]);
            for(int i=0; i<(n>>3); ++i){
                p.fitness = p.fitness*256 + p.bits[i];
            }
        }
        return p.fitness;
    }
    
    // update order
    void update(){
        random_shuffle(order->begin(), order->end());
    }

    string name(){
        string function = string("DynamicBinval-");
        return function + to_string(n);
    }
};

// implementation of the binary function
// bitcount: helps to count bits here
class Binary{
public:
    int n;
    Bitcount * bitcount;
    const bool dynamic = false;

    Binary(int numBits){
        n = numBits;
        bitcount = new Bitcount();
    }

    ~Binary(){
        delete bitcount;
    }

    double evaluate(Point& p, bool multiFunc=false){
        if(p.fitnessAvailable(multiFunc) == false){
            int cnt = 0;
            for(int i=0; i<(n>>4); ++i)
                cnt += bitcount->count[p.bits[i]];
            p.fitness = cnt * n;
            for(int i=(n>>4); i<(n>>3); ++i)
                p.fitness += bitcount->count[p.bits[i]];
        }
        return p.fitness;
    }

    bool smaller(Point& p1, Point& p2, bool multiFunc=false){
        return evaluate(p1, multiFunc) < evaluate(p2, multiFunc);
    }

    string name(){
        string function = string("Binary-");
        return function + to_string(n);
    }
    
    void update(){}
};

// a helper function for hot topic
void subset(int k, int n, int *a){
    int cur = 0;
    int left = n;
    while(k --){
        int r = rand()%left + cur;
        int tmp = a[cur];
        a[cur] = a[r];
        a[r] = tmp;
        ++ cur;
        -- left;
    }
}

// implementation of hot topic
// l: L
// a: alpha
// b: beta
// e: eps
// sizeA: size of set A
// sizeB: size of set B
class Hotopic
{
public:
    int n, l;
    double a, b, e;
    int sizeA, sizeB;
    unsigned char **A;
    unsigned char **B;
    const bool dynamic = false;
    
    int * bits; // array for subset sampling
    int count[256] = {
        0, 1, 1, 2, 1, 2, 2, 3, 
        1, 2, 2, 3, 2, 3, 3, 4
    }; // array of precomputed bits count
    
    Hotopic(int numBits, int numLevel, double alpha, double beta, double eps){
        n = numBits;
        l = numLevel;
        a = alpha;
        b = beta;
        e = eps;
        sizeA = n * alpha;
        sizeB = n * beta;
        
        A = new unsigned char * [numLevel];
        B = new unsigned char * [numLevel];
        for(int i=0; i<numLevel; ++i){
            A[i] = new unsigned char[numBits>>3];
            memset(A[i], 0, (n>>3));
            B[i] = new unsigned char[numBits>>3];
            memset(B[i], 0, (n>>3));
        }
        
        bits = new int[numBits];
        
        for(int i=0; i<n; ++i){
            bits[i] = i;
        }
        for(int i=0; i<l; ++i){
            // sample Ai
            subset(sizeA, n, bits);
            for(int j=0; j<sizeA; ++j){
                int x = (bits[j] >> 3);
                unsigned char y = bit(bits[j] & 0x7);
                A[i][x] |= y;
            }
            // sample Bi
            subset(sizeB, sizeA, bits);
            for(int j=0; j<sizeB; ++j){
                int x = (bits[j] >> 3);
                unsigned char y = bit(bits[j] & 0x7);
                B[i][x] |= y;
            }
        }
        
        for(int i=16; i<256; ++i){
            count[i] = count[i&0xf] + count[i>>4];
        }
    };
    
    ~Hotopic(){
        for(int i=0; i<l; ++i){
            delete A[i];
            delete B[i];
        }
        delete A;
        delete B;
        delete bits;
    }
    
    double evaluate(Point& p, bool multiFunc=false){
        if(p.fitnessAvailable(multiFunc) == false){
            // calculate the onemax
            if(p.onebitsAvailable() == false){
                p.onemax = 0;
                for(int i=0; i<(n>>3); ++i){
                    p.onemax += count[p.bits[i]];
                }
            }
            
            // calculate the level
            int level = -1;
            for(int i=l-1; i>=0; --i){
                int cnt = 0;
                for(int j=0; j<(n>>3); ++j){
                    cnt += count[B[i][j] & p.bits[j]];
                }
                if(cnt >= (1.0-e)*sizeB){
                    level = i;
                    break;
                }
            }
            level += 1;
            
            // calculate numAlpo
            int numAlpo = 0;
            if(level != l){
                for(int i=0; i<(n>>3); ++i)
                    numAlpo += count[A[level][i] & p.bits[i]];
            }
            
            p.fitness = 1.0*n*n*level + n*numAlpo + (p.onemax-numAlpo);
        }
        return p.fitness;
    };

    bool smaller(Point& p1, Point& p2, bool multiFunc=false){
        return evaluate(p1, multiFunc) < evaluate(p2, multiFunc);
    }

    string name(){
        string function = string("Hottopic-");
        return function + to_string(n);
    }
    
    void update(){}
};
