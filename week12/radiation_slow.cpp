// Result: ok tl tl tl ok tl
#include <iostream>
#include <vector>

#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpzf.h>

typedef CGAL::Gmpzf ET;
typedef CGAL::Quadratic_program<double> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

int nH, nT, N;
vector< vector<double> > x, y, z;

bool lp(int degree) {
	Program lp = Program(CGAL::LARGER, false, 0.0, false, 0.0);
	
	for (int cell = 0; cell < nH; ++cell) { // also: constraint_index
		lp.set_b(cell, 1);
		int variable_index = 0;
		for (int i = 0; i <= degree; ++i) {
			for (int j = 0; j <= degree - i; ++j) {
				for (int k = 0; k <= degree - i - j; ++k) {
					// term x_1^i * x_2^j * x_3^k */
					lp.set_a(variable_index, cell, x[cell][i] * y[cell][j] * z[cell][k]);
					variable_index++;
				}
			}
		}
	}

	for (int cell = nH; cell < N; ++cell) { // also: constraint_index
		lp.set_b(cell, -1);
		lp.set_r(cell, CGAL::SMALLER);
		int variable_index = 0;
		for (int i = 0; i <= degree; ++i) {
			for (int j = 0; j <= degree - i; ++j) {
				for (int k = 0; k <= degree - i - j; ++k) {
					// term x_1^i * x_2^j * x_3^k */
					lp.set_a(variable_index, cell, x[cell][i] * y[cell][j] * z[cell][k]); // can reuse
					variable_index++;
				}
			}
		}
	}

	CGAL::Quadratic_program_options options;
	options.set_pricing_strategy(CGAL::QP_BLAND);
	Solution s = CGAL::solve_linear_program(lp, ET(), options);

	if (s.is_infeasible())
		return false;
	else
		return true;
}

void radiation() {

	cin >> nH >> nT;
	N = nH + nT;
	x.clear(); x.resize(N, vector<double>(31, 1));
	y.clear(); y.resize(N, vector<double>(31, 1));
	z.clear(); z.resize(N, vector<double>(31, 1));

	for (int i = 0; i < N; ++i) {
		cin >> x[i][1] >> y[i][1] >> z[i][1];
		for (int d = 2; d <= 30; ++d) {
			x[i][d] = x[i][1] * x[i][d - 1];
			y[i][d] = y[i][1] * y[i][d - 1];
			z[i][d] = z[i][1] * z[i][d - 1];
		}
	}

	if (nH * nT == 0) {
		cout << "0" << endl;
		return;
	}

	if (!lp(30)) { // slow
		cout << "Impossible!" << endl;
		return;
	}

	int low = 0, high = 30;
	while (low < high) {
		int mid = (low + high) / 2;
		if (!lp(mid))
			low = mid + 1;
		else
			high = mid;
	}
	cout << low << endl;
}

int main(void) {
	int T;
	cin >> T;
	for (int t = 0; t < T; ++t) {
		radiation();
	}
}