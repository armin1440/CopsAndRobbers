#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void clear_matrix( int* , int , int );
void get_input(void);
void locate_robbers( int* , int , int);
void print_matrix( int* , int , int);
void locate_cops( int* , int , int , int , int);
int power( int , int);
int digit_counter( int);
double distance( int*  , int*  , int * ,  int  );
int * search_A_near_B5x5 ( int * ,  int , int * , int );
int cop_decision(int * ,  int *, int );
double degree ( int * , int *, int * , int);
void move(int * , int , int );
int robber_decision(int * , int * , int);
int tell_sb_move(int * , int * , int*  , int );
int is_Captured( int * , int , int );
int check( int* , int , int , int);
void show_arrested_robber( int* , int* , int , int);


int width,length,offices,cops_of_office[1000];


int main() {
    get_input();
    int playground[length][width];
    int state[length][width];
    int decision[length][width];
    clear_matrix(playground , length , width);
    clear_matrix(state , length , width);
    clear_matrix(decision,length,width);
    srand(time(NULL));
    locate_robbers(playground , length , width );
    for ( int i = 1 ; i <= offices ; i++)
        locate_cops(playground , length , width , cops_of_office[i] , i);
    print_matrix(playground , length , width);
    printf("\n");
    int t=0 , num_of_robber_move = 0  , *robber_address , num_of_cops_move = 0;
    while (1) {
        if ( is_Captured(playground , length ,width) == 1)
        {
            sleep(2);
           // system("cls");
            print_matrix(playground , length , width);
            printf("\n\n");
            printf("**The robber is trapped** \n");
            printf("The robber moved %d times\n",num_of_robber_move);
            printf("The cops moved %d times \n",num_of_cops_move);
            return 14400;
        }
        for (int j = 0; j < length ; ++j)
        {
            for (int i = 0; i < width ; ++i)
            {
                robber_address = search_A_near_B5x5(&playground[j][i] , -11 , playground , width);
                if ( playground[j][i] > 0 && robber_address != 0 && distance(&playground[j][i] , robber_address ,  playground , width) < 2.9 )
                {
                    decision[j][i] = cop_decision(&playground[j][i] , playground ,width);
                    for (int k = 0; k < length ; ++k)
                    {
                        for (int l = 0; l < width ; ++l)
                        {
                            if ( playground[k][l] == playground[j][i] )
                            {
                                decision[k][l] = tell_sb_move(robber_address , &playground[k][l] , playground ,width);
                            }
                        }
                    }
                }
                else if (playground[j][i] > 0 && decision[j][i] == 0 )
                {
                    decision[j][i] = robber_decision(&playground[j][i] , playground , width);
                }
                if ( playground[j][i] < 0)
                {
                    decision[j][i] = robber_decision(&playground[j][i] , playground ,width);
                }
            }
        }
        for (int n = 0; n < length ; ++n)
        {
            for (int i = 0; i < width ; ++i)
            {
                if ( playground[n][i] < 0 )
                {
                    if ( decision[n][i] != 0 && num_of_robber_move == t)
                    {
                        num_of_robber_move += 1;
                        state[n][i] -= 1;
                        move(&playground[n][i] , decision[n][i] , width);
                        move(&state[n][i] , decision[n][i] , width);
                    }
                    if ( decision[n][i] == 0 && num_of_robber_move == t)
                    {
                        sleep(2);
                       // system("cls");
                        print_matrix(playground , length , width);
                        printf("\n\n");
                        printf("**The robber is trapped**\n");
                        printf("The robber moved %d times\n",num_of_robber_move);
                        printf("The cops moved %d times \n",num_of_cops_move);
                        return 14401;
                    }
                }
            }
        }
        for (int a = 0; a < length ; ++a)
        {
            for (int b = 0; b < width ; ++b)
            {
                if( playground[a][b] > 0 && state[a][b] == t )
                {
                    state[a][b] += 1;
                    if ( decision[a][b] != 0 && check( &playground[a][b] , 1 , decision[a][b] , width) != 1 )
                    {
                        num_of_cops_move += 1;
                    }
                    if ( check( &playground[a][b] , -1 , decision[a][b] , width ) != 1 )
                    {
                        move(&playground[a][b], decision[a][b], width);
                        move(&state[a][b], decision[a][b], width);
                    }
                    else
                    {
                        show_arrested_robber(&playground[a][b] , playground , decision[a][b] , width);
                        sleep(2);
                     //   system("cls");
                        print_matrix(playground , length , width);
                        printf("\n\n");
                        printf("**A cop wants to go to the current place of the robber**\n");
                        printf("The robber moved %d times\n",num_of_robber_move);
                        printf("The cops moved %d times \n",num_of_cops_move);
                        return 14402;
                    }
                }
            }
        }
        clear_matrix(decision,length,width);
        t++;
        sleep(2);
      //  system("cls");
        print_matrix(playground , length , width);
        printf("\n\n");
    }
}


