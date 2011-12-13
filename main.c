/*Author: Rui Pedro Paiva
Teoria da Informação, LEI, 2008/2009*/

int unDES (char* inFileName, unsigned long long key);
int DES (char* inFileName, unsigned long long key);

//função principal, a qual gere todo o processo de encriptação e decriptação
int main(void)
{
	int erro;
	unsigned long long key;

	char fileName1[] = "FAQ.txt.gz";
	char fileName2[] = "FAQ.txt.gz.DES";

	//encriptação
	key = 0x0123456789ABCDEF;
	erro = DES(fileName1, key);
	if (erro != 0)
		return erro;
		
	//decriptação
	erro = unDES(fileName2, key);

	return erro;
}
