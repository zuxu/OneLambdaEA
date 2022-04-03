#include <function.hpp>
#include <cmath>
#include <iostream>
#include <fstream>

using namespace std;

// implementation for the self-adjust (1,lambda)-EA
// lambda: number of offsprings
// c: mutation parameter
// s: success rate
// F: update rate
// T: function to optimize
// elitist: setting this to true leads to the self-adjust (1+lambda)-EA
template <class T>
class Saea{
public:
	float lambda;
	float c;
	float s;
	float F;   
	T * function;
	bool elitist;
    
    // elitist is set to aliveParent
	Saea(int offspringSize, float mutationParameter, float successRate, float updateRate, T& targetFunction, bool aliveParent=false){
		lambda = offspringSize;
		c = mutationParameter;
		s = successRate;
		F = updateRate;
		function = &targetFunction;
		elitist = aliveParent;
	}
    
    // fromzero: starting from a zero string
    // details: whether to collect details during the run
    // cnt: the array counts how many times the algorithm is at each fitness level
    // lbd: the number of evaluations at each fitness level
    // drift: the sum of the emprical drifts at each fitness level
    // return: (number of generations, number of evaluations)
	pair<int, double> run(int maxGeneration, int seed=0, bool fromzero=false, bool details=false, int * cnt=NULL, double * lbd=NULL, double * drift=NULL){
		char buf[256];
        // log of this run
        sprintf(buf, "n-%s-%.2lf-%.2lf-%.2lf-%.2lf-%d-%d.csv", function->name().c_str(), lambda, c, s, F, maxGeneration, seed);
        if(elitist == true)
            buf[0] = 'e';
        ofstream output(buf);
        
    	srand(seed);
    	default_random_engine gen;
    	binomial_distribution<int> dis(function->n, c/function->n);
    	Point parent(function->n);
    	if(fromzero == true)
    		parent.setzero();
    	Point bestChild(function->n), nextChild(function->n);
        
    	long t = 0; // generations
    	double eva = 0; // evaluations
    	Bitcount bitcount;
    	
    	while(t < maxGeneration){
            
            // mutations
    		bestChild.mutate(parent, gen, dis);
    		for(int i=1; i<int(lambda+0.5); ++i){
    			nextChild.mutate(parent, gen, dis);
    			if(function->smaller(bestChild, nextChild))
    				bestChild.copy(nextChild);
    		}
    		eva += int(lambda+0.5);
            
            // record the details
            if(details == true){
                cnt[bitcount.evaluate(parent)] += 1;
                lbd[bitcount.evaluate(parent)] += lambda;
                drift[bitcount.evaluate(parent)] += (bitcount.evaluate(bestChild)-bitcount.evaluate(parent));
            }
            
            // higher fitness than the parent
    		if(function->smaller(parent, bestChild)){
    			parent.copy(bestChild);
    			lambda = lambda/F;
    			if(lambda < 1)
    				lambda = 1;
    		}
            // no improvement on fitness
    		else{
    			if(elitist == false)
    				parent.copy(bestChild);
    			lambda = lambda*pow(F, 1/s);
    		}
            
            // for dynamic binval
            if(function->dynamic == true)
    			function->update();
            
            // record every n generations in the log
            if(t%function->n == 0){
                output << t << ',';
                output << eva << ',';
                output << function->evaluate(parent) << ',';
                output << bitcount.evaluate(parent) << ',';
                output << lambda << endl;
            }
            
            // optimum is found
        	if(bitcount.evaluate(parent) == function->n)
            	break;
            ++ t;
    	}
        output.close();
    	return make_pair(t, eva);
    }
};

/*
template <class T>
    pair<float, float> calcDrift(int n, int numOnes, float c, T& func, int lambda, int repeat){
        Point parent(n);
        parent.setones(numOnes);
        Bitcount bitcount;	
        
    	default_random_engine gen;
    	binomial_distribution<int> dis(n, c/n);
        
        float drift = 0;
        float success = 0;
        for(int r=0; r<repeat; ++r){
            srand(r);
            Point bestChild(n), nextChild(n);
            bestChild.mutate(parent, gen, dis);
            for(int i=1; i<lambda; ++i){
                nextChild.mutate(parent, gen, dis);
                if(func.smaller(bestChild, nextChild))
                    bestChild.copy(nextChild);
            }
            drift += (bitcount.evaluate(bestChild) - bitcount.evaluate(parent));
            if(func.smaller(parent, bestChild)){
                success += 1;
            }
        }
        //cout << drift << ' ' << success << endl;
        return make_pair(drift/repeat, success/repeat);
    }
*/
