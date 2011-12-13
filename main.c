/*Author: Rui Pedro Paiva
Teoria da Informa��o, LEI, 2008/2009*/

int unDES (char* inFileName, unsigned long long key);
int DES (char* inFileName, unsigned long long key);

//fun��o principal, a qual gere todo o processo de encripta��o e decripta��o
int main(void)
{
	int erro;
	unsigned long long key;

	char fileName1[] = "FAQ.txt.gz";
	char fileName2[] = "FAQ.txt.gz.DES";

	//encripta��o
	key = 0x0123456789ABCDEF;
	erro = DES(fileName1, key);
	if (erro != 0)
		return erro;
		
	//decripta��o
	erro = unDES(fileName2, key);

	return erro;
}
