
unsigned long long g(unsigned long long U, int invertido)
{
   // Input 64bits U retorna chave para DES 56bits
   // Parametro invertido == 0 entao o 2 e 3 bits passa a 10
   // Se invertido == 1 entao o 2 e 3 bits sao 01
   int i;
   // U = U1 1 0 U4 U5 ... U63 ?? porque 63 e nao 64??
   unsigned long long semOitavos = 0;
   for (i = 1; i <= 64; i++)
   {
      // Elimina os oitavos bits
      if (i % 8 == 0)
      {
         printf("Estou a eliminar %d\n", i);
      } else {
         // Mascara para aceder aos bits no U
         unsigned long long mask = pow(2,i-1);
         // Mete no bit mais a direita (menos significante)
         // e vai mexende-o casa a casa para a esquerda
         semOitavos |= (U & mask);
         semOitavos = semOitavos << 1;
      }
   }
   
   // Faz o set adequado ao segundo e terceiro bits
   if (invertido == 0)
   {
      // 10
     semOitavos |= 0x4000000000000000; //Mete o 1
     semOitavso &= 0xDFFFFFFFFFFFFFFF; // Forca o 0
   } else if (invertido == 1)
   {
      // 01
     semOitavso &= 0xBFFFFFFFFFFFFFFF; // Forca o 0
     semOitavos |= 0x2000000000000000; //Mete o 1
   }
   return semOitavos;
   
}
#define NOT_INVERTED 0
#define INVERTED 1

unsigned long long MDC2(unsigned long long *x, unsigned int t)
{
   // Divide x em blocos de 64 bits (ja esta)
   //Define IV e N_IV
   unsigned long long IV = 0x5252525252525252, N_IV = 0x2525252525252525;
   int i;
   unsigned long long H[t+1], N_H[t+1], K[t+1], N_K[t+1], C[t+1], N_C[t+1];
   H[0] = IV;
   N_H[0] = N_IV;
   for (i = 1; i <= t; i++)
   {
      K[i] = g(H[i-1], NOT_INVERTED);
      N_K[i] = g(N_H[i-1], INVERTED);
      
      // des ao plaintext x[i] com chave K[i]
      C[i] = XOR(DES(x[i], K[i]), x[i]);
      N_C[i] = XOR(DES(x[i], N_K[i]), x[i]);
      
      unsigned long long CL, CR, N_CL, N_CR;
      CL = C[i] >> 32; // Left 32 bits de C[i]
      CR = C[i] & 0x80000000; // Right 32 bits de C[i]
      N_CL = N_C[i] >> 32;
      N_CR = N_C[i] & 0x80000000;
      
      // Concatena
      H[i] = (CL << 32) | N_CR;
      N_H[i] = (N_CL << 32) | CR;
   }
}

unsigned long long MDC4(unsigned long long *x, unsigned int t)
{
   // Divide x em blocos de 64 bits (ja esta)
   //Define IV e N_IV
   unsigned long long IV = 0x5252525252525252, N_IV = 0x2525252525252525;
   int i;
   unsigned long long G[t+1], N_G[t+1], K[t+1], N_K[t+1], C[t+1], N_C[t+1];
   unsigned long long J[t+1], N_J[t+1];
   G[0] = IV;
   N_G[0] = N_IV;
   for (i = 1; i <= t; i++)
   {
      // IGUAL AO MDC2
      K[i] = g(G[i-1], NOT_INVERTED);
      N_K[i] = g(N_G[i-1], INVERTED);
            
      // des ao plaintext x[i] com chave K[i]
      C[i] = XOR(DES(x[i], K[i]), x[i]);
      N_C[i] = XOR(DES(x[i], N_K[i]), x[i]);
      
      unsigned long long CL, CR, N_CL, N_CR;
      CL = C[i] >> 32; // Left 32 bits de C[i]
      CR = C[i] & 0x80000000; // Right 32 bits de C[i]
      N_CL = N_C[i] >> 32;
      N_CR = N_C[i] & 0x80000000;
      
      // Concatena
      H[i] = (CL << 32) | N_CR;
      N_H[i] = (N_CL << 32) | CR;
      
      // NOVO PEDACO
      J[i] = g(H[i], NOT_INVERTED);
      N_J[i] = g(N_H[i], INVERTED);
      
      D[i] = XOR(DES(N_G[i-1], J[i]), N_G[i-1]);
      N_D[i] = XOR(DES(G[i-1], N_J[i]), G[i-1]);
      
      // Divide em duas partes
      unsigned long DL, DR, N_DL, N_DR;
      DL = D[i] >> 32; // Left 32 bits de C[i]
      DR = D[i] & 0x80000000; // Right 32 bits de C[i]
      N_DL = N_D[i] >> 32;
      N_DR = N_D[i] & 0x80000000;
      
      //Concatena
      G[i] = (DL<<32) | N_DR;
      N_G[i] =  (N_DL<<32) | DR;
      
   }
   printf("OUTPUT:%llX\n", (G[t]<<32)|(N_G[t]));
}