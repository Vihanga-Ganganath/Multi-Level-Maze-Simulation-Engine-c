
#include"game.h"

struct Player{
	short movement_points;
	short currentfloor;
	short currentwidth;
	short currentlength;
    short initialfloor;
	short initialwidth;
	short initiallength;
    short oldFloor;
    short oldWidth;
    short oldLength;
	short current_direction;
	short initial_direction;
	short player_counter;
	short die_move_result;
    short playerstate;
    short playerstatecount;
    char player_id;
};

struct Stair
{
    short startFloor;
    short startWidth;
    short startLength;
    short endFloor;
    short endWidth;
    short endLength;
};

struct Wall
{
    short floor;
    short startWidth;
    short startLength;
    short endWidth;
    short endLength;
};

struct Pole
{
    short startFloor;
    short endFloor;
    short blockWidth;
    short blockLength;
};

struct Stair*stairs=NULL;
int stairsCount=0;//number of correct stairs

struct Wall*walls=NULL;
int wallsCount=0;//number of correct walls

struct Pole*poles=NULL;
int polesCount=0;//number of correct poles

struct Player players[3];// which holds players and their attributes

short flag_floor;//flag position
short flag_width;
short flag_length;

short cellValue[3][10][25];// holds cell values
short bawanaArray[3][4];// holds bawana effects

short loopounter=0;//holds how many loops are running if stair pole loop there
int gamecounter=0;//holds game rounds

FILE *logs=NULL;
//after round position holder
short afloor=-1;
short awidth=-1;
short alength=-1;

short bfloor=-2;
short bwidth=-2;
short blength=-2;

short cfloor=-3;
short cwidth=-3;
short clength=-3;

int isstairAt(short stf,short enf,short stw,short enw,short stl,short enl){// check the validity of the stairs
    if(enf<stf){
        fprintf(logs,"Stair at [%hd, %hd, %hd, %hd, %hd, %hd] is ignored it is not initializet to lower floor to higher\n",stf,stw,stl,enf,enw,enl);
        return 1;
    }
    if((stf==1) && (stw>=0 && stw <=5) && (stl>=8 && stl<=16)){
        fprintf(logs,"Stair at [%hd, %hd, %hd, %hd, %hd, %hd] is ignored it is at first floor no cell area\n",stf,stw,stl,enf,enw,enl);
        return 1;
    }
    if((stf==1 && enf==2) && (enw>=0 && enw <=9) && (enl>=0 && enl<=7)){
        fprintf(logs,"Stair at [%hd, %hd, %hd, %hd, %hd, %hd] is ignored it is heading to second floor left no cell area\n",stf,stw,stl,enf,enw,enl);
        return 1;
    }
    if((stf==1 && enf==2) && (enw>=0 && enw <=9) && (enl>=17 && enl<=24)){
        fprintf(logs,"Stair at [%hd, %hd, %hd, %hd, %hd, %hd] is ignored it is heading to second floor right no cell area\n",stf,stw,stl,enf,enw,enl);
        return 1;
    }
    if((stf==2) && (stw>=0 && stw <=9) && (stl>=0 && stl<=7)){
        fprintf(logs,"Stair at [%hd, %hd, %hd, %hd, %hd, %hd] is ignored it is at second floor left no cell area\n",stf,stw,stl,enf,enw,enl);
        return 1;
    }
    if((stf==2) && (stw>=0 && stw <=9) && (stl>=17 && stl<=24)){
        fprintf(logs,"Stair at [%hd, %hd, %hd, %hd, %hd, %hd] is ignored it is at second floor right no cell area\n",stf,stw,stl,enf,enw,enl);
        return 1;
    }
    if((stf==2 && enw ==1) && (enw>=0 && enw <=5) && (enl>=8 && enl<=16)){
        fprintf(logs,"Stair at [%hd, %hd, %hd, %hd, %hd, %hd] is ignored it is at first floor no cell area\n",stf,stw,stl,enf,enw,enl);
        return 1;
    }
    if((stf==0 && enf==1) && (enw>=0 && enw <=5) && (enl>=8 && enl<=16)){
        fprintf(logs,"Stair at [%hd, %hd, %hd, %hd, %hd, %hd] is ignored it is heading to first floor no cell area\n",stf,stw,stl,enf,enw,enl);
        return 1;
    }
    if((stf==0 && enf==2) && (enw>=0 && enw <=9) && (enl>=0 && enl<=7)){
        fprintf(logs,"Stair at [%hd, %hd, %hd, %hd, %hd, %hd] is ignored it is heading to second floor left no cell area\n",stf,stw,stl,enf,enw,enl);
        return 1;
    }
    if((stf==0 && enf==2) && (enw>=0 && enw <=9) && (enl>=17 && enl<=24)){
        fprintf(logs,"Stair at [%hd, %hd, %hd, %hd, %hd, %hd] is ignored it is heading to second floor right no cell area\n",stf,stw,stl,enf,enw,enl);
        return 1;
    }
    return 0;
}

void loadStairs()// where the stairs loads to the stairs array
{
    FILE *file=fopen("stairs.txt","r");
    if(file==NULL)
    {
        fprintf(logs,"Couldn't open stairs.txt file\n");
        printf("Warning Couldn't open stairs.txt file\n");
        return;
    }

    int expected=10;
    int count=0;
    stairs=calloc(expected,sizeof(struct Stair));
    if(stairs==NULL)
    {
        fprintf(logs,"Couldn't allocate memory for stairs\n");
        printf("Warning memory allocation for stairs issue\n");
        return;
    }

    char lineholder[64];
    while(fgets(lineholder, sizeof(lineholder), file)){
        struct Stair linestore;
        int full_line=sscanf(lineholder,"[%hd, %hd, %hd, %hd, %hd, %hd]",
        &linestore.startFloor,
        &linestore.startWidth,
        &linestore.startLength,
        &linestore.endFloor,
        &linestore.endWidth,
        &linestore.endLength);
        
        if(full_line==6){
            if((linestore.startFloor>=0 && linestore.startFloor<=2) && (linestore.endFloor>=0 && linestore.endFloor<=2) && 
            (linestore.startWidth>=0 && linestore.startWidth<=9) && (linestore.endWidth>=0 && linestore.endWidth<=9) && 
            (linestore.startLength>=0 && linestore.startLength<=24) && (linestore.endLength>=0 && linestore.endLength<=24) &&
            (linestore.startFloor!=linestore.endFloor)){
                if(isstairAt(linestore.startFloor,linestore.endFloor,linestore.startWidth,linestore.endWidth,linestore.startLength,linestore.endLength)==1){
                        continue;
                    }
                    else{

                    stairs[count]=linestore;
                    count++;
                    if(count>=expected)
                    {
                        expected*=2;
                        stairs =realloc(stairs,expected*sizeof(struct Stair));
                        if(stairs==NULL)
                        {
                            fprintf(logs,"Memory reallocation for stairs Failed\n");
                            printf("Warning memory allocation for stairs issue\n");
                            free(stairs);
                            return;
                        }
                    }
                }
        }
            else{
                fprintf(logs,"stair [%hd, %hd, %hd, %hd, %hd, %hd] is ignored. not in range\n",linestore.startFloor,linestore.startWidth,linestore.startLength,linestore.endFloor,linestore.endWidth,linestore.endLength);
            }
        }
        else{
            fprintf(logs,"Stair [%hd, %hd, %hd, %hd, %hd, %hd] is ignored. not initialized correct\n",linestore.startFloor,linestore.startWidth,linestore.startLength,linestore.endFloor,linestore.endWidth,linestore.endLength);
            fscanf(file, "%*[^\n]\n");
        }
    }
    stairsCount=count;
    fclose(file);
    return;    
    
}

