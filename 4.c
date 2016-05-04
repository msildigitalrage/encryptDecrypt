/*
 ============================================================================
 Name        : 4.c
 Author      :
 Version     :
 Copyright   : thomas.zafeiropoulos
 Description : cryptography encrypt / decrypt, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <unistd.h>
#include <string.h>
gmp_randstate_t stat;
#define BASE 256//oloi oi chars se integer
#define MAXCHARS 21
int main(void) {
	long sd = 0;
	int t = 20;
	int s,j_rab;//miller
	int result = 0; //miller
	//metavlites gia metatropi keimenou se int k tubalin
	char mystring[MAXCHARS];//my text to encrypt - decrypt hope so
    long int str_len;
    char c;
    mpz_t max_int, c_int, str_int, encrypted,decrypted;
    mpz_init(max_int); mpz_init(c_int); mpz_init(str_int);mpz_init(encrypted); mpz_init(decrypted);
	mpz_t psi, d, n_minus_one;//miller
mpz_t n_prime,n,three,two,a,one,p,q,phi,p_minus_one,q_minus_one,e,gcd,d_priv,t2;
mpz_t seed;
mpz_t ro;//for encry-decry
gmp_randinit(stat,GMP_RAND_ALG_LC,120);
mpz_init(n_prime);
mpz_init(n);//iniatialize
mpz_init(three);
mpz_init(a);
mpz_init(two);
mpz_init(one);
mpz_init(seed);
mpz_init(psi);//for miller-rabin
mpz_init(p);
mpz_init(q);
mpz_init(phi);
mpz_init(p_minus_one);
mpz_init(q_minus_one);
mpz_init(e);
mpz_init(gcd);
mpz_init(d_priv);
mpz_init(ro);
mpz_init(t2);
mpz_set_str(three, "3", 10);
mpz_set_str(two, "2", 10);
mpz_set_str(one, "1", 10);
srand((unsigned)getpid()); //initialize stat
sd = rand();
mpz_set_ui(seed,sd);
gmp_randseed(stat,seed);
int countpq=0;//0 primes pros to paron, kantous 2 (p kai q)
int i = 0;
//printf("Give a message (till %d chars):\n",MAXCHARS-1);
//fgets(mystring,MAXCHARS,stdin);
//

FILE *fp;            /* declare the file pointer */
 fp = fopen ("file.txt", "r");
 while(fgets(mystring, MAXCHARS, fp) != NULL)
    { sscanf (mystring, "%d"); }
fclose(fp);
//
do{ // mehri na vreis 2 prime

do{//RANDOM NUMBER
	mpz_urandomb(n_prime,stat,512);//create a random number
}while((mpz_even_p(n_prime))&& (n_prime<=three));//checking n to be >=3 && n be odd

mpz_init(n_minus_one); //initialize
mpz_sub_ui(n_minus_one, n_prime, 1);//n_minus_one = n-1
	s = 0;
	mpz_init_set(d, n_minus_one);
	while (mpz_even_p(d)) {// gia oso ine artios
		mpz_fdiv_q_2exp(d, d, 1); //shift right
		s++;//inc s
	}
	for (i = 0; i < t; ++i) {
		do{
		mpz_urandomb(a,stat,20);//create random number
		}while(!((a<=(n_prime-two)) && (a>=two)));//checking a must be (2<=a<=n-2)
		 mpz_powm(psi,a,d,n_prime);
		 if(mpz_cmp(psi,one)!=0 && mpz_cmp(psi,n_minus_one)){
			 j_rab=1;
			 while(j_rab<s &&  mpz_cmp(psi,n_minus_one)){
				 mpz_mul(psi,psi,psi); // y^2
				 mpz_mod(psi,psi,n_prime); //psi= psi^2 mod n
				 	 if(mpz_cmp(psi,one)==0){//if y=1
				 		  result = 1; goto exit_miller;
				 	 }
				 	 j_rab++;
			 }
		 	 if(mpz_cmp(psi,n_minus_one)!=0){//if y=n-1
		 		 result = 1; goto exit_miller;
		 	 }
		 }//end external if
}//end for

	if(result!=1){ countpq++; //an ine prime tote save
	if(countpq==1){mpz_set(p,n_prime);}//save p
	else{
		mpz_set(q,n_prime);}//save q
	}
	exit_miller: result = 0;
	if(mpz_cmp(p,q)==0){countpq=0;}//an p kai q idioi pame pali
}while(countpq<2);

	gmp_printf ("p = %Zd\n", p);
	gmp_printf ("q = %Zd\n", q);
