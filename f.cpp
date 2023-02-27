#include <ea.hpp>

void fDynamicBinval(){
	int n = 1000;
	float lbd = 1;
	float s = 1.8;
	int repeat = 50;
	bool fromzero = false;
    bool randomRounding = false;
    
    int numc = 5;
    float c[5] = {0.95, 0.98, 1, 1.02, 1.05};
    int numf = 15;
    float f[15] = {1.25, 1.5, 1.75, 2, 2.5, 3, 3.5, 4, 4.75, 5.5, 6.25, 7, 8, 9, 10};
    
    for(int j=0; j<numc; ++j){
        string roundingLabel = "_";
        if(randomRounding == true)
            roundingLabel = "_random";
            
        ofstream output("fDynamicBinval_c" + to_string(c[j]) + roundingLabel +".csv");
        for(int r=0; r<repeat; ++r){
            cout << c[j] << ", " << r << endl;
            for(int i=0; i<numf; ++i){
                DynamicBinval dbv(n);
                Saea <DynamicBinval> saea(lbd, c[j], s, f[i], dbv);
                pair<int, double> res = saea.run(500*n, r+10, fromzero, false, NULL, NULL, NULL, randomRounding);
                output << r << ',' << i << ',' << res.first << ',' << res.second << endl;
            }
        }
    }
}

/*
void fBinval(){
	int n = 1000;
	float lbd = 1;
	float c = 1;
	float F = 1;
	float s = 2.2;
	int repeat = 10;
	bool fromzero = false;
	ofstream output("fBinval.csv");

	float low = 0.01;
	float high = 9;
	int step = 20;

	for(int r=0; r<repeat; ++r){
		for(int i=0; i<step; ++i){
			Binval binval(n);
			Saea <Binval> saea(lbd, c, s, F+low*pow(high/low, 1.0*i/(step-1)), binval);
			pair<int, double> res = saea.run(500*n, r, fromzero);
			output << r << ',' << i << ',' << res.first << ',' << res.second << endl;
		}
	}
}

void fBinary(){
	int n = 1000;
	float lbd = 1;
	float c = 1;
	float F = 1;
	float s = 2.8;
	int repeat = 10;
	bool fromzero = false;
	ofstream output("fBinary.csv");

	float low = 0.01;
	float high = 9;
	int step = 20;

	for(int r=0; r<repeat; ++r){
		for(int i=0; i<step; ++i){
			Binary binary(n);
			Saea <Binary> saea(lbd, c, s, F+low*pow(high/low, 1.0*i/(step-1)), binary);
			pair<int, double> res = saea.run(500*n, r, fromzero);
			output << r << ',' << i << ',' << res.first << ',' << res.second << endl;
		}
	}
}

void fOnemax(){
	int n = 1000;
	float lbd = 1;
	float c = 1;
	float F = 1;
	float s = 3.2;
	int repeat = 10;
	bool fromzero = false;
	ofstream output("fOnemax.csv");

	float low = 0.01;
	float high = 9;
	int step = 20;

	for(int r=0; r<repeat; ++r){
		for(int i=0; i<step; ++i){
			Bitcount onemax(n);
			Saea <Bitcount> saea(lbd, c, s, F+low*pow(high/low, 1.0*i/(step-1)), onemax);
			pair<int, double> res = saea.run(500*n, r, fromzero);
			output << r << ',' << i << ',' << res.first << ',' << res.second << endl;
		}
	}
}

void fHotopic(){
	int n = 1000;
	float lbd = 1;
	float c = 1;
	float F = 1;
	float s = 2.8;
	bool fromzero = false;

	int L = 100;
	double alpha = 0.25;
	double beta = 0.05;
	double eps = 0.05;

	int repeat = 10;
	float low = 0.01;
	float high = 9;
	int step = 20;
	ofstream output("fHotopic.csv");
	
	for(int r=0; r<repeat; ++r){
		for(int i=0; i<step; ++i){
			Hotopic hotopic(n, L, alpha, beta, eps);
			Saea <Hotopic> saea(lbd, c, s, F+low*pow(high/low, 1.0*i/(step-1)), hotopic);
			pair<int, double> res = saea.run(500*n, r, fromzero);
			output << r << ',' << i << ',' << res.first << ',' << res.second << endl;
		}
	}
}
*/

int main(){
	fDynamicBinval();
	//fBinval();
	//fBinary();
	//fOnemax();
	//fHotopic();
}