int is_Captured( int * playground , int f_lenght , int f_width )
{
    for (int k = 0 ; k < f_lenght ; k++ )
        for ( int u = 0 ; u < f_width ; u++ ) {
            if (*(playground + k * f_width + u) < 0) {
                int * robber_address = playground + k * f_width + u;
                for (int i = -1; i <= 1; i++)
                    for (int j = -1; j <= 1; ++j) {
                        if (distance(robber_address + i * f_width + j, robber_address, playground, f_width) < 1.5 &&
                            *(robber_address + i * f_width + j) == 0)
                            return 0;
                    }
            }
        }
    return 1;
}


void show_arrested_robber( int* police_address , int* playground , int move_num , int f_width)
{
    switch (move_num) {
        case 1:
            if (*(police_address - 1 - f_width) < 0)
                *(police_address - 1 - f_width) = -99;
        case 2:
            if (*(police_address - f_width) < 0)
                *(police_address - f_width) = -99;
        case 3:
            if (*(police_address + 1 - f_width) < 0)
                *(police_address + 1 - f_width) = -99;
        case 4:
            if (*(police_address - 1) < 0)
                *(police_address - 1 ) = -99;
        case 5:
            if (*(police_address + 1) < 0)
                *(police_address + 1) = -99;
        case 6:
            if (*(police_address - 1 + f_width) < 0)
                *(police_address - 1 + f_width) = -99;
        case 7:
            if (*(police_address + f_width) < 0)
                *(police_address + f_width) = -99;
        case 8:
            if (*(police_address + 1 + f_width) < 0)
                *(police_address + 1 + f_width) = -99;
    }
    *(police_address) = 0;
}




int check(int * police_address ,int check_num ,  int move_num , int f_width)
{
    if ( check_num < 0)
    {
        switch (move_num) {
            case 1:
                if (*(police_address - 1 - f_width) < 0)
                    return 1;
                return 0;
            case 2:
                if (*(police_address - f_width) < 0)
                    return 1;
                return 0;
            case 3:
                if (*(police_address + 1 - f_width) < 0)
                    return 1;
                return 0;
            case 4:
                if (*(police_address - 1) < 0)
                    return 1;
                return 0;
            case 5:
                if (*(police_address + 1) < 0)
                    return 1;
                return 0;
            case 6:
                if (*(police_address - 1 + f_width) < 0)
                    return 1;
                return 0;
            case 7:
                if (*(police_address + f_width) < 0)
                    return 1;
                return 0;
            case 8:
                if (*(police_address + 1 + f_width) < 0)
                    return 1;
                return 0;
        }
    }
    else{
        switch (move_num) {
            case 1:
                if (*(police_address - 1 - f_width) > 0)
                    return 1;
                return 0;
            case 2:
                if (*(police_address - f_width) > 0)
                    return 1;
                return 0;
            case 3:
                if (*(police_address + 1 - f_width) > 0)
                    return 1;
                return 0;
            case 4:
                if (*(police_address - 1) > 0)
                    return 1;
                return 0;
            case 5:
                if (*(police_address + 1) > 0)
                    return 1;
                return 0;
            case 6:
                if (*(police_address - 1 + f_width) > 0)
                    return 1;
                return 0;
            case 7:
                if (*(police_address + f_width) > 0)
                    return 1;
                return 0;
            case 8:
                if (*(police_address + 1 + f_width) > 0)
                    return 1;
                return 0;
        }
    }
}