mpz_mul(n,p,q); //calculate p*q
	gmp_printf ("n = p*q = %Zd\n", n);
mpz_sub(p_minus_one,p,one);
mpz_sub(q_minus_one,q,one);
	gmp_printf ("p-1 = %Zd\n", p_minus_one);
	gmp_printf ("q-1 = %Zd\n", q_minus_one);
mpz_mul(phi,p_minus_one,q_minus_one);
	gmp_printf ("phi = %Zd\n", phi);

	do{
	mpz_urandomm(e,stat,phi);//create random number e opou < tou phi
    mpz_add(e,e,two);//add two to be bigger the e from ena
	mpz_gcd(gcd,e,phi);
	}while((!(mpz_cmp(gcd,one)==0)));//checking..gcd=1
		gmp_printf ("e = %Zd\n", e);
		gmp_printf ("gcd = %Zd\n", gcd);
	mpz_invert(d_priv,e,phi);//ypologismos antistrofou e mod phi
		gmp_printf ("private key (d) = %Zd\n", d_priv);
			gmp_printf ("public key (n,e) = (%Zd , %Zd)\n", n,e);
////// convert myText to myIntegerText
		    str_len = strlen(mystring);
		    if(mystring[str_len - 1] == '\n')
		       mystring[str_len - 1] = '\0';
		       str_len = strlen(mystring);
		       printf("%s -> %ld \n", mystring, str_len);
		              for(i = str_len - 1; i >= 0; i--){
		                  c = mystring[i];
		                  mpz_mul_ui(ro,ro,BASE); // r=r*BASE
		                  mpz_add_ui(ro, ro, c);     // r=r+c
		              	  }//now ro is mystring as Integers
		     gmp_printf("My text is: %s and has %ld chars.\nAs Integer is:%Zd",mystring, strlen(mystring), ro);
////// encrypt text
		     mpz_powm(encrypted,ro,e,n);//
		     gmp_printf("\nEncrypted message is: %Zd",encrypted);
		     ////
//// create encrypted file
		     fp= fopen("encrypted_file.txt","w");
		     fprintf(fp, encrypted);
		     fclose(fp);
		     ////
////// decrypt text
		     mpz_powm(decrypted,encrypted,d_priv,n);//
		     gmp_printf("\nDecrypted message is: %Zd",decrypted);
////// convert myIntegerText to myText
		     mpz_set(str_int, ro);//integerText to mytext
		     mpz_set_ui(max_int, 1UL);//larger int set
		     for(i = 0; i < 10; i++){// maxlength =100
		         if(mpz_cmp(str_int, max_int) <= 0){
		        	str_len = i;
		        	break;}
		         mpz_mul_ui(max_int, max_int, BASE);}
		     for(i = 0; i < str_len; i++){
		     mpz_mod_ui(c_int, str_int,BASE);        // ekxoreitai sthn metablhth c_int=str_int mod BASE
		     mpz_sub(str_int, str_int, c_int);
		     mpz_tdiv_q_ui(str_int, str_int,BASE);
		     mystring[i] = mpz_get_ui(c_int);}
		     mystring[str_len] = '\0';
		     //printf("Num of Chars--> %ld\n", str_len);
///////plaintext
		     gmp_printf("\nPlaintext message is: %s",mystring);
mpz_clear(n_prime);
mpz_clear(n);//clear
mpz_clear(three);
mpz_clear(a);
mpz_clear(two);
mpz_clear(seed);
mpz_clear(one);
mpz_clear(d);
mpz_clear(n_minus_one);
mpz_clear(psi);
mpz_clear(p);
mpz_clear(q);
mpz_clear(phi);
mpz_clear(p_minus_one);
mpz_clear(q_minus_one);
mpz_clear(e);
mpz_clear(gcd);
	mpz_clear(d_priv);
	mpz_clear(ro);
    mpz_clear(max_int);
    mpz_clear(c_int);
    mpz_clear(str_int);
	mpz_clear(t2);
	mpz_clear(encrypted);
	mpz_clear(decrypted);
	return 0;
}