int isWallAt(short floor,short stw,short enw,short stl,short enl) {// where the cheking happenes for full maze walls and is there walls at player start areas
        if (floor == 0) {
            if (stw== enw) {
                if (5 == stw &&
                    12>= stl &&
                    12 <= enl) {
                    fprintf(logs,"Wall at [%hd, %hd, %hd, %hd, %hd] is ignored it blocks player start cell\n",floor,stw,stl,enw,enl);
                    return 1;
                }
                else if (9 == stw &&
                        7>= stl &&
                        7 <= enl) {
                        fprintf(logs,"Wall at [%hd, %hd, %hd, %hd, %hd] is ignored it blocks player start cell\n",floor,stw,stl,enw,enl);
                        return 1;
                }
                else if (9 == stw &&
                        17>= stl &&
                        17 <= enl) {
                        fprintf(logs,"Wall at [%hd, %hd, %hd, %hd, %hd] is ignored it blocks player start cell\n",floor,stw,stl,enw,enl);
                        return 1;
                }
                else if (9 == stw &&
                        19>= stl &&
                        19 <= enl) {
                        fprintf(logs,"Wall at [%hd, %hd, %hd, %hd, %hd] is ignored it blocks bawana start cell\n",floor,stw,stl,enw,enl);
                        return 1;
                }
                else if((stw>=7 && stw <=9) && (stl>=21 && stl<=24)&&(enl>=21 && enl <=24)){
                    fprintf(logs,"Wall at [%hd, %hd, %hd, %hd, %hd] is ignored its inside bawana\n",floor,stw,stl,enw,enl);
                    return 1;
                }
                else if((stw>=6 && stw <=9) && (stl>=8 && stl<=16)&&(enl>=8 && enl <=16)){
                    fprintf(logs,"Wall at [%hd, %hd, %hd, %hd, %hd] is ignored it is at starting area\n",floor,stw,stl,enw,enl);
                        return 1;
                }
                
            }
            else if (stl == enl) {
                if (12 == stl &&
                    5 >= stw &&
                    5 <= enw) {
                    fprintf(logs,"Wall at [%hd, %hd, %hd, %hd, %hd] is ignored it blocks player start cell\n",floor,stw,stl,enw,enl);
                    return 1;
                }
                else if (7 == stl &&
                        9 >= stw &&
                        9 <= enw) {
                        fprintf(logs,"Wall at [%hd, %hd, %hd, %hd, %hd] is ignored it blocks player start cell\n",floor,stw,stl,enw,enl);
                        return 1;
                }
                else if (17 == stl &&
                        9 >= stw &&
                        9 <= enw) {
                        fprintf(logs,"Wall at [%hd, %hd, %hd, %hd, %hd] is ignored it blocks player start cell\n",floor,stw,stl,enw,enl);
                        return 1;
                }
                else if (19 == stl &&
                        9 >= stw &&
                        9 <= enw) {
                        fprintf(logs,"Wall at [%hd, %hd, %hd, %hd, %hd] is ignored it blocks bawana entrence cell\n",floor,stw,stl,enw,enl);
                        return 1;
                }
                else if((stl>=21 && stl<=24)&&(stw>=7 && stw<=9)&&(enw>=7 && enw<=9)){
                    fprintf(logs,"Wall at [%hd, %hd, %hd, %hd, %hd] is ignored its inside bawana\n",floor,stw,stl,enw,enl);
                    return 1;
                }
                else if((stl>=8 && stl<=16)&&(stw>=7 && stw<=9)&&(enw>=7 && enw<=9)){
                    fprintf(logs,"Wall at [%hd, %hd, %hd, %hd, %hd] is ignored it is at starting area\n",floor,stw,stl,enw,enl);
                        return 1;
                }
                
            }
        }
        if ((floor == 1)&&(stw>=0 && stw<=5)&&(enw>=0 && enw<=5)&&(stl>=8 && stl <=16)&&(enl>=8 && enl<=16)){
            fprintf(logs,"Wall at [%hd, %hd, %hd, %hd, %hd] is ignored it is at first floor no cell area\n",floor,stw,stl,enw,enl);
                return 1;
        }
        if ((floor == 2)&&(stw>=0 && stw<=9)&&(enw>=0 && enw<=9)&&(stl>=0 && stl <=7)&&(enl>=0 && enl<=7)){
            fprintf(logs,"Wall at [%hd, %hd, %hd, %hd, %hd] is ignored it is at second floor left no cell area\n",floor,stw,stl,enw,enl);
                return 1;
        }
        if ((floor == 2)&&(stw>=0 && stw<=9)&&(enw>=0 && enw<=9)&&(stl>=17 && stl <=24)&&(enl>=17 && enl<=24)){
            fprintf(logs,"Wall at [%hd, %hd, %hd, %hd, %hd] is ignored it is at second floor right no cell area\n",floor,stw,stl,enw,enl);
                return 1;
        }
        if((stw== enw)&&(stl==0 && enl==24)){
            fprintf(logs,"Wall at [%hd, %hd, %hd, %hd, %hd] is ignored it runs trough the whole maze\n",floor,stw,stl,enw,enl);
            return 1;
        }
        if((stl == enl)&&(stw==0 && enw==9)){
            fprintf(logs,"Wall at [%hd, %hd, %hd, %hd, %hd] is ignored it runs trough the whole maze\n",floor,stw,stl,enw,enl);
            return 1;
        }
    return 0;
} 

void loadWalls()// whwre the walls loads to the walls array
{
        FILE *file=fopen("walls.txt","r");
        if(file==NULL)
        {
            fprintf(logs,"couldn't open walls.txt file\n");
            printf("Warning Couldn't open walls.txt file\n");
            return;
        }
        int expected=10;
        int count=0;
        walls=calloc(expected,sizeof(struct Wall));
        if(walls==NULL)
        {
            fprintf(logs,"couldn't allocate memory for walls\n");
            printf("Warning memory allocation for walls issue\n");
            return;
        }
        char lineholder[64];
        while(fgets(lineholder, sizeof(lineholder), file)){
            struct Wall linestore;
            int full_line=sscanf(lineholder,"[%hd, %hd, %hd, %hd, %hd]",
            &linestore.floor,
            &linestore.startWidth,
            &linestore.startLength,
            &linestore.endWidth,
            &linestore.endLength);
            
            if(full_line==5){
                if((linestore.floor>=0 && linestore.floor<=2) && 
                ((linestore.startWidth==linestore.endWidth) ||
                (linestore.startLength==linestore.endLength))){
                    if(isWallAt(linestore.floor,linestore.startWidth,linestore.endWidth,linestore.startLength,linestore.endLength)==1){
                        continue;
                    }
                    else{
                        walls[count]=linestore;
                        count++;
                        if(count>=expected)
                        {
                            expected*=2;
                            walls=realloc(walls,expected*sizeof(struct Wall));
                            if(walls==NULL)
                            {
                                fprintf(logs,"memory reallocation of walls failed\n");
                                printf("Warning memory allocation for walls issue\n");
                                free(walls);
                                return;
                            }
                        }
                    }
            }
            else{
                fprintf(logs,"wall [%hd, %hd, %hd, %hd, %hd] is ignored. Diagonal\n",linestore.floor,linestore.startWidth,linestore.endWidth,linestore.startLength,linestore.endLength);
            }
        }
        else{
            fprintf(logs,"wall [%hd, %hd, %hd, %hd, %hd] is ignored. not initialized correct\n",linestore.floor,linestore.startWidth,linestore.endWidth,linestore.startLength,linestore.endLength);
            fscanf(file, "%*[^\n]\n");
        }
    }
    wallsCount=count;
    fclose(file);
    return;
}

