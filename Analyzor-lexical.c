#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define MAX_NBR 5
#define MAX_LENGTH 30


int nbr_states,nbr_terminal,nbr_transition;
char initial_state,final_state[MAX_NBR],str[MAX_LENGTH];


typedef struct adjc{
    char name_state;
    char trans_value;
    struct adj *next;
}adj;

typedef struct one_state{
    char name_state;
    struct adj *adj_state;
    struct one_state* next;
}state;

state * head=NULL;


/////////////////////////////////////////////////////////////////////////////////////////////////////


void add_state(){
        state* fake_head=head,*read=(state*)malloc(sizeof(state));
        scanf(" %c",&read->name_state);
        read->adj_state=NULL;
        read->next=NULL;
        if(head==NULL){
            head=read;
            fake_head=head;
            printf("-");
      }else{
        while(fake_head->next!=NULL){
            fake_head=fake_head->next;
        }
            fake_head->next=read;
      }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////


void read_states(){
    for(int i=0;i<nbr_states;i++) add_state();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////


void add_tran(){
    char s1,s2,tran;
     scanf(" %c %c %c",&s1,&s2,&tran);
     state* fake_head=head,*stat=NULL;
     while(fake_head!=NULL){
      if(fake_head->name_state==s1) stat=fake_head;
      fake_head=fake_head->next;
     }
      adj * read=(adj*)malloc(sizeof(adj));
      read->name_state=s2;
      read->trans_value=tran;
      read->next=NULL;
      if(stat->next==NULL){
            stat->adj_state=read;
      }else{
            adj *transition=stat;
        while(transition->next!=NULL){
            transition=transition->next;
        }
            transition->next=read;
      }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////


void read_trans(){
 for(int i=0;i<nbr_transition;i++) {printf("  ==> ");add_tran();}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////


void display(){
    state *fake_head=head;
    adj *tran;
    while(fake_head!=NULL){
        tran=fake_head->adj_state;
        while(tran!=NULL){
            printf("  %c -- %c --> %c \n",fake_head->name_state,tran->trans_value,tran->name_state);
            tran=tran->next;
        }
        fake_head=fake_head->next;
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////


void read_str(){
    FILE* f= fopen("tp.txt","r");
    if(f==NULL){
        printf(" ** ERROR OPZNINF FILE ** \n");
    }else{
        fscanf(f,"%s",str);
    }
    fclose(f);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

state* search_state(char c) {
    state* fake_head = head;
    while (fake_head != NULL) {
        if (fake_head->name_state == c) {
            return fake_head;
        }
        fake_head = fake_head->next;
    }
    return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

bool terminal(char c){
    for(int i=0;i<nbr_terminal;i++){
        if(final_state[i]==c) return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void testing(){
    FILE* f= fopen("tp.txt","r");
    if(f==NULL){
        printf(" ** ERROR OPENINE FILE ** \n");
    }else{
        char ch;
        state *q = head;
        adj *p  = q->adj_state;
        bool ok = 0;
        int idx = 0;
        char s[10000] = "";
        char buffer[1000] = "";
        int length = 0;
        if (fgets(buffer, sizeof(buffer), f) != NULL) {
            length = strlen(buffer);
        }
        fseek(f,0,SEEK_SET);
        while((ch = fgetc(f)) != EOF || length > idx){
            p = q->adj_state;
            while(p!=NULL){
                if(p->trans_value == ch){
                    ok = 1;
                    q = search_state(p->name_state);
                    break;
                }
                p = p->next;
            }

            //printf(" - %c - %c" , q->name_state,ch);
            if(!ok){
                fseek(f,idx,SEEK_SET);
                ch = fgetc(f);
                printf("\n\t\t%c == > Rejected\n",ch);
                idx++;
                ok = 0;
                p = head;
                strcpy(s, "");
            }else{
                strncat(s,&ch, 1);
                ok = 0;
                if(terminal(q->name_state) == 1){
                    idx = ftell(f);
                    printf("\n\t\t%s == > Accepted\n",s);
                    strcpy(s, "");
                    q = head;
                }
            }

        }

    }

}


int main()
{
    printf(" - the number of states ==> "); scanf("%d",&nbr_states);
    printf(" - the list of states ==> ");
    read_states();

    printf(" - the number of terminal states ==> "); scanf("%d",&nbr_terminal);
    printf(" - the list of terminal states ==> ");
    for(int i=0;i<nbr_terminal;i++) scanf(" %c",&final_state[i]);

    printf(" - the number of transition ==> "); scanf("%d",&nbr_transition);
    printf(" - the list of transition ==> "); read_trans();
    display();
    //read_str();
    //printf("%s",str);
    testing();
    return 0;

}
