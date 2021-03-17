unsigned total_cost(solution* x) {
    unsigned* pos_tracker = (unsigned*)malloc(sizeof(unsigned)*jobs_count);
    memset(pos_tracker, 0, jobs_count);

    unsigned total = 0;

    for(int i=0; i<jobs_count*machines_count; i++) {
        unsigned job = x->permutation[i];
        unsigned machine = pos_tracker[job];
        
        total += costs[job][machine];
        pos_tracker[job]++;
    }

    free(pos_tracker);
    return total;
}