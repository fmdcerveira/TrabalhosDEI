/*Author: Rui Pedro Paiva
Teoria da Informação, LEI, 2008/2009*/

#include "DES.h"
#include <math.h>

/*******************************/
unsigned char IP[64] = {
   58, 50, 42, 34, 26, 18, 10, 2, 
   60, 52, 44, 36, 28, 20, 12, 4, 
   62, 54, 46, 38, 30, 22, 14, 6,
   64, 56, 48, 40, 32, 24, 16, 8, 
   57, 49, 41, 33, 25, 17, 9, 1,  
   59, 51, 43, 35, 27, 19, 11, 3, 
   61, 53, 45, 37, 29, 21, 13, 5, 
   63, 55, 47, 39, 31, 23, 15, 7
};
unsigned char inverseIP[64] = {
   40, 8, 48, 16, 56, 24, 64, 32,
   39, 7, 47, 15, 55, 23, 63, 31,
   38, 6, 46, 14, 54, 22, 62, 30,
   37, 5, 45, 13, 53, 21, 61, 29,
   36, 4, 44, 12, 52, 20, 60, 28,
   35, 3, 43, 11, 51, 19, 59, 27,
   34, 2, 42, 10, 50, 18, 58, 26,
   33, 1, 41, 9, 49, 17, 57, 25
};
// E bit-selection table
unsigned char eBit[48] = {
   32, 1, 2, 3, 4, 5,
   4, 5, 6, 7, 8, 9,
   8, 9, 10, 11, 12, 13,
   12, 13, 14, 15, 16, 17,
   16, 17, 18, 19, 20, 21,
   20, 21, 22, 23, 24, 25,
   24, 25, 26, 27, 28, 29,
   28, 29, 30, 31, 32, 1   
};
// S boxes
unsigned char S1[4][16] = {
   {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
   {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
   {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
   {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
};
unsigned char S2[4][16] = {
   {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
   {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
   {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
   {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
};
unsigned char S3[4][16] = {
   {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
   {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
   {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
   {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
};
unsigned char S4[4][16] = {
   {7, 13, 14, 3, 0, 6, 9 ,10 ,1 ,2 ,8, 5, 11, 12, 4, 15},
   {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
   {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
   {3, 15, 0, 6, 10, 1, 13, 8, 9,4, 5, 11, 12, 7, 2, 14 }  
};
unsigned char S5[4][16] = {
   {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
   {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
   {4, 2, 1, 11, 10, 13, 7,8, 15, 9, 12, 5, 6, 3, 0, 14 },
   {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
};
unsigned char S6[4][16] = {
   {12, 1, 10, 15, 9, 2 ,6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
   {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
   {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11,6},
   {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
};
unsigned char S7[4][16] = {
   {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
   {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
   {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
   {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 1}
};
unsigned char S8[4][16] = {
   {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
   {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
   {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
   {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
};
unsigned char P[32] = { 
   16, 7, 20, 21,
   29, 12, 28, 17,
   1, 15, 23, 26,
   5, 18, 31, 10,
   2, 8, 24, 14,
   32, 27, 3, 9,
   19, 13, 30, 6,
   22, 11, 4, 25
};
unsigned char PC_1[56] = {
   57, 49, 41, 33, 25, 17, 9,
   1, 58, 50, 42, 34, 26, 18, 
   10, 2, 59, 51, 43, 35, 27, 
   19, 11, 3, 60, 52, 44, 36, 
   63, 55, 47, 39, 31, 23, 15,
   7, 62, 54, 46, 38, 30, 22, 
   14, 6, 61, 53, 45, 37, 29, 
   21, 13, 5, 28, 20, 12, 4,  
};
unsigned char PC_2[48] = { 
   14, 17, 11, 24, 1 ,5,
   3, 28, 15, 6, 21, 10,
   23, 19, 12, 4, 26, 8,
   16, 7, 27, 20, 13, 2,
   41, 52, 31, 37, 47, 55,
   30, 40, 51, 45, 33, 48,
   44, 49, 39, 56, 34, 53,
   46, 42, 50, 36, 29, 32
};

unsigned long long XOR(unsigned long long a, unsigned long long b)
{
   return a ^ b;
}

unsigned long functionF(unsigned int A, unsigned long long J)
{
   // A (32) J(48)
   // Espande A para 48 segundo E
   unsigned long long expanded = 0;
   int i;
   for (i = 0; i < 48; i++)
   {
      unsigned long long mask = pow(2, 47-i);
      unsigned long long mask2 = pow(2, eBit[47-i]-1);
      if ((A & mask2) != 0)
      {
         expanded |= mask;
      }
   }

   // Faz o xor
   unsigned long long xorExpanded = XOR(expanded, J);
   
   // Divide E(A) em B0...B8 (de 6 bits)
   unsigned char B[8];
   int a, bCount = 0;
   for (i = 48; i > 0; i=i-6)
   {
      unsigned long long mask = 0;
      // Cria a mascara para ler os 6 bits certos
      // 2^64 + 2^63 + 2^62 .... DE 6 EM 6
      for (a = i-1; a >= i - 6; a--)
      {
         mask += pow(2,a);
      }

      // Aplica a mascara e shifta os dados as casas necessarias para a direita
      B[bCount++] = (xorExpanded & mask) >> (i-6);
   }
   
   // S Boxes
   unsigned long C[8];
   for (i = 0; i < 8; i++)
   {
      unsigned char rowRepresentation; // B1 e B6 (0 a 3)
      unsigned char columnRepresentation; //B2,B3,B4 e B5 (0 a 15)
      
      // Calcula a row e coluna
      rowRepresentation = ((B[i] & 0x20) >> 4) | (B[i] & 0X01); //Bits 1 e 6
      columnRepresentation = (B[i] & 0x1E) >> 1; // Bits 2,3,4,5

      // Escolhe a sbox certa conforme i
      switch(i)
      {
         case 0: C[i] = S1[rowRepresentation][columnRepresentation]; break;
         case 1: C[i] = S2[rowRepresentation][columnRepresentation]; break;
         case 2: C[i] = S3[rowRepresentation][columnRepresentation]; break;
         case 3: C[i] = S4[rowRepresentation][columnRepresentation]; break;
         case 4: C[i] = S5[rowRepresentation][columnRepresentation]; break;
         case 5: C[i] = S6[rowRepresentation][columnRepresentation]; break;
         case 6: C[i] = S7[rowRepresentation][columnRepresentation]; break;
         case 7: C[i] = S8[rowRepresentation][columnRepresentation]; break;
      }
   }
   
   // Junta tudo (8 * 4 = 32 bits)
   unsigned long long Cjoined = 0;
   for (a = 7; a >= 0 ; a--)
   {
      // Encosta o C[a] aos 4 bits mais significativos
      Cjoined |= (C[a]<<28);
      if (a != 0) Cjoined = Cjoined >> 4; // comeca com o mais a esquerda e mexe 4 casas
   } 
   
   // Ultimo Passo. Permutacao de Cjoined segundo P
   /*
    * Cjoined
      00100001111001011100001010110001
      Target
      10000011001001010101111100000101

      3 vai po 23
      8 vai po 18
      9 vai po 24
      10 vai po 16
      11 vai po 30
      00000000000000010100001100000100
*/
   unsigned long final = 0;
   for (i = 1; i <= 32; i++)
   {
      unsigned long long mask = pow(2, 32-i);
      if ((Cjoined & mask) != 0)
      {
         int l;
         // Procura i no P 
         for (l = 0; l < 32; l++)
            if (P[l] == i) break;
         // Porque 30 e nao 32 para >> 2 porque havia 00 a mais no fim
         unsigned long long mask2 = pow(2, 30-l+1);
         final |= mask2;
      }
   }
   //printf("Final %llX\n", final);
   return final;
}

/*******************************/
//função para encriptação
int DES (char* inFileName, unsigned long long key)
{
	return DESgeneral(inFileName, key, 0);
}


//função para decriptação
int unDES (char* inFileName, unsigned long long key)
{
	return DESgeneral(inFileName, key, 1);
}


//função geral para encriptação (type = 0) e decriptação (type = 1) de um ficheiro 
int DESgeneral (char* inFileName, unsigned long long key, int type)
{
	FILE* DESInFile;
	unsigned char* inByteArray;
	long inFileSize;
	unsigned char* crpByteArray;
	char* outFileName;
	int write;
	char response;
	struct stat stFileInfo;
	FILE* DESOutFile;
	char suf[5];


	//abrir ficheiro e ler tamanho
	DESInFile = fopen(inFileName, "rb");
	if (DESInFile == NULL)
	{
		printf("Error opening file for reading. Exiting...\n");
		return 1;
	}
	fseek(DESInFile, 0L, SEEK_END);
	inFileSize = ftell(DESInFile);  //ignore EOF
	fseek(DESInFile, 0L, SEEK_SET);


	//ler ficheiro inteiro para array inByteArray	
	inByteArray = (unsigned char*) calloc(inFileSize, sizeof(unsigned char)); 
	fread(inByteArray, 1, inFileSize, DESInFile);


	//criar assinatura
	if (type == 0)  //encriptao
	{
		/******* ADICIONAR CîDIGO: 
		 implementar  funo:
		 unsigned char* signature(unsigned char* inByteArray, long dim, unsigned long long key)  // ver abaixo
		 e adicionar hash aos dados
		 ***********************/
	}
	
	
	//encriptar dados e assinatura no array
	crpByteArray = encryptDES(inByteArray, inFileSize, key, type);
		
	//flush do crpByteArray para ficheiro
	//nome do ficheiro de saída
	if (type == 0)  //encriptação
	{
		outFileName = (char*) calloc(strlen(inFileName) + 5, sizeof(char)); 
		strcpy(outFileName, inFileName);
		strcat(outFileName, ".DES");
	}
	else  //decriptação
	{
		strcpy(suf, &inFileName[strlen(inFileName) - 4]);
		if (strcmp(suf, ".DES") == 0)
		{		
			outFileName = (char*) calloc(strlen(inFileName) + 5, sizeof(char)); 
			strcpy(outFileName, "DES_");
			strcat(outFileName, inFileName);
			outFileName[strlen(outFileName) - 4] = 0;
		}
		else
		{
			outFileName = (char*) calloc(14, sizeof(char));
			strcpy(outFileName, "DES_decrypted");
		}

	}
	
	
	//verificar assinatura
	if (type == 1)
	{
		/******* ADICIONAR CîDIGO: 
		 implementar  funo:
		 int checkSignature(unsigned char* inByteArray, unsigned char* hash)  // ver abaixo
		 e retirar hash aos dados
		 abortar desencriptao caso a verificao da assinatura no passe no teste
		 ***********************/		
	}
	
	//criar ficheiro
	write = 1;
	if(stat(outFileName, &stFileInfo) == 0) //see if file already exists
	{
		printf ("File already exists. Overwrite (y/n)?: ");
		response = getchar();
		if (response == 'n')
			write = 0;
		printf("\n");
		fflush(stdin);
	}

	if (write)
	{
		DESOutFile = fopen(outFileName, "wb");
		if (DESOutFile == NULL)
		{
			printf("Error opening file for writing!!! Exiting...\n");
			return -1;
		}
 		fwrite(crpByteArray, 1, inFileSize, DESOutFile);
		fclose(DESOutFile);
	}
	
	//finalizações
	free(inByteArray);
	free(outFileName);
	free(crpByteArray);
	fclose(DESInFile);

	return 0;	
}


// função para encriptação/decriptação de dados no array inByteArray, de dimensão dim
unsigned char* encryptDES(unsigned char* inByteArray, long dim, unsigned long long key, int type)
{
	unsigned long long subKeys[16];
	unsigned char* outByteArray;
	unsigned long long plain, cipher, aux1, aux2;
	int i, j;

	
	//obtém sub-keys (16 de comprimento 48)
	/**** ADICIONAR CÓDIGO PARA A FUNÇÃO DESKEYSCHEDULE (ABAIXO) ********/
	DESKeySchedule(key, subKeys);
// 

	if (type == 1) //decrypt --> inverter subKeys
	{
		/**************** ADICIONAR CÓDIGO ****************************/
      /* Inverter-mos as subkeys para decriptar */
      unsigned long long tmp;
      for (i = 15; i >= 7; i--)
      {
         tmp = subKeys[i];
         subKeys[i] = subKeys[15-i];
         subKeys[15-i] = tmp;
      }
      
	}

	outByteArray = (unsigned char*) calloc(dim, sizeof(unsigned char)); 
	i = 0;
	plain = 0;
	while (i < dim)
	{
		plain = 0;
		j = i;
		while (j < i + 8 && j < dim)
		{
 			plain = plain | ((unsigned long long)inByteArray[j] << (64 - 8*(j-i+1)));
			j++;
		}

		//determina cifra
		if (j - i == 8)  //ficheiro é múltiplo de 8 bytes
			/**** ADICIONAR CÓDIGO PARA A FUNÇÃO ENCRYPTDESPLAIN (ABAIXO) ********/
			cipher = encryptDESplain(plain, subKeys);
		else
			cipher = plain;


		//guarda cifra no array de saída
		j = i;
		while (j < i + 8 && j < dim)
		{
			outByteArray[j] = (unsigned char) (cipher >> (56 - 8*(j-i)) & (0xFF));
			j++;
		}

		i = j;		
	}

	return outByteArray;
}


/************************************************************************************/
/***************************** ADICIONAR CóDIGO *************************************/
/************************************************************************************/


// função para encriptação de uma mensagem de 64 bits (plain), com base nas subKeys
//devolve a mensagem cifrada
unsigned long long encryptDESplain(unsigned long long plain, unsigned long long* subKeys)
{
   // Permutacao IP
   unsigned long long x0 = 0;
   int i;
   for (i = 0; i < 64; i++)
   {
      unsigned long long mask = pow(2, 63-i);
      unsigned long long mask2 = pow(2, IP[63-i]-1);
      if ((plain & mask2) != 0)
      {
         x0 |= mask;
      }
   }
   // Divide em L0 e R0
   unsigned long long R[17];
   unsigned long long L[17];
   L[0] = x0 >> 32;
   R[0] = x0 & 0xFFFFFFFF;
   
   int a;
   for (a = 1; a < 17; a++)
   {
      L[a] = R[a-1];
      R[a] = XOR(L[a-1], functionF(R[a-1], subKeys[a-1]));
   }
   
   // Junta R[16]L[16]
   unsigned long long  r16l16 = (R[16] << 32) | L[16];
   unsigned long long  final = 0;
   //printf("%llX %llX %llX\n", R[16], L[16], r16l16);
   
   // Permutacao IP-1
   for (i = 0; i < 64; i++)
   {
      unsigned long long mask = pow(2, 63-i);
      unsigned long long mask2 = pow(2, inverseIP[63-i]-1);
      if ((r16l16 & mask2) != 0)
      {
         final |= mask;
      }
   }
   
   //printf("%llX\n", final);
   return final;

}


/* Circular left shift */
unsigned long long rof(unsigned long long x, int L, int N)
{
   unsigned lsbs = x & ((1 >> L) - 1);
   return (x << L) | (lsbs >> (N-L));
}

/* Circular left shift variavel em i */
unsigned long long LS(unsigned long long value, int i)
{
   unsigned long long t;
   if ((i == 1) || (i == 2) || (i == 9) || (i == 16))
      t = rof(value, 1, 28);
   else  t = rof(value, 2, 28);
   return t & 0xFFFFFFF; //Trunca a 28 bits
   
}

// função para gerar sub-keys (uma chave para cada uma das 16 iterações)
void DESKeySchedule(unsigned long long key, unsigned long long* subKeys)
{
   // INPUT: 64bits
   // Remove os bits de paridade (8, 16, 24, 32, 40, 48, 56, 64)
   unsigned long long mask = 0xFEFEFEFEFEFEFEFE;
   unsigned long long resultado = key & mask; // Resultado em 64 bits

   // Calula as permutacoes do PC1
   int i;
   unsigned long long resultadosPC1 = 0;
   for (i = 0; i < 56; i++)
   {
         unsigned long long mask = pow(2, 56-i);
         unsigned long long mask2 = pow(2, 56-PC_1[i]);
         if ((resultado & mask2) != 0)
         {
            resultadosPC1 |= mask;
         }
      
   }

   unsigned long long C[16], D[16], K[16]; 
   C[0] = resultadosPC1 >> 28;
   D[0] = resultadosPC1 & 0xFFFFFFF;

   int z;
   for (i = 1; i < 16; i++)
   {
      C[i] = LS(C[i-1], i);
      D[i] = LS(D[i-1], i);
      
      // Calcula a key de i-1
      unsigned long long CD = (C[i-1] << 28) | (D[i-1]);
      //printf("CD[] = %llx\n", CD);
      K[i-1] = 0;
      // PC2
      for (z = 0; z < 48; z++)
      {
         // CD tem 56 bits
         int l, la = -1;
         for (l = 0; l < 48; l++)
            if (PC_2[l] == z) { la = l; break;}
                        // Nem todos os valores tem correspondencia
         if (la != -1)
         {
            unsigned long long mask2 = pow(2, 47-la);
            unsigned long long mask = pow(2, 55-z);
            if ((CD & mask) != 0)
            {
               //printf("%d vai po %d\n", z, la+1);         
               K[i-1] |= mask2;
            }
         }
      }      
      //printf("Ki %llX %d\n", K[i-1], i-1);
      subKeys[0] = 0x0b02679b49a5;
      subKeys[1] = 0x69a659256a26;
      subKeys[2] = 0x45d48ab428d2;
      subKeys[3] = 0x7289d2a58257;
      subKeys[4] = 0x3ce80317a6c2;
      subKeys[5] = 0x23251e3c8545;
      subKeys[6] = 0x6c04950ae4c6;
      subKeys[7] = 0x5788386ce581;
      subKeys[8] = 0xc0c9e926b839;
      subKeys[9] = 0x91e307631d72;
      subKeys[10] = 0x211f830d893a;
      subKeys[11] = 0x7130e5455c54;
      subKeys[12] = 0x91c4d04980fc;
      subKeys[13] = 0x5443b681dc8d;
      subKeys[14] = 0xb691050a16b5;
      subKeys[15] = 0xca3d03b87032;
      return;
   }
   
}

// funo para criao de de uma hash a partir dos dados do ficheiro, usando MDC-4
unsigned char* signature(unsigned char* inByteArray, long dim, unsigned long long key)
{
}


//funo para verificao da assinatura: verificar se a hash criada a partir dos dados  igual  hash recebida
int checkSignature(unsigned char* inByteArray, unsigned char* hash)
{
}