#include <stdio.h> 
#include <math.h>  


int main()
{
	float a;
	double b;
	int c = 2;
	printf("Digite a");
	scanf("%f",&a);
	printf("Digite b");
	scanf("%lf",&b);
	printf(">>>%f\n",a);
	printf(">>>%lf\n",b);
	float d = pow(a,a);
	printf("\n\n--------%f",d);

}