int isPoleAt(short stf,short enf,short bw,short bl){// check the validity of the poles
    if(stf==1 && (bw>=0 && bw<=5) && (bl>=8 && bl<=16)){
        fprintf(logs,"pole at [%hd, %hd, %hd, %hd] is ignored it starts from first floor no cell area\n",enf,stf,bw,bl);
        return 1;
    }
    if((stf==2 && enf==1) && (bw>=0 && bw<=9) && (bl>=0 && bl<=7)){
        fprintf(logs,"pole at [%hd, %hd, %hd, %hd] is ignored it is from second floor left no cell area to first floor\n",enf,stf,bw,bl);
        return 1;
    }
    if((stf==2 && enf==1) && (bw>=0 && bw<=9) && (bl>=17 && bl<=24)){
        fprintf(logs,"pole at [%hd, %hd, %hd, %hd] is ignored it is from second floor right no cell area to first floor\n",enf,stf,bw,bl);
        return 1;
    }
    if((stf==2 && enf==1) && (bw>=0 && bw<=5) && (bl>=8 && bl<=16)){
        fprintf(logs,"pole at [%hd, %hd, %hd, %hd] is ignored it is from second floor to first floor no cell area\n",enf,stf,bw,bl);
        return 1;
    }
    return 0;
}

void loadPoles(){// where the poles loads to the poles array
    FILE *file=fopen("poles.txt","r");
    if(file==NULL){
        fprintf(logs,"Error opening poles.txt\n");
        printf("Warning Couldn't open poles.txt file\n");
        return;
    }
    int expected=10;
    int count=0;
    poles=calloc(expected,sizeof(struct Pole));
    if(poles==NULL){
        fprintf(logs,"memory allocation for poles failed\n");
        printf("Warning memory allocation for poles issue\n");
        return;
    }
    char lineholder[64];
    while(fgets(lineholder, sizeof(lineholder), file)){ 
        struct Pole linestore;
        int full_line=sscanf(lineholder,"[%hd, %hd, %hd, %hd]",
        &linestore.endFloor,
        &linestore.startFloor,
        &linestore.blockWidth,
        &linestore.blockLength);
         
    if(full_line==4){
        if(((linestore.startFloor>0 && linestore.startFloor<=2) && 
                (linestore.endFloor<=2 && linestore.endFloor>=0)) &&
                ((linestore.blockWidth>=0 && linestore.blockWidth<=9) &&
                (linestore.blockLength>=0 && linestore.blockLength<=24 ))&&
                (linestore.startFloor!=linestore.endFloor)){
                    if(isPoleAt(linestore.startFloor,linestore.endFloor,linestore.blockWidth,linestore.blockLength)==1){
                        continue;
                    }
                    else{
                        poles[count]=linestore;            
                        count++;
                        if(count>=expected){
                            expected*=2;
                            poles=realloc(poles,expected*sizeof(struct Pole));
                            if(poles==NULL){
                                fprintf(logs,"memory reallocation for poles Failed\n");
                                printf("Warning memory allocation for poles issue\n");
                                free(poles);
                                return;
                            }   
                        }
                    }
            }
            else{
                fprintf(logs,"pole at line [%hd, %hd, %hd, %hd] is ignored. not in range\n",linestore.endFloor,linestore.startFloor,linestore.blockWidth,linestore.blockLength);
            }
        }
        else{
            fprintf(logs,"pole at line [%hd, %hd, %hd, %hd] is ignored. not initialized correct\n",linestore.endFloor,linestore.startFloor,linestore.blockWidth,linestore.blockLength);
            fscanf(file, "%*[^\n]\n");
        }
    }
    polesCount=count;
    fclose(file);
    return;
}
int loadSeed(){// getting seed to the game it should be unsigned int srand works with positive
    unsigned int seed;
    FILE *file=fopen("seed.txt","r");
    if(file==NULL){
        fprintf(logs,"Error opening seed.txt file\n");
        printf("Warning Couldn't open seed.txt file\n");
        return 1;
    }
        if(fscanf(file,"%u",&seed)!=1){
            fprintf(logs,"seed is not valid in the file\n");
        }
        fclose(file);
        return seed;
}
void loadFlag(){// loading flag to the flag variables
    FILE *file=fopen("flag.txt","r");
    if(file==NULL){
        fprintf(logs,"error openinng flag.txt file\n");
        printf("Warning Couldn't open flag.txt file\n");
        return;
    }
    if(fscanf(file,"[%hd, %hd, %hd]",&flag_floor,&flag_width,&flag_length)!=3){
        fprintf(logs,"flag is not valid in the file\n");
    }
    return;
}
void gameClean(){// where the memory freeing happens to the dinemically alocated arrays and closes the log file pointer
    free(stairs);
    free(walls);   
    free(poles);
    fclose(logs);
}