int tell_sb_move( int * robber_address , int * police_address , int * playground ,  int f_width)
{
    double direction;
    direction = degree(police_address , robber_address , playground , f_width);
    if ( (direction >= 358 || direction <= 2) && *(police_address+1) <= 0 )
        return 5;
    if ( direction > 2 && direction < 88 && *(police_address+1-f_width) <= 0)
        return 3;
    if ( direction >= 88 && direction <= 92 && *(police_address-f_width) <= 0)
        return 2;
    if ( direction > 92 && direction < 178 && *(police_address-1-f_width) <= 0)
        return 1;
    if ( direction >= 178 && direction <= 182 && *(police_address-1) <= 0)
        return 4;
    if ( direction > 182 && direction < 268 && *(police_address-1+f_width) <= 0 )
        return 6;
    if ( direction >= 268 && direction <= 272 && *(police_address+f_width) <= 0)
        return 7;
    if ( direction > 272 && direction < 358 && *(police_address+1+f_width) <= 0 )
        return 8;
    return 0;
}



void move(int * Address , int move_num , int f_width)
{
    int temp ;
    switch( move_num ) {
        case 0:
            // doesn't move.
            break;
        case 1:
            if ( *(Address - 1 - f_width) == 0 ) {
                temp = *(Address);
                *(Address) = *(Address - 1 - f_width);
                *(Address - 1 - f_width) = temp;
            }
            break;
        case 2:
            if ( *(Address  - f_width) == 0 ) {
                temp = *(Address);
                *(Address) = *(Address - f_width);
                *(Address - f_width) = temp;
            }
            break;
        case 3:
            if ( *(Address + 1 - f_width) == 0 ) {
                temp = *(Address);
                *(Address) = *(Address - f_width + 1);
                *(Address - f_width + 1) = temp;
            }
            break;
        case 4:
            if ( *(Address - 1 ) == 0 ) {
                temp = *(Address);
                *(Address) = *(Address - 1);
                *(Address - 1) = temp;
            }
            break;
        case 5:
            if ( *(Address + 1 ) == 0 ) {
                temp = *(Address);
                *(Address) = *(Address + 1);
                *(Address + 1) = temp;
            }
            break;
        case 6:
            if ( *(Address - 1 + f_width) == 0 ) {
                temp = *(Address);
                *(Address) = *(Address + f_width - 1);
                *(Address + f_width - 1) = temp;
            }
            break;
        case 7:
            if ( *(Address + f_width) == 0 ) {
                temp = *(Address);
                *(Address) = *(Address + f_width);
                *(Address + f_width) = temp;
            }
            break;
        case 8:
            if ( *(Address + 1 + f_width) == 0 ) {
                temp = *(Address);
                *(Address) = *(Address + f_width + 1);
                *(Address + f_width + 1) = temp;
            }
            break;
    }
}



