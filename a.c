/**                by Ali Mahzoun                **/
/**     mathematical expressions interpreter     **/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


/**     Data Structure     **/
struct node{
    int data;
    struct node *next;
};
struct node *head;
int len_list = 0;

void push(int data) {
    struct node *link = (struct node*) malloc(sizeof(struct node));
    link->data = data;
    link->next = head;
    head = link;
    len_list++;
}

struct node* pop() {
    struct node *tempLink = head;
    head = head->next;
    len_list--;
    return tempLink;
}

void print_list() {
    struct node *ptr = head;
    printf("[ ");
    while(ptr != NULL) {
        printf("%d ",ptr->data);
        ptr = ptr->next;
    }
    printf("] (len: %d)\n", len_list);
}


/**     main - start interpreter    **/
void run_interpreter();
int main(){
    printf("    Welcome to my simple mathematical interpreter!\n");
    printf("    Valid characters are: + - * / %% ^ ()\n");
    printf("    To Exit please enter exit()\n");
    run_interpreter();
    return 0;
}


/**     lexical and syntax check     **/
int if_chars_valid(char* inp){
    int i = 0;
    char ch;
    while(*(inp+i) != '\n'){
        ch = *(inp+i);
        if(ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%'
           || ch == '^' || ch == ')' || ch == '(' || isdigit(ch) || ch == ' ')
            i++;
        else
            return 0;
    }
    return 1;
}

void remove_spaces(char* s) {
    const char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while ((*s++ = *d++));
}

int if_correct_par(char* inp){
    int par = 0, i = 0;
    char ch;
    while(*(inp+i) != '\n'){
        ch = *(inp+i);
        i++;
        if(ch == '(')
            par++;
        else if(ch == ')')
        {
            par--;
            if(par < 0)
                return 0;
        }
    }
    if(par > 0)
        return 0;
    return 1;
}

void start_calculate(char*);
void run_interpreter(){
    char inp[120];
    while(1){
        printf(">>");
        fgets(inp, 256, stdin);
        if(strcmp(inp, "exit()\n") == 0)
            exit(0);
        else if(strcmp(inp, "print()\n") == 0)
            print_list();
        else if(strcmp(inp, "\n") == 0)
            printf("Empty input! Try again.\n");
        else if(!if_correct_par(inp))
            printf("Wrong pair of parentheses! Try again.\n");
        else if(!if_chars_valid(inp))
            printf("Invalid Characters! Try again.\n");
        else
            remove_spaces(inp), start_calculate(inp);
    }
}


/**     semantic check and output     **/
int calc_exp_p();
void calc_exp();
void start_calculate(char* inp){
    int i = 0, alamat = 1;
    char ch;

    while(*(inp+i) != '\n'){
        ch = *(inp+i);

        if(ch == '+' || ch == '*' || ch == '/'
            || ch == '%'|| ch == '^' || ch == '('){
            push(ch);
            i++;
        }

        else if(ch == '-'){
            push('+');
            i++;
            alamat = -1;
        }

        else if(isdigit(ch)){
            char temp[120] = "";
            int j = 0;
            while(isdigit(ch)) {
                temp[j++] = ch;
                temp[j] = '\0';
                i++;
                ch = *(inp+i);
            }
            double f = atof(temp);
            push((int)(f * alamat));   /*   Just integers for now!   */
            alamat = 1;
        }

        else if(ch == ')'){
            int t = calc_exp_p();
            if(t) {
                printf("Empty parentheses found! Try again!\n");
                head = NULL;
                len_list = 0;
                return;
            }
            i++;
        }
    }

    calc_exp();

    if(head->data == (int)'+' || head->data == (int)'-' || head->data == (int)'*' || head->data == (int)'/'
        || head->data == (int)'%' || head->data == (int)'^') {
        pop();
        len_list = 0;
        head = NULL;
        printf("Wrong expression. Try again\n");
    }
    else
        printf("%d\n", pop()->data);
}