void flagCheck(){// where you check the flag position with restricted areas is its in restrited area game terminates
    if(flag_floor<0 || flag_width<0 || flag_length<0 ||
        flag_floor>2 || flag_width>9 || flag_length>24){ //cheking the bounndries in all floors
            printf("Flag not in valid cell\n");
            fprintf(logs,"Flag not in valid cell\n");
            printf("Exitting Game...\n");
            gameClean();
            exit(1);   
        }
    else if(flag_floor==0 && 
        (flag_width>=6 && flag_width<=9) && 
        (flag_length>=8 && flag_length<=16)){
            printf("Flag is in starting area \n");
            fprintf(logs,"Flag is in starting area \n");
            printf("Exitting Game...\n");
            gameClean();
            exit(1);
    }
    else if(flag_floor==0 && 
        (flag_width>=6 && flag_width<=9) && 
        (flag_length>=20 && flag_length<=24)){ 
            printf("Flag is in bawana\n");
            fprintf(logs,"Flag is in bawana\n");
            printf("Exitting Game...\n");
            gameClean();
            exit(1);
    }
    else if(flag_floor==1 && 
        (flag_width>=0 && flag_width<=5) && 
        (flag_length>=8 && flag_length<=16)){
            printf("Flag is initialized to 1st floor no cell area\n");
            fprintf(logs,"Flag is initialized to 1st floor no cell area\n");
            printf("Exitting Game...\n");
            gameClean();
            exit(1);
    }
    else if(flag_floor==2 && 
        (flag_width>=0 && flag_width<=5) && 
        (flag_length>=0 && flag_length<=7)){
            printf("Flag is in 2nd floor left no cell area\n");
            fprintf(logs,"Flag is in 2nd floor left no cell area\n");
            printf("Exitting Game...\n");
            gameClean();
            exit(1);
    }
    else if(flag_floor==2 && 
        (flag_width>=0 && flag_width<=9) && 
        (flag_length>=17 && flag_length<=24)){
            printf("Flag is in 2nd floor left no cell area\n");
            fprintf(logs,"Flag is in 2nd floor left no cell area\n");
            printf("Exitting Game...\n");
            gameClean();
            exit(1);

    }
    else{
        for(int i=0; i<wallsCount; i++){
        if((flag_floor==walls[i].floor)&&
        (flag_width>=walls[i].startWidth && flag_width<=walls[i].endWidth)&&
        (flag_length>=walls[i].startLength && flag_length<=walls[i].endLength)){
            printf("Flag can't be initialized because it is on the wall at [%hd, %hd, %hd, %hd, %hd]\n",walls[i].floor,walls[i].startWidth,walls[i].startLength,walls[i].endWidth,walls[i].endLength);
            fprintf(logs,"Flag can't be initialized because it is on the wall at [%hd, %hd, %hd, %hd, %hd]\n",walls[i].floor,walls[i].startWidth,walls[i].startLength,walls[i].endWidth,walls[i].endLength);
            printf("Exitting Game...\n");
            gameClean();
            exit(1);
        }
    }
    }
}

void cellValuesInitialitation(){ // where you initialize the cells values commented some parts for viva

    short localCellArr[750];
    for(int i=0;i<188;i++){
        localCellArr[i]=0;
    }
    for(int i=188;i<450;i++){
        localCellArr[i]=-((rand()%4)+1);
    }
    for(int i=450;i<637;i++){
        localCellArr[i]=(rand()%2)+1;
    }
    for(int i=637;i<712;i++){
        localCellArr[i]=(rand()%3)+3;
    }
    for(int i=712;i<750;i++){
        localCellArr[i]=((rand()%2)+10);
    }
    short cn=0;
    for(int i=0;i<3;i++){
        for(int j=0;j<10;j++){
            for(int k=0;k<25;k++){
                cellValue[i][j][k]=localCellArr[cn];
                cn++;
                }
            }
        }
        /*for(int i=0;i<3;i++){
            for(int j=0;j<10;j++){
                for(int k=0;k<25;k++){
                    printf("%hd",cellValue[i][j][k]);
                }
            printf("\n");
            }
        }*/

        for(int i=0;i<3;i++){
            for(int j=0;j<10;j++){
                for(int k=0;k<25;k++){
                    short a=(rand()%3);
                    short b=(rand()%10);
                    short c=(rand()%25);
                    short temp=cellValue[i][j][k];
                    cellValue[i][j][k]=cellValue[a][b][c];
                    cellValue[a][b][c]=temp;
                }
            }
        }

        /*for(int i=0;i<3;i++){ // print the randomized array
            for(int j=0;j<10;j++){
                for(int k=0;k<25;k++){
                    printf("%hd,",cellValue[i][j][k]);
                }
                printf("\n");
            }
            
        }*/

}
void bawanaArrayinitialization(){ // where you intialize the bawana cell values
    bawanaArray[0][0]=(rand()%2)+1;
    bawanaArray[0][1]=(rand()%2)+1;
    bawanaArray[0][2]=(rand()%2)+3;
    bawanaArray[0][3]=(rand()%2)+3;
    bawanaArray[1][0]=(rand()%2)+5;
    bawanaArray[1][1]=(rand()%2)+5;
    bawanaArray[1][2]=(rand()%2)+7;
    bawanaArray[1][3]=(rand()%2)+7;
    bawanaArray[2][0]=(rand()%4)+9;
    bawanaArray[2][1]=(rand()%4)+9;
    bawanaArray[2][2]=(rand()%4)+9;
    bawanaArray[2][3]=(rand()%4)+9;
}
int movementDie(struct Player *player){ // moment die function. if you call it  it will definitly cost you 2 movement points
    player->movement_points-=2;
    return (rand()%6)+1;
}
int directionDie(){// direction die funtion returns direction from numbers
    short a;
    a=(rand()%6)+1;
    if(a==2){
        printf("Changed his direction to North\n");
    }
    if(a==3){
        printf("Changed his direction to East\n");
    }
    if(a==4){
        printf("Changed his direction to South\n");
    }
    if(a==5){
        printf("Changed his direction to West\n");
    }
    return a;
}

void setting_player_start_cell(struct Player *player){// used for entering maze
        if(player->player_id=='A'){
        player->currentfloor=0;
        player->currentwidth=5;
        player->currentlength=12;
    }
    else if(player->player_id=='B'){
        player->currentfloor=0;
        player->currentwidth=9;
        player->currentlength=7;
    }
    else if(player->player_id=='C'){
        player->currentfloor=0;
        player->currentwidth=9;
        player->currentlength=17;
    }
    return;
}
const char *direction_to_words(struct Player *player){ //where direction die result converted to the its words
	
		if (player->current_direction==2){
                	return "North";
        	}	
        	else if (player->current_direction==3){
                	return "East";
        	}
        	else if (player->current_direction==4){
                	return "South";
        	}
        	else if (player->current_direction==5){
                	return "West";
        	}	
	
}
int mainFlowMomentDieSituation(struct Player*player){// checks some states and apply to the moment die results
    if(player->currentfloor!=player->initialfloor && player->currentwidth!=player->initialwidth && player->currentlength!=player->initiallength){
        if(player->playerstate==0 || player->playerstate==7 || player->playerstate==8 ||
            player->playerstate==9 || player->playerstate==10 || player->playerstate==11 || player->playerstate==12){
            player->die_move_result=player->die_move_result;
        }
        else if(player->playerstate==3 || player->playerstate==4){
            player->die_move_result=player->die_move_result;

        }
        return 0;
    }
}

int movementDieCall(struct Player *player){ //moment die calling and cheks the state accoding to state it calls moment die or not
    short result=movementDie(player);
    if(player->playerstate==5 && player->player_counter>0){
            printf("Player %c is triggered and rolls and %hd on the movement dice and will consider that as %hd \n",player->player_id,result,2* (result));
            result=2*result;
        }
    if(player->playerstate==1){
            result=0;
            printf("Player %c is still food poisoned and misses the turn\n",player->player_id);
            if(player->movement_points<0){
                player->movement_points=0;
            }
            return 0;
        }
    if(player->player_counter==0 && result!=6){
        player->player_counter=0;
        printf("%c is at the starting area and rolls %hd on the movement dice cannot enter the maze\n",player->player_id,result);
        return 1;
    }
    else if(player->player_counter==0 && result==6){
        player->player_counter++;
        if(player->playerstate==5){//prevent player tring to scape starting area triggerd when 3 rolls
            result=result/2;
        }
        player->die_move_result=result;
        setting_player_start_cell(player);
        printf("Player %c is at the starting area and rolls 6 on the movement dice and is placed on [%hd, %hd, %hd] of the maze.\n",player->player_id,player->currentfloor,player->currentwidth,player->currentlength);
        return 0;
    }
    else if(player->player_counter>0 && player->playerstate!=1){
        player->player_counter++;
        player->die_move_result=result;
        if(player->playerstate!=5){
            printf("%c rolled %d on movement die\n",player->player_id,result);
        }
        return 0;
    }
}

