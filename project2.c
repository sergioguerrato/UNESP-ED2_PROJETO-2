/*** Sérgio Paulo Guerrato Alves - BCC UNESP - Rio Claro ***/
/*** Project 1: Files - Compression and Dinamic Recovery ***/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**Functions**/
void f_insert(FILE *);
void f_search(void);

/*=*=*= MAIN =*=*=*/
int main() {
  bool menu = 1;
  int option;
  FILE *insert, *remove;

  /*** Load Files ***/
  /*system("clear"); // for Linux */
  system("cls"); // for windows
  printf("\n\n\n    (insere.bin)\n    (remove.bin)\n\n    Loading Files...");

  if ((insert = fopen("insere.bin", "rb")) == NULL) {
    fprintf(stderr, "Insert file, opening error.\n\n");
    exit(-1);
  }

  getchar();
  /*** End Loading ***/

  /**Main Menu**/
  while (menu) {
    /*system("clear"); // for Linux */
    system("cls"); // for windows
    printf("\n\n     .=================== MENU ===================.\n");
    printf("     |                                            |\n");
    printf("     |    1. Insert                               |\n");
    printf("     |    2. Search                               |\n");
    printf("     |    3. Exit                                 |\n");
    printf("     |                                            |\n");
    printf("     |    Choose and option...                    |\n");
    printf("     |                                            |\n");
    printf("     '============================================'\n");
    printf("\n\n     Option: ");
    scanf("%d", &option);
    fflush(stdin);

    switch (option) {
    case 1:
      f_insert(insert);
      break;
    case 2:
      f_search();
      break;
    case 3:
      menu = 0;
      break;
    }
  }
  fclose(insert);
  /*system("clear"); // for Linux */
  system("cls"); // for windows
  return 0;
}
/*=*=*= END MAIN =*=*=*/

/*=*=*= INSERT =*=*=*/
void f_insert(FILE *insert) {
  /*system("clear"); // for Linux */
    system("cls"); // for windows

  /** Open data.bin to update, if don't exist, create **/
  FILE *data;
  if ((data = fopen("data.bin", "a+b")) == NULL) {
    fprintf(stderr, "Data file, opening error.\n\n");
    exit(-3);
  }

  /** Open index.bin to append, if don't exist, create **/
  FILE *index;
  if ((index = fopen("index.bin", "a+b")) == NULL) {
      fprintf(stderr, "Index file, opening error.\n\n");
      exit(-3);
  }


  /** Registers Struct **/
  typedef struct vehicle {
    char len;
    char cod_cli[12];
    char cod_vei[8];
    char client[50];
    char veiculo[50];
    char dias[4];
  };

  /** Search for number of regs (n)**/
  fseek(insert, 0, SEEK_SET);
  int n = 0;
  char c[124];
  while (!feof(insert)) {
    fread(&c, sizeof(char), 124, insert);
    if (!feof(insert))
      n++;
  }
  fseek(insert, 0, SEEK_SET);
  /** found n value! **/

  /** Store all data of insere.bin in a struct buffer**/
  int i = 0;
  struct vehicle reg[n];
  while (i < n) {

    fread(reg[i].cod_cli, sizeof(char) * 12, 1, insert);
    fread(reg[i].cod_vei, sizeof(char) * 8, 1, insert);
    fread(reg[i].client, sizeof(char) * 50, 1, insert);
    fread(reg[i].veiculo, sizeof(char) * 50, 1, insert);
    fread(reg[i].dias, sizeof(char) * 4, 1, insert);

    printf("\n\n%s,%s,%s,%s,%s\n\n", reg[i].cod_cli, reg[i].cod_vei,
           reg[i].client, reg[i].veiculo, reg[i].dias);
    i++;
  }
  getchar();

  fseek(insert, 0, SEEK_SET);
  bool menu = 1;
  int num;
  int len; /* length */

  while (menu) {
    /*system("clear"); // for Linux */
    system("cls"); // for windows
    printf("\n\n ===== INSERT =====\n");
    printf("\n\n  Which register must be included?");
    printf("\n  (Type 0 to exit)");
    printf("\n  Number: ");
    scanf("%d", &num);
    getchar();
    if (num == 0)
      menu = 0;

    else if (num <= n) {

      /** Stores the length of the register on 'len' **/
      len = strlen(reg[num - 1].cod_cli) + strlen(reg[num - 1].cod_vei) +
            strlen(reg[num - 1].client) + strlen(reg[num - 1].veiculo) +
            strlen(reg[num - 1].dias);
      len = len + 6;

      /*** WRITE ***/
      num--;

      int byteos = ftell(data);
      fwrite(&byteos, 1, sizeof(int), index);

      char c_ = len + '0';
      fwrite(&c_, 1, sizeof(char), data);

      len = strlen(reg[num].cod_cli);
      fwrite(reg[num].cod_cli, len, sizeof(char), data);
      fwrite(reg[num].cod_cli, len, sizeof(char), index);
      c_ = '|';
      fwrite(&c_, 1, sizeof(char), data);

      len = strlen(reg[num].cod_vei);
      fwrite(reg[num].cod_vei, len, sizeof(char), data);
      fwrite(reg[num].cod_vei, len, sizeof(char), index);
      fwrite(&c_, 1, sizeof(char), data);

      len = strlen(reg[num].client);
      fwrite(reg[num].client, len, sizeof(char), data);
      fwrite(&c_, 1, sizeof(char), data);

      len = strlen(reg[num].veiculo);
      fwrite(reg[num].veiculo, len, sizeof(char), data);
      fwrite(&c_, 1, sizeof(char), data);

      len = strlen(reg[num].dias);
      fwrite(reg[num].dias, len, sizeof(char), data);
      fwrite(&c_, 1, sizeof(char), data);

      printf("\n\n Data saved!\n");
      getchar();
    }
  }

  fclose(data);
  fclose(index);
}

