#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
using namespace std;

// choose exact integral type
#ifdef CGAL_USE_GMP
#include <CGAL/Gmpz.h>
typedef CGAL::Gmpz ET;
#else
#include <CGAL/MP_Float.h>
typedef CGAL::MP_Float ET;
#endif

// program and solution types
typedef CGAL::Quadratic_program<double> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int main() {
    cout.setf(ios::fixed);
    cout.precision(0);
    while (true)
    {
        int n, m;
        cin >> n >> m;
        if (n == 0 && m == 0)
            break;

        // by default, we have a nonnegative LP with Ax >= b
        Program lp (CGAL::LARGER, true, 0, false, 0);
        for (int i = 0; i < n; i++)
        {
            int min, max;
            cin >> min >> max;
            lp.set_b(i, min);
            lp.set_b(i + n, max);
            lp.set_r(i + n, CGAL::SMALLER);
        }
        for (int i = 0; i < m; i++)
        {
            int price;
            cin >> price;
            lp.set_c(i, price);
            for (int j = 0; j < n; j++)
            {
                int C;
                cin >> C;
                lp.set_a(i, j, C);
                lp.set_a(i, j + n, C);
            }
        }
        // solve the program, using ET as the exact type
        Solution s = CGAL::solve_linear_program(lp, ET());
        assert (s.solves_linear_program(lp));
        if (s.is_infeasible())
            cout << "No such diet." << endl;
        else
            cout << floor(CGAL::to_double(s.objective_value())) << endl;
    }
    return 0;
}