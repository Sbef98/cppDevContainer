double potential(int base, int exponential){
    double result = 1;
    for(int i = 0; i < exponential; i++){
        result *= base;
    }
    return result;
}