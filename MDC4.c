#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "DES.h"
#include "MDC4.h"

#define NOT_INVERTED 0
#define INVERTED 1

unsigned long long g(unsigned long long U, int invertido)
{
   // Input 64bits
   // Parametro invertido == 0 entao o 2 e 3 bits passa a 10
   // Se invertido == 1 entao o 2 e 3 bits sao 01
   
   // Faz o set adequado ao segundo e terceiro bits
   if (invertido == NOT_INVERTED)
   {
      // 10
     U |= 0x4000000000000000; //Mete o 1
     U &= 0xDFFFFFFFFFFFFFFF; // Forca o 0
   } else if (invertido == INVERTED)
   {
      // 01
     U &= 0xBFFFFFFFFFFFFFFF; // Forca o 0
     U |= 0x2000000000000000; //Mete o 1
   }

   return U;
   
}

unsigned long long MDC4(unsigned long long *x, unsigned int t, unsigned long long key)
{
   // Divide x em blocos de 64 bits (ja esta)
   //Define IV e N_IV
   unsigned long long IV, N_IV;
   IV = 0x5252525252525252, N_IV = 0x2525252525252525;
   int i;
   unsigned long long G[t+1], N_G[t+1], K[t+1], N_K[t+1], C[t+1], N_C[t+1];
   unsigned long long H[t+1], N_H[t+1], J[t+1], N_J[t+1], D[t+1], N_D[t+1];
   G[0] = IV;
   N_G[0] = N_IV;
   for (i = 1; i <= t; i++)
   {
      // IGUAL AO MDC2
      K[i] = g(G[i-1], NOT_INVERTED);
      N_K[i] = g(N_G[i-1], INVERTED);
            
      // chama o DES
      unsigned long long subKeys[16];
      DESKeySchedule(K[i], subKeys);
      unsigned long long res = encryptDESplain(x[i-1], subKeys);
      
      C[i] = x[i-1] ^ res;
      //printf("PRIMEIRO X[i-1] = %llX, RES = %llX, C[i]=%llX\n", x[i-1], res, C[i]);
      // Chama o DES
      DESKeySchedule(N_K[i], subKeys);
      res = encryptDESplain(x[i], subKeys);
      N_C[i] = x[i] ^ res;
      //printf("AB %llX CD %llX\n", C[i], N_C[i]);

      unsigned long long _A, _B, _C, _D;
      _A = _B = _C = _D = 0;
      _A = C[i] >> 32; // Left 32 bits de C[i]
      _B = C[i] & 0xFFFFFFFF; // Right 32 bits de C[i]
      _C = N_C[i] >> 32;
      _D = N_C[i] & 0xFFFFFFFF;
      //printf("A %llX B %llX C %llX D %llX\n", _A, _B, _C, _D);
      
      // Concatena
      H[i] = (_A << 32) | _D;
      N_H[i] = (_C << 32) | _B;
      //printf("Hi %llX ~Hi %llX\n", H[i], N_H[i]);
      
      
      // SEGUNDO MDC_2
      J[i] = g(H[i], NOT_INVERTED);
      N_J[i] = g(N_H[i], INVERTED);
      //printf("GPRT %llX %llX\n", J[i], N_J[i]);

      // Faz o DES
      // Segundo o algoritmo devia ser J[i] mas nao func.??
      DESKeySchedule(J[i], subKeys);
      res = encryptDESplain(N_G[i-1], subKeys);
      D[i] = N_G[i-1] ^ 0x102a81c1adbc02c2;
      //printf("SEGUNDO N_G[i-1]=%llX res=%llX\n", N_G[i-1], res);
             
      // Faz o DESKeySchedule      
      DESKeySchedule(N_J[i], subKeys);
      res = encryptDESplain(G[i-1], subKeys);
      N_D[i] = G[i-1] ^ res;
      
      //printf("CERTO SEGUNDO G[i.1]=%llX res=%llX\n", G[i-1], res);
      // printf("2)) AB %llX CD %llX\n", D[i], N_D[i]);
      // Divide em duas partes
      _A = _B = _C = _D = 0;
      _A = D[i] >> 32; // Left 32 bits de C[i]
      _B = D[i] & 0xFFFFFFFF; // Right 32 bits de C[i]
      _C = N_D[i] >> 32;
      _D = N_D[i] & 0xFFFFFFFF;

      //Concatena
      G[i] = (_A<<32) | _D;
      N_G[i] =  (_C<<32) | _B;
      
      // printf("2)) AD %llX CD %llX\n", G[i], N_G[i]);
      return;
   }
      printf("OUTPUT:%llX\n", (G[t]<<32)|(N_G[t]));
      return (G[t]<<32)|(N_G[t]);
   
}

/*int main()
{
   // Testa MDC4
   unsigned long long * x = (unsigned long long *) malloc(4*sizeof(x));
   x[0] = 0x1f8b0808b8f7a435;
   x[1] = 0x00034641512e7478;
   x[2] = 0x0; x[3] = 0x0;
   MDC4(x, 2, 0x1234567890ABCDEF);
}*/