int robber_decision(int * robber_address , int * playground , int f_width )
{
    int num ;
    if( (robber_address - playground) % f_width == 0 )
    {
        while( robber_address == playground ) {
            num = rand()%3;
            if (*(robber_address + f_width + 1) == 0 && num== 0)
                return 8;
            if (*(robber_address + 1) == 0 && num == 1)
                return 5;
            if (*(robber_address + f_width) == 0 && num == 2)
                return 7;
            if ( *(robber_address + 1) != 0 && *(robber_address + f_width + 1) != 0 && *(robber_address + f_width) != 0)
                return 0;
        }
        while( robber_address == playground + (length-1)*f_width )
        {
            num = rand()%3;
            if ( *(robber_address - f_width + 1) == 0 && num == 0)
                return 3;
            if ( *(robber_address + 1) == 0 && num == 1)
                return 5;
            if ( *(robber_address - f_width) == 0 && num == 2)
                return 2;
            if ( *(robber_address + 1) != 0 && *(robber_address - f_width + 1) != 0 && *(robber_address - f_width) != 0)
                return 0;        }
        while(1)
        {
            num = rand()%5;
            if ( *(robber_address - f_width + 1) == 0 && num == 0)
                return 3;
            if ( *(robber_address + 1) == 0 && num == 1)
                return 5;
            if ( *(robber_address - f_width) == 0 && num == 2)
                return 2;
            if ( *(robber_address + f_width + 1) == 0 && num == 3)
                return 8;
            if ( *(robber_address + f_width) == 0 && num == 4)
                return 7;
            if ( *(robber_address + 1) != 0 && *(robber_address + f_width + 1) != 0 && *(robber_address + f_width) != 0 && *(robber_address - f_width) != 0 && *(robber_address - f_width+1) != 0)
                return 0;
        }
    }
    if ( robber_address - playground < f_width )
    {
        while ( robber_address == playground + f_width - 1 )
        {
            num = rand()%3;
            if ( *(robber_address - 1) == 0 && num == 0 )
                return  4;
            if ( *(robber_address - 1 + f_width) == 0 && num == 1 )
                return  6;
            if ( *(robber_address + f_width) == 0 && num == 2 )
                return  7;
            if ( *(robber_address - 1) != 0 && *(robber_address + f_width - 1) != 0 && *(robber_address + f_width) != 0)
                return 0;
        }
        while(1)
        {
            num = rand()%5;
            if ( *(robber_address - 1) == 0 && num == 0)
                return 4;
            if ( *(robber_address + 1) == 0 && num == 1)
                return 5;
            if ( *(robber_address + f_width - 1) == 0 && num == 2)
                return 6;
            if ( *(robber_address + f_width + 1) == 0 && num == 3)
                return 8;
            if ( *(robber_address + f_width) == 0 && num == 4)
                return 7;
            if ( *(robber_address + 1) != 0 && *(robber_address + f_width + 1) != 0 && *(robber_address + f_width) != 0 && *(robber_address - 1) != 0 && *(robber_address + f_width - 1) != 0)
                return 0;
        }
    }
    if ( (robber_address + 1 - playground) % (f_width) == 0 )
    {
        while ( robber_address + 1 == playground + (length)*f_width  )
        {
            num = rand()%3;
            if ( *(robber_address - f_width - 1) == 0 && num == 0)
                return 1;
            if ( *(robber_address - f_width ) == 0 && num == 2)
                return 2;
            if ( *(robber_address - 1) == 0 && num == 1)
                return 4;
            if ( *(robber_address - f_width - 1) != 0 && *(robber_address - f_width) != 0 && *(robber_address - 1) != 0 )
                return 0;
        }
        while (1)
        {
            num = rand()%5;
            if ( *(robber_address - f_width - 1) == 0 && num == 0)
                return 1;
            if ( *(robber_address - f_width) == 0 && num == 1)
                return 2;
            if ( *(robber_address - 1) == 0 && num == 2)
                return 4;
            if ( *(robber_address + f_width - 1) == 0 && num == 3)
                return 6;
            if ( *(robber_address + f_width) == 0 && num == 4)
                return 7;
            if ( *(robber_address - f_width - 1) != 0 && *(robber_address - f_width) != 0 && *(robber_address - 1) != 0 && *(robber_address - 1 + f_width) != 0 && *(robber_address + f_width) != 0)
                return 0;
        }
    }
    if ( (robber_address - playground) > ( (length-1)*f_width) && (robber_address - playground) < (length)*f_width - 1 )
    {
        while (1)
        {
            num = rand()%5;
            if ( *(robber_address - f_width - 1) == 0 && num == 0)
                return 1;
            if ( *(robber_address - f_width) == 0 && num == 1)
                return 2;
            if ( *(robber_address - 1) == 0 && num == 2)
                return 4;
            if ( *(robber_address + 1) == 0 && num == 3)
                return 5;
            if ( *(robber_address - f_width + 1) == 0 && num == 4)
                return 3;
            if ( *(robber_address + 1) != 0 && *(robber_address - f_width + 1) != 0 && *(robber_address - f_width) != 0 && *(robber_address - 1) != 0 && *(robber_address - f_width - 1) != 0)
                return 0;
        }
    }
    while (1)
    {
        num = rand()%8;
        if ( *(robber_address - f_width - 1) == 0 && num == 0)
            return 1;
        if ( *(robber_address - f_width) == 0 && num == 1)
            return 2;
        if ( *(robber_address - 1) == 0 && num == 2)
            return 4;
        if ( *(robber_address - f_width + 1) == 0 && num == 3)
            return 3;
        if ( *(robber_address + 1) == 0 && num == 4)
            return 5;
        if ( *(robber_address + f_width - 1) == 0 && num == 5)
            return 6;
        if ( *(robber_address + f_width) == 0 && num == 6)
            return 7;
        if ( *(robber_address + f_width + 1) == 0 && num == 7)
            return 8;
        if ( *(robber_address - f_width - 1) != 0 && *(robber_address - f_width) != 0 && *(robber_address - f_width + 1) != 0 && *(robber_address - 1) != 0 && *(robber_address + 1) != 0 && *(robber_address + f_width - 1) != 0 && *(robber_address + f_width) != 0 && *(robber_address + f_width + 1) != 0 )
            return 0;
    }
}