void initialitaionPart(){//game environment settingup handeller. creates game environment some codes are commented if there is a requiremnt when in vive so i can show 
    loadStairs();
    loadWalls();
    loadPoles();
    loadFlag();
    flagCheck();
    cellValuesInitialitation();
    bawanaArrayinitialization();
    /*for(int i=0; i<stairsCount;i++){
        printf("Stair %hd %hd %hd %hd %hd %hd\n",stairs[i].startFloor,stairs[i].startWidth,stairs[i].startLength,stairs[i].endFloor,stairs[i].endWidth,stairs[i].endLength);
    }
    for(int i=0; i<polesCount;i++){
        printf("pole %hd %hd %hd %hd\n",poles[i].endFloor,poles[i].startFloor,poles[i].blockWidth,poles[i].blockLength);
    }
    for(int i=0; i<wallsCount;i++){
        printf("wall %hd %hd %hd %hd %hd\n",walls[i].floor,walls[i].startWidth,walls[i].endWidth,walls[i].startLength,walls[i].endLength);
    }
    for(int i=0;i<3;i++){
        for(int j=0;j<4;j++){
            printf("%hd\t",bawanaArray[i][j]);
        }
        printf("\n");
    }*/
}

void playerManualInitialitation(){//player initial details added here
    players[0].player_id='A';
    players[0].currentfloor=0;
	players[0].currentlength=12;
	players[0].currentwidth=6;
    players[0].initialfloor=0;
    players[0].initiallength=12;
    players[0].initialwidth=6;
	players[0].current_direction=2;
	players[0].initial_direction=2;
	players[0].player_counter=0;
    players[0].movement_points=100;
    players[0].playerstate=0;
    players[0].playerstatecount=0;

	players[1].player_id='B';
    players[1].currentfloor=0;
	players[1].currentlength=8;
    players[1].currentwidth=9;
    players[1].initialfloor=0;
    players[1].initiallength=8;
    players[1].initialwidth=9;
    players[1].current_direction=5;
	players[1].initial_direction=5;
	players[1].player_counter=0;
    players[1].movement_points=100;
    players[1].playerstate=0;
    players[1].playerstatecount=0;

	players[2].player_id='C';
    players[2].currentfloor=0;
	players[2].currentlength=16;
    players[2].currentwidth=9;
    players[2].initialfloor=0;
    players[2].initiallength=16;
    players[2].initialwidth=9;
    players[2].current_direction=3;
	players[2].initial_direction=3;
	players[2].player_counter=0;
    players[2].movement_points=100;
    players[2].playerstate=0;
    players[2].playerstatecount=0;

}



int mainFlowDirectionDieSituation(struct Player*player){// cheks with player states and calls direction die
    if(player->playerstatecount>0 && (player->playerstate==1 || player->playerstate==3)){
        player->playerstatecount-=1;
    }
        
        if(player->playerstate==1 && player->playerstatecount==0){
            player->playerstate=0;
            printf("Player %c is now fit to proceed from the food poisoning episode\n",player->player_id);
        }
        if(player->playerstate==3 && player->playerstatecount==0){
            printf("Player %c has recovered from disorientation\n",player->player_id);
            player->playerstate=0;
        }
    
    if(player->player_counter>0){
        
        if(player->playerstate==3){
            player->initial_direction=player->current_direction;
            player->current_direction=directionDie();
            if(player->current_direction==1 || player->current_direction==6){
                player->current_direction=player->initial_direction;
            }
            
            printf("Player %c rolls and %hd on the movement dice and is disoriented and move in the %s \n",player->player_id,player->die_move_result,direction_to_words(player));
        }
        else{
            if((player->player_counter%4)==0){
                player->initial_direction=player->current_direction;
                player->current_direction=directionDie();
            }
        }
        return 0;
    }
}

void movePointCalForCells(struct Player *player){// calculating moment points accoding to cell values
    
    short cellvalue=cellValue[player->currentfloor][player->currentwidth][player->currentlength];
    if(cellvalue==0){
        player->movement_points+=0;
        printf("%c steped normal cell\n",player->player_id);
    }
    else if(cellvalue<0){
        player->movement_points+=cellvalue;
        printf("%c steped consumable cell value and lost movement points by %hd\n",player->player_id,cellvalue);
    }
    else if(cellvalue>0 && cellvalue<3){
        player->movement_points+=cellvalue;
        printf("%c steped bonus cell value and increse movement points by %hd\n",player->player_id,cellvalue);
    }
    else if(cellvalue>=3 && cellvalue<6){
        player->movement_points+=cellvalue;
        printf("%c steped bonus cell value and increse movement points by %hd\n",player->player_id,cellvalue);
    }
    else if(cellvalue==10 || cellvalue==11){
        if(cellvalue==10){
            player->movement_points*=2;
            printf("%c steped on multiplier cell and multiply movement points by 2\n",player->player_id);
        }
        if(cellvalue==11){
            player->movement_points*=3;
            printf("%c steped on multiplier cell and multiply movement points by 3\n",player->player_id);
        }
    }
}

int isWallInfront(struct Player *player){ // where you check is walls are bloking the moment
    for(int i=0; i<wallsCount; i++){
        if((player->currentfloor==walls[i].floor)&&
        (player->currentwidth>=walls[i].startWidth && player->currentwidth<=walls[i].endWidth)&&
        (player->currentlength>=walls[i].startLength && player->currentlength<=walls[i].endLength)){
            printf("Player %c rolls %d on the moment die cannot move in the %s because of a wall. Player will remain at his start cell\n",player->player_id,player->die_move_result,direction_to_words(player));
            player->die_move_result=0;
            return 1;
        }
        
        
    }
    return 0;
}
int isPoleInfront(struct Player *player){// where you check is poles are in the moment used recursion
    for(int i=0; i<polesCount; i++){
        if((player->currentfloor>0 && player->currentfloor<3) && 
        (player->currentwidth==poles[i].blockWidth) && 
        (player->currentlength==poles[i].blockLength)){
            loopounter++;
            player->currentfloor=poles[i].endFloor;
            player->currentwidth=poles[i].blockWidth;
            player->currentlength=poles[i].blockLength;
            printf("Player %c lands on [%hd, %hd] which is a pole cell. \n%c slides down and now placed at [%hd, %hd] in floor %hd\n",player->player_id,poles[i].blockWidth,poles[i].blockLength,player->player_id,poles[i].blockWidth,poles[i].blockLength,poles[i].endFloor);
            return 0;
        }
    }
}

