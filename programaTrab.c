/*
- Gabriel Sousa Santos de Almeida - 13837432
- Luís Henrique Giorgetti Dantas - 13782369
- João Gabriel Manfre Nazar - 13733652
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcionalidades/funcionalidades.h"


int main(){
    int variavelMenu;

    scanf("%d", &variavelMenu);

    switch(variavelMenu){
        case 1:{
            funcionalidade1();
            break;
        }
        case 2:{   
            funcionalidade2();
            break;
        }
        case 3:{
            funcionalidade3();
            break;
        }
        case 4:{
            funcionalidade4();
            break;
        }
    }

    return 0;
}