#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
using namespace std;

// choose exact floating-point type
#ifdef CGAL_USE_GMP
#include <CGAL/Gmpzf.h>
typedef CGAL::Gmpzf ET; 
#else
#include <CGAL/MP_Float.h>
typedef CGAL::MP_Float ET;
#endif

// program and solution types
typedef CGAL::Quadratic_program<double> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int main() {
	while (true)
	{
		int type, a, b;
		cin >> type;
		if (type == 0)
			break;
		cin >> a >> b;
		//cout.unsetf(ios::scientific);  //终止科学记数法状态
		cout.setf(ios::fixed);  //指定用定点形式输出
		cout.precision(0);  //保留位小数
		if (type == 1)
		{
			// max -a * x^2 + b * y, Equivalent to: min a * x^2 - b * y
			// by default, we have a nonnegative QP with Ax <= b
			Program qp (CGAL::SMALLER, true, 0, false, 0);
			// now set the non-default entries: 
			const int X = 0; 
			const int Y = 1;
			qp.set_a(X, 0,  1); qp.set_a(Y, 0, 1); qp.set_b(0, 4);  	//  x + y  <= 4
			qp.set_a(X, 1,  4); qp.set_a(Y, 1, 2); qp.set_b(1, a * b);  // 4x + 2y <= ab
			qp.set_a(X, 2, -1); qp.set_a(Y, 2, 1); qp.set_b(2, 1);  	// -x + y  <= 1
			qp.set_d(X, X, 2 * a);  // !!specify 2D!!    a * x^2
			qp.set_c(Y, -b);   		// - b * y
			// solve the program, using ET as the exact type
			Solution s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
			assert (s.solves_quadratic_program(qp));
			// output
			if (s.is_infeasible())
				cout << "no" << endl;
			else
			{
				if (s.is_unbounded())
					cout << "unbounded" << endl;
				else
					cout << floor(-CGAL::to_double(s.objective_value())) << endl;
			}
		}
		else
		{
			// z = z^2
			// min a * x^2 + b * y + z^2
			// by default, we have a nonpositive QP with Ax >= b
			Program qp (CGAL::LARGER, false, 0, true, 0);
			// now set the non-default entries: 
			const int X = 0;
			const int Y = 1;
			const int Z = 2;
			qp.set_u(Z, false, 0);
			qp.set_a(X, 0,  1); qp.set_a(Y, 0, 1); qp.set_a(Z, 0, 0); qp.set_b(0, -4);  	//  x + y  >= -4
			qp.set_a(X, 1,  4); qp.set_a(Y, 1, 2); qp.set_a(Z, 1, 1); qp.set_b(1, -a * b);  // 4x + 2y + z >= -ab
			qp.set_a(X, 2, -1); qp.set_a(Y, 2, 1); qp.set_a(Z, 2, 0); qp.set_b(2, -1);  	// -x + y  >= -1 
			qp.set_d(X, X, 2 * a); qp.set_d(Z, Z, 2);  // !!specify 2D!!    a * x^2 + z^2
			qp.set_c(Y, b);   		// b * y
			// solve the program, using ET as the exact type
			Solution s = CGAL::solve_quadratic_program(qp, ET());
			assert (s.solves_quadratic_program(qp));
			// output
			//cout << "s.is_infeasible() = " << s.is_infeasible() << endl;
			//cout << "s.is_unbounded() = " << s.is_unbounded() << endl;
			//cout << "s.status() = " << s.status() << endl;
			if (s.is_infeasible())
				cout << "no" << endl;
			else
			{
				if (s.is_unbounded())
					cout << "unbounded" << endl;
				else
					cout << ceil(CGAL::to_double(s.objective_value())) << endl;
			}
		}
	}
	return 0;
}

/*
http://doc.cgal.org/latest/QP_solver/classCGAL_1_1Quadratic__program.html

CGAL::Quadratic_program< NT >::Quadratic_program	(	
	CGAL::Comparison_result 	default_r = CGAL::EQUAL,
	bool 	default_fl = true,  // finite lower bound?
	const NT & 	default_l = 0,  // lower bound value if lower bound is finite
	bool 	default_fu = false,  // finite upper bound?
	const NT & 	default_u = 0   // upper bound value if upper bound is finite
)

The output of this is an object of Quadratic_program_solution<ET> which you can in turn query for various things: 
what is the status of the program (optimally solved, infeasible, or unbounded?), 
what are the values of the optimal solution x∗, what is the associated objective function value, etc.

If there is no x that satisfies all the (in)equalities, the program is called infeasible, 
otherwise, it is feasible, and any x that satisfies all (in)equalities is called a feasible solution.

If the objective function value becomes arbitrarily small over the feasible region (the set of feasible solutions), 
the program is called unbounded, and bounded otherwise.

Any program that is both feasible and bounded has at least one feasible solution x∗ 
whose objective function value is not larger than that of any other feasible solution. 
This is called an optimal solution.
*/