// Function starts a multi sensor simulator
// After calling this function with a descriptor array of 10
// descriptor elements, the sensor values can be read from the
// descripptors in the array. 
// The parameter steps specifies how many values sensors send
// before they cause the end of file situation.
// Function returns the number of opened sensors. It is always 10.

int StartSimulator(int fd_array[], int steps);

// When you read from the descriptors opened with the function 
// StartSimulator, you get structures defined below. 

#include <time.h>
typedef struct {
     struct timespec moment;
     int value;
} Tmeas;  


struct timespec diff_timespec(const struct timespec* t1 , const struct timespec* t2);
void increment_timespec(struct timespec* sum , const struct timespec* ts);

long long int getnanosec(const struct timespec* ts);
struct timespec maketimespec(long long int ns);

