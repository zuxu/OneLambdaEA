#include <ea.hpp>

void runBinval(){
	int N[] = {96, 1000, 10000};
    int numn = 3; // number of n
	float lbd = 1;
	float c = 1;
	float F = 1.5;
	float s = 0.2; // smallest s
    int nums = 25; // number of s
	int repeat = 10;

	ofstream output("binval.csv");
	for(int r=0; r<repeat; ++r){
		for(int i=0; i<numn; ++i){
			Binval binval(N[i]);
			for(int j=1; j<nums+1; ++j){
				Saea <Binval> saea(lbd, c, j*s, F, binval);
				pair<int, double> res = saea.run(500*N[i], r);
				output << r << ',' << i << ',' << j << ',' << res.first << ',' << res.second << endl;
			}
		}
	}
}

void runBinary(){
	int N[] = {96, 1000, 10000};
    int numn = 3;
	float lbd = 1;
	float c = 1;
	float F = 1.5;
	float s = 0.2;
    int nums = 25;
	int repeat = 10;

	ofstream output("binary.csv");
	for(int r=0; r<repeat; ++r){
		for(int i=0; i<numn; ++i){
			Binary binary(N[i]);
			for(int j=1; j<nums+1; ++j){
				Saea <Binary> saea(lbd, c, j*s, F, binary);
				pair<int, double> res = saea.run(500*N[i], r);
				output << r << ',' << i << ',' << j << ',' << res.first << ',' << res.second << endl;
			}
		}
	}
}

void runHotopic(){
	int N[] = {96, 1000, 10000};
    int numn = 3;
	float lbd = 1;
	float c = 1;
	float F = 1.5;
	float s = 0.2;
    int nums = 25;
	int repeat = 10;
    
	int L = 100;
	double alpha = 0.25;
	double beta = 0.05;
	double eps = 0.05;

	ofstream output("hotopic.csv");
	for(int r=0; r<repeat; ++r){
		for(int i=0; i<numn; ++i){
			Hotopic hotopic(N[i], L, alpha, beta, eps);
			for(int j=1; j<nums+1; ++j){
				Saea <Hotopic> saea(lbd, c, j*s, F, hotopic);
				pair<int, double> res = saea.run(500*N[i], r);
				output << r << ',' << i << ',' << j << ',' << res.first << ',' << res.second << endl;
			}
		}
	}
}

void runOnemax(){
	int N[] = {96, 1000, 10000};
    int numn = 3;
	float lbd = 1;
	float c = 1;
	float F = 1.5;
	float s = 0.2;
    int nums = 25;
	int repeat = 10;
	bool fromzero = false;
    
	ofstream output("onemax.csv");
	for(int r=0; r<repeat; ++r){
		for(int i=0; i<numn; ++i){
			Bitcount onemax(N[i]);
			for(int j=1; j<nums+1; ++j){
				Saea <Bitcount> saea(lbd, c, j*s, F, onemax);
				pair<int, double> res = saea.run(500*N[i], r, fromzero);
				output << r << ',' << i << ',' << j << ',' << res.first << ',' << res.second << endl;
			}
		}
	}
}

void runDynamicBinval(){
	int N[] = {10000};
    int numn = 1;
	float lbd = 1;
	float c = 1;
	float F = 1.5;
	float s = 0.2;
    int nums = 15;
	int repeat = 10;
	bool fromzero = false;
    
	ofstream output("dynamicBinval.csv");
	for(int r=0; r<repeat; ++r){
        cout << "repeatition: " << r << endl;
		for(int i=0; i<numn; ++i){
			DynamicBinval dbv(N[i]);
			for(int j=1; j<nums+1; ++j){
				Saea <DynamicBinval> saea(lbd, c, j*s, F, dbv);
				pair<int, double> res = saea.run(500*N[i], r, fromzero);
				output << r << ',' << i << ',' << j << ',' << res.first << ',' << res.second << endl;
			}
		}
	}
}


int main(){
    runOnemax();
	runBinval();
	runBinary();
	runHotopic();
    runDynamicBinval();
}