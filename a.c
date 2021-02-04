/** by Ali Mahzoun **/


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

/**     start interpreter    **/
void run_interpreter();
int main(){
    printf("    Welcome to my simple mathematical interpreter!\n");
    printf("    Valid characters are: + - * / %% ^ ()\n");
    printf("    To Exit please enter exit()\n");
    run_interpreter();
    return 0;
}


/**     input validation     **/
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
    } while (*s++ = *d++);
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
        else if(!if_chars_valid(inp))
            printf("Invalid Characters! Try again.\n");
        else
            remove_spaces(inp), start_calculate(inp);

    }
}


/**     all evaluations     **/
void calc_exp_p();
void calc_exp();
void start_calculate(char* inp){
    int i = 0;
    int alamat = 1;
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
            calc_exp_p();
            i++;
        }
    }
    calc_exp();
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
void calc_pow_p(){   //list is global
    struct node *ptr = head->next;
    struct node *prev_ptr = head;
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
}
void calc_mdr_p(){   // * / %
    struct node *ptr = head->next;
    struct node *prev_ptr = head;
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
}
void calc_pm_p(){   // + -
    struct node *ptr = head->next;
    struct node *prev_ptr = head;
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
}
void calc_exp_p(){
    calc_pow_p();
    calc_mdr_p();
    calc_pm_p();
    int temp = pop()->data;
    pop();
    push(temp);
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
    calc_pow();
    calc_mdr();
    calc_pm();
}
