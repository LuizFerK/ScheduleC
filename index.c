#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT 0 // valor fixo para a opção que finaliza a aplicação

//Struct que representa a data.
typedef struct {
	int day;
	int month;
	int year;
} Date;

// Estrutura que contém os campos dos registros da agenda
struct MREC {
  char name[30];
  Date birth; 
  char email[40];
  char phone[15];
  struct MREC *next;
  struct MREC *prev;
};

// Tipo criado para instanciar variaveis do tipo agenda
typedef struct MREC Contact;

// Carrega o conteúdo da agenda para a memória
void getContacts(Contact **head_ref) {
  FILE *fp = fopen("contacts.dat", "r+b");
  Contact *prev_ctt = *head_ref;
  Contact *ctt = (Contact*)malloc(sizeof(Contact));
  
  fread(prev_ctt, sizeof(prev_ctt), 1, fp);
  prev_ctt->next = NULL;
  prev_ctt->prev = NULL;

  while (fread(ctt, sizeof(ctt), 1, fp)) {
    ctt->prev = prev_ctt;
    ctt->next = NULL;
    prev_ctt->next = ctt;

    prev_ctt = ctt;
    ctt = (Contact*)malloc(sizeof(Contact));
  }
}

// Salva o conteúdo da agenda no arquivo
void saveContacts(Contact *contact) {
  if (contact == NULL) return;

  FILE *fp = fopen("contacts.dat", "w+b");

  fwrite(contact, sizeof(contact), 1, fp);
  
  while (contact->next != NULL) {
    contact = contact->next;
    fwrite(contact, sizeof(contact), 1, fp);
  }
  
  fclose(fp);
}

// Lista o conteudo da agenda (todos os campos)
void listContacts(Contact *contact) {
  if (contact == NULL) {
    printf("Empty list!\n\n");
    return;
  }

  printf("Contact list:\n");
  printf("%s\n", contact->name);
  
  while (contact->next != NULL) {
    contact = contact->next;
    printf("%s\n", contact->name);
  }

  printf("\n");
}

// Gera uma struct de contato
void buildContact(Contact **contact) {
  Contact *new_contact = *contact;

  char name[30];
  char email[40];
  char phone[15];
	Date birth;

  printf("Name: ");
  scanf("%s", name);
  strcpy(new_contact->name, name);

  printf("Birthday: ");
  scanf("%d/%d/%d", &birth.day, &birth.month, &birth.year);
  new_contact->birth.day = birth.day;
  new_contact->birth.month = birth.month;
  new_contact->birth.year = birth.year;

  printf("Phone: ");
  scanf("%s", phone);
  strcpy(new_contact->phone, phone);

  printf("Email: ");  
  scanf("%s", email);
  strcpy(new_contact->email, email);
}

// Permite o cadastro de um contato
void insContact(Contact **head_ref) {
  Contact *contact = *head_ref;
  Contact *new_contact = (Contact*)malloc(sizeof(Contact));

  printf("Create contact:\n");
  buildContact(&new_contact);

  if (*head_ref == NULL) {
    new_contact->prev = NULL;
    *head_ref = new_contact;
    return;
  }

  while (contact->next != NULL) contact = contact->next;

  contact->next = new_contact;
  new_contact->prev = contact;

  printf("\nSuccessfully created contact %s\n\n", new_contact->name);
}

// Permite consultar um contato da agenda por nome
void queryContact(Contact *contact) {
  if (contact == NULL) {
    printf("Empty list!\n\n");
    return;
  }

  char name[30];
  printf("Get contact:\nName: ");
  scanf("%s", name);
  
  while (contact != NULL) {
    if (strcmp(contact->name, name) != 0) {
      contact = contact->next;
    } else {
      break;
    }
  }

  if (contact == NULL) {
    printf("\nContact not found!\n\n");
    return;
  }

  printf("\nContact:\n%s\n\n", contact->name);
}

// Permite excluir um contato da agenda
void delContact(Contact **head_ref) {
  Contact *contact = *head_ref;
  if (contact == NULL) {
    printf("Empty list!\n\n");
    return;
  }

  char name[30];
  printf("Delete contact:\nName: ");
  scanf("%s", name);
  
  while (contact != NULL) {
    if (strcmp(contact->name, name) != 0) {
      contact = contact->next;
    } else {
      break;
    }
  }

  if (contact == NULL) {
    printf("\nContact not found!\n\n");
    return;
  }

  if (contact->prev != NULL ) contact->prev->next = contact->next;
  if (contact->next != NULL ) contact->next->prev = contact->prev;

  printf("\nSuccessfully deleted contact %s\n\n", contact->name);
  free(contact);
}

// Permite a atualização dos dados de um contato da agenda
void upContact(Contact **head_ref) {
  Contact *contact = *head_ref;
  if (contact == NULL) {
    printf("Empty list!\n\n");
    return;
  }

  char name[30];
  printf("Update contact:\nCurrent contact name: ");
  scanf("%s", name);
  
  while (contact != NULL) {
    if (strcmp(contact->name, name) != 0) {
      contact = contact->next;
    } else {
      break;
    }
  }

  if (contact == NULL) {
    printf("\nContact not found!\n\n");
    return;
  }

  Contact *up_contact = (Contact*)malloc(sizeof(Contact));

  printf("\nNew contact data:\n");
  buildContact(&up_contact);

  up_contact->next = contact->next;
  up_contact->prev = contact->prev;
  if (contact->prev != NULL ) contact->prev->next = up_contact;
  if (contact->next != NULL ) contact->next->prev = up_contact;

  printf("\nSuccessfully updated contact %s to %s\n\n", contact->name, up_contact->name);
  free(contact);
}

// Apresenta o menu da aplicação e retorna a opção selecionada
int menu() {
  int op = 10;

  printf("Schedule:\n");
  printf("1 - Create contact\n");
  printf("2 - Delete contact\n");
  printf("3 - Update contact\n");
  printf("4 - Get contact\n");
  printf("5 - List contacts\n");
  printf("%d - Exit\n\nOption: ", EXIT);
  scanf("%d", &op);
  printf("\n");

  return op;
}

// Programa principal
int main() {
  int op = 10;
  Contact *contact = (Contact*)malloc(sizeof(Contact));
  getContacts(&contact);

  while (op != EXIT) {
    op = menu();
    
    if (op == 1) insContact(&contact);
    if (op == 2) delContact(&contact);
    if (op == 3) upContact(&contact);
    if (op == 4) queryContact(contact);
    if (op == 5) listContacts(contact);
  }

  saveContacts(contact);

  return 0;
}
