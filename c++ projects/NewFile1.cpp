#include <stdio.h>
int main()
{
int counter,inner_counter;
for (counter=3; counter<=12; counter+=3)
{
for (inner_counter=3; inner_counter<=counter; inner_counter+=3)
{
printf("%i\t",counter);
}
printf("\n");
}
return 0;
}
