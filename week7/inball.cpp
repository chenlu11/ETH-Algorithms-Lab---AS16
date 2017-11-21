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
        int n, d;
        cin >> n;
        if (n == 0)
            break;
        cin >> d;

        // by default, we have a LP with Ax >= b
        Program lp (CGAL::SMALLER, false, 0, false, 0);
        const int R = 0;
        lp.set_l(R, true, 0);
        lp.set_c(R, -1);
        for (int i = 0; i < n; i++)
        {
            int len_a = 0;
            for (int j = 1; j <= d; j++)
            {
                int a; cin >> a;
                lp.set_a(j, i, a);
                len_a += a * a;
            }
            len_a = sqrt(len_a);
            lp.set_a(R, i, len_a);
            int b; cin >> b;
            lp.set_b(i, b);
        }
        
        // solve the program, using ET as the exact type
        Solution s = CGAL::solve_linear_program(lp, ET());
        assert (s.solves_linear_program(lp));
        if (s.is_infeasible())
            cout << "none" << endl;
        else
        {
            if (s.is_unbounded())
                cout << "inf" << endl;
            else
                cout << floor(-CGAL::to_double(s.objective_value())) << endl;
        }
    }
    return 0;
}