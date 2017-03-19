#include <iostream>
#include <string>
#include <string.h>
#include <strstream>
#include <math.h>
#include <time.h>
#include <sstream>
#include <stdlib.h>
#include <array>
//
//This will be the class for the AI
class TTTAI{
    public:
        TTTAI(); //The constructor
        int Play(int HPositions[], int Positions[]); //Will call the AI to play the game
        int Positions[10]; // An array to hold the positions of the AI
        bool AIWon(bool setting); // Set it so the ai has won // the bool setting is used to determine if setting the victory bool if true you will set the victory setting to true
        int GetMoves(){return Moves;}
        void AIRematch(); // Rematch function
    private:
        int Memory[][81]; // A storage for different positions and their percentage of winning
        int ChoosePlace(int OPositions[], int Positions[]); // OPositions for opponent positions // Chooses the position to put the O (choose place will check the opponent in the decision making process)
        bool Victory=false; //Determines if ai have won
        int Moves=0; // Number of moves made by ai
};
class NN : private TTTAI{ // The AI's 'brain' per say which will be a simple Neural Net
    protected:
        struct Neuron{
            // Constructor
            Neuron(int inputdata){
                input = inputdata;
                // DOutput = desiredoutput;
            }
            //Function that allows the input of desired function
            int DPut(int desiredoutput){ DOutput = desiredoutput; return desiredoutput;}
            int input;
            int output;
            int adjust; // This is the value used to adjust the input variable to the output value
            // function to adjust the input towards a desired output
                int Adjust(int input, int output){
                    output = input + adjust;
                return output;
                }
            // Desired output
            int DOutput;
        };
    private: // REMINDER:: REMINDER:: Function that links the neurons and Function that handles back-propagation of the NN AND MAKE FUNCTION THAT DELETES POINTERS FROM BI
        Neuron * net[][5]; // An array of struct Neuron pointers that will have all the neurons // Has a max size for the height goes to 6
        void BI(int board[], int dim){ // Function for Board input // Takes an array of all board positions, the dimensions (ie if dim == 5 it is a 5x5 board)
            using namespace std;
            int cc=0; //Current column
            int cr=0; //Current row
            for(int i=0; i< ( (sizeof(board) /4) +1); i++){ // For loop that creates the neurons
                switch( ( (i +1) % dim ) ){ // switch statement that determines the row and column from the single dimensional array
                    case 0: net[cr][cc] = new Neuron(i) ; cr++; cc=0; break; // Moves to the next row when it reaches the last column and then resets the column number
                    default: net[cr][cc] = new Neuron(i); cc++; break;  //Moves to the next column
                };
            }
        return;
        }
        void Empty_Net(int board[], int dim){ // empties net
            using namespace std;
            int cc=0; int cr=0;
            for(int i=0; i< ( (sizeof(board) /4) +1); i++){ // For loop that creates the neurons
                switch( ( (i +1) % dim ) ){ // switch statement that determines the row and column from the single dimensional array
                    case 0: delete net[cr][cc]; cr++; cc=0; break; // Moves to the next row when it reaches the last column and then resets the column number
                    default: delete net[cr][cc]; cc++; break;  //Moves to the next column
                };
            }
            delete[] net;
        return;
        }
};
//Class for the human player
class TTTHuman{
    public:
        TTTHuman(); //Constructor
        int HPlay(int HPositions[], int Positions[]); // Will call the Human player to play
        int HPositions[10]; //An array to hold the positions of the human player // The first bracket is the place on that row, the second bracket is the row number starting from 0 //
        bool HWon(bool setting); // Set it so the player has won // the bool setting is used to determine if setting the victory bool if true you will set the victory setting to true
        int GetMoves(){return CMN;}
        void HumanRematch(); //Function for rematches
    private:
        int CMN; //Current Move Number
        bool Victory; //Determines if they have won
};
/////////////////////////////////////////////////////////////////////////////////////
//////// Where the class functions for TTTAI are defined /////////
///////////////////////////////////////////////////////////////////////////////////

