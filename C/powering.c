/*
 * Comparison of recursive powering methods.
 * By Zachary Jones
 * http://www.circuitslikefreeways.com/
 *
 * Method 1 is the typical "straight recursion" 
 * taught to beginners. Method 2 is an optimization
 * I made to include another Mathematical property.
 * The optimization drastically reduces the amount 
 * of recursive function calls necessary to compute 
 * the result.
 *
 * Method 1 relies on:
 *   b^p = b * b^(p-1)
 *   b^1 = b
 *
 * Method 2 includes another:
 *   b^p = b * b^(p-1)
 *   b^p = b^(p/2) * b^(p/2) when p is even
 *   b^1 = b
 *
 * Method 2 allows it to take a shortcut by realizing 
 * that if the power is an even integer we can calculate 
 * the base raised to half the power, then multiply that 
 * number by itself, cutting our work for that task in half.
 */

#include <stdio.h>

double pow1(double, int);
double pow2(double, int);
void determining(double, int, int);
void determined(double, int, int, double);

int count = 0, max = 0, pow1_max;
double result;

int main(void) 
{
   double user_base;
   int user_power;
   double func_result;
   
   printf("\nGive me a base  : ");
   scanf("%lf", &user_base);
   printf("Give me a power : ");
   scanf("%d", &user_power);
   
   printf("\nMethod 1: Strict Recursion, No Halving\n");
   func_result = pow1(user_base, user_power);
   printf("  Result: %.2lf in %d steps\n", func_result, max);
   
   pow1_max = max; max = count = 0;
   printf("\nMethod 2: Recursion with Halving\n");
   func_result = pow2(user_base, user_power);
   printf("  Result: %.2lf in %d steps\n\n", func_result, max);
   
   printf("Recursion with halving took %.2lf percent less calculations.\n", 
     ( ((double) (pow1_max) / max )*100));
  
   return 1;
}


double pow1(double base, int power) 
{
   determining(base, power, 1);
   
   if (power == 0) return 1;
   
   if (power > 0) {
      if (power == 1) {
         result = base;
      } else {
         result = base * pow1(base, power-1);
      }
   } else {
      result = 1 / pow1(base, power * -1);
   }
   
   determined(base, power, -1, result);
   return result;
}

double pow2(double base, int power) 
{
   determining(base, power, 1);
   
   if (power == 0) return 1;

   if(power > 0) {
      if(power == 1) {
         result = base;
      } else if(power % 2 == 0) {
         double part = pow2(base, power/2);
         result = part * part;
      } else {
         result = base * pow2(base, power-1);
      }
   } else {
      result = 1 / pow2(base, power * -1);
   }
   
   determined(base, power, -1, result);
   return result;
}


void determining(double base, int power, int direction) 
{
   if(direction < 1) count--; else count++;
   if(count > max) max = count;
   printf("  %3d Determining: %lf^%d\n", count, base, power);
}

void determined(double base, int power, int direction, double result) 
{
   if(direction < 1) count--; else count++;
   if(count > max) max = count;
   printf("  %3d Determined %lf^%d = %lf\n", count, base, power, result);
}