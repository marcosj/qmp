#include <iostream>
#include <vector>

using namespace std;

typedef uint64_t T;		// Warning: Handles up to sizeof(T) prime implicants
#define FORMAT "%lu"

typedef struct {
	vector<T> minterms;
	T binary;
	T mask;
	bool essential;
} Implicant;

int count1s(T x) {
	int cnt = 0;
	for ( ; x; ++cnt)
		x &= x - 1;
	return cnt;
}

int main() {
	clock_t tic = clock();
	int vars;
	scanf("%d", &vars);

	bool dcares = false;
	T term;
	vector<T> minterms;
	vector<Implicant> implicants;
	vector<vector<Implicant>> table;
	while (1) {
		int r = scanf(FORMAT, &term);
		if (r == EOF) {
			table.push_back(implicants);
			implicants.clear();
			break;
		}
		if (r == 0) {
			dcares = true;
			getchar();
		} else {
			if (!dcares) minterms.push_back(term);
			implicants.push_back({vector<T>{term}, term, 0, true});
		}
	}
	//for (auto m: minterms) cout << m << ' '; cout << endl;
	printf("Reading data: %f s\n", (double)(clock() - tic) / CLOCKS_PER_SEC);

	tic = clock();
	bool over = false;
	while (!over) {
		over = true;
		for (size_t i = 0; i < table.back().size(); ++i) {
			Implicant &x = table.back()[i];
			for (size_t j = i+1; j < table.back().size(); ++j) {
				Implicant &y = table.back()[j];
				if (x.mask == y.mask && count1s(x.binary ^ y.binary) == 1) {
					over = false;
					x.essential = false;
					y.essential = false;
					Implicant imp;
					imp.binary = x.binary & y.binary;
					imp.mask = x.mask + (x.binary ^ y.binary);
					bool duplicate = false;
					for (size_t k = 0; k < implicants.size(); ++k)
						if (imp.binary == implicants[k].binary && imp.mask == implicants[k].mask)
						{ duplicate = true; break; }
					if (!duplicate) {
						imp.essential = true;
						imp.minterms = x.minterms;
						imp.minterms.insert(imp.minterms.end(), y.minterms.begin(), y.minterms.end());
						implicants.push_back(imp);
					}
				}
			}
		}
		if (!implicants.empty()) {
			table.push_back(implicants);
			implicants.clear();
		}
	}

	vector<Implicant> primes;
	for (auto it = table.rbegin(); it != table.rend(); ++it)
		for (size_t j = 0; j < (*it).size(); ++j) {
			Implicant x = (*it)[j];
			if (x.essential)
				primes.push_back(x);
		}

	vector<vector<T>> chart(minterms.size());
	for (size_t i = 0; i < minterms.size(); ++i)
		for (size_t j = 0; j < primes.size(); ++j)
			for (size_t k = 0; k < primes[j].minterms.size(); ++k)
				if (primes[j].minterms[k] == minterms[i]) {
					chart[i].push_back(1UL << j);
					break;
				}
	printf("Quine: %f s\n", (double)(clock() - tic) / CLOCKS_PER_SEC);

	tic = clock();
	vector<T> maxterms(chart[0]);
	for (size_t i = 1; i < minterms.size(); ++i) {
		vector<T> aux;
		for (size_t j = 0; j < maxterms.size(); ++j)
			for (size_t k = 0; k < chart[i].size(); ++k)
				aux.push_back(maxterms[j] | chart[i][k]);
		size_t j = 0;
		while (j < aux.size()) {
			bool increment = true;
			for (size_t k = aux.size()-1; k > j; --k) {
				T x = aux[j];
				T y = aux[k];
				if (((x ^ y) | x) == y)
					aux.erase(aux.begin() + k);
				else if (((x ^ y) | y) == x) {
					aux.erase(aux.begin() + j);
					increment = false;
					break;
				}
			}
			if (increment) ++j;
		}
		maxterms = aux;
	}

	vector<T> Y;
	int size = count1s(maxterms[0]);
	for (size_t i = 0; i < maxterms.size(); ++i) {
		int s = count1s(maxterms[i]);
		if (size == s)
			Y.push_back(maxterms[i]);
		else if (size > s) {
			size = s;
			Y.clear();
			Y.push_back(maxterms[i]);
		}
	}

	for (size_t i = 0; i < Y.size(); ++i) {
		printf("Y = ");
		for (size_t j = 0; j < primes.size(); ++j)
			if (Y[i] & (1UL << j)) {
				Implicant x = primes[j];
				for (int k = vars-1; k >= 0; --k) {
					T bit = 1UL << k;
					if (bit & x.mask) putchar('-');
					else if (bit & x.binary) putchar('1');
					else putchar('0');
				}
				printf(" + ");
			}
		putchar('\n');
	}
	printf("Petrick: %f s\n", (double)(clock() - tic) / CLOCKS_PER_SEC);
}