TTTAI::TTTAI(){
    using namespace std;
    srand(time(NULL)); //Make sure random variables are a bit more random
    string Name;
    int name_roll = rand()%5;   ////The random name selection
    switch(name_roll){
        case 0: Name="Johnny"; break;
        case 1: Name="Marvin"; break;
        case 2: Name="GLaDOS"; break;
        case 3: Name="Data"; break;
        case 4: Name="Hal"; break;
        default: Name="R2-Tic-Tac"; break;
    }
    cout<<"\nYou will be playing against "<<Name<<endl;
    //Calling of other functions that need to be started by the constructor
    //Setting Positions to zero
    Positions[1]=0;
    Positions[2]=0;
    Positions[3]=0;
    Positions[4]=0;
    Positions[5]=0;
    Positions[6]=0;
    Positions[7]=0;
    Positions[8]=0;
    Positions[9]=0;
    Victory=false;
    Moves=0;
return;
}
void TTTAI::AIRematch(){ // Destructor
    Positions[1]=0;
    Positions[2]=0;
    Positions[3]=0;
    Positions[4]=0;
    Positions[5]=0;
    Positions[6]=0;
    Positions[7]=0;
    Positions[8]=0;
    Positions[9]=0;
    Victory=false;
    Moves=0;
return;
}
int TTTAI::ChoosePlace(int OPositions[], int Positions[]){ //Function that determines the place with the highest chance of winning
        using namespace std;
    if(Moves >=4){return -1;}
    int ChosenPlace; //Place that was chosen // Number corresponds to the place on the board
    //It now determines the places that could potentially win for the human
    int vp[9];
    vp[1] = ( OPositions[1]+OPositions[2]+OPositions[3] ); // [1,2,3] // vp1 = victory possibility 1
    vp[2] = ( OPositions[4]+OPositions[5]+OPositions[6] ); // [4,5,6]
    vp[3] = ( OPositions[7]+OPositions[8]+OPositions[9] ); // [7,8,9]
    vp[4] = ( OPositions[1]+OPositions[4]+OPositions[7] ); // [1,4,7]
    vp[5] = ( OPositions[2]+OPositions[5]+OPositions[8] ); // [2,5,8]
    vp[6] = ( OPositions[3]+OPositions[6]+OPositions[9] ); // [3,6,9]
    vp[7] = ( OPositions[1]+OPositions[5]+OPositions[9] ); // [1,5,9]
    vp[8] = ( OPositions[3]+OPositions[5]+OPositions[7] ); // [3,5,7]
    int CV=0; //
    bool CS=false; // CS = Choosing Successful
    int rollbb; // Roll baby // used if the ai has to place
    int vpNum=0; // victory possibility number
        for(int i=1; i<9; i++){ //for loop that decides which condition is closest to victory //
            if(vp[i] >= 1){ if(vp[i] == 3){ChosenPlace=0; CS=true;} if(vp[i]>CV){CV=vp[i]; vpNum=i;} }

        }
        int WC; // While Counter // used to stop while loop if ai lost
        while(CS!=true){
            WC++;
            rollbb= (rand() % 8) +1;
            switch(vpNum){
                case 0: break;
                case 1: if((OPositions[1] == 1)){/*2nd if*/if((OPositions[3] == 1)){if(Positions[2] == 1){ vpNum=rollbb; break;}ChosenPlace=2; CS=true;break;}else{if(Positions[3] == 1){ vpNum=rollbb; break; cout<<"TEST"<<endl;}ChosenPlace=3; CS=true;break;} }else{if(Positions[1] == 1){ vpNum=rollbb; break;}ChosenPlace=1; CS=true;break;} break;
                case 2: if((OPositions[4] == 1)){/*2nd if*/if((OPositions[6] == 1)){if(Positions[5] == 1){ vpNum=rollbb; break;}ChosenPlace=5; CS=true;break;}else{if(Positions[6] == 1){ vpNum=rollbb; break;}ChosenPlace=6; CS=true;break;} }else{if(Positions[4] == 1){ vpNum=rollbb; break;}ChosenPlace=4; CS=true;break;} break;
                case 3: if((OPositions[7] == 1)){/*2nd if*/if((OPositions[9] == 1)){if(Positions[8] == 1){ vpNum=rollbb; break;}ChosenPlace=8; CS=true;break;}else{if(Positions[9] == 1){ vpNum=rollbb; break;}ChosenPlace=9; CS=true;break;} }else{if(Positions[7] == 1){ vpNum=rollbb; break;}ChosenPlace=7; CS=true;break;} break;
                case 4: if((OPositions[1] == 1)){/*2nd if*/if((OPositions[7] == 1)){if(Positions[4] == 1){ vpNum=rollbb; break;}ChosenPlace=4; CS=true;break;}else{if(Positions[7] == 1){ vpNum=rollbb; break;}ChosenPlace=7; CS=true;break;} }else{if(Positions[1] == 1){ vpNum=rollbb; break;}ChosenPlace=1; CS=true;break;} break;
                case 5: if((OPositions[2] == 1)){/*2nd if*/if((OPositions[8] == 1)){if(Positions[5] == 1){ vpNum=rollbb; break;}ChosenPlace=5; CS=true;break;}else{if(Positions[8] == 1){ vpNum=rollbb; break;}ChosenPlace=8; CS=true;break;} }else{if(Positions[2] == 1){ vpNum=rollbb; break;}ChosenPlace=2; CS=true;break;} break;
                case 6: if((OPositions[3] == 1)){/*2nd if*/if((OPositions[9] == 1)){if(Positions[6] == 1){ vpNum=rollbb; break;}ChosenPlace=6; CS=true;break;}else{if(Positions[9] == 1){ vpNum=rollbb; break;}ChosenPlace=9; CS=true;break;} }else{if(Positions[3] == 1){ vpNum=rollbb; break;}ChosenPlace=3; CS=true;break;} break;
                case 7: if((OPositions[1] == 1)){/*2nd if*/if((OPositions[9] == 1)){if(Positions[5] == 1){ vpNum=rollbb; break;}ChosenPlace=5; CS=true;break;}else{if(Positions[9] == 1){ vpNum=rollbb; break;}ChosenPlace=9; CS=true;break;} }else{if(Positions[1] == 1){ vpNum=rollbb; break;}ChosenPlace=1; CS=true;break;} break;
                case 8: if((OPositions[3] == 1)){/*2nd if*/if((OPositions[7] == 1)){if(Positions[5] == 1){ vpNum=rollbb; break;}ChosenPlace=5; CS=true;break;}else{if(Positions[7] == 1){ vpNum=rollbb; break;}ChosenPlace=7; CS=true;break;} }else{if(Positions[3] == 1){ vpNum=rollbb; break;}ChosenPlace=3; CS=true;break;} break;
                default: cout<<"ERROR:ChoosePlace, switch "<<endl;break;
            }
        }
    Moves++;
return ChosenPlace;
}
int TTTAI::Play(int HPositions[], int AIPositions[]){ //Play function will use chooseplace to put down a spot on the board.
        using namespace std;
    // bool SP; // SP = Successful placement // This bool is used to make sure the placement wasn't on a controlled space or otherwise not possible
    int chosenplace = (ChoosePlace(HPositions, AIPositions)) ;
    switch( chosenplace ){
            case -1: chosenplace=-1; break;
            case 1: if( HPositions[1] == 1 ){AIPositions[1] = 0; break;} if( AIPositions[1] == 1){break;} Positions[1]=1; break;
            case 2: if( HPositions[2] == 1 ){AIPositions[2] = 0; break;} if( AIPositions[2] == 1){break;} Positions[2]=1; break;
            case 3: if( HPositions[3] == 1 ){AIPositions[3] = 0; break;} if( AIPositions[3] == 1){break;} Positions[3]=1; break;
            case 4: if( HPositions[4] == 1 ){AIPositions[4] = 0; break;} if( AIPositions[4] == 1){break;} Positions[4]=1; break;
            case 5: if( HPositions[5] == 1 ){AIPositions[5] = 0; break;} if( AIPositions[5] == 1){break;} Positions[5]=1; break;
            case 6: if( HPositions[6] == 1 ){AIPositions[6] = 0; break;} if( AIPositions[6] == 1){break;} Positions[6]=1; break;
            case 7: if( HPositions[7] == 1 ){AIPositions[7] = 0; break;} if( AIPositions[7] == 1){break;} Positions[7]=1; break;
            case 8: if( HPositions[8] == 1 ){AIPositions[8] = 0; break;} if( AIPositions[8] == 1){break;} Positions[8]=1; break;
            case 9: if( HPositions[9] == 1 ){AIPositions[9] = 0; break;} if( AIPositions[9] == 1){break;} Positions[9]=1; break;
            default: cout<<"ERROR: TTTAI::PLAY, Switch"<<endl; break;
        }
return chosenplace;
}
bool TTTAI::AIWon(bool setting){ // Function that sets / returns the victory status
    if(setting == true){ // Sets victory to true
        Victory=true;
        return false;
    }else{ // This else will return victory
        return Victory;
        }
}
/////////////////////////////////////////////////////////////////////////////////////
//////// Where the class functions for both TTTHuman are defined /////////
///////////////////////////////////////////////////////////////////////////////////
TTTHuman::TTTHuman(){ //Constructor
    CMN=0;
    HPositions[1]=0;
    HPositions[2]=0;
    HPositions[3]=0;
    HPositions[4]=0;
    HPositions[5]=0;
    HPositions[6]=0;
    HPositions[7]=0;
    HPositions[8]=0;
    HPositions[9]=0;
    Victory=false;
return;
}
void TTTHuman::HumanRematch(){ //Destructor
    CMN=0;
    HPositions[1]=0;
    HPositions[2]=0;
    HPositions[3]=0;
    HPositions[4]=0;
    HPositions[5]=0;
    HPositions[6]=0;
    HPositions[7]=0;
    HPositions[8]=0;
    HPositions[9]=0;
    Victory=false;
return;
}
bool TTTHuman::HWon(bool setting){ // Function that sets / returns the victory status
    if(setting == true){ // Sets victory to true
        Victory=true;
        return false;
    }else{ // This else will return victory
        return Victory;
        }
}
int TTTHuman::HPlay(int HPositions[], int AIPositions[]){ //Functions needs objects of the classes to use display, so it takes them inside of its parameters. //
    using namespace std;
    int HP;
    CMN++;
    cout<<"\nInput your move onto the board."<<endl;
    cin>>HP;
        switch(HP){
            case 1: if( AIPositions[1] == 1 ){HPositions[1] = 0; cout<<endl<<"Position is occupied."<<endl;break;} if( HPositions[1] == 1){cout<<endl<<"Position is occupied."<<endl;break;} HPositions[1]=1; break;
            case 2: if( AIPositions[2] == 1 ){HPositions[2] = 0; cout<<endl<<"Position is occupied."<<endl;break;} if( HPositions[2] == 1){cout<<endl<<"Position is occupied."<<endl;break;} HPositions[2]=1; break;
            case 3: if( AIPositions[3] == 1 ){HPositions[3] = 0; cout<<endl<<"Position is occupied."<<endl;break;} if( HPositions[3] == 1){cout<<endl<<"Position is occupied."<<endl;break;} HPositions[3]=1; break;
            case 4: if( AIPositions[4] == 1 ){HPositions[4] = 0; cout<<endl<<"Position is occupied."<<endl;break;} if( HPositions[4] == 1){cout<<endl<<"Position is occupied."<<endl;break;} HPositions[4]=1; break;
            case 5: if( AIPositions[5] == 1 ){HPositions[5] = 0; cout<<endl<<"Position is occupied."<<endl;break;} if( HPositions[5] == 1){cout<<endl<<"Position is occupied."<<endl;break;} HPositions[5]=1; break;
            case 6: if( AIPositions[6] == 1 ){HPositions[6] = 0; cout<<endl<<"Position is occupied."<<endl;break;} if( HPositions[6] == 1){cout<<endl<<"Position is occupied."<<endl;break;} HPositions[6]=1; break;
            case 7: if( AIPositions[7] == 1 ){HPositions[7] = 0; cout<<endl<<"Position is occupied."<<endl;break;} if( HPositions[7] == 1){cout<<endl<<"Position is occupied."<<endl;break;} HPositions[7]=1; break;
            case 8: if( AIPositions[8] == 1 ){HPositions[8] = 0; cout<<endl<<"Position is occupied."<<endl;break;} if( HPositions[8] == 1){cout<<endl<<"Position is occupied."<<endl;break;} HPositions[8]=1; break;
            case 9: if( AIPositions[9] == 1 ){HPositions[9] = 0; cout<<endl<<"Position is occupied."<<endl;break;} if( HPositions[9] == 1){cout<<endl<<"Position is occupied."<<endl;break;} HPositions[9]=1; break;
            default: cout<<"Please enter a number 1-9 that corresponds on the board appropriately."<<endl;cout<<"ERROR: HBPLAY, Switch"<<endl; break;
        }
    cout<<"\nThe AI will now input its move using the board below.\n"<<endl;
return HP;
}


