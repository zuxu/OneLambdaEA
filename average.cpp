#include <ea.hpp>

int n = 1000;
float lambda = 1;
float c = 1;
float F = 1.5;
const int nums = 4; 
float s[5] = {1.0, 2.0, 3.0, 4.0};
int repeat = 100;
bool fromzero = true;

void runHotopic(){
    int cnt[1000];
    double lbd[1000], drift[1000];
    
	int L = 100;
	double alpha = 0.25;
	double beta = 0.05;
	double eps = 0.05;

    Hotopic hotopic(n, L, alpha, beta, eps);
    
	ofstream output("hotopic_average.csv");
    ofstream details("hotopic_details.csv");
    
    for(int r=0; r<repeat; ++r){
		for(int j=1; j<nums+1; ++j){
            for(int i=0; i<n; ++i){
                cnt[i] = 0;
                lbd[i] = 0;
                drift[i] = 0;
            }
			Saea <Hotopic> saea(lambda, c, s[j-1], F, hotopic);
			pair<int, double> res = saea.run(500*n, r+10, fromzero, true, cnt, lbd, drift);
            output << r << ',' << 0 << ',' << j << ',' << res.first << ',' << res.second << endl;
            
			for(int i=0; i<n; ++i){
                details << cnt[i] << ',' << lbd[i] << ',' << drift[i] << endl;
            }
		}
	}
}

void runBinary(){
	int cnt[1000];
    double lbd[1000], drift[1000];
    Binary binary(n);

	ofstream output("binary_average.csv");
    ofstream details("binary_details.csv");
    
    for(int r=0; r<repeat; ++r){
		for(int j=1; j<nums+1; ++j){
            for(int i=0; i<n; ++i){
                cnt[i] = 0;
                lbd[i] = 0;
                drift[i] = 0;
            }
			Saea <Binary> saea(lambda, c, s[j-1], F, binary);
			pair<int, double> res = saea.run(500*n, r+10, fromzero, true, cnt, lbd, drift);
            output << r << ',' << 0 << ',' << j << ',' << res.first << ',' << res.second << endl;
            
			for(int i=0; i<n; ++i){
                details << cnt[i] << ',' << lbd[i] << ',' << drift[i] << endl;
            }
		}
	}
}

void runDynamicBinval(){
	int cnt[1000];
    double lbd[1000], drift[1000];
    DynamicBinval dbv(n);
    
	ofstream output("dynamicBinval_average.csv");
    ofstream details("dynamicBinval_details.csv");
    
    for(int r=0; r<repeat; ++r){
		for(int j=1; j<nums+1; ++j){
            for(int i=0; i<n; ++i){
                cnt[i] = 0;
                lbd[i] = 0;
                drift[i] = 0;
            }
			Saea <DynamicBinval> saea(lambda, c, s[j-1], F, dbv);
			pair<int, double> res = saea.run(500*n, r+10, fromzero, true, cnt, lbd, drift);
            output << r << ',' << 0 << ',' << j << ',' << res.first << ',' << res.second << endl;
            
			for(int i=0; i<n; ++i){
                details << cnt[i] << ',' << lbd[i] << ',' << drift[i] << endl;
            }
		}
	}
}

void runBinval(){
	int cnt[1000];
    double lbd[1000], drift[1000];
    Binval binval(n);
    
	ofstream output("binval_average.csv");
    ofstream details("binval_details.csv");
    
    for(int r=0; r<repeat; ++r){
		for(int j=1; j<nums+1; ++j){
            for(int i=0; i<n; ++i){
                cnt[i] = 0;
                lbd[i] = 0;
                drift[i] = 0;
            }
			Saea <Binval> saea(lambda, c, s[j-1], F, binval);
			pair<int, double> res = saea.run(500*n, r+10, fromzero, true, cnt, lbd, drift);
            output << r << ',' << 0 << ',' << j << ',' << res.first << ',' << res.second << endl;
            
			for(int i=0; i<n; ++i){
                details << cnt[i] << ',' << lbd[i] << ',' << drift[i] << endl;
            }
		}
	}
}

void runOnemax(){
    int cnt[1000];
    double lbd[1000], drift[1000];
    Bitcount onemax(n);
        
	ofstream output("onemax_average.csv");
    ofstream details("onemax_details.csv");

	for(int r=0; r<repeat; ++r){
		for(int j=1; j<nums+1; ++j){
            for(int i=0; i<n; ++i){
                cnt[i] = 0;
                lbd[i] = 0;
                drift[i] = 0;
            }
			Saea <Bitcount> saea(lambda, c, s[j-1], F, onemax);
			pair<int, double> res = saea.run(500*n, r+10, fromzero, true, cnt, lbd, drift);
            output << r << ',' << 0 << ',' << j << ',' << res.first << ',' << res.second << endl;
            
			for(int i=0; i<n; ++i){
                details << cnt[i] << ',' << lbd[i] << ',' << drift[i] << endl;
            }
		}
	}
}



int main(){
    runDynamicBinval();
    runOnemax();
	runBinval();
	runBinary();
	runHotopic();
}