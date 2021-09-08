#include <stdio.h>
#include <math.h>

enum solve_status_t  
{
    inf_roots = -1,
    no_roots,
    one_root,
    two_roots
};

enum solve_status_t solve_quad_eq (double a, double b, double c, double * root1, double * root2);

int main ()
{
    double a, b, c, root1, root2;
    enum solve_status_t solve_status = inf_roots;
    char user_input = 0; 

    a = b = c = root1 = root2 = 0.;

    while (user_input != 'n')
    {
        printf ("enter equation coeffitients: ");

        if (scanf ("%lf %lf %lf", &a, &b, &c) != 3)
        {
            printf ("wrong coeffitients count!\n");
            return 0;
        }
        
        solve_status = solve_quad_eq (a, b, c, &root1, &root2);
        
        switch (solve_status)
        {
            case no_roots:
                printf ("no roots\n");
                break;

            case inf_roots:
                printf ("infinite roots\n");
                break;
                
            case one_root:
                printf ("root: %.2lf\n", root1);
                break;

            case two_roots:
                printf ("roots: %.2lf %.2lf\n", root1, root2);
                break;

            default:
                printf ("unexpected solve result\n");
        }

        printf ("continue<n/y>? ");

        if (scanf (" %c", &user_input) != 1)
        {
           printf ("expected exit condition\n");
           return 0; 
        }
    }

    return 0;
}

enum solve_status_t solve_quad_eq (double a, double b, double c, double * root1, double * root2)
{
    double D;
    
    if (!a && !b && !c)
        return inf_roots;

    if (!a && !b)
        return no_roots;

    if (!a)
    {
        *root1 = -c / b;
        return one_root;
    }

    D = b * b - 4 * a * c;

    if (D < 0) 
        return no_roots;
    
    if (D == 0)
    {
        *root1 = -b / (2 * a);

        return one_root;
    }

    *root1 = (-b + sqrt(D)) / (2 * a);
    *root2 = (-b - sqrt(D)) / (2 * a);

    return two_roots;
}