//Important Function that determines if someone has won the game // This function checks if the AI or Human Player has one
int EndGame(TTTHuman HP, TTTAI RAI, bool restart);
bool checkWin(TTTHuman HP, TTTAI RAI){ // A list of all victory positions // Across ([1,2,3] [4,5,6] [7,8,9]) // Up and Down  ([1,4,7] [2,5,8] [3,6,9]) // Diagonal ([1,5,9] [3,5,7])
    bool searching=true; //bool for If the function is still searching
    bool AIcheck=false; // determines if the ai has won
    bool Hcheck=false; // checks if the human has won
    bool decision; //The decision if a party has indeed won or not
        using namespace std;
    // Making new integers to represent certain places for clarity
    int bp1 = HP.HPositions[1]; // bp1(Board Place 1) this is the naming convention for these
    int bp2 = HP.HPositions[2];
    int bp3 = HP.HPositions[3];
    int bp4 = HP.HPositions[4];
    int bp5 = HP.HPositions[5];
    int bp6 = HP.HPositions[6];
    int bp7 = HP.HPositions[7];
    int bp8 = HP.HPositions[8];
    int bp9 = HP.HPositions[9];
    // AI positions
    int AIbp1 = RAI.Positions[1]; // AIbp1(Board Place 1) this is the naming convention for these
    int AIbp2 = RAI.Positions[2];
    int AIbp3 = RAI.Positions[3];
    int AIbp4 = RAI.Positions[4];
    int AIbp5 = RAI.Positions[5];
    int AIbp6 = RAI.Positions[6];
    int AIbp7 = RAI.Positions[7];
    int AIbp8 = RAI.Positions[8];
    int AIbp9 = RAI.Positions[9];
    //If statements checking for winning orientations of the Human side
    if( (bp1+bp2+bp3) == 3 ){ Hcheck=true;} // [1,2,3]
    if( (bp4+bp5+bp6) == 3 ){ Hcheck=true;} // [4,5,6]
    if( (bp7+bp8+bp9) == 3 ){ Hcheck=true;} // [7,8,9]
    if( (bp1+bp4+bp7) == 3 ){ Hcheck=true;} // [1,4,7]
    if( (bp2+bp5+bp8) == 3 ){ Hcheck=true;} // [2,5,8]
    if( (bp3+bp6+bp9) == 3 ){ Hcheck=true;} // [3,6,9]
    if( (bp1+bp5+bp9) == 3 ){ Hcheck=true;} // [1,5,9]
    if( (bp3+bp5+bp7) == 3 ){ Hcheck=true;} // [3,5,7]
    //If statements checking for winning orientations of the AI side
    if( (AIbp1+AIbp2+AIbp3) == 3 ){ AIcheck=true;} // [1,2,3]
    if( (AIbp4+AIbp5+AIbp6) == 3 ){ AIcheck=true;} // [4,5,6]
    if( (AIbp7+AIbp8+AIbp9) == 3 ){ AIcheck=true;} // [7,8,9]
    if( (AIbp1+AIbp4+AIbp7) == 3 ){ AIcheck=true;} // [1,4,7]
    if( (AIbp2+AIbp5+AIbp8) == 3 ){ AIcheck=true;} // [2,5,8]
    if( (AIbp3+AIbp6+AIbp9) == 3 ){ AIcheck=true;} // [3,6,9]
    if( (AIbp1+AIbp5+AIbp9) == 3 ){ AIcheck=true;} // [1,5,9]
    if( (AIbp3+AIbp5+AIbp7) == 3 ){ AIcheck=true;} // [3,5,7]
    //Ensures that no mistake was made that lead to both parties winning
        if( ( AIcheck==true && Hcheck==true ) ){cout<<"ERROR: Checkwin, Double true"<<endl;}
            if((Hcheck == true) ){decision=true; HP.HWon(true); }
                if((AIcheck == true) ){decision=true; RAI.AIWon(true); }
                    switch( EndGame(HP, RAI, false) ){
                        case -1: decision=false; break;
                        case 0: decision = true; break;
                        case 1: decision = true; break; // Human won
                        case 2: decision = true; break; // Ai won
                        case 3: decision = false; break; // Rematch
                        case 4: decision = false; break;
                        default: cout<<"ERROR: Checkwin, switch"<<endl; break;
                        }
return decision;
}
//Function that determines who won the game(Ai or Human) and if the game ended in a tie
int EndGame(TTTHuman HP, TTTAI RAI, bool restart){
    using namespace std;
    int EndGamedec=-1; //The decision of the game
    if(HP.GetMoves() >= 5){cout<<"Tie"<<endl; EndGamedec=0;}   // EndGameDec = 0 is a tie // =1 is Human victory // =2 is AI victory // =3 restart match // =4 end game
    if(HP.HWon(false) == true){ cout<<"Human victory"<<endl; EndGamedec=1;}
    if(RAI.GetMoves() >= 5){cout<<"Tie"<<endl; EndGamedec=0;}
    if(RAI.AIWon(false)==true){ cout<<"AI victory"<<endl; EndGamedec=2;}
    if(restart==true){
        cout<<endl<<"Do you wish to rematch the current AI?"<<endl<<"Yes: 1"<<endl<<"No: 2"<<endl;
        int RD; // rematch decision
        cin>>RD;
            if(RD == 1){ HP.HumanRematch(); RAI.AIRematch(); cout<<"Rematch will commence."<<endl; EndGamedec=3;}else if(RD !=2){cout<<"That is not one of the proposed options."<<endl; EndGamedec=4;}else{cout<<"Bye and have a good time."<<endl;EndGamedec=4;}
    }
return EndGamedec;
}
//Important Function that displays the board
void DisplayB(int Places[], int AIPlaces[]){
    using namespace std;
    // Making new integers to represent certain places for clarity // Excluding the use of 0 in array here because it wouldn't sync up with the board numbering system
    int Hbp[10];
    Hbp[1] = Places[1]; // Hbp[1](Human Board Place 1) this is the naming convention for these
    Hbp[2] = Places[2];
    Hbp[3] = Places[3];
    Hbp[4] = Places[4];
    Hbp[5] = Places[5];
    Hbp[6] = Places[6];
    Hbp[7] = Places[7];
    Hbp[8] = Places[8];
    Hbp[9] = Places[9];
    // AI positions
    int AIbp[10];
    AIbp[1] = AIPlaces[1]; // AIbp[1](Board Place 1) this is the naming convention for these
    AIbp[2] = AIPlaces[2];
    AIbp[3] = AIPlaces[3];
    AIbp[4] = AIPlaces[4];
    AIbp[5] = AIPlaces[5];
    AIbp[6] = AIPlaces[6];
    AIbp[7] = AIPlaces[7];
    AIbp[8] = AIPlaces[8];
    AIbp[9] = AIPlaces[9];
    //Places on the board in the eyes of the display function
    char bp[10]; //So for the what numbers represent // 0 == Not controlled //
    //Loop that checks for positions // Now realized i dont need a switch statement but its fine
    int i; // increment for for loop
        for(i=1; i<10;i++){
            char p='0'+i;
            int Hp = (Hbp[i]); // Hp means Human player
            switch(Hp){
                case 0: bp[i] = p; break;
                case 1: bp[i] = 'X'; break; // Sets it to X for human player
                default: cout<<"ERROR: DisplayB, Switch"<<endl; break;
            };
            int AIp = (AIbp[i]);
            switch( (AIp) ) {
                case 0: break;
                case 1: bp[i] = 'O'; break; // Sets it to O for AI
                default: cout<<"ERROR: DisplayB, Switch2"<<endl; break;
            };
        }
        //If everything goes well it will output the board correctly
        // int testing = bp[9]; //
        cout<<endl<<bp[1]<<"|"<<bp[2]<<"|"<<bp[3]<<endl<<"- - -"<<endl<<bp[4]<<"|"<<bp[5]<<"|" <<bp[6]<<endl<<"- - -"<<endl<<bp[7]<<"|"<<bp[8]<<"|"<<bp[9]<<endl<<endl;
        cout<<"Make your move."<<endl<<endl;
return;
}