int isStareInfront(struct Player *player,short a, short b, short c){ // where you check is stairs are in the moment used recrsion
    for(int i=0;i<stairsCount;i++){
        if((player->currentfloor==stairs[i].startFloor)&&
        (player->currentwidth==stairs[i].startWidth)&&
        (player->currentlength==stairs[i].startLength)){
            player->currentfloor=stairs[i].endFloor;
            player->currentwidth=stairs[i].endWidth;
            player->currentlength=stairs[i].endLength;
            loopounter++;
            printf("Player %c lands on [%hd, %hd, %hd] which is a stair cell.\nPlayer %c takes the stairs and now placed at [%hd, %hd] in floor %hd\n",player->player_id,stairs[i].startFloor,stairs[i].startWidth,stairs[i].startLength,player->player_id,stairs[i].endWidth,stairs[i].endLength,stairs[i].endFloor);
            isPoleInfront(player);
            if(loopounter>=8 || (player->currentfloor==a && player->currentwidth==b && player->currentlength==c)){
                printf("%c caught to a infinite loop so moving to starting area\n",player->player_id);
                player->oldFloor=player->initialfloor;// if player in infinite loop he will transport to standing area
                player->oldWidth=player->initialwidth;
                player->oldLength=player->initiallength;
                player->currentfloor=player->initialfloor;
                player->currentwidth=player->initialwidth;
                player->currentlength=player->initiallength;
                player->current_direction=player->initial_direction;
                player->player_counter=0;
                loopounter=0;
                return 0;
            }
            else{
                isStareInfront(player,a,b,c);
            }
            return 0;
        }
    }
}

int objectCheck(struct Player *player,short nextFl,short nextWid,short nextLen){// where the call happenes to check the walls poles and stairs and if player wins
    if(isWallInfront(player)==0){
        if(player->currentfloor==flag_floor && player->currentwidth==flag_width && player->currentlength==flag_length){
            printf("\t\t__________________________________________________________________");
            printf("\t...||player %c captured the flag at [%hd, %hd, %hd] and wins!||...",player->player_id,flag_floor,flag_width,flag_length);
            printf("\t__________________________________________________________________\n");
            fprintf(logs,"\t\t...||player %c wins!||...\n",player->player_id);
            gameClean();
            exit(0);
        }
        isPoleInfront(player);
        isStareInfront(player,nextFl,nextWid,nextLen);
            
    }
}

int boundryCheck(struct Player *player){// where the boundry cheking happenes like restricted ares and game boundries
    if(player->currentfloor<0 || player->currentwidth<0 || player->currentlength<0 ||
        player->currentfloor>2 || player->currentwidth>9 || player->currentlength>24){ 
            printf("%c tried to move out of bounds and stays in old position\n",player->player_id);
            player->currentfloor=player->oldFloor;
            player->currentwidth=player->oldWidth;
            player->currentlength=player->oldLength;
            player->die_move_result=0;
            return 1;
        }
    else if(player->currentfloor==0 && 
        (player->currentwidth>=6 && player->currentwidth<=9) && 
        (player->currentlength>=8 && player->currentlength<=16)){
            printf("%c tried to move standing area manually so stays in old position\n",player->player_id);
            player->currentfloor=player->oldFloor;
            player->currentwidth=player->oldWidth;
            player->currentlength=player->oldLength;
            player->die_move_result=0;
            return 1;
    }
    else if(player->currentfloor==0 && 
        (player->currentwidth>=6 && player->currentwidth<=9) && 
        (player->currentlength>=20 && player->currentlength<=24)){
            printf("%c tried to move to bawana manually so blocked\n",player->player_id);
            player->currentfloor=player->oldFloor;
            player->currentwidth=player->oldWidth;
            player->currentlength=player->oldLength;
            player->die_move_result=0;
            return 1;

    }
    else if(player->currentfloor==1 && 
        (player->currentwidth>=0 && player->currentwidth<=5) && 
        (player->currentlength>=8 && player->currentlength<=16)){
            printf("%c tried to move to 1st floor no cell area so blocked\n",player->player_id);
            player->currentfloor=player->oldFloor;
            player->currentwidth=player->oldWidth;
            player->currentlength=player->oldLength;
            player->die_move_result=0;
            return 1;

    }
    else if(player->currentfloor==2 && 
        (player->currentwidth>=0 && player->currentwidth<=5) && 
        (player->currentlength>=0 && player->currentlength<=7)){
            printf("%c tried to move 2nd floor left no cell area so blocked\n",player->player_id);
            player->currentfloor=player->oldFloor;
            player->currentwidth=player->oldWidth;
            player->currentlength=player->oldLength;
            player->die_move_result=0;
            return 1;

    }
    else if(player->currentfloor==2 && 
        (player->currentwidth>=0 && player->currentwidth<=9) && 
        (player->currentlength>=17 && player->currentlength<=24)){
            printf("%c tried to move 2nd floor right no cell area so blocked\n",player->player_id);
            player->currentfloor=player->oldFloor;
            player->currentwidth=player->oldWidth;
            player->currentlength=player->oldLength;
            player->die_move_result=0;
            return 1;

    }
    else{
        return 0;
    }
}

int canhego(struct Player *player, short a){// cheks if player can complete the moment according to game boundries
    if (player->current_direction==2)
    {
        player->currentwidth-=a;
        int b=boundryCheck(player);
        player->currentwidth+=a;
        return b;
    }
    else if(player->current_direction==3){
        player->currentlength+=a;
        int b=boundryCheck(player);
        player->currentlength-=a;
        return b;
    }
    else if(player->current_direction==4){
        player->currentwidth+=a;
        int b=boundryCheck(player);
        player->currentwidth-=a;
        return b;
    }
    else if(player->current_direction==5){
        player->currentlength-=a;
        int b=boundryCheck(player);
        player->currentlength+=a;
        return b;
    }
    
}

int willheblockbywall(struct Player *player,int a){// cheks moment by moment if the player will blocked by wall used where player took stair and he cant complete moment if wll there
    short c=player->currentwidth;
    short d=player->currentlength;
    if (player->current_direction==2)
    {
        while(a>0){
            player->currentwidth-=1;
            int b=isWallInfront(player);
            if(b==1){
                return b;
            }
            a--;
        }
        player->currentwidth=c;        
    }
    else if(player->current_direction==3){
        while(a>0){
        player->currentlength+=1;
        int b=isWallInfront(player);
        if(b==1){
            return b;
            }
            a--;
        }
        player->currentlength=d;
    }
    else if(player->current_direction==4){
        while(a>0){
            player->currentwidth+=1;
            int b=isWallInfront(player);
            if(b==1){
            return b;
            }
            a--;
        }
        player->currentwidth=c;
    }
    else if(player->current_direction==5){
        while(a>0){
            player->currentlength-=1;
            int b=isWallInfront(player);
            if(b==1){
                return b;
            }
            a--;
        }
        player->currentlength=d;
    }
}