int cop_decision ( int * his_Address , int * playground , int f_width )
{
    int* target;
    double direction ;
    target = search_A_near_B5x5(his_Address , -11 , playground , f_width);
    if ( target == 0 || distance(target , his_Address , playground , f_width) > 2.9  )
        return robber_decision(his_Address,playground,width);
    else {
        direction = degree(his_Address, target, playground, f_width);
        if ((direction >= 358 || direction <= 2) && *(his_Address + 1) <= 0)
            return 5;
        if (direction > 2 && direction < 88 && *(his_Address + 1 - f_width) <= 0)
            return 3;
        if (direction >= 88 && direction <= 92 && *(his_Address - f_width) <= 0)
            return 2;
        if (direction > 92 && direction < 178 && *(his_Address - 1 - f_width) <= 0)
            return 1;
        if (direction >= 178 && direction <= 182 && *(his_Address - 1) <= 0)
            return 4;
        if (direction > 182 && direction < 268 && *(his_Address - 1 + f_width) <= 0)
            return 6;
        if (direction >= 268 && direction <= 272 && *(his_Address + f_width) <= 0)
            return 7;
        if (direction > 272 && direction < 358 && *(his_Address + 1 + f_width) <= 0)
            return 8;
    }
    return 0;
}



double degree ( int * origin , int * destination ,  int * playground , int f_width)
{
    int row_dif , column_dif , row_O , row_D , column_O , column_D ;
    double tang;
    row_O = (origin - playground)/f_width+1;
    row_D = (destination - playground)/f_width+1;
    column_O = origin - playground - (row_O-1)*f_width +1;
    column_D = destination - playground - (row_D-1)*f_width +1;
    row_dif = row_D-row_O ;
    column_dif = column_D-column_O ;
    if ( column_dif == 0 && row_dif != 0 )
    {
        if ( row_dif < 0 )
            return 90;
        if ( row_dif > 0 )
            return 270;
    }
    tang = (double)row_dif/column_dif ;
    if ( tang == 0 )
    {
        if ( column_dif > 0 )
            return atan(tang)/3.141592*180 ;
        if ( column_dif < 0 )
            return atan(tang)/3.141592*180 + 180 ;
    }
    if ( tang > 0 )
    {
        if ( row_dif < 0 )
            return atan(-1*tang)/3.141592*180 + 180  ;
        if ( row_dif > 0 )
            return atan(-1*tang)/3.141592*180 + 360  ;
    }
    if ( tang < 0 )
    {
        if ( row_dif < 0 )
            return atan(-1*tang)/3.141592*180  ;
        if ( row_dif > 0 )
            return  atan(-1*tang)/3.141592*180 + 180  ;
    }

}



