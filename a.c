#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}


struct node{
    int data;
    struct node *next;
};
struct node *head = NULL;
struct node *current = NULL;
void push(int data) {
    struct node *link = (struct node*) malloc(sizeof(struct node));
    link->data = data;
    link->next = head;
    head = link;
}
struct node* pop() {
    struct node *tempLink = head;
    head = head->next;
    return tempLink;
}


void run_interpreter();


int main(){
    printf("    Welcome to my simple mathematical interpreter!\n");
    printf("    Valid characters are: + - * / %% ^ ()\n");
    printf("    To Exit please enter exit()\n");
    run_interpreter();
    return 0;
}

/*         input validation         */
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
        else if(!if_chars_valid(inp))
            printf("Invalid Characters! Try again.\n");
        else
            remove_spaces(inp), start_calculate(inp);

    }
}

void calc_exp();
void calc_pow();

void start_calculate(char* inp){
    int i = 0;
    char ch;
    while(*(inp+i) != '\n'){
        ch = *(inp+i);
        if(ch == '+' || ch == '-' || ch == '*' || ch == '/'
            || ch == '%'|| ch == '^' || ch == '(')
            push(ch), i++;
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
            push((int)f);   /*   Just integers for now!   */
        }
        else if(ch == ')'){
            calc_exp();
            i++;
        }
    }
    calc_exp();
    printf("%d\n", head->data);
}

int power(int x, unsigned int y)
{
    if (y == 0)
        return 1;
    else if (y%2 == 0)
        return power(x, y/2)*power(x, y/2);
    else
        return x*power(x, y/2)*power(x, y/2);
}
void calc_pow(){   //list is global
    struct node *ptr = head->next;
    struct node *prev_ptr = head;

    while(ptr->data != (int)'(' && ptr != NULL) {
        if(ptr->data == (int)'^'){
            prev_ptr->data = power(ptr->next->data, prev_ptr->data);
            prev_ptr->next = ptr->next->next;
            ptr = prev_ptr->next;
        }
        else
            prev_ptr = ptr, ptr = ptr->next;
    }
}
void calc_mdr(){   // * / %
    struct node *ptr = head->next;
    struct node *prev_ptr = head;

    while(ptr->data != (int)'(' && ptr != NULL) {
        if(ptr->data == (int)'*'){
            prev_ptr->data = (ptr->next->data * prev_ptr->data);
            prev_ptr->next = ptr->next->next;
            ptr = prev_ptr->next;
        }
        else if(ptr->data == (int)'/'){
            prev_ptr->data = (ptr->next->data / prev_ptr->data);
            prev_ptr->next = ptr->next->next;
            ptr = prev_ptr->next;
        }
        else if(ptr->data == (int)'%'){
            prev_ptr->data = (ptr->next->data % prev_ptr->data);
            prev_ptr->next = ptr->next->next;
            ptr = prev_ptr->next;
        }
        else
            prev_ptr = ptr, ptr = ptr->next;
    }
}
void calc_pm(){   // + -
    struct node *ptr = head->next;
    struct node *prev_ptr = head;

    while(ptr->data != (int)'(' && ptr != NULL) {
        if(ptr->data == (int)'+'){
            prev_ptr->data = (ptr->next->data + prev_ptr->data);
            prev_ptr->next = ptr->next->next;
            ptr = prev_ptr->next;
        }
        else if(ptr->data == (int)'-'){
            prev_ptr->data = (ptr->next->data - prev_ptr->data);
            prev_ptr->next = ptr->next->next;
            ptr = prev_ptr->next;
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