void moveNorth(struct Player*player){//north moment happens here and also cheks if he can complete the moment and moment cal call 
    short dieholder=player->die_move_result;
    short tempmp=player->movement_points;
    if(player->player_counter!=1){
        while(player->die_move_result>0){

            

                if(boundryCheck(player)|| canhego(player,dieholder)==1 || willheblockbywall(player,dieholder)==1){
                    player->currentfloor=player->oldFloor;
                    player->currentwidth=player->oldWidth;
                    player->currentlength=player->oldLength;
                    player->die_move_result=0;
                    player->movement_points=tempmp;
                    loopounter=0;
                    break;
                }
                    short nextFloor  = player->currentfloor;
                    short nextWidth  = player->currentwidth - 1;
                    short nextLength = player->currentlength;

                    player->currentfloor = nextFloor;
                    player->currentwidth = nextWidth;
                    player->currentlength = nextLength;
                    objectCheck(player,nextFloor,nextWidth,nextLength);
                    loopounter=0;
                    movePointCalForCells(player);
                    if(player->movement_points>250){
                        player->movement_points=250;
                    }
                    if(player->movement_points<0){
                        player->movement_points=0;
                    }
                    player->die_move_result--;
            
        }
    }
}
void moveEast(struct Player*player){//east moment happens here and also cheks if he can complete the moment and moment cal call
    short dieholder=player->die_move_result;
    short tempmp=player->movement_points;
    if(player->player_counter!=1){
        while(player->die_move_result>0){

            

                if(boundryCheck(player)|| canhego(player,dieholder)==1 || willheblockbywall(player,dieholder)==1){
                    player->currentfloor=player->oldFloor;
                    player->currentwidth=player->oldWidth;
                    player->currentlength=player->oldLength;
                    player->die_move_result=0;
                    player->movement_points=tempmp;
                    loopounter=0;
                    break;
                }
                    short nextFloor  = player->currentfloor;
                    short nextWidth  = player->currentwidth;
                    short nextLength = player->currentlength +1 ;
                
                    player->currentfloor = nextFloor;
                    player->currentwidth = nextWidth;
                    player->currentlength = nextLength;
                    objectCheck(player,nextFloor,nextWidth,nextLength);
                    loopounter=0;
                    movePointCalForCells(player);
                    if(player->movement_points>250){
                        player->movement_points=250;
                    }
                    if(player->movement_points<0){
                        player->movement_points=0;
                    }
                    player->die_move_result--;
           
        }
    }
}
void moveSouth(struct Player*player){//south moment happens here and also cheks if he can complete the moment and moment cal call
    short dieholder=player->die_move_result;
    short tempmp=player->movement_points;
    if(player->player_counter!=1){
        while(player->die_move_result>0){

            
            
                if(boundryCheck(player)|| canhego(player,dieholder)==1 || willheblockbywall(player,dieholder)==1){
                    player->currentfloor=player->oldFloor;
                    player->currentwidth=player->oldWidth;
                    player->currentlength=player->oldLength;
                    player->die_move_result=0;
                    player->movement_points=tempmp;
                    loopounter=0;
                    break;
                }
                    short nextFloor  = player->currentfloor;
                    short nextWidth  = player->currentwidth + 1;
                    short nextLength = player->currentlength;

                    player->currentfloor = nextFloor;
                    player->currentwidth = nextWidth;
                    player->currentlength = nextLength;
                    objectCheck(player,nextFloor,nextWidth,nextLength);
                    loopounter=0;
                    movePointCalForCells(player);
                    if(player->movement_points>250){
                        player->movement_points=250;
                    }
                    if(player->movement_points<0){
                        player->movement_points=0;
                    }
                    player->die_move_result--;
            
        }
    }
}
void moveWest(struct Player*player){//west moment happens here and also cheks if he can complete the moment and moment cal call
    short tempmp=player->movement_points;
    short dieholder=player->die_move_result;
    if(player->player_counter!=1){
        while(player->die_move_result>0){

            
            
                if(boundryCheck(player)|| canhego(player,dieholder)==1 || willheblockbywall(player,dieholder)==1){
                    player->currentfloor=player->oldFloor;
                    player->currentwidth=player->oldWidth;
                    player->currentlength=player->oldLength;
                    player->die_move_result=0;
                    player->movement_points=tempmp;
                    loopounter=0;
                    break;
                }
                    short nextFloor  = player->currentfloor;
                    short nextWidth  = player->currentwidth;
                    short nextLength = player->currentlength - 1;

                    player->currentfloor = nextFloor;
                    player->currentwidth = nextWidth;
                    player->currentlength = nextLength;
                    objectCheck(player,nextFloor,nextWidth,nextLength);
                    loopounter=0;
                    movePointCalForCells(player);
                    if(player->movement_points>250){
                        player->movement_points=250;
                    }
                    if(player->movement_points<0){
                        player->movement_points=0;
                    }
                    player->die_move_result--;
                
            
            
        }
    }
}

const char *cellTypeToWords(struct Player *player){// where bawana cell efect gets to the words
    if(player->playerstate==1){
        return "food poisend";
    }
    else if(player->playerstate==3){
        return "disorented";
    }
    else if(player->playerstate==5){
        return "triggerd";
    }
    else if(player->playerstate==7){
        return "happy";
    }
    else if(player->playerstate==0 ){
        return "normal";
    }
    else if(player->playerstate==9){
        return "Bonus bawana ";
    }
}

void bawanaTransport(struct Player* player){// where actual bawana logic happenes
    short a = (rand() % 3);
    short b = (rand() % 4);
    
    
    player->playerstate = 0;
    player->playerstatecount = 0;
    
    
    
    if(bawanaArray[a][b] ==1 || bawanaArray[a][b] ==2){
        player->playerstate =1;
        player->playerstatecount =3;
        printf("Player %c is place on a %s cell and effects take place\n",player->player_id,cellTypeToWords(player)); 
        printf("Player %c eats from Bawana and has a bad case of food poisoning. Will need three rounds to recover. \n", player->player_id);
    }
    else if(bawanaArray[a][b] ==3 || bawanaArray[a][b] ==4){
        player->playerstate =3;
        player->movement_points =50;
        player->playerstatecount =4;
        printf("Player %c eats from Bawana and is disoriented for 4 rounds.\n", player->player_id);
    }
    else if(bawanaArray[a][b] ==5 || bawanaArray[a][b] ==6){
        player->playerstate = 5;
        player->movement_points =50;
        printf("Player %c is triggered due to bad quality of food.\n", player->player_id);
    }
    else if(bawanaArray[a][b] ==7 || bawanaArray[a][b] ==8){
        player->playerstate = 7;
        player->movement_points =200;
        printf("Player %c is happy with the food. \n", player->player_id);
    }
    else if(bawanaArray[a][b]>=9 && bawanaArray[a][b] <=12){
        player->playerstate = 9;
        player->movement_points = (rand()%90) + 10;
        printf("Player %c earns %hd movement points from Bawana. \n", player->player_id, player->movement_points);
    }
    
    player->current_direction =2;
    player->currentfloor =0;
    player->currentwidth = 9;
    player->currentlength =19;
    player->die_move_result = 0;
    
    printf("Player %c is placed at the entrance of Bawana [0, 9, 19]\n", player->player_id);
}

