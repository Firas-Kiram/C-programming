#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_LISTE 10
#define MAX_RIGHT 10
#define MAX_SIZE 30

char Terminal[MAX_SIZE] , NTerminal[MAX_SIZE] , str[MAX_SIZE];
int T_count = 0, NT_count = 0 , useEpsilong = 0 ,findEpsilong = 0 , str_count = 0 , count[MAX_SIZE];
bool vis[256];


typedef struct typeRule {
    char rightPart[MAX_RIGHT];
    struct typeRule *nextRule;
} rule;

typedef struct typeNonterminal {
    char leftPart;
    int eps;
    char premier[MAX_SIZE];
    char suivant[MAX_SIZE];
    struct typeNonterminal *nextState;
    struct typeRule *nextRule;
} states;

typedef struct Suivant{
    int idx;
    char Arr[20];
    char Racine;
};

struct Suivant SV[256][20];

states * tete = NULL;


int j=0;
int k=0;


bool isTerminal(char x){
    for(int i=0;i<T_count;i++){
        if(Terminal[i] == x)
            return 1;
    }
    return 0;
}

bool isNTerminal(char x){
    for(int i=0;i<NT_count;i++){
        if(NTerminal[i] == x)
            return 1;
    }
    return 0;
}

void remove_hash(char* str) {
    int i, j;
    for (i=0,j=0;i<strlen(str);i++) {
        if (str[i] != '#') {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}


void unique(char* s) {
    int length = strlen(s),idx = 0;
    for (int i = 0; i < length; i++) {
        bool ok = false;
        for (int j = 0; j < idx; j++) {
            if (s[i] == s[j]) {
                ok = true;
                break;
            }
        }
        if (!ok) {s[idx++] = s[i];}
    }
    s[idx] = '\0';
}


states *search(char x){
    states * st = tete;
    while(st != NULL){
        if(st->leftPart == x){
            return st;
        }
        st = st->nextState;
    }
    return NULL;
}

bool haveEps(char *t){
    for(int i=0;i<strlen(t);i++){
            if(t[i] == '#') {
                    return true;
            }
    }
    return false;
}


void readInfo(){
    FILE *file = fopen("text.txt", "r");
    if (file == NULL) {printf("Failed to open the file.\n");return 1;}
    char buffer[100] = {0};
    fgets(buffer,100,file);
    for(int i=0;i<strlen(buffer);i++){
        if(buffer[i] != ' '){
            NTerminal[NT_count++] = buffer[i];
            ajouter_state(buffer[i] );
        }
    }
    fgets(buffer,100,file);
    for(int i=0;i<strlen(buffer);i++){
        if(buffer[i] != ' '){
            Terminal[T_count++] = buffer[i];
        }
    }
    //printf("%s \n",Terminal);
    //printf("%s \n",NTerminal);
    while (fgets(buffer, 100, file) != NULL){
        states* st = search(buffer[0]);
        char temp[100];
        int c = 0 , j = 3;
        while(buffer[j]!='\n' && buffer[j] != '\0'){
            if(buffer[j]=='#'){
                if(st != NULL){
                    st->eps = 1;
                }
            }
            temp[c++] = buffer[j++];
        }
        temp[c] = '\0';
       // printf("(%s)\n",temp);
        if(strlen(buffer)>=2){
            ajouter_rule(buffer[0],temp);
        }
    }

}


void ajouter_state(char t){

    states * st = tete, *p = (states*)malloc(sizeof(states));
    p->leftPart = t;
    p->eps = 0;
    p->nextRule = NULL;
    p->nextState = NULL;
    strcpy(p->premier,"");

    if(st == NULL){
        tete = p;
        st = tete;
    }else{
        while(st->nextState != NULL){
            st = st->nextState;
        }
        st->nextState = p;
    }
}


void ajouter_rule(char ch , char *temp){
    states * st = tete , * p = NULL;
    p = search(ch);
    rule * nov = (rule*)malloc(sizeof(rule));
    strcpy(nov->rightPart , temp);
    nov->nextRule = NULL;
    if(p->nextRule == NULL){
        p->nextRule = nov;
    }else{
        rule * l = p->nextRule;
        while(l->nextRule != NULL){
            l = l->nextRule;
        }
        l->nextRule = nov;
    }
}

void init_suive_tab(){
    states *st = tete;
    while(st != NULL){
        rule * p = st->nextRule;
        while(p){
            for(int i=0;i<strlen(p->rightPart);i++){
                if(isNTerminal(p->rightPart[i])){
                    states * var = search(p->rightPart[i]);
                    char ch = var->leftPart;
                    SV[ch][count[ch]].Racine = st->leftPart;
                    strcpy(SV[ch][count[ch]].Arr , p->rightPart);
                    SV[ch][count[ch]].idx = i;
                    count[ch]++;
                }
            }
            p = p->nextRule;
        }
        st = st->nextState;
    }
}


void suivant(states *st,states *anc){
    char tmp[100] = {};int c = 0;
    for(int i=0;i<count[st->leftPart];i++){
        int idx = SV[st->leftPart][i].idx;
        char* string = SV[st->leftPart][i].Arr;
        char Root = SV[st->leftPart][i].Racine;
        if(idx  == strlen(string) - 1 ){
            states *l = search(Root);
            strcat(tmp,l->suivant);
        }else{
            idx++;
            char ch = SV[st->leftPart][i].Arr[idx];
            if(isNTerminal(ch)){
                states *l = search(ch);
                strcat(tmp,l->premier);
                bool ok = 0;
                bool  check = haveEps(l->premier);
                if(l->eps == 1||check){
                    while(idx < strlen(string)){
                        ch = SV[st->leftPart][i].Arr[idx];
                        states *l = search(ch);
                        if(isNTerminal(ch)){
                            strcat(tmp,l->premier);
                            bool check2 = haveEps(l->premier);
                            if(l->eps != 1 && !check2){
                                break;
                            }else{
                                if(idx == strlen(string)-1){
                                    states *l2 = search(Root);
                                    strcat(tmp,l2->suivant);
                                }
                            }
                        }else{
                            char temp[2];
                            temp[0] = ch;
                            temp[1] = '\0';
                            strcat(tmp,temp);
                            ok = 1;
                            break;
                        }

                        idx ++;
                    }
                }else{
                    strcat(tmp,l->premier);
                }

            }else{
                char temp[2];temp[0] = ch;
                temp[1] = '\0';
                strcat(tmp,temp);
            }

        }

    }

    unique(tmp);
    remove_hash(tmp);
    strcat(st->suivant,tmp);
}


bool premier(states *st , states *anc) {
    if(vis[st->leftPart] == true) return;
    vis[st->leftPart] = true;
    rule * p = st->nextRule;
    states * l = NULL;
    bool res = false;
    while(p){
        int j = 0;
        //printf("Root : %c String : %s\n" ,st->leftPart, str);
        if(isTerminal(p->rightPart[j])){
            str[str_count++] = p->rightPart[j];
            str[str_count] = '\0';
        }else if(p->rightPart[j] == '#'){
            findEpsilong++;
            res = true;
        }else if(isNTerminal(p->rightPart[j])){
            l = search(p->rightPart[j]);
            if(l->eps == 1 || premier(l , st)){
                useEpsilong++;
                while((premier(l , st) || l->eps == 1) && j  < strlen(p->rightPart)){
                    if(isNTerminal(p->rightPart[j])){
                        l = search(p->rightPart[j]);
                    }else{
                        str[str_count++] = p->rightPart[j];
                        str[str_count] = '\0';
                        break;
                    }
                    j++;
                    if(l != NULL && l->eps == 1) premier(l , st);
                }
                res = true;
            }else{
                if(l != NULL) premier(l , st);
            }
        }

        if(useEpsilong < findEpsilong){
            str[str_count++] = '#';
            str[str_count] = '\0';
        }

        p = p->nextRule;
    }
    return res;
}

void P_Grammaire(){
    states * st = tete;
    while(st){
        rule * p = st->nextRule;
        printf("%c",st->leftPart);
        while(p){
            printf(" -- > %s",p->rightPart);
            p = p->nextRule;
        }
        printf(" -- > [/] \n");
        st = st->nextState;
    }
}

void P_TAB(){
    states * st = tete;
        printf("+-------------------------------------------------------------------------------+\n");
        printf("|   Non Terminal       |        Premier             |             Suivant       |\n");
        printf("+-------------------------------------------------------------------------------+\n");
    while(st){
        if(st->nextRule != NULL){
            printf("|        %c            |          %7s            |          %7s          |\n",st->leftPart , st->premier , st->suivant);
            printf("+-------------------------------------------------------------------------------+\n");
        }
            st = st->nextState;
    }

}



int main()
{
    readInfo();
    states * st = tete;
    while(st){
        memset(vis,false,sizeof(vis));
        premier(st , st);
        if(st->eps == 1){
            str[str_count++] = '#';
            str[str_count] = '\0';
        }
        unique(str);
        strcpy(st->premier , str);
        strcpy(str , "");
        findEpsilong = useEpsilong = 0;
        str_count = 0;
        st=st->nextState;
    }

    init_suive_tab();



    st = tete;
    char c[2];
    c[0] = '$';c[1] = '\0';
    strcat(st->suivant,c);
    while(st){
        suivant(st,st);
        st = st->nextState;
    }

    P_TAB();
    P_Grammaire();

    return 0;
}