int* search_A_near_B5x5( int* B_address ,  int A , int * playground ,  int f_width )
{
    for ( int i =  -2   ; i <= 2  ; i++ )
    {
        for (int j = -2 ; j <= 2 ; j++ )
        {
            if (B_address + i*f_width + j != B_address && *(B_address + i*f_width + j) == A)
                return B_address + i*f_width + j;
        }
    }
    return 0;
}



double distance( int* A_address ,  int* B_address ,  int * playground ,int f_width  )
{
    int row_A , column_A , row_B , column_B , row_dif , column_dif ;
    double result;
    row_A = (A_address - playground)/f_width+1;
    row_B = (B_address - playground)/f_width+1;
    column_A = A_address - playground - (row_A-1)*f_width +1;
    column_B = B_address - playground - (row_B-1)*f_width +1;
    row_dif = abs(row_B-row_A);
    column_dif = abs(column_B-column_A);
    result = sqrt( power(row_dif,2) + power(column_dif,2) );
    return result;
}



int digit_counter(int num)
{
    int counter = 0;
    while(num > 0)
    {
        num /= 10;
        counter++;
    }
    return counter;
}



int power( int num , int power )
{
    int result = 1;
    for (int i = 0; i < power ; i++ )
    {
        result *= num;
    }
    return result;
}



void locate_cops(  int* playground , int f_length , int f_width , int cop , int cop_office)
{
    int x,y;
    for (int i = 0; i < cop ; i++)
    {
        x = rand()%f_length;
        y = rand()%f_width;
        if ( *(playground+x*f_width+y) == 0 )
            *(playground+x*f_width+y) = power(10 , cop_office);
        else
        {
            i -= 1;
            continue;
        }
    }
}



void print_matrix(int* matrix , int f_length , int f_width)
{
    for (int i = 0 ; i < f_length ; i++)
    {
        for (int j = 0; j < f_width ; j++ )
        {
            if (*(matrix+i*f_width+j) == -99 )
            {
                printf("|  X  |");
            }
            if ( *(matrix+i*f_width+j) < 0 && *(matrix+i*f_width+j) != -99 )
                printf("|  T  |");
            if ( *(matrix+i*f_width+j) > 0 )
                printf("|  D%d |",digit_counter(*(matrix+i*f_width+j))-1);
            if ( *(matrix+i*f_width+j) == 0 )
                printf("|     |");
            if ( j == f_width-1 )
            {
                printf("\n");
                for (int k = 0; k < f_width*7 ; ++k)
                {
                    printf("-");
                }
                printf("\n");
            }
        }
    }
}



void locate_robbers( int* playground , int f_length , int f_width )
{
    int x,y;
    for ( int i = 0 ; i < 1 ; i++ )
    {
        x = rand()%f_length;
        y = rand()%f_width;
        if ( *(playground+x*f_width+y) == 0 )
            *(playground+x*f_width+y) = -11 ;
        else
        {
            i -= 1;
            continue;
        }
    }
}


void get_input(void)
{
    printf("Please enter length and width respectively :\n");
    scanf("%d %d",&length,&width);
    printf("Please enter the number of police stations :\n");
    scanf("%d",&offices);
    for ( int office_num = 1 ; office_num <= offices ; office_num++)
    {
        printf("Please enter the number of cops of the %dth police stations :\n",office_num);
        scanf("%d",&cops_of_office[office_num]);
    }
}


void clear_matrix( int *matrix , int f_length , int f_width  )
{
    for ( int i = 0 ; i < f_length*f_width ; i++)
        *(matrix+i) = 0;
}