void whichDirectionToGo(struct Player*player){// where you pick the direction if moment points are 0 or less calls bawana and if player won
    
    
    player->oldFloor=player->currentfloor;
    player->oldWidth=player->currentwidth;  //storing old position
    player->oldLength=player->currentlength;

    if(player->currentfloor==flag_floor && player->currentwidth==flag_width && player->currentlength==flag_length){
            printf("\t\t__________________________________________________________________");
            printf("\t...||player %c captured the flag at [%hd, %hd, %hd] and wins!||...",player->player_id,flag_floor,flag_width,flag_length);
            printf("\t__________________________________________________________________\n");
            fprintf(logs,"\t\t...||player %c wins!||...\n",player->player_id);
            gameClean();
            exit(0);
        }

    if(player->current_direction==1 || player->current_direction==6){
        player->current_direction=player->initial_direction;
        printf("%c got empty direction die face\n",player->player_id);
    }

    if(player->current_direction==2 && player->movement_points>0){ // desiding the direction
        moveNorth(player);
    }
    else if(player->current_direction==3 && player->movement_points>0){
        moveEast(player);
    }
    else if(player->current_direction==4 && player->movement_points>0){
        moveSouth(player);
    }
    else if(player->current_direction==5 && player->movement_points>0){
        moveWest(player);
    }
    else if(player->movement_points<=0 && player->playerstatecount<=0){ //checking movement pints and transporting bawana 
        printf("Player %c movement points are depleted and requires replenishment Transporting to Bawana.\n",player->player_id);
        bawanaTransport(player);
        player->oldFloor=player->currentfloor;
        player->oldWidth=player->currentwidth;  //storing old position
        player->oldLength=player->currentlength;
        if(player->movement_points<0){
            player->movement_points=0;
        }
        return;
    }
    
    boundryCheck(player);


    
}
void whoCaptureWho(struct Player*player){// where player capture check happens
    if(player->player_id=='A'){
        afloor=player->currentfloor;
        awidth=player->currentwidth;
        alength=player->currentlength;
    }
    if(player->player_id=='B'){
        bfloor=player->currentfloor;
        bwidth=player->currentwidth;
        blength=player->currentlength;
    }
    if(player->player_id=='C'){
        cfloor=player->currentfloor;
        cwidth=player->currentwidth;
        clength=player->currentlength;
    }

    if(player->player_id=='A'){
        if(player->currentfloor==bfloor && player->currentwidth==bwidth && player->currentlength==blength){
            printf("Player %c Captured Player B and B is moving to his starting position\n",player->player_id);
            players[1].currentfloor=players[1].initialfloor;
            players[1].currentwidth=players[1].initialwidth;
            players[1].currentlength=players[1].initiallength;
            players[1].player_counter=0;
            return;
        }
        else if(player->currentfloor==cfloor && player->currentwidth==cwidth && player->currentlength==clength){
            printf("Player %c Captured Player C and C is moving to his starting position\n",player->player_id);
            players[2].currentfloor=players[2].initialfloor;
            players[2].currentwidth=players[2].initialwidth;
            players[2].currentlength=players[2].initiallength;
            players[2].player_counter=0;
            return;
        }
    }
    if(player->player_id=='B'){
        if(player->currentfloor==afloor && player->currentwidth==awidth && player->currentlength==alength){
            printf("Player %c Captured Player A and A is moving to his starting position\n",player->player_id);
            players[0].currentfloor=players[0].initialfloor;
            players[0].currentwidth=players[0].initialwidth;
            players[0].currentlength=players[0].initiallength;
            players[0].player_counter=0;
            return;
        }
        else if(player->currentfloor==cfloor && player->currentwidth==cwidth && player->currentlength==clength){
            printf("Player %c Captured Player C and C is moving to his starting position\n",player->player_id);
            players[2].currentfloor=players[2].initialfloor;
            players[2].currentwidth=players[2].initialwidth;
            players[2].currentlength=players[2].initiallength;
            players[2].player_counter=0;
            return;
        }
    }
    if(player->player_id=='C'){
        if(player->currentfloor==afloor && player->currentwidth==awidth && player->currentlength==alength){
            printf("Player %c Captured Player A and A is moving to his starting position\n",player->player_id);
            players[0].currentfloor=players[0].initialfloor;
            players[0].currentwidth=players[0].initialwidth;
            players[0].currentlength=players[0].initiallength;
            players[0].player_counter=0;
            return;
        }
        else if(player->currentfloor==bfloor && player->currentwidth==bwidth && player->currentlength==blength){
            printf("Player %c Captured Player B and B is moving to his starting position\n",player->player_id);
            players[1].currentfloor=players[1].initialfloor;
            players[1].currentwidth=players[1].initialwidth;
            players[1].currentlength=players[1].initiallength;
            players[1].player_counter=0;
            return;
        }
    }
}

void playingGame(struct Player*player){
                movementDieCall(player);//rolls movement die and confirms is he can enter maze if can sets player position to starting cell and sets player counter to 1.movementdiecall-movementdie-player first cell
                mainFlowMomentDieSituation(player);//check player state and give moment die result
                mainFlowDirectionDieSituation(player);//check player state and give direction die result    
                whichDirectionToGo(player);//deside where to move and moves player to direction and until moment die hit 0
                whoCaptureWho(player);//player capturing part

                printf("After the turn Player %c is at floor %hd width %hd length %hd with direction %s and movement points %hd\n",
                    player->player_id,player->currentfloor,
                    player->currentwidth,player->currentlength,
                    direction_to_words(player),
                    player->movement_points);
                printf("\n");
                    
}
int stairInverter(){// where the stairs flip the direction
    short a=(rand()%stairsCount);
    short b=0;
    short tempf,tempw,templ;
   
    for(short i=0;i<a;i++){
        b=(rand()%stairsCount);
        tempf=stairs[b].startFloor;
        tempw=stairs[b].startWidth;
        templ=stairs[b].startLength;
        stairs[b].startFloor=stairs[b].endFloor;
        stairs[b].startWidth=stairs[b].endWidth;
        stairs[b].startLength=stairs[b].endLength;
        stairs[b].endFloor=tempf;
        stairs[b].endWidth=tempw;
        stairs[b].endLength=templ;
        fprintf(logs,"Stair [%hd, %hd, %hd] is reverted\n",stairs[b].endFloor,stairs[b].endWidth,stairs[b].endLength);
        
    }
    return a;
}

void startGame(){// where the game starts and runs 
    initialitaionPart();
    playerManualInitialitation();

    while(1){//where the whole game runs untill flag capture
        printf("***Rounds %d started***\n",gamecounter);
        for(int k=0;k<3;k++){
            playingGame(&players[k]);
        }
        gamecounter++;
        if(gamecounter%5==0){
            fprintf(logs,"***Round %d***\n",gamecounter);
            printf("From next round onwerds %hd Stairs are reverted\n",stairInverter());
               
        }
        printf("\n\n");
    }
}
void logFileOpen(){// where the log txt opens
    logs=fopen("log.txt","w");
    if(logs==NULL){
        fprintf(logs,"can't open log.txt\n");
        return;
    }
}
void game(){// game initial start
    srand(loadSeed());
    logFileOpen();
    startGame();
}