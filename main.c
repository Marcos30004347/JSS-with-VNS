#include <stdlib.h>

typedef struct job {
    // TODO
} job;

typedef struct solution {

} solution;

typedef struct solution {
    job* jobs;
    unsigned* permutation;
    unsigned count;
} solution;

void solution_free(solution* x) {
    free(x->permutation);
    free(x);
}

long d(solution* x, solution* y) {
    // TODO
    exit(EXIT_FAILURE);
    return 0;
}

long f(solution* x) {
    // TODO
    exit(EXIT_FAILURE);
    return 0;
}


// Shaking creates a new neighbohr by taking a random permutation
// from the current solution.
// choose x' ∈ Nk(x) at random and return x';
solution* shaking(solution* x) {
    int count = x->count;
	int pos1 = 1 + rand() % (x->count / 4);
	int pos2 = pos1 + 1 + rand() % (x->count / 4);
	int pos3 = pos2 + 1 + rand() % (x->count / 4);

	int i;

    unsigned tmp[x->count];
    int it;

    solution* y = (solution*)malloc(sizeof(solution));
    y->jobs = x->jobs;
    y->count = x->count;

	for (it = 0; it < pos1; i++)
        y->permutation[it++] = x->permutation[it];
	for (it = pos3; it < count; it++)
        y->permutation[it++] = x->permutation[it];
	for (it = pos2; it < pos3; it++)
        y->permutation[it++] = x->permutation[it];
	for (it = pos1; it < pos2; it++)
        y->permutation[it++] = x->permutation[it];

    return y;
}

solution* solution_copy(solution* x) {
    solution* y = (solution*)malloc(sizeof(solution));
    y->jobs = x->jobs;
    y->count = x->count;
	for (int it = 0; it < x->count; it++)
        y->permutation[it] = x->permutation[it];

    return x;
}


void solution_assign(solution* to, solution* from) {
    if(to != NULL)
        solution_free(to);
    to = solution_copy(from);
}

solution* argmin(solution* x, long p) {
    solution* y = solution_copy(x);
    for(int i=0; i<p; i++) {
        solution* tmp = shaking(x);
        if(f(tmp) < f(y))
            solution_assign(y, tmp);
        solution_free(tmp);
    }
    return y;
}

// ************************************
// 2.2 Neightborhood change step
// ************************************

void sequential_neighborhood_change_step(solution* x, solution* y, int* k) {
    if(f(y) < f(x)) {
        solution_assign(x, y); // x <- y
        *k = 1;
    } else {
        *k = *k + 1;
    }
}

void cyclic_neighborhood_change_step(solution* x, solution* y, int* k) {
    *k = *k + 1;

    if(f(y) < f(x)) {
        solution_assign(x, y);  // x <- y
    }
}

void pipe_neighborhood_change_step(solution* x, solution* y, int* k) {
    if(f(y) < f(x)) {
        solution_assign(x, y);  // x <- y
    } else {
        *k = *k + 1;
    }
}

void skewed_neighborhood_change_step(solution* x, solution* y, int* k, float a) {
    if(f(y) - f(x) < a*d(y,x)) {
        solution_assign(x, y);  // x <- y
        *k = 1;
    } else {
        *k = *k + 1;
    }
}

// ************************************
// 2.3 Improvement procedures within VNS
// ************************************

// *** IMPORTANT
// *** -> Note that a metaheuristic can also be used as a local search for example Simulated Annealing, Tabu Search, Iterated local search, GRASP, Genetic algorithm, etc

solution* local_search_first_improvement(solution* x, long p) {
    // Neighborhood structire are being generated randomly in the
    // shaking method

    solution* xi = NULL;
    solution* y = NULL;

    do {
        int i = 0;
        y = solution_copy(x);

        do {
            i = i+1;
            
            // xi is the N[i] equivalent of the neighborhood structure N
            xi = shaking(x);
            
            if(f(xi) < f(x)) {
                x = solution_copy(xi);
                solution_free(xi);
                break;
            }
            solution_free(xi);
        } while(i != p);
    } while(f(y) > f(x));
    
    return y;
}

solution* local_search_best_improvement(solution* x, long p) {
    // Neighborhood structire are being generated randomly in the
    // shaking method

    solution* y = NULL;

    do {
        y = solution_copy(x);

        solution* min_neighborhood = argmin(x, p);
        solution_assign(x, min_neighborhood);
        solution_free(min_neighborhood);
    } while(f(y) > f(x));

    return y;
}


// ***********************************************
// 2.3.2 Variable neighborhood descent procedures
// ***********************************************

solution* vnd_best_improvement(solution* x, long lmax, long p) {
    int stop, l;
    solution* y = NULL;

    do {
        stop = 0;
        l = 1;
        solution_assign(y, x);
        do {
            solution* yy;

            // argmin is the cause for the best improvement
            solution* min_neighborhood = argmin(x, p);
            solution_assign(yy, min_neighborhood);
            solution_free(min_neighborhood);

            // All the following algorithms can be used!
        
            sequential_neighborhood_change_step(x, yy, &l);
            // cyclic_neighborhood_change_step(x, yy, &l);
            // pipe_neighborhood_change_step(x, yy, &l);
            // skewed_neighborhood_change_step(x, yy, &l);
        } while(l != lmax);
    
        if(f(y) <= f(x)) {
            stop = 1;
        }
    
    } while(!stop);
    return y;
}

// Algorithm 9 of VNS-survey
void nested_vnd_best_improvement(solution* x, long lmax, long p) {
    // TODO
    exit(EXIT_FAILURE);
}

// Algorithm 10 of VNS-survey
void mixed_vnd_best_improvement(solution* x, long lmax, long p) {
    // TODO
    exit(EXIT_FAILURE);
}


// ****************************************
// 3 Variable neighborhood search variants
// ****************************************

solution* basic_variable_neighborhood_search(solution* x, long kmax) {
	int max_iterations = 5;

	int count = 0, it = 0;
    long k = 0;
    long it = 0;

	do 
	{
        it++;
        k = 1;
        while(k < kmax) {
            solution* y = shaking(x);
            
            solution* yy = local_search_best_improvement(y, 10);
            // solution* yy = local_search_first_improvement(y, 10);
            
            sequential_neighborhood_change_step(x, yy, &k);
            // cyclic_neighborhood_change_step(x, yy, &l);
            // pipe_neighborhood_change_step(x, yy, &l);
            // skewed_neighborhood_change_step(x, yy, &l);

            solution_free(y);
            solution_free(yy);
        }

	} while (it < max_iterations);
    return x;
}


solution* general_variable_neighborhood_search(solution* x, long kmax) {
	int max_iterations = 5;

	int count = 0, it = 0;
    long k = 0;
    long it = 0;

	do 
	{
        it++;
        k = 1;
        while(k < kmax) {
            solution* y = shaking(x);
            
            // can also be first improvement
            solution* yy = vnd_best_improvement(y, 5, 10);
            
            // can also be any of the neighborhood_change_step algorithms, I beleave
            sequential_neighborhood_change_step(x, yy, &k);

            solution_free(y);
            solution_free(yy);
        }

	} while (it < max_iterations);
    return x;
}