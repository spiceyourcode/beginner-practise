#include <stdio.h>
int main()
{
int counter,inner_counter;
for (counter=1; counter<=3; counter+=1)
{
for (inner_counter=1; inner_counter=counter; inner_counter+=1)
{
printf("%i\t",counter);
}
printf("\n");
}
return 0;
}
