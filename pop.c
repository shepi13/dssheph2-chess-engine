//Test function to see if __builtin_popcount() worked as expected or if my own
//popcount function may be better
int main(int argc, char* argv[])
{
    return __builtin_popcount(atoi(argv[1]));
}