void f_search()
{
  /*system("clear"); // for Linux */
    system("cls"); // for windows

  /** Open index.bin to search **/
  FILE *index;
  if ((index = fopen("index.bin", "r+b")) == NULL) {
    fprintf(stderr, "Index file, opening error. File must not exist\n\n");
    exit(-5);
  }

  /** Open index.bin to search **/
  FILE *data;
  if ((data = fopen("data.bin", "r+b")) == NULL) {
    fprintf(stderr, "Data file, opening error. File must not exist\n\n");
    exit(-6);
  }

  /** Open busca_p.bin as source for search **/
  FILE *busca_p;
  if ((busca_p = fopen("busca_p.bin", "r+b")) == NULL) {
    fprintf(stderr, "busca_p file, opening error.\n\n");
    exit(-7);
 }

 typedef struct vehicle {
    int bos;
    char cod_cli[12];
    char cod_vei[8];
  };

 typedef struct dvehicle {
    char len;
    char cod_cli[12];
    char cod_vei[8];
    char client[50];
    char veiculo[50];
    char dias[4];
  };

 /** Search for number of keys (n)***/
 fseek(index, 0, SEEK_END);
 int n = ftell(index);
 n = n / 22;
 //printf("\n\nN: %d\n", n);
 fseek(index, 0 , SEEK_SET);
 /** Found n value! **/
 getchar();

 struct vehicle reg_b[n];
 struct vehicle reg_i[n];
 struct dvehicle reg_d;
 int i = 0;
 while (i <= n) {
	 
    fread(reg_b[i].cod_cli, 12 * sizeof(char), 1, busca_p);
    fread(reg_b[i].cod_vei, 8 * sizeof(char), 1, busca_p);
    reg_b[i].cod_cli[11] = '\0';
    reg_b[i].cod_vei[7] = '\0';

    printf("\n\nClient: %s, Vehicle: %s", reg_b[i].cod_cli, reg_b[i].cod_vei);
    i++;
 }

 getchar();
 int num;


 /*system("clear"); // for Linux */
  system("cls"); // for windows
  printf("\n\n ===== SEARCH =====\n");
  printf("\n\n  Which key you want to search");
  printf("\n  (Type 0 to exit)");
  printf("\n  Number: ");
  fflush(stdin);
  scanf("%d", &num);
  getchar();
  
  if (num > 0) {
  num--;

  char aux_buff1[12];
  char aux_buff2[8];
  strcpy(aux_buff1, reg_b[num].cod_cli);
  strcpy(aux_buff2, reg_b[num].cod_vei);
  getchar();  

  fseek(index, 0, SEEK_SET);
  while(!feof(index)){
    fread(&reg_i[i].bos, 1 * sizeof(int), 1 , index);
    fread(reg_i[i].cod_cli, 11 * sizeof(char), 1, index);
    fread(reg_i[i].cod_vei, 7 * sizeof(char), 1, index);
    reg_i[i].cod_cli[11] = '\0';
    reg_i[i].cod_vei[7] = '\0';

    printf("\n\n REG_I: %d, %s, %s\n", reg_i[i].bos, reg_i[i].cod_cli, reg_i[i].cod_vei);
	printf("\n REG_B: %s, %s\n", aux_buff1, aux_buff2);
	getchar();
    if ((strcmp(reg_i[i].cod_cli, aux_buff1) == 0) && (strcmp(reg_i[i].cod_vei, aux_buff2) == 0))  {
    
	  int byteos = reg_i[i].bos;
	  printf("\n\n BYTE OF SET (DATA.BIN): %d\n", byteos);
	  getchar();
      
      fseek(data, byteos, SEEK_SET);
      
      char c;
      char buffer[124];
      int position = ftell(data); /** Store atual location **/
      fread(&c, 1 * sizeof(char), 1, data); /** Read length **/
      int len = c - '0'; /** Convert to int **/
      fread(buffer, sizeof(char) * len, 1, data); /** Read the registration on a buffer **/

      printf("\n\nSEARCHING ON DATA.BIN\n");
      printf("\n\nSelected Register Searched by KEY: %s\n", buffer);
      getchar();
      break;
      }
    }
 }
}
