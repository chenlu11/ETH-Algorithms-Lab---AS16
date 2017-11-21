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
        int n, m;
        cin >> n >> m;
        if (n == 0 && m == 0)
            break;
        // by default, we have a nonnegative QP with Ax >= b
        Program qp (CGAL::LARGER, true, 0, false, 0);
        for (int i = 0; i < n; i++)
        {
            int cost, expectedReturn;
            cin >> cost >> expectedReturn;
            qp.set_a(i, 0, expectedReturn);
            qp.set_a(i, 1, cost);
        }
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                int covariance;
                cin >> covariance;
                if (i <= j)
                    qp.set_d(j, i, 2 * covariance);                    
            }
        }
        for (int i = 0; i < m; i++)
        {
            int C, R, V; // C: maximum cost, R: minimum expected return, V: maximum variance
            cin >> C >> R >> V;
            qp.set_b(0, R);
            qp.set_b(1, C);
            qp.set_r(1, CGAL::SMALLER);
            // solve the program, using ET as the exact type
            Solution s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
            assert (s.solves_quadratic_program(qp));
            // output
            if (s.is_infeasible())
                cout << "No." << endl;
            else
            {
                if (s.is_unbounded())
                    cout << "Yes." << endl;
                else
                {
                    double variance = CGAL::to_double(s.objective_value());
                    if (variance <= V)
                        cout << "Yes." << endl;
                    else
                        cout << "No." << endl;
                }
            }
        }
    }
    return 0;
}