//////////////////////////////////////////
/////////////////////////////////////////
////////////////////////////////////////



//Starting prompt function
bool StartPrompt(){
    using namespace std;
    cout<<"This is the beginning of the program.\nYou will compete against AI in a game of Tic-Tac-Toe.\nBelow is the layout of the board.\n"<<endl;
    cout<<"1|2|3\n- - -\n4|5|6\n- - -\n7|8|9"<<endl<<endl;
    cout<<"To place your X you shall input the number that corresponds to the position you see on the board.\n"<<endl;
return true;
}

//Main
int main(){
    using namespace std;
    bool playing=true; // bool for if the game is being played by the human
    StartPrompt();
    class TTTAI RAI;    //Running AI is made
    class TTTHuman P1; //P1 == Player1
    while(playing){

        while( checkWin(P1, RAI) != true){
            DisplayB(P1.HPositions, RAI.Positions);
            P1.HPlay(P1.HPositions, RAI.Positions);
            RAI.Play(P1.HPositions, RAI.Positions);
            }
            DisplayB(P1.HPositions, RAI.Positions); // Look at end board
            switch( EndGame(P1, RAI, true) ){
                    case 3: P1.HumanRematch(); RAI.AIRematch(); break;
                    case 4: playing=false;break;
                    default: break;
                }
    }
    cout<<"Game End"<<endl; // Exit text
    cout<<endl;
return 0;
}