/**     calculate inside of parentheses     **/
int power(int x, unsigned int y)
{
    if (y == 0)
        return 1;
    else if (y%2 == 0)
        return power(x, y/2)*power(x, y/2);
    else
        return x*power(x, y/2)*power(x, y/2);
}
int calc_pow_p(){   //list is global
    struct node *ptr = head->next;
    struct node *prev_ptr = head;
    if(head->data == (int)'('){   //empty parentheses
        pop();
        return 1;
    }
    while(ptr->data != (int)'(') {
        if(ptr->data == (int)'^'){
            prev_ptr->data = power(ptr->next->data, prev_ptr->data);
            prev_ptr->next = ptr->next->next;
            ptr = prev_ptr->next;
            len_list -= 2;
        }
        else
            prev_ptr = ptr, ptr = ptr->next;
    }
    return 0;
}
int calc_mdr_p(){   // * / %
    struct node *ptr = head->next;
    struct node *prev_ptr = head;
    if(head->data == (int)'('){   //empty parentheses
        pop();
        return 1;
    }
    while(ptr->data != (int)'(') {
        if(ptr->data == (int)'*'){
            prev_ptr->data = (ptr->next->data * prev_ptr->data);
            prev_ptr->next = ptr->next->next;
            ptr = prev_ptr->next;
            len_list -= 2;
        }
        else if(ptr->data == (int)'/'){
            prev_ptr->data = (ptr->next->data / prev_ptr->data);
            prev_ptr->next = ptr->next->next;
            ptr = prev_ptr->next;
            len_list -= 2;
        }
        else if(ptr->data == (int)'%'){
            prev_ptr->data = (ptr->next->data % prev_ptr->data);
            prev_ptr->next = ptr->next->next;
            ptr = prev_ptr->next;
            len_list -= 2;
        }
        else
            prev_ptr = ptr, ptr = ptr->next;
    }
    return 0;
}
int calc_pm_p(){   // + -
    struct node *ptr = head->next;
    struct node *prev_ptr = head;
    if(head->data == (int)'('){   //empty parentheses
        pop();
        return 1;
    }
    while(ptr->data != (int)'(') {
        if(ptr->data == (int)'+'){
            prev_ptr->data = (ptr->next->data + prev_ptr->data);
            prev_ptr->next = ptr->next->next;
            ptr = prev_ptr->next;
            len_list -= 2;
        }
        else
            prev_ptr = ptr, ptr = ptr->next;
    }
    return 0;
}

int calc_exp_p(){

    int isEmptyPar;

    isEmptyPar = calc_pow_p();
    if(isEmptyPar)
        return 1;

    isEmptyPar = calc_mdr_p();
    if(isEmptyPar)
        return 1;

    isEmptyPar = calc_pm_p();
    if(isEmptyPar)
        return 1;

    int temp = pop()->data;
    pop();
    push(temp);
    return 0;
}


/**     calculate last expresion without any parentheses     **/
void calc_pow(){   //list is global
    struct node *ptr = head->next;
    struct node *prev_ptr = head;
    while(len_list > 1 && ptr != NULL) {
        if(ptr->data == (int)'^'){
            prev_ptr->data = power(ptr->next->data, prev_ptr->data);
            prev_ptr->next = ptr->next->next;
            ptr = prev_ptr->next;
            len_list -= 2;
        }
        else
            prev_ptr = ptr, ptr = ptr->next;
    }
}
void calc_mdr(){   // * / %
    struct node *ptr = head->next;
    struct node *prev_ptr = head;
    while(len_list > 1 && ptr != NULL) {
        if(ptr->data == (int)'*'){
            prev_ptr->data = (ptr->next->data * prev_ptr->data);
            prev_ptr->next = ptr->next->next;
            ptr = prev_ptr->next;
            len_list -= 2;
        }
        else if(ptr->data == (int)'/'){
            prev_ptr->data = (ptr->next->data / prev_ptr->data);
            prev_ptr->next = ptr->next->next;
            ptr = prev_ptr->next;
            len_list -= 2;
        }
        else if(ptr->data == (int)'%'){
            prev_ptr->data = (ptr->next->data % prev_ptr->data);
            prev_ptr->next = ptr->next->next;
            ptr = prev_ptr->next;
            len_list -= 2;
        }
        else
            prev_ptr = ptr, ptr = ptr->next;
    }
}
void calc_pm(){   // + -
    struct node *ptr = head->next;
    struct node *prev_ptr = head;
    while(len_list > 1 && ptr != NULL) {
        if(ptr->data == (int)'+'){
            prev_ptr->data = (ptr->next->data + prev_ptr->data);
            prev_ptr->next = ptr->next->next;
            ptr = prev_ptr->next;
            len_list -= 2;
        }
        else
            prev_ptr = ptr, ptr = ptr->next;
    }
}

void calc_exp(){
    if(len_list < 2) {
        return;
    }
    calc_pow();
    calc_mdr();
    calc_pm();
}
