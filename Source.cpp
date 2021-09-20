#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <strstream>
#include <math.h>
#include <complex>
#include <time.h>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <array>
#include <vector>
#include <algorithm>
#include <iomanip>
#include "TTTHeader.h"
#include <armadillo>
#include <dlib/global_optimization.h>
#include <dlib/optimization.h>
#include <dlib/svm.h>
//#include <dlib/dnn.h>
//#include <dlib/data_io.h>
#include <dlib/matrix.h>
using namespace arma;

//Typedef for training data
typedef mat::fixed<2,9> training_pair; //1st row is input 2nd row is expected output
std::vector<training_pair> TData;
bool tFlag=false;

//
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
        int CMN=0; //Current Move Number
        bool Victory=false; //Determines if they have won
};
//This will be the class for the AI
class TTTAI{
    public:
        TTTAI(); //The constructor
        int Play(int HPositions[], int Positions[]); //Will call the AI to play the game
        int AIPositions[10]; // An array to hold the positions of the AI
        int HPositions[10]; // An array to hold the positions of the Human
        bool AIWon(bool setting); // Set it so the ai has won // the bool setting is used to determine if setting the victory bool if true you will set the victory setting to true
        int GetMoves(){return Moves;}
        void AddMove(int place){
                this->Memory.push_back(place);
                this->Moves++;
        }
        int LastMove(){return Memory.at(Memory.size() - 1); }//Returns last move of the AI
        void AIRematch(); // Rematch function
    private:
        std::vector<int> Memory; // A storage for positions
        int ChoosePlace(int OPositions[], int Positions[]); // OPositions for opponent positions // Chooses the position to put the O (choose place will check the opponent in the decision making process)
        bool Victory=false; //Determines if ai have won
        int Moves=0; // Number of moves made by ai
};

class NN{ // The AI's 'brain' per say
     protected:
          double Cost(bool testing); // Cost function
          struct Synapse{
               void RWeight(){ //Randomly determines the weight
                    double r=0.0;
                    int rd=rand()%99;
                    if(rd>=50){
                         r=rand()%99 +1;
                         weight= -1*( (r/5000) );
                    }else{
                         r=rand()%99 +1;
                         weight=(r/5000);
                    }
               return;
               }
               void CWeight(double c){} //Changes the Weight
               long double weight=0.0000000000; //Weight of the synapse
          };
          struct Neuron{
               Neuron(){//Constructor
               return;
               }
               void RBias(){ //Randomly a number that determines the bias
                    double r=rand()%99;
                    //double rw=rand()%5;
                    double rd=rand()%99;
                    if(rd>=50){
                         bias= -1*( (r/100000) );
                    }else{
                         bias= (r/100000);
                    }
               return;
               }
               void CBias(double c);    //Changes the Bias
               double bias=0.0;
               int input=0; //Used mainly for input neurons
               std::vector<Synapse> Connections;
            };

    public:
          void Ultimate(){ultimate=true;}
         void Load();
         void Save();
          bool Fill( dlib::matrix<double> CM, bool weights );
          void startNN(bool ultimate=false); //Constructor
          void BI(int Hpos[], int Apos[], int dim){ // Function for Board input // Takes an array of all board positions, the dimensions (ie if dim == 5 it is a 5x5 board)
               Board.set_size(dim,dim);
               Board.fill(0.1);
               int k=0,j=1; //Used to translate the singular value to matrix form
               for(int i=0; i<(dim*dim); i++){
                    NIL.at(i).input=0.1;
                    Input(i)=0.1;
                    if(i>dim-1){
                         j= (i/dim) +1; k=i%dim +1;
                         if(i%dim ==0){k=1;}
                    }else{k++;}
                    // TODO: Maybe make one of these -1 so the nn has more to work with
                    if(Hpos[i+1]!=0){
                         Board(i)=-1;
                         NIL.at(i).input=-1;
                         Input(i)=-1;
                    }
                    if(Apos[i+1]!=0){
                         Board(i)=1;
                         NIL.at(i).input=1;
                         Input(i)=1;
                    }
               }
               if(Board.n_cols>3 || Board.n_rows>3){

               }else{

               }
               return;
          }
          void UBI(std::vector<iBoard> IOUBoard, std::vector<int> IOPBoards){ //Board input for ultimate ttt
               OUBoard=IOUBoard;
               OPBoards=IOPBoards;
               int counter=0,rCounter=0,cCounter=0;
               for(int i=0; i<UL1.size(); i++){
                    if( OPBoards.at(counter) == 0 && counter<9){
                         if( OUBoard.at(counter).Board(rCounter,cCounter) == 0){UL1.at(i).input=1;}else if( OUBoard.at(counter).Board(rCounter,cCounter) == -1 ){UL1.at(i).input= -1;}else{ UL1.at(i).input=0;}
                         if( OPBoards.at(counter)==0 && UL1.at(i).input==1 ){ UL1.at(i).input=5;}
                         if(cCounter==2){cCounter=0; rCounter++;}else{cCounter++;}
                         if(rCounter>2){rCounter=0; cCounter=0; counter++;}
                    }
               }

          }
          void Randomize(){ //Randomizes the weights and neuron biases
               srand( time(NULL) );
               //
               if(ultimate){
                    //
                    for(int i=0; i<UL1.size(); i++){ //Randomizes the first Neuron layer
                         //UL1.at(i).RBias();
                         for(int j=0; j<UL1.at(i).Connections.size(); j++){
                              UL1.at(i).Connections.at(j).RWeight();
                              UW1(i,j)=UL1.at(i).Connections.at(j).weight;
                         }
                    }
                    //
                    for(int i=0; i<UL2.size(); i++){ //Randomizes the second Neuron layer
                         UL2.at(i).RBias();
                         for(int j=0; j<UL2.at(i).Connections.size(); j++){
                              UL2.at(i).Connections.at(j).RWeight();
                              UW2(i,j)=UL2.at(i).Connections.at(j).weight;
                         }
                    }
                    //
                    for(int i=0; i<UL3.size(); i++){ //Randomizes the third Neuron layer
                         UL3.at(i).RBias();
                         for(int j=0; j<UL3.at(i).Connections.size(); j++){
                              UL3.at(i).Connections.at(j).RWeight();
                              UW3(i,j)=UL3.at(i).Connections.at(j).weight;
                         }
                    }
                    //
                    for(int i=0; i<UL4.size(); i++){ //Randomizes the fourth Neuron layer
                         UL4.at(i).RBias();
                         for(int j=0; j<UL4.at(i).Connections.size(); j++){
                              UL4.at(i).Connections.at(j).RWeight();
                              UW4(i,j)=UL4.at(i).Connections.at(j).weight;
                         }
                    }
                    //
                    for(int i=0; i<UL5.size(); i++){ //Randomizes the fifth Neuron layer
                         UL5.at(i).RBias();
                         for(int j=0; j<UL5.at(i).Connections.size(); j++){
                              UL5.at(i).Connections.at(j).RWeight();
                         }
                    }
               }
               // Not ultimate
               for(int i=0; i<NIL.size(); i++){ //Randomizes the first Neuron layer
                    NIL.at(i).RBias();
                    for(int j=0; j<NIL.at(i).Connections.size(); j++){
                         NIL.at(i).Connections.at(j).RWeight();
                         W1(i,j)=NIL.at(i).Connections.at(j).weight;
                    }
               }
               //
               for(int i=0; i<NHL.size(); i++){ //Randomizes the second Neuron layer
                    NHL.at(i).RBias();
                    for(int j=0; j<NHL.at(i).Connections.size(); j++){
                         NHL.at(i).Connections.at(j).RWeight();
                         W2(i,j)=NHL.at(i).Connections.at(j).weight;
                    }
               }
               //
               for(int i=0; i<NHL2.size(); i++){ //Randomizes the third Neuron layer
                    NHL2.at(i).RBias();
                    for(int j=0; j<NHL2.at(i).Connections.size(); j++){
                         NHL2.at(i).Connections.at(j).RWeight();
                         W3(i,j)=NHL2.at(i).Connections.at(j).weight;
                    }
               }
               //
               for(int i=0; i<NHL3.size(); i++){ //Randomizes the fourth Neuron layer
                    NHL3.at(i).RBias();
                    for(int j=0; j<NHL3.at(i).Connections.size(); j++){
                         NHL3.at(i).Connections.at(j).RWeight();
                         W4(i,j)=NHL3.at(i).Connections.at(j).weight;
                    }
               }
               //
               for(int i=0; i<NHL4.size(); i++){ //Randomizes the fifth Neuron layer
                    NHL4.at(i).RBias();
                    for(int j=0; j<NHL4.at(i).Connections.size(); j++){
                         NHL4.at(i).Connections.at(j).RWeight();
                         W5(i,j)=NHL4.at(i).Connections.at(j).weight;
                    }
               }
               //
               for(int i=0; i<NOL.size(); i++){ //Randomizes the sixth Neuron layer
                    NOL.at(i).RBias();
                    for(int j=0; j<NOL.at(i).Connections.size(); j++){
                         NOL.at(i).Connections.at(j).RWeight();
                         W6(i,j)=NOL.at(i).Connections.at(j).weight;
                    }
               }
               //
               for(int i=0; i<OL.size(); i++){ //Randomizes the output Neuron layer
                    OL.at(i).RBias();
                    for(int j=0; j<OL.at(i).Connections.size(); j++){
                         OL.at(i).Connections.at(j).RWeight();
                    }
               }
               //
          return;
          }
          //
          mat getBoard(){return Board; } //Returns the board the way the NN sees it
          mat getInput(){return Input;} //Returns the input the NN is receiving
          std::vector<Neuron> getNIL(){ return NIL;}
          std::vector<Neuron> getNHL(){ return NHL;}
          std::vector<Neuron> getNHL2(){ return NHL2;}
          std::vector<Neuron> getNHL3(){ return NHL3;}
          std::vector<Neuron> getNHL4(){ return NHL4;}
          std::vector<Neuron> getNOL(){ return NOL;}
          std::vector<Neuron> getOL(){ return OL;}
          mat getW1(){return W1;} //More get functions
          mat getW2(){return W2;}
          mat getW3(){return W3;}
          mat getW4(){return W4;}
          mat getW5(){return W5;}
          mat getW6(){return W6;}
          //
          mat getUW1(){return UW1;}
          mat getUW2(){return UW2;}
          mat getUW3(){return UW3;}
          mat getUW4(){return UW4;}
          //
          std::vector<Neuron> getUL1(){ return UL1;}
          std::vector<Neuron> getUL2(){ return UL2;}
          std::vector<Neuron> getUL3(){ return UL3;}
          std::vector<Neuron> getUL4(){ return UL4;}
          std::vector<Neuron> getUL5(){ return UL5;}
          //
          void OPT(bool testing);
          mat coutput(){ return OutputM;} //Output of the NN (kinda like the Placement decision)
          int Place(TTTHuman PBuddy); //Returns the Output of the NN in a 1-9 manner
          //
          TTTAI NNAI;
          int CC=0; //Correct Move Choice
          mat VP(mat In); //Victory Possibilities
          void Calstd (bool testing); // Calculates the values of the matrices
          double UScore(dlib::matrix<double> CM);
    private: // REMINDER:: REMINDER:: Function that links the neurons and Function that handles back-propagation of the NN
          //
          bool ultimate;

          bool GradCheck(); // Numerical grade checking
          void BackProp(); //Back-propagation
          void Train(bool testing); //Training of the network
          mat W1; //Matrix of weights
          mat W2;
          mat W3;
          mat W4;
          mat W5;
          mat W6;
          mat W1Copy; //Copy of weight matrices
          mat W2Copy;
          mat W3Copy;
          mat W4Copy;
          mat W5Copy;
          mat W6Copy;
          mat Board;//Matrix representation of the board the ai uses
          mat CostM; //Matrix of the Error/Cost
          //dTotal Error dWeight matrix
          mat TEW1;
          mat TEW2;
          mat TEW3;
          mat TEW4;
          mat TEW5;
          mat TEW6;
          //dTotal Error dBias matrix
          mat TEB1;
          mat TEB2;
          mat TEB3;
          mat TEB4;
          mat TEB5;
          mat TEB6;
          //
          mat Input;//Input matrix
          mat MZ1;
          mat MZ2;
          mat MZ3;
          mat MZ4;
          mat MZ5;
          mat MZ6;
          mat MA1;  //Activated matrix
          mat MA2;
          mat MA3;
          mat MA4;
          mat MA5;
          mat MA6;
          //Creation of Input Neurons
          std::vector<Neuron> NIL; //Size 9
          /*//First Layer
          Neuron NF1,NF2,NF3;
          std::vector<Neuron> NFL={NF1,NF2,NF3};
          */
          //Hidden Layer
          std::vector<Neuron> NHL; //Size 36
          //Second Hidden Layer
          std::vector<Neuron> NHL2; //Size 43
          //Third Hidden Layer
          std::vector<Neuron> NHL3; //Size 54
          //Fourth Hidden Layer
          std::vector<Neuron> NHL4; //Size 36
          //
          std::vector<Neuron> NOL; //Size 12
          //
          std::vector<Neuron> OL;//size 9
          mat OutputM;
          mat OPMCopy;
          //
          ///Ultimate
          //
          std::vector<Neuron> UL1; //Size 81
          std::vector<Neuron> UL2; //Size 976
          std::vector<Neuron> UL3; //Size 416
          std::vector<Neuron> UL4; //Size 256
          std::vector<Neuron> UL5; //Size 81
          //
          mat UW1;
          mat UW2;
          mat UW3;
          mat UW4;
          //
          mat UOutput;
          //
          std::vector<iBoard> OUBoard; //Observed Ultimate board
          std::vector<int> OPBoards;  //Observed playable boards

}Testnet;


/////////////////////////////////////////////////////////////////////////////////////
//////// Where the class functions for TTTAI are defined /////////
///////////////////////////////////////////////////////////////////////////////////

TTTAI::TTTAI(){
    using namespace std;
    //srand( time(NULL) );
    string Name;
    int name_roll = rand()%5;   ////The random name selection
    switch(name_roll){
// TODO (Blackweb#1#): Add back in the names and the well played after a game ...
//
         /*
        case 0: Name="Johnny"; break;
        case 1: Name="Marvin"; break;
        case 2: Name="GLaDOS"; break;
        case 3: Name="Data"; break;
        case 4: Name="Hal"; break;
        default: Name="R2-Tic-Tac"; break;
        */
    }
    //std::cout<<"\nYou will be playing against "<<Name<<std::endl<<std::endl;
    //Calling of other functions that need to be started by the constructor
    //Setting Positions to zero
    for(int i=1; i<10; i++){
     AIPositions[i]=0;
     HPositions[i]=0;
    }
    Victory=false;
    Moves=0;
return;
}
void TTTAI::AIRematch(){ // Destructor
    for(int i=1; i<10; i++){
     AIPositions[i]=0;
     HPositions[i]=0;
    }
    this->Memory.clear();
    Victory=false;
    this->Moves=0;
return;
}

// Determines if there are no valid moves left
bool movesLeft( char b[3][3] ) {
    for (int i = 0; i<3; i++)
        for (int j = 0; j<3; j++)
            if (b[i][j]=='_')
                return true;
    return false;
}

//Evaluate function evaluates boards. ChoosePlace
int Evaluate(char b[3][3]) {
    char player = 'x', opponent = 'o';
    // Checking rows for X or O victory.
    for (int row = 0; row<3; row++) {
        if (b[row][0]==b[row][1] && b[row][1]==b[row][2]) {
            if (b[row][0]==player)
                return +10;
            else if (b[row][0]==opponent)
                return -10;
        }
    }

    // Checking columns for X or O victory.
    for (int col = 0; col<3; col++) {
        if (b[0][col]==b[1][col] && b[1][col]==b[2][col]) {
            if (b[0][col]==player)
                return +10;
            else if (b[0][col]==opponent)
                return -10;
        }
    }

    // Checking diagonals for X or O victory.
    if (b[0][0]==b[1][1] && b[1][1]==b[2][2])
    {
        if (b[0][0]==player)
            return +10;
        else if (b[0][0]==opponent)
            return -10;
    }

    if (b[0][2]==b[1][1] && b[1][1]==b[2][0])
    {
        if (b[0][2]==player)
            return +10;
        else if (b[0][2]==opponent)
            return -10;
    }

    // Else if none of them have won then return 0
    return 0;
}

// This is the minimax function. It considers all
// the possible ways the game can go and returns the value of the board
// Supports ChoosePlace
int Minimax(char board[3][3], int depth, bool isMax) {
    int score = Evaluate(board);
    char player = 'x', opponent = 'o';
    // If Maximizer has won the game return his/her
    // evaluated score
    if (score == 10) {
        // Subtract to ensure faster victories
        score = score - depth;
        return score;
    }

    // If Minimizer has won the game return his/her
    // evaluated score
    if (score == -10) {
        // Add depth to ensure slower losses
        score = score + depth;
        return score;
    }

    // If there are no more moves and no winner then
    // it is a tie
    if ( !movesLeft( board ) ) {
        return 0;
    }

    // If this maximizer's move
    if (isMax) {
        int best = -1000;

        // Traverse all cells
        for (int i = 0; i<3; i++)
        {
            for (int j = 0; j<3; j++)
            {
                // Check if cell is empty
                if (board[i][j]=='_')
                {
                    // Make the move
                    board[i][j] = player;

                    // Call minimax recursively and choose
                    // the maximum value
                    best = std::max( best, Minimax(board, depth+1, !isMax) );

                    // Undo the move
                    board[i][j] = '_';
                }
            }
        }
        return best;
    }

    // If this minimizer's move
    else
    {
        int best = 1000;

        // Traverse all cells
        for (int i = 0; i<3; i++)
        {
            for (int j = 0; j<3; j++)
            {
                // Check if cell is empty
                if (board[i][j]=='_')
                {
                    // Make the move
                    board[i][j] = opponent;

                    // Call minimax recursively and choose
                    // the minimum value
                    best = std::min(best, Minimax(board, depth+1, !isMax));

                    // Undo the move
                    board[i][j] = '_';
                }
            }
        }
        return best;
    }
}

int TTTAI::ChoosePlace(int OPositions[], int Positions[]){ //Function that determines the place with the highest chance of winning
    int ChosenPlace=0; //Place that was chosen // Number corresponds to the place on the board
    for(int i=1; i<10; i++){
        HPositions[i]=OPositions[i];
    }
    //
    char player = 'x', opponent = 'o';
    char b[3][3];
    // Fill the board
    int c = 1;
    for( int i=0; i<3; i++ ) {
        for ( int j=0; j<3; j++ ) {
            if ( OPositions[c] == 1 ) {
                //std::cout<<"TTO: "<< OPositions[c] <<std::endl;
                b[i][j]='o';
            }
            else if ( Positions[c] == 1 ) {
                //std::cout<<"TTP: "<< Positions[c] <<std::endl;
                b[i][j]='x';
            }
            else b[i][j]='_';
            c++;
        }
    }
    // Print
    /*
    for( int i=0; i<3; i++ ) {
        for ( int j=0; j<3; j++ ) {
            std::cout<<b[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
    */
    //

    // This will return the best possible move
    int bestVal = -1000;
    int bestMoveRow = -1;
    int bestMoveCol = -1;
    int moveVal = 0;
    // Traverse all cells, evaluate minimax function for
    // all empty cells. And return the cell with optimal
    // value.
    for (int i = 0; i<3; i++){
        for (int j = 0; j<3; j++){
            // Check if cell is empty
            if (b[i][j]=='_'){
                // Make the move
                b[i][j] = player;

                // compute evaluation function for this
                // move.
                moveVal = Minimax(b, 0, false);

                // Undo the move
                b[i][j] = '_';

                // If the value of the current move is
                // more than the best value, then update
                if (moveVal > bestVal)
                {
                    bestMoveRow = i;
                    bestMoveCol = j;
                    bestVal = moveVal;
                }
            }
        }
    }
    // std::cout<<"Best Move Row: "<<bestMoveRow<<"\nBest Move Col"<<bestMoveCol<<std::endl;
    // turns bestMoveRow and bestMoveCol into a single spot on a 1-9 board
    c = 1;
    for( int i=0; i<3; i++ ) {
        for ( int j=0; j<3; j++ ) {
            if ( i == bestMoveRow && j == bestMoveCol ) {
                ChosenPlace = c;
            }
            c++;
        }
    }
    // std::cout<<"Chosen Move: "<<ChosenPlace<<std::endl;
return ChosenPlace;
}
int TTTAI::Play(int HPositions[], int Positions[]){ //Play function will use chooseplace to put down a spot on the board.
        using namespace std;
    // bool SP; // SP = Successful placement // This bool is used to make sure the placement wasn't on a controlled space or otherwise not possible
    int chosenplace = (ChoosePlace(HPositions, AIPositions)) ;
    switch( chosenplace ){
            case -1: /* std::cout<<"Well played."<<std::endl; */ break; //The ai will have chosenplace as zero/-1 if there are no places it can go
            case 0:  /* std::cout<<"Well played."<<std::endl; */ break; //The ai will have chosenplace as zero if there are no places it can go
            case 1: if( HPositions[1] == 1 ){Positions[1] = 0; break;} if( Positions[1] == 1){break;} AIPositions[1]=1; break;
            case 2: if( HPositions[2] == 1 ){Positions[2] = 0; break;} if( Positions[2] == 1){break;} AIPositions[2]=1; break;
            case 3: if( HPositions[3] == 1 ){Positions[3] = 0; break;} if( Positions[3] == 1){break;} AIPositions[3]=1; break;
            case 4: if( HPositions[4] == 1 ){Positions[4] = 0; break;} if( Positions[4] == 1){break;} AIPositions[4]=1; break;
            case 5: if( HPositions[5] == 1 ){Positions[5] = 0; break;} if( Positions[5] == 1){break;} AIPositions[5]=1; break;
            case 6: if( HPositions[6] == 1 ){Positions[6] = 0; break;} if( Positions[6] == 1){break;} AIPositions[6]=1; break;
            case 7: if( HPositions[7] == 1 ){Positions[7] = 0; break;} if( Positions[7] == 1){break;} AIPositions[7]=1; break;
            case 8: if( HPositions[8] == 1 ){Positions[8] = 0; break;} if( Positions[8] == 1){break;} AIPositions[8]=1; break;
            case 9: if( HPositions[9] == 1 ){Positions[9] = 0; break;} if( Positions[9] == 1){break;} AIPositions[9]=1; break;
            default: std::cout<<"ERROR: TTTAI::PLAY, Switch"<<std::endl; break;
        }
        AddMove(chosenplace);
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
//////// Where the class functions for both TTTHuman are implemented /////////
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
    bool deciding=true;//Determines if the user is still trying to make a correct placement
    CMN++;
    while( deciding == true){ // while loop that ensures the move made is legal
    std::cout<<"\nInput your move onto the board."<<std::endl;
    cin>>HP; std::cout<<std::endl;
        switch(HP){
            case 1: if( AIPositions[1] == 1 ){HPositions[1] = 0; std::cout<<std::endl<<"Position is occupied."<<std::endl;break;} if( HPositions[1] == 1){std::cout<<std::endl<<"Position is occupied."<<std::endl;break;} HPositions[1]=1;deciding=false;break;
            case 2: if( AIPositions[2] == 1 ){HPositions[2] = 0; std::cout<<std::endl<<"Position is occupied."<<std::endl;break;} if( HPositions[2] == 1){std::cout<<std::endl<<"Position is occupied."<<std::endl;break;} HPositions[2]=1;deciding=false;break;
            case 3: if( AIPositions[3] == 1 ){HPositions[3] = 0; std::cout<<std::endl<<"Position is occupied."<<std::endl;break;} if( HPositions[3] == 1){std::cout<<std::endl<<"Position is occupied."<<std::endl;break;} HPositions[3]=1;deciding=false;break;
            case 4: if( AIPositions[4] == 1 ){HPositions[4] = 0; std::cout<<std::endl<<"Position is occupied."<<std::endl;break;} if( HPositions[4] == 1){std::cout<<std::endl<<"Position is occupied."<<std::endl;break;} HPositions[4]=1;deciding=false;break;
            case 5: if( AIPositions[5] == 1 ){HPositions[5] = 0; std::cout<<std::endl<<"Position is occupied."<<std::endl;break;} if( HPositions[5] == 1){std::cout<<std::endl<<"Position is occupied."<<std::endl;break;} HPositions[5]=1;deciding=false;break;
            case 6: if( AIPositions[6] == 1 ){HPositions[6] = 0; std::cout<<std::endl<<"Position is occupied."<<std::endl;break;} if( HPositions[6] == 1){std::cout<<std::endl<<"Position is occupied."<<std::endl;break;} HPositions[6]=1;deciding=false;break;
            case 7: if( AIPositions[7] == 1 ){HPositions[7] = 0; std::cout<<std::endl<<"Position is occupied."<<std::endl;break;} if( HPositions[7] == 1){std::cout<<std::endl<<"Position is occupied."<<std::endl;break;} HPositions[7]=1;deciding=false;break;
            case 8: if( AIPositions[8] == 1 ){HPositions[8] = 0; std::cout<<std::endl<<"Position is occupied."<<std::endl;break;} if( HPositions[8] == 1){std::cout<<std::endl<<"Position is occupied."<<std::endl;break;} HPositions[8]=1;deciding=false;break;
            case 9: if( AIPositions[9] == 1 ){HPositions[9] = 0; std::cout<<std::endl<<"Position is occupied."<<std::endl;break;} if( HPositions[9] == 1){std::cout<<std::endl<<"Position is occupied."<<std::endl;break;} HPositions[9]=1;deciding=false;break;
            default: std::cout<<"Please enter a number 1-9 that corresponds on the board appropriately."<<std::endl;std::cout<<"ERROR: HBPLAY, Switch"<<std::endl; break;
        }
    }
    std::cout<<"\nThe AI will now input its move using the board below.\n"<<std::endl;
return HP;
}

/////////////////////////////////////////////////////////////////////////////////////
//////// Where the class functions for NN are implemented /////////
///////////////////////////////////////////////////////////////////////////////////

//Constructor for NN
void NN::startNN(bool ultimate){

     //
     if(ultimate){
          //
          for(int i=0; i<81; i++){ // layer //81
               UL1.push_back( createObject<Neuron>() );
          }
          for(int i=0; i<108; i++){ // layer //108
               UL2.push_back( createObject<Neuron>() );
          }
          for(int i=0; i<96; i++){ //layer //96
               UL3.push_back( createObject<Neuron>() );
          }
          for(int i=0; i<84; i++){ //layer //84
               UL4.push_back( createObject<Neuron>() );
          }
          for(int i=0; i<81; i++){ //layer //81
               UL5.push_back( createObject<Neuron>() );
          }

          //
          ///
          //
          for(int i=0; i<UL1.size(); i++){
               for(int j=0; j<UL2.size(); j++){
                    UL1.at(i).Connections.push_back( createObject<Synapse>() );
               }
          }
          //
          for(int i=0; i<UL2.size(); i++){
               for(int j=0; j<UL3.size(); j++){
                    UL2.at(i).Connections.push_back( createObject<Synapse>() );
               }
          }
          //
          for(int i=0; i<UL3.size(); i++){
               for(int j=0; j<UL4.size(); j++){
                    UL3.at(i).Connections.push_back( createObject<Synapse>() );
               }
          }
          for(int i=0; i<UL4.size(); i++){
               for(int j=0; j<UL5.size(); j++){
                    UL4.at(i).Connections.push_back( createObject<Synapse>() );
               }
          }
          //
          //
          ///
          //
          UW1.set_size(UL1.size(), UL2.size());
          UW2.set_size(UL2.size(), UL3.size());
          UW3.set_size(UL3.size(), UL4.size());
          UW4.set_size(UL4.size(), UL5.size());
          //
          UOutput.set_size(1,UL1.size());
     }
     //If not Ultimate TTT
     //Filling of Neuron Vectors
     for(int i=0; i<9; i++){ //Input layer //9
          NIL.push_back( createObject<Neuron>() );
     }
     //hidden Layer 1 // 16
     for(int i=0; i<64; i++){
          NHL.push_back( createObject<Neuron>() );
     }
     //Second hidden Layer // 25
     for(int i=0; i<48; i++){
          NHL2.push_back( createObject<Neuron>() );
     }
     //Third hidden Layer // 36
     for(int i=0; i<32; i++){
          NHL3.push_back( createObject<Neuron>() );
     }
     //fourth hidden Layer // 24
     for(int i=0; i<24; i++){
          NHL4.push_back( createObject<Neuron>() );
     }
     // Layer// 18
     for(int i=0; i<16; i++){
          NOL.push_back( createObject<Neuron>() );
     }
     //Output Layer// 9
     for(int i=0; i<9; i++){
          OL.push_back( createObject<Neuron>() );
     }
     //
     //Creation of Synapses
     //
     for(int i=0; i<NIL.size(); i++){ //Input layer
          for(int j=0; j<NHL.size(); j++){
               NIL.at(i).Connections.push_back( createObject<Synapse>() );
          }
     }

     //hidden Layer 1
     for(int i=0; i<NHL.size(); i++){
          for(int j=0; j<NHL2.size(); j++){
               NHL.at(i).Connections.push_back( createObject<Synapse>() );
          }
     }
     //Second hidden Layer
     //Creation of Synapses
     //9 synapses for each Neuron
     for(int i=0; i<NHL2.size(); i++){
          for(int j=0; j<NHL3.size(); j++){
               NHL2.at(i).Connections.push_back( createObject<Synapse>() );
          }
     }
     //
     for(int i=0; i<NHL3.size(); i++){
          for(int j=0; j<NHL4.size(); j++){
               NHL3.at(i).Connections.push_back( createObject<Synapse>() );
          }
     }
     //
     for(int i=0; i<NHL4.size(); i++){
          for(int j=0; j<NOL.size(); j++){
               NHL4.at(i).Connections.push_back( createObject<Synapse>() );
          }
     }
     //
     for(int i=0; i<NOL.size(); i++){
          for(int j=0; j<OL.size(); j++){
               NOL.at(i).Connections.push_back( createObject<Synapse>() );
          }
     }
     //
     CostM.set_size(1,9); //Sets size for matrix
     W1.set_size(  NIL.size(),  NHL.size()  );
     W2.set_size(  NHL.size() , NHL2.size()  );
     W3.set_size( NHL2.size() , NHL3.size() );
     W4.set_size( NHL3.size() , NHL4.size() );
     W5.set_size( NHL4.size() , NOL.size() );
     W6.set_size( NOL.size() , OL.size() );
     //TW.set_size(NOL.size(),1);
     //
     W1Copy.set_size( NIL.size() ,  NHL.size()  );
     W2Copy.set_size( NHL.size() , NHL2.size()  );
     W3Copy.set_size( NHL2.size() , NHL3.size()  );
     W4Copy.set_size( NHL3.size() ,NHL4.size() );
     W5Copy.set_size( NHL4.size() ,NOL.size()  );
     W6Copy.set_size( NOL.size() , OL.size()   );
     //TWCopy.set_size(NOL.size(),1);
     //
     TEW1.set_size( NIL.size(),  NHL.size() );
     TEW1.fill(fill::zeros);
     //
     TEW2.set_size( NHL.size() , NHL2.size() );
     TEW2.fill(fill::zeros);
     //
     TEW3.set_size( NHL2.size() , NHL3.size()  );
     TEW3.fill(fill::zeros);
     //
     TEW4.set_size( NHL3.size() , NHL4.size()  );
     TEW4.fill(fill::zeros);
     //
     TEW5.set_size( NHL4.size() , NOL.size()  );
     TEW5.fill(fill::zeros);
     //
     TEW6.set_size( NOL.size() , OL.size()  );
     TEW6.fill(fill::zeros);
     //Bias
     TEB1.set_size( 1, NIL.size() );
     TEB1.fill(fill::zeros);
     //
     TEB2.set_size( 1, NHL.size() );
     TEB2.fill(fill::zeros);
     //
     TEB3.set_size( 1, NHL2.size());
     TEB3.fill(fill::zeros);
     //
     TEB4.set_size( 1, NHL3.size());
     TEB4.fill(fill::zeros);
     //
     TEB5.set_size( 1, NHL4.size());
     TEB5.fill(fill::zeros);
     //
     TEB6.set_size( 1, NOL.size() );
     TEB6.fill(fill::zeros);
     //
     //TEW4.set_size(NOL.size(),1);
     //TEW4.fill(fill::zeros);
     //
     Input.set_size(1,NIL.size());
     //
     MZ1.set_size(NIL.size() , NHL.size() );
     MZ2.set_size(NHL.size() , NHL2.size());
     MZ3.set_size(NHL2.size(), NHL3.size());
     MZ4.set_size(NHL3.size(), NHL4.size());
     MZ5.set_size(NHL4.size(), NOL.size() );
     MZ6.set_size(NOL.size() , OL.size()  );
     //MZ4.set_size(3,3);
     MA1.set_size(NIL.size() , NHL.size()  );
     MA2.set_size(NHL.size() , NHL2.size()  );
     MA3.set_size(NHL2.size(), NHL3.size()  );
     MA4.set_size(NHL3.size(), NHL4.size()  );
     MA5.set_size(NHL4.size(), NOL.size()  );
     MA6.set_size(NOL.size() , OL.size()   );
     //MA4.set_size(3,3);
     //
     MZ1.fill(fill::zeros);
     MZ2.fill(fill::zeros);
     MZ3.fill(fill::zeros);
     MZ4.fill(fill::zeros);
     MZ5.fill(fill::zeros);
     MZ6.fill(fill::zeros);
     //MZ4.fill(fill::zeros);
     MA1.fill(fill::zeros);
     MA2.fill(fill::zeros);
     MA3.fill(fill::zeros);
     MA4.fill(fill::zeros);
     MA5.fill(fill::zeros);
     MA6.fill(fill::zeros);
     //MA4.fill(fill::zeros);
     //
     OutputM.set_size(1,9);
     OutputM.fill(fill::zeros);
     OPMCopy=OutputM;
     //
return;
}
//
mat NN::VP(mat In){
     Input=In;
     mat matVP;
     matVP.set_size(1,9); matVP.fill(fill::zeros);
     //
     int AIvp[9];
     AIvp[1] = ( Input(1-1)+Input(2-1)+Input(3-1) ); // [1,2,3]
     AIvp[2] = ( Input(4-1)+Input(5-1)+Input(6-1) ); // [4,5,6]
     AIvp[3] = ( Input(7-1)+Input(8-1)+Input(9-1) ); // [7,8,9]
     AIvp[4] = ( Input(1-1)+Input(4-1)+Input(7-1) ); // [1,4,7]
     AIvp[5] = ( Input(2-1)+Input(5-1)+Input(8-1) ); // [2,5,8]
     AIvp[6] = ( Input(3-1)+Input(6-1)+Input(9-1) ); // [3,6,9]
     AIvp[7] = ( Input(1-1)+Input(5-1)+Input(9-1) ); // [1,5,9]
     AIvp[8] = ( Input(3-1)+Input(5-1)+Input(7-1) ); // [3,5,7]
     int Hvp[9];
     Hvp[1] = ( Input(1-1)+Input(2-1)+Input(3-1) ); // [1,2,3]
     Hvp[2] = ( Input(4-1)+Input(5-1)+Input(6-1) ); // [4,5,6]
     Hvp[3] = ( Input(7-1)+Input(8-1)+Input(9-1) ); // [7,8,9]
     Hvp[4] = ( Input(1-1)+Input(4-1)+Input(7-1) ); // [1,4,7]
     Hvp[5] = ( Input(2-1)+Input(5-1)+Input(8-1) ); // [2,5,8]
     Hvp[6] = ( Input(3-1)+Input(6-1)+Input(9-1) ); // [3,6,9]
     Hvp[7] = ( Input(1-1)+Input(5-1)+Input(9-1) ); // [1,5,9]
     Hvp[8] = ( Input(3-1)+Input(5-1)+Input(7-1) ); // [3,5,7]
     if(AIvp[1]==-2){ // [1,2,3]/
               if(Input(0,1-1)==-1 && Input(0,2-1)==-1 && Input(0,3-1)!=-1){matVP(0,3-1)+=1; }
               if(Input(0,1-1)==-1 && Input(0,3-1)==-1 && Input(0,2-1)!=-1){matVP(0,2-1)+=1; }
               if(Input(0,3-1)==-1 && Input(0,2-1)==-1 && Input(0,1-1)!=-1){matVP(0,1-1)+=1; }
          }
          if(AIvp[2]==-2){ // [4,5,6]/
               if(Input(0,4-1)==-1 && Input(0,5-1)==-1 && Input(0,6-1)!=-1){matVP(0,6-1)+=1; }
               if(Input(0,4-1)==-1 && Input(0,6-1)==-1 && Input(0,5-1)!=-1){matVP(0,5-1)+=1; }
               if(Input(0,5-1)==-1 && Input(0,6-1)==-1 && Input(0,4-1)!=-1){matVP(0,4-1)+=1; }
          }
          if(AIvp[3]==-2){ // [7,8,9]/
               if(Input(0,7-1)==-1 && Input(0,8-1)==-1 && Input(0,9-1)!=-1){matVP(0,9-1)+=1; }
               if(Input(0,7-1)==-1 && Input(0,9-1)==-1 && Input(0,8-1)!=-1){matVP(0,8-1)+=1; }
               if(Input(0,8-1)==-1 && Input(0,9-1)==-1 && Input(0,7-1)!=-1){matVP(0,7-1)+=1; }
          }
          if(AIvp[4]==-2){ // [1,4,7]/
               if(Input(0,1-1)==-1 && Input(0,4-1)==-1 && Input(0,7-1)!=-1){matVP(0,7-1)+=1; }
               if(Input(0,1-1)==-1 && Input(0,7-1)==-1 && Input(0,4-1)!=-1){matVP(0,4-1)+=1; }
               if(Input(0,4-1)==-1 && Input(0,7-1)==-1 && Input(0,1-1)!=-1){matVP(0,1-1)+=1; }
          }
          if(AIvp[5]==-2){ // [2,5,8]/
               if(Input(0,2-1)==-1 && Input(0,5-1)==-1 && Input(0,8-1)!=-1){matVP(0,8-1)+=1; }
               if(Input(0,2-1)==-1 && Input(0,8-1)==-1 && Input(0,5-1)!=-1){matVP(0,5-1)+=1; }
               if(Input(0,5-1)==-1 && Input(0,8-1)==-1 && Input(0,2-1)!=-1){matVP(0,2-1)+=1; }
          }
          if(AIvp[6]==-2){  // [3,6,9]/
               if(Input(0,3-1)==-1 && Input(0,6-1)==-1 && Input(0,9-1)!=-1){matVP(0,9-1)+=1; }
               if(Input(0,3-1)==-1 && Input(0,9-1)==-1 && Input(0,6-1)!=-1){matVP(0,6-1)+=1; }
               if(Input(0,6-1)==-1 && Input(0,9-1)==-1 && Input(0,3-1)!=-1){matVP(0,3-1)+=1; }
          }
          if(AIvp[7]==-2){ // [1,5,9]/
               if(Input(0,1-1)==-1 && Input(0,5-1)==-1 && Input(0,9-1)!=-1){matVP(0,9-1)+=1; }
               if(Input(0,1-1)==-1 && Input(0,9-1)==-1 && Input(0,5-1)!=-1){matVP(0,5-1)+=1; }
               if(Input(0,5-1)==-1 && Input(0,9-1)==-1 && Input(0,1-1)!=-1){matVP(0,1-1)+=1; }
          }
          if(AIvp[8]==-2){ // [3,5,7]/
               if(Input(0,3-1)==-1 && Input(0,5-1)==-1 && Input(0,7-1)!=-1){matVP(0,7-1)+=1; }
               if(Input(0,3-1)==-1 && Input(0,7-1)==-1 && Input(0,5-1)!=-1){matVP(0,5-1)+=1; }
               if(Input(0,5-1)==-1 && Input(0,7-1)==-1 && Input(0,3-1)!=-1){matVP(0,3-1)+=1; }
          }
     //
     if(Hvp[1]==-4){ // [1,2,3]/
               if(Input(0,1-1)==-2 && Input(0,2-1)==-2 && Input(0,3-1)!=-2){matVP(0,3-1)+=1; }
               if(Input(0,1-1)==-2 && Input(0,3-1)==-2 && Input(0,2-1)!=-2){matVP(0,2-1)+=1; }
               if(Input(0,3-1)==-2 && Input(0,2-1)==-2 && Input(0,1-1)!=-2){matVP(0,1-1)+=1; }
          }
          if(Hvp[2]==-4){ // [4,5,6]/
               if(Input(0,4-1)==-2 && Input(0,5-1)==-2 && Input(0,6-1)!=-2){matVP(0,6-1)+=1; }
               if(Input(0,4-1)==-2 && Input(0,6-1)==-2 && Input(0,5-1)!=-2){matVP(0,5-1)+=1; }
               if(Input(0,5-1)==-2 && Input(0,6-1)==-2 && Input(0,4-1)!=-2){matVP(0,4-1)+=1; }
          }
          if(Hvp[3]==-4){ // [7,8,9]/
               if(Input(0,7-1)==-2 && Input(0,8-1)==-2 && Input(0,9-1)!=-2){matVP(0,9-1)+=1; }
               if(Input(0,7-1)==-2 && Input(0,9-1)==-2 && Input(0,8-1)!=-2){matVP(0,8-1)+=1; }
               if(Input(0,8-1)==-2 && Input(0,9-1)==-2 && Input(0,7-1)!=-2){matVP(0,7-1)+=1; }
          }
          if(Hvp[4]==-4){ // [1,4,7]/
               if(Input(0,1-1)==-2 && Input(0,4-1)==-2 && Input(0,7-1)!=-2){matVP(0,7-1)+=1; }
               if(Input(0,1-1)==-2 && Input(0,7-1)==-2 && Input(0,4-1)!=-2){matVP(0,4-1)+=1; }
               if(Input(0,4-1)==-2 && Input(0,7-1)==-2 && Input(0,1-1)!=-2){matVP(0,1-1)+=1; }
          }
          if(Hvp[5]==-4){ // [2,5,8]/
               if(Input(0,2-1)==-2 && Input(0,5-1)==-2 && Input(0,8-1)!=-2){matVP(0,8-1)+=1; }
               if(Input(0,2-1)==-2 && Input(0,8-1)==-2 && Input(0,5-1)!=-2){matVP(0,5-1)+=1; }
               if(Input(0,5-1)==-2 && Input(0,8-1)==-2 && Input(0,2-1)!=-2){matVP(0,2-1)+=1; }
          }
          if(Hvp[6]==-4){  // [3,6,9]/
               if(Input(0,3-1)==-2 && Input(0,6-1)==-2 && Input(0,9-1)!=-2){matVP(0,9-1)+=1; }
               if(Input(0,3-1)==-2 && Input(0,9-1)==-2 && Input(0,6-1)!=-2){matVP(0,6-1)+=1; }
               if(Input(0,6-1)==-2 && Input(0,9-1)==-2 && Input(0,3-1)!=-2){matVP(0,3-1)+=1; }
          }
          if(Hvp[7]==-4){ // [1,5,9]/
               if(Input(0,1-1)==-2 && Input(0,5-1)==-2 && Input(0,9-1)!=-2){matVP(0,9-1)+=1; }
               if(Input(0,1-1)==-2 && Input(0,9-1)==-2 && Input(0,5-1)!=-2){matVP(0,5-1)+=1; }
               if(Input(0,5-1)==-2 && Input(0,9-1)==-2 && Input(0,1-1)!=-2){matVP(0,1-1)+=1; }
          }
          if(Hvp[8]==-4){ // [3,5,7]/
               if(Input(0,3-1)==-2 && Input(0,5-1)==-2 && Input(0,7-1)!=-2){matVP(0,7-1)+=1; }
               if(Input(0,3-1)==-2 && Input(0,7-1)==-2 && Input(0,5-1)!=-2){matVP(0,5-1)+=1; }
               if(Input(0,5-1)==-2 && Input(0,7-1)==-2 && Input(0,3-1)!=-2){matVP(0,3-1)+=1; }
          }
     //
return matVP;
}
//
bool NN::Fill ( dlib::matrix<double>CM, bool weights ) {
    bool s = true; // success
    int c = 0;
    if ( weights ) {
        c = 0;

        for ( int i = 0; i < W1.n_rows; i++ ) { //First weight matrix
            for ( int j = 0; j < W1.n_cols; j++ ) {
                W1 ( i, j ) = CM ( 0, c );
                c++;
            }
        }
        for ( int i = 0; i < W2.n_rows; i++ ) { //Second weight matrix
            for ( int j = 0; j < W2.n_cols; j++ ) {
                W2 ( i, j ) = CM ( 0, c );
                c++;
            }
        }
        for ( int i = 0; i < W3.n_rows; i++ ) { //Third weight matrix
            for ( int j = 0; j < W3.n_cols; j++ ) {
                W3 ( i, j ) = CM ( 0, c );
                c++;
            }
        }
        for ( int i = 0; i < W4.n_rows; i++ ) { //Fourth weight matrix
            for ( int j = 0; j < W4.n_cols; j++ ) {
                W4 ( i, j ) = CM ( 0, c );
                c++;
            }
        }
        for ( int i = 0; i < W5.n_rows; i++ ) { //Fifth weight matrix
            for ( int j = 0; j < W5.n_cols; j++ ) {
                W5 ( i, j ) = CM ( 0, c );
                c++;
            }
        }
        for ( int i = 0; i < W6.n_rows; i++ ) { //sixth weight matrix
            for ( int j = 0; j < W6.n_cols; j++ ) {
                W6 ( i, j ) = CM ( 0, c );
                c++;
            }
        }

        c = 0;
        Save();
        Load();
    } else {
        c = 0; //Alters biases
        for ( int i = 0; i < NIL.size(); i++ ) { //1st Neuron layer biases
            NIL.at ( i ).bias = CM ( 0, c );
            c++;
        }
        for ( int i = 0; i < NHL.size(); i++ ) { //2nd Neuron layer biases
            NHL.at ( i ).bias = CM ( 0, c );
            c++;
        }
        for ( int i = 0; i < NHL2.size(); i++ ) { //3rd Neuron layer biases
            NHL2.at ( i ).bias = CM ( 0, c );
            c++;
        }
        for ( int i = 0; i < NHL3.size(); i++ ) { //4th Neuron layer biases
            NHL3.at ( i ).bias = CM ( 0, c );
            c++;
        }
        for ( int i = 0; i < NHL4.size(); i++ ) { //5th Neuron layer biases
            NHL4.at ( i ).bias = CM ( 0, c );
            c++;
        }
        for ( int i = 0; i < NOL.size(); i++ ) { //6th Neuron layer biases
            NOL.at ( i ).bias = CM ( 0, c );
            c++;
        }
        for ( int i = 0; i < OL.size(); i++ ) { //7th Neuron layer biases
            OL.at ( i ).bias = CM ( 0, c );
            c++;
        }
        c = 0;
        Save();
        Load();
    }
    return s;
}
//
void NN::Calstd (bool testing=false){
     int c=0; //counter for for loops
     if(ultimate){
          //
          for(int i=0; i<UL1.size(); i++){//
               for(int j=0; j<UL1.at(i).Connections.size(); j++){
                    UW1(i,j) = UL1.at(i).Connections.at(j).weight;
               }
          }
          for(int i=0; i<UL2.size(); i++){//
               for(int j=0; j<UL2.at(i).Connections.size(); j++){
                    UW2(i,j) = UL2.at(i).Connections.at(j).weight;
               }
          }
          for(int i=0; i<UL3.size(); i++){//
               for(int j=0; j<UL3.at(i).Connections.size(); j++){
                    UW3(i,j) = UL3.at(i).Connections.at(j).weight;
               }
          }
          for(int i=0; i<UL4.size(); i++){//
               for(int j=0; j<UL4.at(i).Connections.size(); j++){
                    UW4(i,j) = UL4.at(i).Connections.at(j).weight;
               }
          }
          //
          for(int i=0; i<UL1.size(); i++){ UOutput(0,i) = UL1.at(i).input; }
          //
          c=0;
          UOutput = UOutput * UW1; // 1x81 * 81x976
          UOutput.for_each( [&c, this](mat::elem_type& val ){ val+= UL2.at(c).bias; c++; } );
          c=0;
          UOutput = UOutput * UW2; // 1x976 * 976x416
          UOutput.for_each( [&c, this](mat::elem_type& val ){ val+= UL3.at(c).bias; c++; } );
          c=0;
          UOutput = UOutput * UW3; // 1x416 * 416x256
          UOutput.for_each( [&c, this](mat::elem_type& val ){ val+= UL4.at(c).bias; c++; } );
          c=0;
          UOutput = UOutput * UW4; // 1x256 * 256x81
          //
          //std::cout<<"UOUTPUT Calstd: "<<std::endl;
          //UOutput.print(); std::cout<<std::endl;
     return;
     }
     mat In;
     In = Input;
     In.for_each( [&c , this](mat::elem_type& val ){ val+= NIL.at(c).bias; c++; } );
     c=0;
     if(testing){
          // OPMCopy= (In + (.33*VP(Input)) ) * W1Copy; //
          OPMCopy= (In) * W1Copy; //
          OPMCopy.for_each( [&c, this](mat::elem_type& val ){ val+= NHL.at(c).bias; } );
          c=0;
          OPMCopy.for_each( [](mat::elem_type& val ){ val= tanh(val); } );
          OPMCopy = OPMCopy * W2Copy; // 1x9 * 9x18 = 1x18
          OPMCopy.for_each( [&c, this](mat::elem_type& val ){ val+= NHL2.at(c).bias; } );
          c=0;
          OPMCopy.for_each( [](mat::elem_type& val ){ val= tanh(val); } );
          OPMCopy = OPMCopy * W3Copy; // 1x18 * 18x36 = 1x36
          OPMCopy.for_each( [&c, this](mat::elem_type& val ){ val+= NHL3.at(c).bias; } );
          c=0;
          OPMCopy.for_each( [](mat::elem_type& val ){ val= tanh(val); } );
          OPMCopy = OPMCopy * W4Copy; // 1x36 * 36x36 = 1x36
          OPMCopy.for_each( [&c, this](mat::elem_type& val ){ val+= NHL4.at(c).bias; } );
          c=0;
          OPMCopy.for_each( [](mat::elem_type& val ){ val= tanh(val); } );
          OPMCopy = OPMCopy * W5Copy; // 1x36 * 36x36 = 1x36
          OPMCopy.for_each( [&c, this](mat::elem_type& val ){ val+= NOL.at(c).bias; } ); //
          c=0;
          OPMCopy.for_each( [](mat::elem_type& val ){ val= tanh(val); } );
          //
          OPMCopy = OPMCopy * W6Copy; // 1x36 * 36x9 = 1x9
          OPMCopy.for_each( [&c, this](mat::elem_type& val ){ val+= OL.at(c).bias; } ); //
          c=0;
          OPMCopy.for_each( [](mat::elem_type& val ){ val= tanh(val); } );
          //OPMCopy = OPMCopy * trans(TWCopy);
          //OPMCopy.for_each( [](mat::elem_type& val ){ val= tanh(val); } );
          return;
     }
     //
     //Fills the matrices
     for(int i=0; i<NIL.size(); i++){ //
          for(int j=0; j<NIL.at(i).Connections.size(); j++){
               W1(i,j) = NIL.at(i).Connections.at(j).weight;
          }
     }
     for(int i=0; i<NHL.size(); i++){ //
          for(int j=0; j<NHL.at(i).Connections.size(); j++){
               W2(i,j) = NHL.at(i).Connections.at(j).weight;

          }
     }
     for(int i=0; i<NHL2.size(); i++){//
          for(int j=0; j<NHL2.at(i).Connections.size(); j++){
               W3(i,j) = NHL2.at(i).Connections.at(j).weight;

          }
     }
     for(int i=0; i<NHL3.size(); i++){//
          for(int j=0; j<NHL3.at(i).Connections.size(); j++){
               W4(i,j) = NHL3.at(i).Connections.at(j).weight;

          }
     }
     for(int i=0; i<NHL4.size(); i++){//
          for(int j=0; j<NHL4.at(i).Connections.size(); j++){
               W5(i,j) = NHL4.at(i).Connections.at(j).weight;

          }
     }
     for(int i=0; i<NOL.size(); i++){//
          for(int j=0; j<NOL.at(i).Connections.size(); j++){
               W6(i,j) = NOL.at(i).Connections.at(j).weight;
          }
     }
     /*
     std::cout.setf(std::ios_base::fixed);
     std::cout.setf(std::ios_base::showpoint);
     std::cout.precision(5);
     std::cout<<std::endl<<"TEST"<<std::endl;
     W1.print(); std::cout<<std::endl;////
     W2.print(); std::cout<<std::endl;////
     W3.print(); std::cout<<std::endl;////
     W4.print(); std::cout<<std::endl;////
     W5.print(); std::cout<<std::endl;////
     */
     //TW.print(); std::cout<<std::endl;////
     //Calculates the hidden layer
     //std::cout<<"INPUT: "<<std::endl;
     Input.print();
     // MZ1= (In + (.33*VP(Input)) ) * W1;
     MZ1= (In) * W1;
     MA1 = MZ1;
     MA1.for_each( [&c, this](mat::elem_type& val ){ val+= NHL.at(c).bias; c++; } );
     c=0;
     MA1.for_each( [](mat::elem_type& val ){ val= tanh(val); } ); //Applies activation function // a1 // 1x9
     //std::cout<<"MA1: "<<std::endl; MA1.print(); std::cout<<std::endl;
     //std::cout<<std::endl<<"TEST"<<std::endl;////
     //Calculates the second hidden layer
     MZ2 = MA1 * W2;
     MA2=MZ2;
     MA2.for_each( [&c, this](mat::elem_type& val ){ val+= NHL2.at(c).bias; c++; } );
     c=0;
     MA2.for_each( [](mat::elem_type& val ){ val= tanh(val); } ); //Applies activation function // a2 // 1x18
     //std::cout<<"MA1: "<<std::endl; MA2.print(); std::cout<<std::endl;
     //Calculates the third hidden layer
     MZ3 = MA2 * W3;
     MA3=MZ3;
     MA3.for_each( [&c, this](mat::elem_type& val ){ val+= NHL3.at(c).bias; c++; } );
     c=0;
     MA3.for_each( [](mat::elem_type& val ){ val= tanh(val); } ); //Applies activation function // a3 // 1x36
     //std::cout<<"MA1: "<<std::endl; MA3.print(); std::cout<<std::endl;
     //Calculates the fourth hidden layer
     MZ4 = MA3 * W4;
     MA4=MZ4;
     MA4.for_each( [&c, this](mat::elem_type& val ){ val+= NHL4.at(c).bias; c++; } );
     c=0;
     MA4.for_each( [](mat::elem_type& val ){ val= tanh(val); } ); //Applies activation function // a4 // 1x18
     //std::cout<<"MA1: "<<std::endl; MA4.print(); std::cout<<std::endl;
     //Calculates the fifth hidden layer
     MZ5 = MA4 * W5;
     MA5=MZ5;
     MA5.for_each( [&c, this](mat::elem_type& val ){ val+= NOL.at(c).bias; c++; } );//
     c=0;
     MA5.for_each( [](mat::elem_type& val ){ val= tanh(val); } ); //Applies activation function // a5 // 1x9
     //
     MZ6 = MA5 * W6;
     MA6=MZ6;
     MA6.for_each( [&c, this](mat::elem_type& val ){ val+= OL.at(c).bias; /*cout<<"Bias at "<<c<<": "<<OL.at(c).bias<<std::endl;*/ c++; } );//
     c=0;
     MA6.for_each( [](mat::elem_type& val ){ val= tanh(val); } ); //Applies activation function // a6 // 1x9
     //Calculates the Output layer
     OutputM = MA6;
     std::cout<<"OUTPUT Calstd: "<<std::endl;
     std::cout<<"\t 1 \t 2 \t 3 \t 4 \t 5 \t 6 \t 7 \t 8 \t 9 \t"<<std::endl;
     OutputM.print(); std::cout<<std::endl;
return;
}
//
double NN::Cost(bool testing=false){
     double TE=0.0; //Total Error
     //CC=4; //Correct Move Choice
     double value=0.0; //Value of Neuron // Value = input + bias
     double TTE=0.0;
     mat TEM;
     TEM.set_size(1,9);
     TEM.fill(fill::ones); //Fills with ones
     int c=0;
     switch(CC){ //Creates a one filled matrix except for the correct choice
               case 1: TEM(0)=-1; break;
               case 2: TEM(1)=-1; break;
               case 3: TEM(2)=-1; break;
               case 4: TEM(3)=-1; break;
               case 5: TEM(4)=-1; break;
               case 6: TEM(5)=-1; break;
               case 7: TEM(6)=-1; break;
               case 8: TEM(7)=-1; break;
               case 9: TEM(8)=-1; break;
               default: std::cout<<"ERROR: Switch"<<CC<<std::endl; break;
          };
     if(testing){
          TEM=TEM+OPMCopy; //Takes the matrix and adds by the output of the NN
          TEM.for_each( [](mat::elem_type& val ){ val= pow(val, 2); } );
          TTE=accu(TEM); //Calculates total error by taking a sum of all elements
          TTE*=.5;
     return TTE;
     }
     CostM.fill(fill::zeros); //Prepares the Cost matrix
          TEM=TEM+OutputM; //Takes the matrix and adds by the output of the NN
          CostM=TEM; //
          TEM.for_each( [](mat::elem_type& val ){ val= pow(val, 2); } );
          //
          TE=accu(TEM); //Calculates total error by taking a sum of all elements
          TE*=.5;
return TE;
}
void NN::BackProp(){
     //Creating of matrices and variables
     int c=0;
     //BP error
     mat BP;
     BP.set_size(1,9);
     BP.fill(fill::zeros);
     //BP2 error
     mat BP2;
     BP2.set_size(9,18);
     BP2.fill(fill::zeros);
     //BP3 error
     mat BP3;
     BP3.set_size(18,36);
     BP3.fill(fill::zeros);
     //BP4 error
     mat BP4;
     BP4.set_size(36,18);
     BP4.fill(fill::zeros);
     //BP5 error
     mat BP5;
     BP5.set_size(18,9);
     BP5.fill(fill::zeros);
     //BPInitial error
     mat BPI;
     BPI.set_size(1,9);
     BPI.fill(fill::zeros);
     //Prime Z6 matrix
     mat PZ6;
     PZ6.set_size(1,9);
     PZ6.fill(fill::zeros);
     //Prime Z5 matrix
     mat PZ5;
     PZ5.set_size(1,9);
     PZ5.fill(fill::zeros);
     //Prime Z4 matrix
     mat PZ4;
     PZ4.set_size(1,18);
     PZ4.fill(fill::zeros);
     //Prime Z3 matrix
     mat PZ3;
     PZ3.set_size(1,36);
     PZ3.fill(fill::zeros);
     //Prime Z2 matrix
     mat PZ2;
     PZ2.set_size(1,18);
     PZ2.fill(fill::zeros);
     //Prime Z1 matrix
     mat PZ1;
     PZ1.set_size(1,9);
     PZ1.fill(fill::zeros);
     //////
     //////The computations
     //////
     //Setting BackProp error
     std::cout<<"Cost: "<<std::endl;
     CostM.print(); std::cout<<std::endl;
     PZ6=MZ6;
     std::cout<<"MZ6: "<<std::endl;
     MZ6.print(); std::cout<<std::endl;
     PZ6.for_each( [&c, this](mat::elem_type& val ){ val+= OL.at(c).bias; } );
     c=0;
     PZ6.for_each( [](mat::elem_type& val ){ val= (1 -  pow( (tanh(val)) , 2) ); } );
     std::cout<<"PZ6: "<<std::endl;
     PZ6.print(); std::cout<<std::endl;
     BPI = (CostM % PZ6); // % is element wise multiplication // 1x9 % 1x9
     std::cout<<"BPI: "<<std::endl;
     BPI.print(); std::cout<<std::endl;
     //Calculating TEW5
     TEW6 = trans(MA5) * BP; // 18x1 * 1x9 = 18x9
     std::cout<<"TEW6: "<<std::endl;
     TEW6.print(); std::cout<<std::endl;
     //BP
     PZ5=MZ5; //PZ5
     PZ5.for_each( [&c, this](mat::elem_type& val ){ val+= NOL.at(c).bias; } );
     c=0;
     PZ5.for_each( [](mat::elem_type& val ){ val= (1 -  pow( (tanh(val)) , 2) ); } );
     BP = (BPI * trans(W6) ) % PZ5; // (1x9 * 9x9) % 1x9
     //TEW5
     TEW5 = trans(MA4) * BP; // 18x1 * 1x9 = 18x9
     //BP2
     PZ4=MZ4; //PZ4
     PZ4.for_each( [&c, this](mat::elem_type& val ){ val+= NHL4.at(c).bias; } );
     c=0;
     PZ4.for_each( [](mat::elem_type& val ){ val= (1 -  pow( (tanh(val)) , 2) ); } );
     BP2 = (BP * trans(W5) ) % PZ4; // (1x9 * 9x18) % 1x18
     //TEW4
     TEW4 = trans(MA3) * BP2; // 36x1 * 1x18 = 36x18
     //BP3////
     PZ3=MZ3; //PZ3
     PZ3.for_each( [&c, this](mat::elem_type& val ){ val+= NHL3.at(c).bias; } );
     c=0;
     PZ3.for_each( [](mat::elem_type& val ){ val= (1 -  pow( (tanh(val)) , 2) ); } );
     BP3 = (BP2 * trans(W4) ) % PZ3; // (1x18 * 18x36) % 1x36
     //TEW3
     TEW3 = trans(MA2) * BP3; // 18x1 * 1x36 = 18x36
     //BP4 ///////
     PZ2=MZ2; //PZ2
     PZ2.for_each( [&c, this](mat::elem_type& val ){ val+= NHL2.at(c).bias; } );
     c=0;
     PZ2.for_each( [](mat::elem_type& val ){ val= (1 -  pow( (tanh(val)) , 2) ); } );
     BP4 = (BP3 * trans(W3) ) % PZ2; // (1x36 * 36x18) % 1x18
     //TEW2
     TEW2 = trans(MA1) * BP4; // 9x1 * 1x18 = 9x18
     //BP5
     PZ1=MZ1; //PZ2
     PZ1.for_each( [&c, this](mat::elem_type& val ){ val+= NHL.at(c).bias; } );
     c=0;
     PZ1.for_each( [](mat::elem_type& val ){ val= (1 -  pow( (tanh(val)) , 2) ) ; } );
     BP5 = ( BP4 * trans(W2) ) % PZ1; // (1x18 * 18x9) % 1x9
     //TEW1
     TEW1 = trans(Input) * BP5; // 9x1 * 1x9 = 9x9
     //////
     //////Bias
     //////

return;
}
bool NN::GradCheck(){
     bool passed=true;
     mat D1Copy; //TEW1 Copy
     mat D2Copy; //TEW2 Copy
     mat D3Copy; //TEW3 Copy
     mat D4Copy; //TEW4 Copy
     mat D5Copy; //TEW5 Copy
     mat D6Copy; //TEW5 Copy
     D1Copy.set_size(9,9);
     D1Copy.fill(fill::zeros);
     D2Copy.set_size(9,18);
     D2Copy.fill(fill::zeros);
     D3Copy.set_size(18,36);
     D3Copy.fill(fill::zeros);
     D4Copy.set_size(36,18);
     D4Copy.fill(fill::zeros);
     D5Copy.set_size(18,9);
     D5Copy.fill(fill::zeros);
     D6Copy.set_size(9,9);
     D6Copy.fill(fill::zeros);
     //D4Copy.set_size(3,1);
     //D4Copy.fill(fill::zeros);
     //Fills the copy matrices
     W1Copy=W1;
     W2Copy=W2;
     W3Copy=W3;
     W4Copy=W4;
     W5Copy=W5;
     W6Copy=W6;
     //TWCopy=TW;
     mat ND; //Norm difference
     mat NDS;
     mat NDP;
     double x=0.0; //Chosen point
     double e=0.0001; //Change in x
     double loss1=0.0, loss2=0.0;//loss doubles used for Total error difference
     for(int i=0; i<W1Copy.n_rows; i++){ //Loop for W1Copy
          for(int j=0; j<W1Copy.n_cols; j++){
               W1Copy(i,j)+=e;
               Calstd(true);
               loss1=Cost(true);
               //
               W1Copy(i,j)-=(2*e);
               Calstd(true);
               loss2=Cost(true);
               //
               D1Copy(i,j) = (loss1-loss2) / (2 * e);
          }
     }
     W1Copy=W1; //Reset values
     loss1=0.0; loss2=0.0;
     for(int i=0; i<W2Copy.n_rows; i++){ //Loop for W2Copy
               for(int j=0; j<W2Copy.n_cols; j++){
                    W2Copy(i,j)+=e;
                    Calstd(true);
                    loss1=Cost(true);
                    //
                    W2Copy(i,j)-=2*e;
                    Calstd(true);
                    loss2= Cost(true);
                    //
                    D2Copy(i,j) = (loss1-loss2) / (2 * e);
               }
     }
     W2Copy=W2;
     loss1=0.0; loss2=0.0;
     for(int i=0; i<W3Copy.n_rows; i++){ //Loop for W3Copy
               for(int j=0; j<W3Copy.n_cols; j++){
                    W3Copy(i,j)+=e;
                    Calstd (true);
                    loss1=Cost(true);
                    //
                    W3Copy(i,j)-=2*e;
                    Calstd (true);
                    loss2= Cost(true);
                    //
                    D3Copy(i,j) = (loss1-loss2) / (2 * e);
               }
     }
     W3Copy=W3;
     loss1=0.0; loss2=0.0;
     for(int i=0; i<W4Copy.n_rows; i++){ //Loop for W4Copy
               for(int j=0; j<W4Copy.n_cols; j++){
                    W4Copy(i,j)+=e;
                    Calstd (true);
                    loss1=Cost(true);
                    //
                    W4Copy(i,j)-=2*e;
                    Calstd (true);
                    loss2= Cost(true);
                    //
                    D4Copy(i,j) = (loss1-loss2) / (2 * e);
               }
     }
     W4Copy=W4;
     loss1=0.0; loss2=0.0;
     for(int i=0; i<W5Copy.n_rows; i++){ //Loop for W5Copy
               for(int j=0; j<W5Copy.n_cols; j++){
                    W5Copy(i,j)+=e;
                    Calstd(true);
                    loss1=Cost(true);
                    //
                    W5Copy(i,j)-=2*e;
                    Calstd(true);
                    loss2= Cost(true);
                    //
                    D5Copy(i,j) = (loss1-loss2) / (2 * e);
               }
     }
     W5Copy=W5;
     loss1=0.0; loss2=0.0;
     for(int i=0; i<W6Copy.n_rows; i++){ //Loop for W6Copy
               for(int j=0; j<W6Copy.n_cols; j++){
                    W6Copy(i,j)+=e;
                    Calstd(true);
                    loss1=Cost(true);
                    //
                    W6Copy(i,j)-=2*e;
                    Calstd(true);
                    loss2= Cost(true);
                    //
                    D6Copy(i,j) = (loss1-loss2) / (2 * e);
               }
     }
     W6Copy=W6;
     loss1=0.0; loss2=0.0;
     //
     //Bias
     //

     //
     std::cout<<"YOLO: "<<std::endl;
     D1Copy.print(); std::cout<<std::endl;
     TEW1.print(); std::cout<<std::endl;
     //
     NDS = D1Copy-TEW1;
     NDS.for_each( [](mat::elem_type& val ){ val =std::norm(val); } );
     NDP = D1Copy+TEW1;
     NDP.for_each( [](mat::elem_type& val ){ val =std::norm(val); } );
     ND= NDS / NDP;
     ND.for_each( [&passed](mat::elem_type& val ){ if(val > 0.01){ passed=false;} } );
     //
     D2Copy.print(); std::cout<<std::endl;
     TEW2.print(); std::cout<<std::endl;
     //
     NDS = D2Copy-TEW2;
     NDS.for_each( [](mat::elem_type& val ){ val =std::norm(val); } );
     NDP = D2Copy+TEW2;
     NDP.for_each( [](mat::elem_type& val ){ val =std::norm(val); } );
     ND= NDS / NDP;
     ND.for_each( [&passed](mat::elem_type& val ){ if(val > 0.01){ passed=false;} } );
     //
     D3Copy.print(); std::cout<<std::endl;
     TEW3.print(); std::cout<<std::endl;
     //
     NDS = D3Copy-TEW3;
     NDS.for_each( [](mat::elem_type& val ){ val =std::norm(val); } );
     NDP = D3Copy+TEW3;
     NDP.for_each( [](mat::elem_type& val ){ val =std::norm(val); } );
     ND= NDS / NDP;
     ND.for_each( [&passed](mat::elem_type& val ){ if(val > 0.01){ passed=false;} } );
     //
     D4Copy.print(); std::cout<<std::endl;
     TEW4.print(); std::cout<<std::endl;
     //
     NDS = D4Copy-TEW4;
     NDS.for_each( [](mat::elem_type& val ){ val =std::norm(val); } );
     NDP = D4Copy+TEW4;
     NDP.for_each( [](mat::elem_type& val ){ val =std::norm(val); } );
     ND= NDS / NDP;
     ND.for_each( [&passed](mat::elem_type& val ){ if(val > 0.01){ passed=false;} } );
     //
     D5Copy.print(); std::cout<<std::endl;
     TEW5.print(); std::cout<<std::endl;
     //
     NDS = D5Copy-TEW5;
     NDS.for_each( [](mat::elem_type& val ){ val =std::norm(val); } );
     NDP = D5Copy+TEW5;
     NDP.for_each( [](mat::elem_type& val ){ val =std::norm(val); } );
     ND= NDS / NDP;
     ND.for_each( [&passed](mat::elem_type& val ){ if(val > 0.01){ passed=false;} } );
     //
     D6Copy.print(); std::cout<<std::endl;
     TEW6.print(); std::cout<<std::endl;
     //
     NDS = D6Copy-TEW6;
     NDS.for_each( [](mat::elem_type& val ){ val =std::norm(val); } );
     NDP = D6Copy+TEW6;
     NDP.for_each( [](mat::elem_type& val ){ val =std::norm(val); } );
     ND= NDS / NDP;
     ND.for_each( [&passed](mat::elem_type& val ){ if(val > 0.01){ passed=false;} } );
return passed;
}
void DisplayB(int Places[], int AIPlaces[]);
bool checkWin(TTTHuman HP, TTTAI RAI);
// Checks if someone has one the board
bool hasWon( int ar1[], int ar2[] ) {
    bool HW=false,AW=false;
    //If statements checking for winning orientations of the Human side
    if( (ar1[1]+ar1[2]+ar1[3]) == 3 ){ HW=true;} // [1,2,3]
    if( (ar1[4]+ar1[5]+ar1[6]) == 3 ){ HW=true;} // [4,5,6]
    if( (ar1[7]+ar1[8]+ar1[9]) == 3 ){ HW=true;} // [7,8,9]
    if( (ar1[1]+ar1[4]+ar1[7]) == 3 ){ HW=true;} // [1,4,7]
    if( (ar1[2]+ar1[5]+ar1[8]) == 3 ){ HW=true;} // [2,5,8]
    if( (ar1[3]+ar1[6]+ar1[9]) == 3 ){ HW=true;} // [3,6,9]
    if( (ar1[1]+ar1[5]+ar1[9]) == 3 ){ HW=true;} // [1,5,9]
    if( (ar1[3]+ar1[5]+ar1[7]) == 3 ){ HW=true;} // [3,5,7]
    //If statements checking for winning orientations of the AI side
    if( (ar2[1]+ar2[2]+ar2[3]) == 3 ){ AW=true;} // [1,2,3]
    if( (ar2[4]+ar2[5]+ar2[6]) == 3 ){ AW=true;} // [4,5,6]
    if( (ar2[7]+ar2[8]+ar2[9]) == 3 ){ AW=true;} // [7,8,9]
    if( (ar2[1]+ar2[4]+ar2[7]) == 3 ){ AW=true;} // [1,4,7]
    if( (ar2[2]+ar2[5]+ar2[8]) == 3 ){ AW=true;} // [2,5,8]
    if( (ar2[3]+ar2[6]+ar2[9]) == 3 ){ AW=true;} // [3,6,9]
    if( (ar2[1]+ar2[5]+ar2[9]) == 3 ){ AW=true;} // [1,5,9]
    if( (ar2[3]+ar2[5]+ar2[7]) == 3 ){ AW=true;} // [3,5,7]
    //
    if ( AW ^ HW ) return true;
    else return false;
}
mat ArrayToMat( int arr[], int arr_size ) {
    mat cc;
    cc.set_size(1,arr_size); // S is the array size
    for ( uword i = 0+1; i < cc.n_elem; i++ ) {
        cc.at(i) = arr[i];
    }
    return cc;
}
void PWCC(bool testing=false){    //Positions with correct choices //Returns a random board position with a labeled correct choice. used for training
    //
    mat cB; //Current board
    mat cE; //Current expected choice
    mat cT; //Current temporary board
    cB.set_size ( 1, 9 ); cB.fill ( 0.1 );
    cE.set_size ( 1, 9 ); cE.fill ( 0.1 );
    training_pair cPair; //Current Pair
    int r = 0;
    int pl = 0;
    bool fturn = true; // first players turn
    bool chosen = false;
    //
    TTTAI PBuddy; //Play buddy
    TTTAI PBuddy2;
    TTTHuman FH;
    // PBuddy2.AIPositions[10] = 1;
    for ( int i = 0; i < 550; i++ ) {
        // Loop for game
        while ( !hasWon( PBuddy.AIPositions, PBuddy2.AIPositions ) && PBuddy.GetMoves() < 5 ) {
            // Make move
            if ( fturn ) {
                // Decide whether to start at random spot or allow ai to choose
                r = rand() % 100 + 1;
                if ( r > 15 ) { // Random start
                    while ( !chosen ) {
                        r = rand()%9 + 1;
                        if ( PBuddy.AIPositions[r] != 1 ) {
                            PBuddy.AIPositions[r] = 1;
                            chosen = true;
                        }
                    }
                    // std::cout<<"YeehAW! "<<r<<std::endl;
                    PBuddy.AddMove ( r );
                    chosen = false;
                } else {
                    PBuddy.Play ( PBuddy2.AIPositions, PBuddy.AIPositions );
                }
                fturn = !fturn;
                pl = PBuddy.LastMove() - 1;
                // std::cout<<"TESTING: "<< pl <<std::endl;
                cB ( pl ) = -1;
                cE ( pl ) = -1;
            }
            else {
                PBuddy2.Play ( PBuddy.AIPositions, PBuddy2.AIPositions );
                fturn = !fturn;
                cE ( PBuddy2.LastMove() - 1 ) = 1;
                // Place the moves into matrices
                cPair.row ( 0 ) = cB;
                cPair.row ( 1 ) = cE;
                TData.push_back ( cPair );
                // Update cB with new move
                cB ( PBuddy2.LastMove() - 1 ) = 1;
            }
            //DisplayB(PBuddy.AIPositions,PBuddy2.AIPositions);
        }
        // Reset
        fturn = true;
        PBuddy.AIRematch();
        PBuddy2.AIRematch();
        cB.fill ( 0.1 );
        cE.fill ( 0.1 );
    }
}
double PT(dlib::matrix<double> CM);//Play training
double CW(dlib::matrix<double> CM){ //Wrapper for cost function
     //
     //std::cout<<std::endl<<"CW Called"<<std::endl;
     double theCost=0.0; //Cost
     double TCost=0.0; //Total Cost
     int CC=0;
     //srand( time(NULL) );
     //
//
     mat W1Copy;
     W1Copy.set_size(Testnet.getW1().n_rows,Testnet.getW1().n_cols);
     W1Copy.fill(fill::zeros);
     mat W2Copy;
     W2Copy.set_size(Testnet.getW2().n_rows,Testnet.getW2().n_cols);
     W2Copy.fill(fill::zeros);
     mat W3Copy;
     W3Copy.set_size(Testnet.getW3().n_rows,Testnet.getW3().n_cols);
     W3Copy.fill(fill::zeros);
     mat W4Copy;
     W4Copy.set_size(Testnet.getW4().n_rows,Testnet.getW4().n_cols);
     W4Copy.fill(fill::zeros);
     mat W5Copy;
     W5Copy.set_size(Testnet.getW5().n_rows,Testnet.getW5().n_cols);
     W5Copy.fill(fill::zeros);
     mat W6Copy;
     W6Copy.set_size(Testnet.getW6().n_rows,9);
     W6Copy.fill(fill::zeros);
     //mat W4Copy;
     //W4Copy.set_size(3,1);
     //W4Copy.fill(fill::zeros);
     //
     mat BL1; //Biases layers one
     BL1.set_size(1, Testnet.getNIL().size());
     mat BL2; //Biases layers
     BL2.set_size(1, Testnet.getNHL().size());
     mat BL3; //Biases layers
     BL3.set_size(1, Testnet.getNHL2().size());
     mat BL4; //Biases layers
     BL4.set_size(1, Testnet.getNHL3().size());
     mat BL5; //Biases layers
     BL5.set_size(1, Testnet.getNHL4().size());
     mat BL6; //Biases layers
     BL6.set_size(1, Testnet.getNOL().size());
     mat BL7; //Biases layers
     BL7.set_size(1, Testnet.getOL().size());
     //
     mat OCopy;
     mat TE;
     TE.set_size(1,9);
     TE.fill(fill::ones);
     mat I;
     int c=0;
     if( CM.size() == Testnet.getNIL().size() + Testnet.getNHL().size() + Testnet.getNHL2().size() + Testnet.getNHL3().size() + Testnet.getNHL4().size() + Testnet.getNOL().size() + Testnet.getOL().size()){
          for(int i=0; i<BL1.size(); i++){ //1st Neuron layer biases
               BL1(i)=CM(0,c);
               c++;
          }
          for(int i=0; i<BL2.size(); i++){ //2nd Neuron layer biases
               BL2(i)=CM(0,c);
               c++;
          }
          for(int i=0; i<BL3.size(); i++){ //3rd Neuron layer biases
               BL3(i)=CM(0,c);
               c++;
          }
          for(int i=0; i<BL4.size(); i++){ //4th Neuron layer biases
               BL4(i)=CM(0,c);
               c++;
          }
          for(int i=0; i<BL5.size(); i++){ //5th Neuron layer biases
               BL5(i)=CM(0,c);
               c++;
          }
          for(int i=0; i<BL6.size(); i++){ //6th Neuron layer biases
               BL6(i)=CM(0,c);
               c++;
          }
          for(int i=0; i<BL7.size(); i++){ //7th Neuron layer biases
               BL7(i)=CM(0,c);
               c++;
          }
          c=0;
          //Takes care of the weights
          W1Copy=Testnet.getW1();
          W2Copy=Testnet.getW2();
          W3Copy=Testnet.getW3();
          W4Copy=Testnet.getW4();
          W5Copy=Testnet.getW5();
          W6Copy=Testnet.getW6();
     }else{
          //W1 matrix
          for(int i=0; i<W1Copy.n_rows; i++){ //First weight matrix
               for(int j=0; j<W1Copy.n_cols; j++){
                    W1Copy(i,j)=CM(0,c);
                    c++;
               }
          }
          for(int i=0; i<W2Copy.n_rows; i++){ //Second weight matrix
               for(int j=0; j<W2Copy.n_cols; j++){
                    W2Copy(i,j)=CM(0,c);
                    c++;
               }
          }
          for(int i=0; i<W3Copy.n_rows; i++){ //Third weight matrix
               for(int j=0; j<W3Copy.n_cols; j++){
                    W3Copy(i,j)=CM(0,c);
                    c++;
               }
          }
          for(int i=0; i<W4Copy.n_rows; i++){ //Fourth weight matrix
               for(int j=0; j<W4Copy.n_cols; j++){
                    W4Copy(i,j)=CM(0,c);
                    c++;
               }
          }
          for(int i=0; i<W5Copy.n_rows; i++){ //Fifth weight matrix
               for(int j=0; j<W5Copy.n_cols; j++){
                    W5Copy(i,j)=CM(0,c);
                    c++;
               }
          }
          for(int i=0; i<W6Copy.n_rows; i++){ //Fifth weight matrix
               for(int j=0; j<W6Copy.n_cols; j++){
                    W6Copy(i,j)=CM(0,c);
                    c++;
               }
          }
          c=0;
          //Takes care of the bias
          BL1.for_each( [&c](mat::elem_type& val ){ val= Testnet.getNIL().at(c).bias; c++; } );
          c=0;
          BL2.for_each( [&c](mat::elem_type& val ){ val= Testnet.getNHL().at(c).bias; c++; } );
          c=0;
          BL3.for_each( [&c](mat::elem_type& val ){ val= Testnet.getNHL2().at(c).bias; c++; } );
          c=0;
          BL4.for_each( [&c](mat::elem_type& val ){ val= Testnet.getNHL3().at(c).bias; c++; } );
          c=0;
          BL5.for_each( [&c](mat::elem_type& val ){ val= Testnet.getNHL4().at(c).bias; c++; } );
          c=0;
          BL6.for_each( [&c](mat::elem_type& val ){ val= Testnet.getNOL().at(c).bias; c++; } );
          c=0;
          BL7.for_each( [&c](mat::elem_type& val ){ val= Testnet.getOL().at(c).bias; c++; } );
          c=0;
     }
     /*
     for(int i=W1Copy.size()+W2Copy.size()+W3Copy.size(); i<W1Copy.size()+W2Copy.size()+W3Copy.size()+W4Copy.size(); i++){ //Fourth weight matrix
          W4Copy(c,0)=CM(0,i);
          c++;
     } */
     for(int i=0; i<TData.size(); i++){


          c=0;
          //Calstd stuff
          I=TData.at(i).row(0);
          //std::cout<<"I: "<<std::endl; I.print(); std::cout<<std::endl;
          I.for_each( [&c, &BL1](mat::elem_type& val ){ val+= BL1(c); c++; } );
          c=0;
          //

          //Adding the bias
          // OCopy= (I + (.33*Testnet.VP(I)) ) * W1Copy; // 1x9 * 9x9 = 1x9
          OCopy= (I) * W1Copy; // 1x9 * 9x9 = 1x9
          OCopy.for_each( [&c, &BL2](mat::elem_type& val ){ val+= BL2(c); c++;} );
          c=0;
          OCopy.for_each( [](mat::elem_type& val ){ val= tanh(val); } );
          OCopy = OCopy * W2Copy; // 1x9 * 9x18 = 1x18
          OCopy.for_each( [&c, &BL3](mat::elem_type& val ){ val+= BL3(c); c++; } );
          c=0;
          OCopy.for_each( [](mat::elem_type& val ){ val= tanh(val); } );
          OCopy = OCopy * W3Copy; // 1x18 * 18x36 = 1x36
          OCopy.for_each( [&c, &BL4](mat::elem_type& val ){ val+= BL4(c); c++; } );
          c=0;
          OCopy.for_each( [](mat::elem_type& val ){ val= tanh(val); } );
          OCopy = OCopy * W4Copy; // 1x36 * 36x18 = 1x18
          OCopy.for_each( [&c, &BL5](mat::elem_type& val ){ val+= BL5(c); c++; } );
          c=0;
          OCopy.for_each( [](mat::elem_type& val ){ val= tanh(val); } );
          OCopy = OCopy * W5Copy; // 1x18 * 18x9 = 1x9
          OCopy.for_each( [&c, &BL6](mat::elem_type& val ){ val+= BL6(c); c++; } ); //
          c=0;
          OCopy.for_each( [](mat::elem_type& val ){ val= tanh(val); } );
          OCopy = OCopy * W6Copy; // 1x9 * 9x9 = 1x9
          OCopy.for_each( [&c, &BL7](mat::elem_type& val ){ val+= BL7(c); c++; } ); //
          c=0;
          OCopy.for_each( [](mat::elem_type& val ){ val= tanh(val); } );
          //Cost
          TE=TData.at(i).row(1);
          //std::cout<<"TE: "<<std::endl; TE.print();
          TE.for_each( [&CC,&c](mat::elem_type& val ){ if(val==2){CC=c;} c++;} );
          //std::cout<<"OCopy: "<<std::endl; OCopy.print();
          TE=TE-OCopy; //Takes the matrix and adds by the output of the NN
          //std::cout<<"TE Compare: "<<std::endl; TE.print();
          TE.for_each( [](mat::elem_type& val ){ val= pow(val, 2); } );

               switch(CC){ //Adds a weight to the correct choice
                    case 0: TE(0)*=10; break;
                    case 1: TE(1)*=10; break;
                    case 2: TE(2)*=10; break;
                    case 3: TE(3)*=10; break;
                    case 4: TE(4)*=10; break;
                    case 5: TE(5)*=10; break;
                    case 6: TE(6)*=10; break;
                    case 7: TE(7)*=10; break;
                    case 8: TE(8)*=10; break;
                    default: std::cout<<"ERROR: Switch"<<std::endl; break;
               };

          theCost=accu(TE); //Calculates total error by taking a sum of all elements
          theCost*=.75;
          //theCost+= (.0001/2)* ( accu( square(W1Copy) ) + accu( square(W2Copy) ) + accu( square(W3Copy) ) );
          TCost+=theCost;
     }
     //
     TCost+=(.425/2)* ( accu( square(W1Copy) ) + accu( square(W2Copy) ) + accu( square(W3Copy) ) + accu( square(W4Copy) ) + accu( square(W5Copy) ) + 2*( accu( square(W6Copy) ) ) );
     return TCost;

}
//
double NN::UScore(dlib::matrix<double> CM){
     int score=0;
     int c=0;
     SBoard UB; int rDec=0; int mCount=0; bool cw=false;
     int ansNum=0;
     //
     if( CM.size() == UW1.size() + UW2.size() + UW3.size() + UW4.size() ){
          //
          for(int i=0; i<UW1.n_rows; i++){ //First weight matrix
               for(int j=0; j<UW1.n_cols; j++){
                    UW1(i,j)=CM(0,c);
                    c++;
               }
          }
          for(int i=0; i<UW2.n_rows; i++){ //weight matrix
               for(int j=0; j<UW2.n_cols; j++){
                    UW2(i,j)=CM(0,c);
                    c++;
               }
          }
          for(int i=0; i<UW3.n_rows; i++){ //weight matrix
               for(int j=0; j<UW3.n_cols; j++){
                    UW3(i,j)=CM(0,c);
                    c++;
               }
          }
          for(int i=0; i<UW4.n_rows; i++){ //weight matrix
               for(int j=0; j<UW4.n_cols; j++){
                    UW4(i,j)=CM(0,c);
                    c++;
               }
          }
     }else{
          for(int i=0; i<UL2.size(); i++){ //Neuron layer biases
               UL2[i].bias=CM(0,c);
               c++;
          }
          for(int i=0; i<UL3.size(); i++){ //Neuron layer biases
               UL3[i].bias=CM(0,c);
               c++;
          }
          for(int i=0; i<UL4.size(); i++){ //Neuron layer biases
               UL4[i].bias=CM(0,c);
               c++;
          }
          for(int i=0; i<UL5.size(); i++){ //Neuron layer biases
               UL5[i].bias=CM(0,c);
               c++;
          }
     }
     //
     c=-1; bool fMove=true;//first players move
     while( cw!=true ){
          if(UB.getWon() !=0){cw=true;} if(cw==true){break;}
          UBI(UB.getTBoard(),UB.getpBoards());
          Calstd();
          for(int i=0; i<UOutput.n_rows; i++){
               for(int j=0; j<UOutput.n_cols; j++){
                    c++;
                    if(UOutput(i,j) >ansNum){ ansNum=UOutput(i,j); rDec=c; }else{}
               }
          }
          if( UB.getpBoards().at( (c/9) ) == 0 ){
               score++;
               if( UB.getTBoard().at( (c/9) ).Board( ( (c - ( (c/9) * 9) ) /3 ) , ( (c+3)%3 ) ) == 0 ){
                    score++;
               }else{
                    score-=10;
                    return score;
               }
          }else{
               score-=15;
               return score;
          }

          if( fMove ){ if(UB.placeMove(rDec,true) !=true){ return score--;} fMove=false; }else{ if(UB.placeMove(rDec,false) !=true){ return score--;} fMove=true; }

     }
     return score;
}
double UScoreWrapper(dlib::matrix<double> CM){
     return Testnet.UScore(CM);
}
//
void NN::Load(){
std::fstream TInput, TUInput;
std::string cLine="";
std::string cWeight="";
int c=0;
char cChar;
TInput.open("TOutput.txt"); //Reads an existing file
if(TInput.is_open()!=true){
     std::cout<<"Could not open file"<<std::endl;
}
while(TInput.is_open()){
                              //W1
                              std::getline(TInput,cLine);
                              if(cLine.find("W1")!=std::string::npos){
                                   std::cout<<std::endl<<"Loading Weights..."<<std::endl;

                                   for(int i=0; i<W1.n_rows; i++){
                                        std::getline(TInput,cLine);
                                             for(int k=0; k<cLine.size(); k++){
                                                  while(cLine.at(k)!=' '){
                                                       cChar=cLine.at(k);
                                                       if(cLine.at(k)==' ')break;
                                                       cWeight+=cLine.at(k);
                                                       k++;
                                                  }
                                                  if(cLine.at(k)==' '){ W1(i,c)=atof(cWeight.c_str()); c++; cWeight="";}
                                             }
                                             c=0;
                                   }
                              }
                              c=0;
                              //W2

                              if(cLine.find("W2")!=std::string::npos){
                                   for(int i=0; i<W2.n_rows; i++){
                                        std::getline(TInput,cLine);
                                        for(int k=0; k<cLine.size(); k++){
                                             while(cLine.at(k)!=' '){
                                                  if(cLine.at(k)==' ')break;
                                                  cWeight+=cLine.at(k);
                                                  k++;
                                             }
                                             if(cLine.at(k)==' '){ W2(i,c)=atof(cWeight.c_str()); c++; cWeight="";}
                                        }
                                        c=0;
                                   }
                              }
                              c=0;
                              //W3

                              if(cLine.find("W3")!=std::string::npos){
                              for(int i=0; i<W3.n_rows; i++){
                                   std::getline(TInput,cLine);
                                             for(int k=0; k<cLine.size(); k++){
                                                  while(cLine.at(k)!=' '){
                                                       if(cLine.at(k)==' ')break;
                                                       cWeight+=cLine.at(k);
                                                       k++;
                                                  }
                                                  if(cLine.at(k)==' '){ W3(i,c)=atof(cWeight.c_str()); c++; cWeight="";}
                                             }
                                             c=0;
                              }
                              }
                              c=0;
                              //W4

                              if(cLine.find("W4")!=std::string::npos){
                              for(int i=0; i<W4.n_rows; i++){
                                   std::getline(TInput,cLine);
                                             for(int k=0; k<cLine.size(); k++){
                                                  while(cLine.at(k)!=' '){
                                                       if(cLine.at(k)==' ')break;
                                                       cWeight+=cLine.at(k);
                                                       k++;
                                                  }
                                                  if(cLine.at(k)==' '){ W4(i,c)=atof(cWeight.c_str()); c++; cWeight="";}
                                             }
                                             c=0;
                              }
                              }
                              c=0;
                              //W5

                              if(cLine.find("W5")!=std::string::npos){
                              for(int i=0; i<W5.n_rows; i++){
                                   std::getline(TInput,cLine);
                                             for(int k=0; k<cLine.size(); k++){
                                                  while(cLine.at(k)!=' '){
                                                       if(cLine.at(k)==' ')break;
                                                       cWeight+=cLine.at(k);
                                                       k++;
                                                  }
                                                  if(cLine.at(k)==' '){ W5(i,c)=atof(cWeight.c_str()); c++; cWeight="";}
                                             }
                                             c=0;
                              }
                              }
                              c=0;
                              //W6

                              if(cLine.find("W6")!=std::string::npos){
                              for(int i=0; i<W6.n_rows; i++){
                                   std::getline(TInput,cLine);
                                             for(int k=0; k<cLine.size(); k++){
                                                  while(cLine.at(k)!=' '){
                                                       if(cLine.at(k)==' ')break;
                                                       cWeight+=cLine.at(k);
                                                       k++;
                                                  }
                                                  if(cLine.at(k)==' '){ W6(i,c)=atof(cWeight.c_str()); c++; cWeight="";}
                                             }
                                             c=0;
                              }
                              }
                              c=0;
                              //Biases

                              if(cLine.find("Bias")!=std::string::npos){
                                   std::cout<<std::endl<<"Loading Biases..."<<std::endl;
                                   std::getline(TInput,cLine);
                                   //1st Neuron layer biases
                                        for(int k=0; k<cLine.size(); k++){
                                             while(cLine.at(k)!=' '){
                                                  if(cLine.at(k)==' ')break;
                                                  cWeight+=cLine.at(k);
                                                  k++;
                                             }
                                             if(cLine.at(k)==' '){ NIL.at(c).bias=atof(cWeight.c_str()); c++; cWeight="";}
                                        }
                                   c=0;

                                   std::getline(TInput,cLine);
                                   //2st Neuron layer biases
                                        for(int k=0; k<cLine.size(); k++){
                                             while(cLine.at(k)!=' '){
                                                  if(cLine.at(k)==' ')break;
                                                  cWeight+=cLine.at(k);
                                                  k++;
                                             }
                                             if(cLine.at(k)==' '){ NHL.at(c).bias=atof(cWeight.c_str());  c++; cWeight="";}
                                        }
                                   c=0;

                                   std::getline(TInput,cLine);
                                   //3rd Neuron layer biases
                                        for(int k=0; k<cLine.size(); k++){
                                             while(cLine.at(k)!=' '){
                                                  if(cLine.at(k)==' ')break;
                                                  cWeight+=cLine.at(k);
                                                  k++;
                                             }
                                             if(cLine.at(k)==' '){ NHL2.at(c).bias=atof(cWeight.c_str()); c++; cWeight="";}
                                        }
                                   c=0;

                                   std::getline(TInput,cLine);
                                   //4th Neuron layer biases
                                        for(int k=0; k<cLine.size(); k++){
                                             while(cLine.at(k)!=' '){
                                                  if(cLine.at(k)==' ')break;
                                                  cWeight+=cLine.at(k);
                                                  k++;
                                             }
                                             if(cLine.at(k)==' '){ NHL3.at(c).bias=atof(cWeight.c_str()); c++; cWeight="";}
                                        }
                                   c=0;

                                   std::getline(TInput,cLine);
                                   //5th Neuron layer biases
                                        for(int k=0; k<cLine.size(); k++){
                                             while(cLine.at(k)!=' '){
                                                  if(cLine.at(k)==' ')break;
                                                  cWeight+=cLine.at(k);
                                                  k++;
                                             }
                                             if(cLine.at(k)==' '){ NHL4.at(c).bias=atof(cWeight.c_str()); c++; cWeight="";}
                                        }
                                   c=0;
                                   std::getline(TInput,cLine);
                                   //Output Neuron layer biases
                                        for(int k=0; k<cLine.size(); k++){
                                             while(cLine.at(k)!=' '){
                                                  if(cLine.at(k)==' ')break;
                                                  cWeight+=cLine.at(k);
                                                  k++;
                                             }
                                             if(cLine.at(k)==' '){ NOL.at(c).bias=atof(cWeight.c_str()); c++; cWeight="";}
                                        }
                                   c=0;

                                   std::getline(TInput,cLine);
                                   //Output Neuron layer biases
                                        for(int k=0; k<cLine.size(); k++){
                                             while(cLine.at(k)!=' '){
                                                  if(cLine.at(k)==' ')break;
                                                  cWeight+=cLine.at(k);
                                                  k++;
                                             }
                                             if(cLine.at(k)==' '){ OL.at(c).bias=atof(cWeight.c_str()); c++; cWeight="";}
                                        }
                                   c=0;
                                   TInput.close();
                              }

                         }
                         //Alters the connections
     for(int i=0; i<NIL.size(); i++){ //
          for(int j=0; j<NIL.at(i).Connections.size(); j++){
               NIL.at(i).Connections.at(j).weight=W1(i,j);
          }
     }
     for(int i=0; i<NHL.size(); i++){
          for(int j=0; j<NHL.at(i).Connections.size(); j++){
               NHL.at(i).Connections.at(j).weight=W2(i,j);
          }
     }
     for(int i=0; i<NHL2.size(); i++){
          for(int j=0; j<NHL2.at(i).Connections.size(); j++){
               NHL2.at(i).Connections.at(j).weight=W3(i,j);
          }
     }
     for(int i=0; i<NHL3.size(); i++){
          for(int j=0; j<NHL3.at(i).Connections.size(); j++){
               NHL3.at(i).Connections.at(j).weight=W4(i,j);
          }
     }
     for(int i=0; i<NHL4.size(); i++){
          for(int j=0; j<NHL4.at(i).Connections.size(); j++){
               NHL4.at(i).Connections.at(j).weight=W5(i,j);
          }
     }
     for(int i=0; i<NOL.size(); i++){
          for(int j=0; j<NOL.at(i).Connections.size(); j++){
               NOL.at(i).Connections.at(j).weight=W6(i,j);
          }
     }
     if(ultimate){
          cLine="";
          cWeight="";
          cChar=' ';
          c=0;
          TUInput.open("TUOutput.txt"); //Reads an existing file
          if(TUInput.is_open()!=true){
               std::cout<<"Could not open file"<<std::endl;
               return;
          }
          while(TUInput.is_open() ){
               //UW1
               std::getline(TUInput,cLine);
               if(cLine.find("UW1")!=std::string::npos){
                     std::cout<<std::endl<<"Loading Ultimate Weights..."<<std::endl;
                    for(int i=0; i<UW1.n_rows; i++){
                         std::getline(TUInput,cLine);
                              for(int k=0; k<cLine.size(); k++){
                                   while(cLine.at(k)!=' '){
                                        cChar=cLine.at(k);
                                        if(cLine.at(k)==' ')break;
                                        cWeight+=cLine.at(k);
                                        k++;
                                   }
                                   if(cLine.at(k)==' '){ UW1(i,c)=atof(cWeight.c_str()); c++; cWeight="";}
                              }
                              c=0;
                    }
               }
               c=0;
               //UW2
               if(cLine.find("W2")!=std::string::npos){
                    for(int i=0; i<UW2.n_rows; i++){
                         std::getline(TUInput,cLine);
                              for(int k=0; k<cLine.size(); k++){
                                   while(cLine.at(k)!=' '){
                                        cChar=cLine.at(k);
                                        if(cLine.at(k)==' ')break;
                                        cWeight+=cLine.at(k);
                                        k++;
                                   }
                                   if(cLine.at(k)==' '){ UW2(i,c)=atof(cWeight.c_str()); c++; cWeight="";}
                              }
                              c=0;
                    }
               }
               c=0;
               //UW3
               if(cLine.find("W3")!=std::string::npos){
                    for(int i=0; i<UW3.n_rows; i++){
                         std::getline(TUInput,cLine);
                              for(int k=0; k<cLine.size(); k++){
                                   while(cLine.at(k)!=' '){
                                        cChar=cLine.at(k);
                                        if(cLine.at(k)==' ')break;
                                        cWeight+=cLine.at(k);
                                        k++;
                                   }
                                   if(cLine.at(k)==' '){ UW3(i,c)=atof(cWeight.c_str()); c++; cWeight="";}
                              }
                              c=0;
                    }
               }
               c=0;
               //UW4
               if(cLine.find("W4")!=std::string::npos){
                    for(int i=0; i<UW4.n_rows; i++){
                         std::getline(TUInput,cLine);
                              for(int k=0; k<cLine.size(); k++){
                                   while(cLine.at(k)!=' '){
                                        cChar=cLine.at(k);
                                        if(cLine.at(k)==' ')break;
                                        cWeight+=cLine.at(k);
                                        k++;
                                   }
                                   if(cLine.at(k)==' '){ UW4(i,c)=atof(cWeight.c_str()); c++; cWeight="";}
                              }
                              c=0;
                    }
               }
               c=0;
               //Biases
               if(cLine.find("Ultimate_Biases")!=std::string::npos){
                     std::cout<<std::endl<<"Loading Ultimate Biases..."<<std::endl;
                    std::getline(TUInput,cLine);
                    //1st Neuron layer biases
                         for(int k=0; k<cLine.size(); k++){
                              while(cLine.at(k)!=' '){
                                   if(cLine.at(k)==' ')break;
                                   cWeight+=cLine.at(k);
                                   k++;
                              }
                              if(cLine.at(k)==' '){ UL1.at(c).bias=atof(cWeight.c_str()); c++; cWeight="";}
                         }
                    c=0;
                    std::getline(TUInput,cLine);
                    //2nd Neuron layer biases
                         for(int k=0; k<cLine.size(); k++){
                              while(cLine.at(k)!=' '){
                                   if(cLine.at(k)==' ')break;
                                   cWeight+=cLine.at(k);
                                   k++;
                              }
                              if(cLine.at(k)==' '){ UL2.at(c).bias=atof(cWeight.c_str()); c++; cWeight="";}
                         }
                    c=0;
                    std::getline(TUInput,cLine);
                    //3rd Neuron layer biases
                         for(int k=0; k<cLine.size(); k++){
                              while(cLine.at(k)!=' '){
                                   if(cLine.at(k)==' ')break;
                                   cWeight+=cLine.at(k);
                                   k++;
                              }
                              if(cLine.at(k)==' '){ UL3.at(c).bias=atof(cWeight.c_str()); c++; cWeight="";}
                         }
                    c=0;
                    std::getline(TUInput,cLine);
                    //4th Neuron layer biases
                         for(int k=0; k<cLine.size(); k++){
                              while(cLine.at(k)!=' '){
                                   if(cLine.at(k)==' ')break;
                                   cWeight+=cLine.at(k);
                                   k++;
                              }
                              if(cLine.at(k)==' '){ UL4.at(c).bias=atof(cWeight.c_str()); c++; cWeight="";}
                         }
                    c=0;
                    std::getline(TUInput,cLine);
                    //5th Neuron layer biases
                         for(int k=0; k<cLine.size(); k++){
                              while(cLine.at(k)!=' '){
                                   if(cLine.at(k)==' ')break;
                                   cWeight+=cLine.at(k);
                                   k++;
                              }
                              if(cLine.at(k)==' '){ UL5.at(c).bias=atof(cWeight.c_str()); c++; cWeight="";}
                         }
                    c=0;
                    TUInput.close();
               }
          }
          }
}
void NN::Save(){
std::ofstream TOutput;
//Saves the discovered weights and biases
     TOutput.open("TOutput.txt", std::ios::in | std::ios::trunc);
     if(TOutput.is_open()){
          //W1
          TOutput<<"\nW1:\n";
          for(int i=0; i<W1.n_rows; i++){
               for(int j=0; j<W1.n_cols; j++){
               TOutput<<""<<W1(i,j)<<" ";
               }
               TOutput<<"\n";
          }
          //W2
          TOutput<<"\nW2:\n";
          for(int i=0; i<W2.n_rows; i++){
               for(int j=0; j<W2.n_cols; j++){
               TOutput<<""<<W2(i,j)<<" ";
               }
               TOutput<<"\n";
          }
          //W3
          TOutput<<"\nW3:\n";
          for(int i=0; i<W3.n_rows; i++){
               for(int j=0; j<W3.n_cols; j++){
               TOutput<<""<<W3(i,j)<<" ";
               }
               TOutput<<"\n";
          }
          //W4
          TOutput<<"\nW4:\n";
          for(int i=0; i<W4.n_rows; i++){
               for(int j=0; j<W4.n_cols; j++){
               TOutput<<""<<W4(i,j)<<" ";
               }
               TOutput<<"\n";
          }
          //W5
          TOutput<<"\nW5:\n";
          for(int i=0; i<W5.n_rows; i++){
               for(int j=0; j<W5.n_cols; j++){
               TOutput<<""<<W5(i,j)<<" ";
               }
               TOutput<<"\n";
          }
          //W5
          TOutput<<"\nW6:\n";
          for(int i=0; i<W6.n_rows; i++){
               for(int j=0; j<W6.n_cols; j++){
               TOutput<<""<<W6(i,j)<<" ";
               }
               TOutput<<"\n";
          }
          //Biases
          TOutput<<"\nBiases:\n";
          for(int i=0; i<NIL.size(); i++){ //1st Neuron layer biases
          TOutput<<NIL.at(i).bias<<" ";
          }
          TOutput<<"\n";
          for(int i=0; i<NHL.size(); i++){ //2nd Neuron layer biases
               TOutput<<NHL.at(i).bias<<" ";
          }
          TOutput<<"\n";
          for(int i=0; i<NHL2.size(); i++){ //3rd Neuron layer biases
               TOutput<<NHL2.at(i).bias<<" ";
          }
          TOutput<<"\n";
          for(int i=0; i<NHL3.size(); i++){ //4th Neuron layer biases
               TOutput<<NHL3.at(i).bias<<" ";
          }
          TOutput<<"\n";
          for(int i=0; i<NHL4.size(); i++){ //5th Neuron layer biases
               TOutput<<NHL4.at(i).bias<<" ";
          }
          TOutput<<"\n";
          for(int i=0; i<NOL.size(); i++){ //6th Neuron layer biases
               TOutput<<NOL.at(i).bias<<" ";
          }
          TOutput<<"\n";
          for(int i=0; i<OL.size(); i++){ //7th Neuron layer biases
               TOutput<<OL.at(i).bias<<" ";
          }
          TOutput.close();
     }
     if(ultimate){
          TOutput.open("TUOutput.txt", std::ios::in | std::ios::trunc);
          if(TOutput.is_open()){
               //UW1
               TOutput<<"\nUW1:\n";
               for(int i=0; i<UW1.n_rows; i++){
                    for(int j=0; j<UW1.n_cols; j++){
                         TOutput<<""<<UW1(i,j)<<" ";
                    }
                    TOutput<<"\n";
               }
               //UW2
               TOutput<<"\nUW2:\n";
               for(int i=0; i<UW2.n_rows; i++){
                    for(int j=0; j<UW2.n_cols; j++){
                         TOutput<<""<<UW2(i,j)<<" ";
                    }
                    TOutput<<"\n";
               }
               //UW3
               TOutput<<"\nUW3:\n";
               for(int i=0; i<UW3.n_rows; i++){
                    for(int j=0; j<UW3.n_cols; j++){
                         TOutput<<""<<UW3(i,j)<<" ";
                    }
                    TOutput<<"\n";
               }
               //UW4
               TOutput<<"\nUW4:\n";
               for(int i=0; i<UW4.n_rows; i++){
                    for(int j=0; j<UW4.n_cols; j++){
                         TOutput<<""<<UW4(i,j)<<" ";
                    }
                    TOutput<<"\n";
               }
               //
               TOutput<<"\nUltimate_Biases:\n";
               for(int i=0; i<UL1.size(); i++){ //1st Ultimate Neuron layer biases
                    TOutput<<UL1.at(i).bias<<" ";
               }
               TOutput<<"\n";
               for(int i=0; i<UL2.size(); i++){ //1st Ultimate Neuron layer biases
                    TOutput<<UL2.at(i).bias<<" ";
               }
               TOutput<<"\n";
               for(int i=0; i<UL3.size(); i++){ //1st Ultimate Neuron layer biases
                    TOutput<<UL3.at(i).bias<<" ";
               }
               TOutput<<"\n";
               for(int i=0; i<UL4.size(); i++){ //1st Ultimate Neuron layer biases
                    TOutput<<UL4.at(i).bias<<" ";
               }
               TOutput<<"\n";
               for(int i=0; i<UL5.size(); i++){ //1st Ultimate Neuron layer biases
                    TOutput<<UL5.at(i).bias<<" ";
               }
               TOutput<<"\n";
               TOutput.close();
          }
     }
     std::cout<<"Save Complete"<<std::endl;
}
void NN::Train(bool testing=false){
     using namespace dlib;
     std::ofstream TOutput; //Input and output for saving weights and biases
     std::ifstream TInput;
     std::string cLine="";
     std::string cWeight="";
     char cChar;
     std::vector<double> acost; //The achieved cost from find_min
     double MeanACost=0.0;
     long ms = (W1.size() + W2.size() + W3.size() + W4.size() + W5.size() + W6.size());
     long bs = (NIL.size() + NHL.size() + NHL2.size() + NHL3.size() + NHL4.size() + NOL.size() + OL.size());
     dlib::matrix<double, 0, 1> WeightM; //Matrix containing all the weights
     WeightM.set_size(ms);
     dlib::matrix<double, 0, 1> BiasM; //Matrix containing all the biases
     BiasM.set_size(bs);
     //Loads from file
     Load();
     int c=0;
     if(ultimate){
          ms = (UW1.size() + UW2.size() + UW3.size() + UW4.size() ); WeightM.set_size(ms);
          bs = ( UL2.size() + UL3.size() + UL4.size() + UL5.size() ); BiasM.set_size(bs);

          for(int i=0; i<UW1.n_rows; i++){ //First weight matrix
               for(int j=0; j<UW1.n_cols; j++){
                    WeightM(0,c) = UW1(i,j);
                    c++;
               }
          }
          for(int i=0; i<UW2.n_rows; i++){ //weight matrix
               for(int j=0; j<UW2.n_cols; j++){
                    WeightM(0,c) = UW2(i,j);
                    c++;
               }
          }
          for(int i=0; i<UW3.n_rows; i++){ //weight matrix
               for(int j=0; j<UW3.n_cols; j++){
                    WeightM(0,c) = UW3(i,j);
                    c++;
               }
          }
          for(int i=0; i<UW4.n_rows; i++){ //weight matrix
               for(int j=0; j<UW4.n_cols; j++){
                    WeightM(0,c) = UW4(i,j);
                    c++;
               }
          }
          //biases
          c=0;
          for(int i=0; i<UL2.size(); i++){ //Neuron layer biases
               BiasM(0,c) = UL2.at(i).bias;
               c++;
          }
          for(int i=0; i<UL3.size(); i++){ //Neuron layer biases
               BiasM(0,c) = UL3.at(i).bias;
               c++;
          }
          for(int i=0; i<UL4.size(); i++){ //Neuron layer biases
               BiasM(0,c) = UL4.at(i).bias;
               c++;
          }
          for(int i=0; i<UL5.size(); i++){ //Neuron layer biases
               BiasM(0,c) = UL5.at(i).bias;
               c++;
          }
     }else{
          for(int i=0; i<W1.n_rows; i++){ //First weight matrix
               for(int j=0; j<W1.n_cols; j++){
                    WeightM(0,c) = W1(i,j);
                    c++;
               }
          }
          for(int i=0; i<W2.n_rows; i++){ //Second weight matrix
               for(int j=0; j<W2.n_cols; j++){
                    WeightM(0,c) = W2(i,j);
                    c++;
               }
          }
          for(int i=0; i<W3.n_rows; i++){ //Third weight matrix
               for(int j=0; j<W3.n_cols; j++){
                    WeightM(0,c) = W3(i,j);
                    c++;
               }
          }
          for(int i=0; i<W4.n_rows; i++){ //4 weight matrix
               for(int j=0; j<W4.n_cols; j++){
                    WeightM(0,c) = W4(i,j);
                    c++;
               }
          }
          for(int i=0; i<W5.n_rows; i++){ //5 weight matrix
               for(int j=0; j<W5.n_cols; j++){
                    WeightM(0,c) = W5(i,j);
                    c++;
               }
          }
          for(int i=0; i<W6.n_rows; i++){ //6 weight matrix
               for(int j=0; j<W6.n_cols; j++){
                    WeightM(0,c) = W6(i,j);
                    c++;
               }
          }
          c=0;
          for(int i=0; i<NIL.size(); i++){ //1st Neuron layer biases
               BiasM(0,c) = NIL.at(i).bias;
               c++;
          }
          for(int i=0; i<NHL.size(); i++){ //2nd Neuron layer biases
               BiasM(0,c) = NHL.at(i).bias;
               c++;
          }
          for(int i=0; i<NHL2.size(); i++){ //3rd Neuron layer biases
               BiasM(0,c) = NHL2.at(i).bias;
               c++;
          }
          for(int i=0; i<NHL3.size(); i++){ //4th Neuron layer biases
               BiasM(0,c) = NHL3.at(i).bias;
               c++;
          }
          for(int i=0; i<NHL4.size(); i++){ //5th Neuron layer biases
               BiasM(0,c) = NHL4.at(i).bias;
               c++;
          }
          for(int i=0; i<NOL.size(); i++){ //6th Neuron layer biases
               BiasM(0,c) = NOL.at(i).bias;
               c++;
          }
          for(int i=0; i<OL.size(); i++){ //7th Neuron layer biases
               BiasM(0,c) = OL.at(i).bias;
               c++;
          }
          c=0;
     }
     /*
     std::cout<<"Big Baller: "<<std::endl;
     W1.print(); std::cout<<std::endl;
     W2.print(); std::cout<<std::endl;
     W3.print(); std::cout<<std::endl;
     std::cout<<"Big Baller Bias: "<<std::endl;
     for(int i=0; i<NIL.size(); i++){ //1st Neuron layer biases
          std::cout<<NIL.at(i).bias<<" ";
     }
     std::cout<<std::endl;
     for(int i=0; i<NHL.size(); i++){ //2nd Neuron layer biases
          std::cout<<NHL.at(i).bias<<" ";
     }
     std::cout<<std::endl;
     for(int i=0; i<NHL2.size(); i++){ //3rd Neuron layer biases
          std::cout<<NHL2.at(i).bias<<" ";
     }
     std::cout<<std::endl;
     for(int i=0; i<NOL.size(); i++){ //4th Neuron layer biases
          std::cout<<NOL.at(i).bias<<" ";
     }
     std::cout<<std::endl;
     //
     */
          if(testing!=true){
                    tFlag=false;
                    if(acost.size() > 3){
                         MeanACost=0;
                         for(int i=0; i<acost.size(); i++){
                              MeanACost += acost.at(i);
                         }
                         MeanACost = MeanACost / acost.size();
                         std::cout<<std::endl<<"MeanACost: "<<MeanACost<<std::endl;
                         std::cout<<std::endl<<"Size: "<<acost.size()<<std::endl;
                    }
                         //
                         ///
                         //
                         if(ultimate){
                         std::cout<<std::endl<<"Starting Ultimate Training..."<<std::endl<<std::endl;
                              find_min_using_approximate_derivatives(lbfgs_search_strategy(30),objective_delta_stop_strategy(2).be_verbose(), UScoreWrapper, WeightM, 0); //
                                   c=0;
                                   for(int i=0; i<UW1.n_rows; i++){ //weight matrix
                                        for(int j=0; j<UW1.n_cols; j++){
                                             UW1(i,j)=WeightM(0,c);
                                             c++;
                                        }
                                   }
                                   for(int i=0; i<UW2.n_rows; i++){ //weight matrix
                                        for(int j=0; j<UW2.n_cols; j++){
                                             UW2(i,j)=WeightM(0,c);
                                             c++;
                                        }
                                   }
                                   for(int i=0; i<UW3.n_rows; i++){ //weight matrix
                                        for(int j=0; j<UW3.n_cols; j++){
                                             UW3(i,j)=WeightM(0,c);
                                             c++;
                                        }
                                   }
                                   for(int i=0; i<UW4.n_rows; i++){ //weight matrix
                                        for(int j=0; j<UW4.n_cols; j++){
                                             UW4(i,j)=WeightM(0,c);
                                             c++;
                                        }
                                   }
                                   Save();
                                   c=0;
                              find_min_using_approximate_derivatives(lbfgs_search_strategy(30),objective_delta_stop_strategy(1).be_verbose(), UScoreWrapper, BiasM, 0); //
                                   for(int i=0; i<UL2.size(); i++){ //Neuron layer biases
                                        UL2.at(i).bias=BiasM(0,c); c++;
                                   }
                                   for(int i=0; i<UL2.size(); i++){ //Neuron layer biases
                                        UL2.at(i).bias=BiasM(0,c); c++;
                                   }
                                   for(int i=0; i<UL3.size(); i++){ //Neuron layer biases
                                        UL3.at(i).bias=BiasM(0,c); c++;
                                   }
                                   for(int i=0; i<UL4.size(); i++){ //Neuron layer biases
                                        UL4.at(i).bias=BiasM(0,c); c++;
                                   }
                                   for(int i=0; i<UL5.size(); i++){ //Neuron layer biases
                                        UL5.at(i).bias=BiasM(0,c); c++;
                                   }
                                   Save();
                         }
                         else{
                         //
                         ///
                         //
                         std::cout<<std::endl<<"Starting Training..."<<std::endl<<std::endl;
                         acost.push_back( find_min_using_approximate_derivatives(lbfgs_search_strategy(125),objective_delta_stop_strategy(1).be_verbose(), CW, WeightM, 0) );
                         // acost.push_back( find_min_using_approximate_derivatives(lbfgs_search_strategy(100),objective_delta_stop_strategy(1).be_verbose(), PT, WeightM, 100) );
                         c=0;

                         for(int i=0; i<W1.n_rows; i++){ //First weight matrix
                              for(int j=0; j<W1.n_cols; j++){
                                   W1(i,j)=WeightM(0,c);
                                   c++;
                              }
                         }
                         for(int i=0; i<W2.n_rows; i++){ //Second weight matrix
                              for(int j=0; j<W2.n_cols; j++){
                                   W2(i,j)=WeightM(0,c);
                                   c++;
                              }
                         }
                         for(int i=0; i<W3.n_rows; i++){ //Third weight matrix
                              for(int j=0; j<W3.n_cols; j++){
                                   W3(i,j)=WeightM(0,c);
                                   c++;
                              }
                         }
                         for(int i=0; i<W4.n_rows; i++){ //Fourth weight matrix
                              for(int j=0; j<W4.n_cols; j++){
                                   W4(i,j)=WeightM(0,c);
                                   c++;
                              }
                         }
                         for(int i=0; i<W5.n_rows; i++){ //Fifth weight matrix
                              for(int j=0; j<W5.n_cols; j++){
                                   W5(i,j)=WeightM(0,c);
                                   c++;
                              }
                         }
                         for(int i=0; i<W6.n_rows; i++){ //sixth weight matrix
                              for(int j=0; j<W6.n_cols; j++){
                                   W6(i,j)=WeightM(0,c);
                                   c++;
                              }
                         }

                         c=0;
                         Save();
                          acost.push_back( find_min_using_approximate_derivatives(lbfgs_search_strategy(125),objective_delta_stop_strategy(1).be_verbose(), CW, BiasM, 0) );
                         //acost.push_back( find_min_using_approximate_derivatives(lbfgs_search_strategy(100),gradient_norm_stop_strategy(1e-1).be_verbose(), PT, WeightM, -100) );

                         c=0; //Alters biases
                         for(int i=0; i<NIL.size(); i++){ //1st Neuron layer biases
                              NIL.at(i).bias=BiasM(0,c);
                              c++;
                         }
                         for(int i=0; i<NHL.size(); i++){ //2nd Neuron layer biases
                              NHL.at(i).bias=BiasM(0,c);
                              c++;
                         }
                         for(int i=0; i<NHL2.size(); i++){ //3rd Neuron layer biases
                              NHL2.at(i).bias=BiasM(0,c);
                              c++;
                         }
                         for(int i=0; i<NHL3.size(); i++){ //4th Neuron layer biases
                              NHL3.at(i).bias=BiasM(0,c);
                              c++;
                         }
                         for(int i=0; i<NHL4.size(); i++){ //5th Neuron layer biases
                              NHL4.at(i).bias=BiasM(0,c);
                              c++;
                         }
                         for(int i=0; i<NOL.size(); i++){ //6th Neuron layer biases
                              NOL.at(i).bias=BiasM(0,c);
                              c++;
                         }
                         for(int i=0; i<OL.size(); i++){ //7th Neuron layer biases
                              OL.at(i).bias=BiasM(0,c);
                              c++;
                         }
                         c=0;
                         Save();
                         }
               //
          }
               //srand(time(NULL));
               //Testnet.NNAI.AIRematch(); //Clears the AI memory
               //find_min_using_approximate_derivatives(lbfgs_search_strategy(50),objective_delta_stop_strategy(1e-15).be_verbose(), PT, WeightM, 0);

    if(testing){
          PT(WeightM);
    }
     //Like putting it in, but in reverse

     c=0;
     for(int i=0; i<W1.n_rows; i++){ //First weight matrix
          for(int j=0; j<W1.n_cols; j++){
               W1(i,j)=WeightM(0,c);
               c++;
          }
     }
     for(int i=0; i<W2.n_rows; i++){ //Second weight matrix
          for(int j=0; j<W2.n_cols; j++){
               W2(i,j)=WeightM(0,c);
               c++;
          }
     }
     for(int i=0; i<W3.n_rows; i++){ //Third weight matrix
          for(int j=0; j<W3.n_cols; j++){
               W3(i,j)=WeightM(0,c);
               c++;
          }
     }
     for(int i=0; i<W4.n_rows; i++){ //Fourth weight matrix
          for(int j=0; j<W4.n_cols; j++){
               W4(i,j)=WeightM(0,c);
               c++;
          }
     }
     for(int i=0; i<W5.n_rows; i++){ //Fifth weight matrix
          for(int j=0; j<W5.n_cols; j++){
               W5(i,j)=WeightM(0,c);
               c++;
          }
     }
     for(int i=0; i<W6.n_rows; i++){ //sixth weight matrix
          for(int j=0; j<W6.n_cols; j++){
               W6(i,j)=WeightM(0,c);
               c++;
          }
     }
     c=0; //Alters biases
     for(int i=0; i<NIL.size(); i++){ //1st Neuron layer biases
          NIL.at(i).bias=BiasM(0,c);
          c++;
     }
     for(int i=0; i<NHL.size(); i++){ //2nd Neuron layer biases
          NHL.at(i).bias=BiasM(0,c);
          c++;
     }
     for(int i=0; i<NHL2.size(); i++){ //3rd Neuron layer biases
          NHL2.at(i).bias=BiasM(0,c);
          c++;
     }
     for(int i=0; i<NHL3.size(); i++){ //4th Neuron layer biases
          NHL3.at(i).bias=BiasM(0,c);
          c++;
     }
     for(int i=0; i<NHL4.size(); i++){ //5th Neuron layer biases
          NHL4.at(i).bias=BiasM(0,c);
          c++;
     }
     for(int i=0; i<NOL.size(); i++){ //6th Neuron layer biases
          NOL.at(i).bias=BiasM(0,c);
          c++;
     }
     for(int i=0; i<OL.size(); i++){ //7th Neuron layer biases
          OL.at(i).bias=BiasM(0,c);
          c++;
     }
     c=0;
     //Alters the connections
     for(int i=0; i<NIL.size(); i++){ //
          for(int j=0; j<NIL.at(i).Connections.size(); j++){
               NIL.at(i).Connections.at(j).weight=W1(i,j);
          }
     }
     for(int i=0; i<NHL.size(); i++){
          for(int j=0; j<NHL.at(i).Connections.size(); j++){
               NHL.at(i).Connections.at(j).weight=W2(i,j);
          }
     }
     for(int i=0; i<NHL2.size(); i++){
          for(int j=0; j<NHL2.at(i).Connections.size(); j++){
               NHL2.at(i).Connections.at(j).weight=W3(i,j);
          }
     }
     for(int i=0; i<NHL3.size(); i++){
          for(int j=0; j<NHL3.at(i).Connections.size(); j++){
               NHL3.at(i).Connections.at(j).weight=W4(i,j);
          }
     }
     for(int i=0; i<NHL4.size(); i++){
          for(int j=0; j<NHL4.at(i).Connections.size(); j++){
               NHL4.at(i).Connections.at(j).weight=W5(i,j);
          }
     }
     for(int i=0; i<NOL.size(); i++){
          for(int j=0; j<NOL.at(i).Connections.size(); j++){
               NOL.at(i).Connections.at(j).weight=W6(i,j);
          }
     }
     for(int i=0; i<OL.size(); i++){
          for(int j=0; j<OL.at(i).Connections.size(); j++){
               OL.at(i).Connections.at(j).weight=W6(i,j);
          }
     }
     Save();
     /*
     for(int i=0; i<NOL.size(); i++){
        NOL.at(i).Connections.at(0).weight=TW(i,0);
     } */
     //
     //Calstd();
     //
     //std::cout<<"New Big Baller Boi: "<<std::endl;
     //std::cout << std::setprecision(4);
     //OutputM.print();
return;
}
int NN::Place(TTTHuman PBuddy){
     int choice=0,c=0;
     double answer=-1.0;
     mat OCopy;
          BI(PBuddy.HPositions, NNAI.AIPositions,3);
          //Calstd stuff
          Calstd();
          OCopy= coutput();
          c=0;
          for(int i=0; i<OCopy.n_rows; i++){ //In this instance it should be 1 row
               for(int j=0; j<OCopy.n_cols; j++){
                    c++;
                    if(OCopy(i,j) > answer){
                         if(PBuddy.HPositions[c]!=1 && NNAI.AIPositions[c]!=1){
                              answer=OCopy(i,j); choice=c;
                         }
                    }
               }
          }
          NNAI.AIPositions[choice]=1;
          NNAI.AddMove(choice);
return choice;
}
void NN::OPT(bool testing){
     // TODO (Blackweb#1#): Remove this OPT function when done testing
               if(testing!=true){
                    //Calstd();
                    /*
                    std::cout<<std::endl<<"Output: "<<std::endl;
                    OutputM.print();    std::cout<<std::endl;
                    Cost();
                    BackProp();
                    std::cout<<GradCheck()<<std::endl;
                    */
                    Train();
               }else{
                    Train(true);
               }
          return;
}
/////////////////////////
//Important Function that determines if someone has won the game // This function checks if the AI or Human Player has one
/////////////////////////
int EndGame(TTTHuman HP, TTTAI RAI, bool restart);
//
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
    int AIbp1 = RAI.AIPositions[1]; // AIbp1(Board Place 1) this is the naming convention for these
    int AIbp2 = RAI.AIPositions[2];
    int AIbp3 = RAI.AIPositions[3];
    int AIbp4 = RAI.AIPositions[4];
    int AIbp5 = RAI.AIPositions[5];
    int AIbp6 = RAI.AIPositions[6];
    int AIbp7 = RAI.AIPositions[7];
    int AIbp8 = RAI.AIPositions[8];
    int AIbp9 = RAI.AIPositions[9];
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
        if( ( AIcheck==true && Hcheck==true ) ){std::cout<<"ERROR: Checkwin, Double true"<<std::endl;}
            else if((Hcheck == true) ){decision=true; HP.HWon(true); }
                else if((AIcheck == true) ){decision=true; RAI.AIWon(true); }
                    switch( EndGame(HP, RAI, false) ){
                        case -1: decision=false; break;
                        case 0: decision = true; break; //Tie
                        case 1: decision = true; break; // Human won
                        case 2: decision = true; break; // Ai won
                        case 3: decision = false; break; // Rematch
                        case 4: decision = false; break;
                        default: std::cout<<"ERROR: Checkwin, switch"<<std::endl; break;
                        }
return decision;
}
//Function that determines who won the game(Ai or Human) and if the game ended in a tie
int EndGame(TTTHuman HP, TTTAI RAI, bool restart){
     using namespace std;
     int EndGamedec=-1; //The decision of the game
     if(HP.GetMoves() >= 5 || RAI.GetMoves() >= 5){
          if(RAI.AIWon(false)==false && HP.HWon(false)==false){
                EndGamedec=0;
          }
     } //Checks for tie but other if statements below are there to make sure
     if(HP.HWon(false) == true){ std::cout<<"Human victory"<<std::endl; EndGamedec=1;}
     else if(RAI.AIWon(false)==true){ std::cout<<"AI victory"<<std::endl; EndGamedec=2;}
     while(restart==true){
          std::cout<<std::endl<<"Do you wish to rematch the current AI?"<<std::endl<<"Yes: 1"<<std::endl<<"No: 2"<<std::endl;
          int RD; // rematch decision
          cin>>RD; std::cout<<std::endl;
               if(RD == 1){ HP.HumanRematch(); RAI.AIRematch(); std::cout<<"Rematch will commence."<<std::endl; EndGamedec=3; restart=false; break;}else if(RD !=2){std::cout<<"That is not one of the proposed options."<<std::endl; }else{std::cout<<"Bye and have a good time."<<std::endl;EndGamedec=4; restart=false; break;}
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
                default: std::cout<<"ERROR: DisplayB, Switch"<<std::endl; break;
            };
            int AIp = (AIbp[i]);
            switch( (AIp) ) {
                case 0: break;
                case 1: bp[i] = 'O'; break; // Sets it to O for AI
                default: std::cout<<"ERROR: DisplayB, Switch2"<<std::endl; break;
            };
        }
        //If everything goes well it will output the board correctly
        // int testing = bp[9]; //
        std::cout<<std::endl<<bp[1]<<"|"<<bp[2]<<"|"<<bp[3]<<std::endl<<"- - -"<<std::endl<<bp[4]<<"|"<<bp[5]<<"|" <<bp[6]<<std::endl<<"- - -"<<std::endl<<bp[7]<<"|"<<bp[8]<<"|"<<bp[9]<<std::endl<<std::endl;
        std::cout<<"Make your move."<<std::endl<<std::endl;
return;
}

double PT(dlib::matrix<double> CM){ //Play Training
     //
     mat OCopy;
     mat I;
     int choice=0, c=0;
     bool chosenPB=false;
     int r= rand()%9+1;
     if( CM.size() == Testnet.getNIL().size() + Testnet.getNHL().size() + Testnet.getNHL2().size() + Testnet.getNHL3().size() + Testnet.getNHL4().size() + Testnet.getNOL().size() + Testnet.getOL().size()){
          Testnet.Fill( CM, false );
     }else{
          Testnet.Fill( CM, true );
     }
     //
     TTTAI PBuddy; //Play buddy
     TTTHuman FH; //Fake human
     bool FHWON = false, AIWON=false;
     bool playing=true;
     double score=0;
     int cl = -1;
     uvec answers;
     double answer=-1;
     double sanswer=-1;
     int ff=0;
     bool chosen=false;
     while(playing){
            cl++;
            //randomly picks a position
            if ( PBuddy.GetMoves() == 0 ) {
                while(chosenPB!=true){
                    r=rand()%9 +1;
                    if( (PBuddy.AIPositions[1]!=1 && Testnet.NNAI.AIPositions[1]!=1) && r==1 ){
                         chosenPB=true; break;
                    }else if( (PBuddy.AIPositions[2]!=1 && Testnet.NNAI.AIPositions[2]!=1) && r==2 ){
                         chosenPB=true; break;
                    }else if( (PBuddy.AIPositions[3]!=1 && Testnet.NNAI.AIPositions[3]!=1) && r==3){
                         chosenPB=true; break;
                    }else if( (PBuddy.AIPositions[4]!=1 && Testnet.NNAI.AIPositions[4]!=1) && r==4){
                         chosenPB=true; break;
                    }else if( (PBuddy.AIPositions[5]!=1 && Testnet.NNAI.AIPositions[5]!=1) && r==5){
                         chosenPB=true; break;
                    }else if( (PBuddy.AIPositions[6]!=1 && Testnet.NNAI.AIPositions[6]!=1) && r==6){
                         chosenPB=true; break;
                    }else if( (PBuddy.AIPositions[7]!=1 && Testnet.NNAI.AIPositions[7]!=1) && r==7){
                         chosenPB=true; break;
                    }else if( (PBuddy.AIPositions[8]!=1 && Testnet.NNAI.AIPositions[8]!=1) && r==8){
                         chosenPB=true; break;
                    }else if( (PBuddy.AIPositions[9]!=1 && Testnet.NNAI.AIPositions[9]!=1) && r==9){
                         chosenPB=true; break;
                    }
                }
               PBuddy.AddMove(r);
               PBuddy.AIPositions[r]=1;
               chosenPB=false;
            } else {
                PBuddy.Play(Testnet.NNAI.AIPositions, PBuddy.AIPositions);
            }
          //
          for(int i=1; i<10; i++){
               FH.HPositions[i]=PBuddy.AIPositions[i];
          }
            // TODO: this
           // DisplayB(PBuddy.AIPositions,Testnet.NNAI.AIPositions);
           // std::cout<<"Moves: "<<Testnet.NNAI.GetMoves()<<"Score: "<<score<<std::endl;
          if(checkWin(FH,Testnet.NNAI)==true || PBuddy.GetMoves()>=5){ //Checks if someone has won or if a tie has occured
                // Check who won
                if( (FH.HPositions[1]+FH.HPositions[2]+FH.HPositions[3]) == 3 ){ FHWON=true;} // [1,2,3]
                if( (FH.HPositions[4]+FH.HPositions[5]+FH.HPositions[6]) == 3 ){ FHWON=true;} // [4,5,6]
                if( (FH.HPositions[7]+FH.HPositions[8]+FH.HPositions[9]) == 3 ){ FHWON=true;} // [7,8,9]
                if( (FH.HPositions[1]+FH.HPositions[4]+FH.HPositions[7]) == 3 ){ FHWON=true;} // [1,4,7]
                if( (FH.HPositions[2]+FH.HPositions[5]+FH.HPositions[8]) == 3 ){ FHWON=true;} // [2,5,8]
                if( (FH.HPositions[3]+FH.HPositions[6]+FH.HPositions[9]) == 3 ){ FHWON=true;} // [3,6,9]
                if( (FH.HPositions[1]+FH.HPositions[5]+FH.HPositions[9]) == 3 ){ FHWON=true;} // [1,5,9]
                if( (FH.HPositions[3]+FH.HPositions[5]+FH.HPositions[7]) == 3 ){ FHWON=true;} // [3,5,7]
                //
                if( (Testnet.NNAI.AIPositions[1]+Testnet.NNAI.AIPositions[2]+Testnet.NNAI.AIPositions[3]) == 3 ){ AIWON=true;} // [1,2,3]
                if( (Testnet.NNAI.AIPositions[4]+Testnet.NNAI.AIPositions[5]+Testnet.NNAI.AIPositions[6]) == 3 ){ AIWON=true;} // [4,5,6]
                if( (Testnet.NNAI.AIPositions[7]+Testnet.NNAI.AIPositions[8]+Testnet.NNAI.AIPositions[9]) == 3 ){ AIWON=true;} // [7,8,9]
                if( (Testnet.NNAI.AIPositions[1]+Testnet.NNAI.AIPositions[4]+Testnet.NNAI.AIPositions[7]) == 3 ){ AIWON=true;} // [1,4,7]
                if( (Testnet.NNAI.AIPositions[2]+Testnet.NNAI.AIPositions[5]+Testnet.NNAI.AIPositions[8]) == 3 ){ AIWON=true;} // [2,5,8]
                if( (Testnet.NNAI.AIPositions[3]+Testnet.NNAI.AIPositions[6]+Testnet.NNAI.AIPositions[9]) == 3 ){ AIWON=true;} // [3,6,9]
                if( (Testnet.NNAI.AIPositions[1]+Testnet.NNAI.AIPositions[5]+Testnet.NNAI.AIPositions[9]) == 3 ){ AIWON=true;} // [1,5,9]
                if( (Testnet.NNAI.AIPositions[3]+Testnet.NNAI.AIPositions[5]+Testnet.NNAI.AIPositions[7]) == 3 ){ AIWON=true;} // [3,5,7]
                //
               if( FHWON ){
                    score+=3;
               }
               else if( AIWON ){
                    score-=1;
                    //tFlag=true;
               }
               else {
                    // score+=0.25;
               }
               FH.HumanRematch();
               PBuddy.AIRematch();
               Testnet.NNAI.AIRematch();
               continue;
          }
          // Place move
          choice = Testnet.Place( FH );
          std::cout<<"choice: "<<choice<<std::endl;
          DisplayB(PBuddy.AIPositions,Testnet.NNAI.AIPositions); //score-=1;
          answer=-1.0; sanswer=-1;
          choice=0;
          c=0;
          //
          if(checkWin(FH,Testnet.NNAI) || PBuddy.GetMoves()>=5){
          // Check who won
                if( (FH.HPositions[1]+FH.HPositions[2]+FH.HPositions[3]) == 3 ){ FHWON=true;} // [1,2,3]
                if( (FH.HPositions[4]+FH.HPositions[5]+FH.HPositions[6]) == 3 ){ FHWON=true;} // [4,5,6]
                if( (FH.HPositions[7]+FH.HPositions[8]+FH.HPositions[9]) == 3 ){ FHWON=true;} // [7,8,9]
                if( (FH.HPositions[1]+FH.HPositions[4]+FH.HPositions[7]) == 3 ){ FHWON=true;} // [1,4,7]
                if( (FH.HPositions[2]+FH.HPositions[5]+FH.HPositions[8]) == 3 ){ FHWON=true;} // [2,5,8]
                if( (FH.HPositions[3]+FH.HPositions[6]+FH.HPositions[9]) == 3 ){ FHWON=true;} // [3,6,9]
                if( (FH.HPositions[1]+FH.HPositions[5]+FH.HPositions[9]) == 3 ){ FHWON=true;} // [1,5,9]
                if( (FH.HPositions[3]+FH.HPositions[5]+FH.HPositions[7]) == 3 ){ FHWON=true;} // [3,5,7]
                //
                if( (Testnet.NNAI.AIPositions[1]+Testnet.NNAI.AIPositions[2]+Testnet.NNAI.AIPositions[3]) == 3 ){ AIWON=true;} // [1,2,3]
                if( (Testnet.NNAI.AIPositions[4]+Testnet.NNAI.AIPositions[5]+Testnet.NNAI.AIPositions[6]) == 3 ){ AIWON=true;} // [4,5,6]
                if( (Testnet.NNAI.AIPositions[7]+Testnet.NNAI.AIPositions[8]+Testnet.NNAI.AIPositions[9]) == 3 ){ AIWON=true;} // [7,8,9]
                if( (Testnet.NNAI.AIPositions[1]+Testnet.NNAI.AIPositions[4]+Testnet.NNAI.AIPositions[7]) == 3 ){ AIWON=true;} // [1,4,7]
                if( (Testnet.NNAI.AIPositions[2]+Testnet.NNAI.AIPositions[5]+Testnet.NNAI.AIPositions[8]) == 3 ){ AIWON=true;} // [2,5,8]
                if( (Testnet.NNAI.AIPositions[3]+Testnet.NNAI.AIPositions[6]+Testnet.NNAI.AIPositions[9]) == 3 ){ AIWON=true;} // [3,6,9]
                if( (Testnet.NNAI.AIPositions[1]+Testnet.NNAI.AIPositions[5]+Testnet.NNAI.AIPositions[9]) == 3 ){ AIWON=true;} // [1,5,9]
                if( (Testnet.NNAI.AIPositions[3]+Testnet.NNAI.AIPositions[5]+Testnet.NNAI.AIPositions[7]) == 3 ){ AIWON=true;} // [3,5,7]
                //
               if(FH.HWon(false)){
                    score+=3;
               }
               if(Testnet.NNAI.AIWon(false)){
                    score-=1;
                    //tFlag=true;
               }
               // score-=1;
               FH.HumanRematch();
               PBuddy.AIRematch();
               Testnet.NNAI.AIRematch();
               continue;
          }
          if(cl>181 && score<10){//If the score is near zero end playing session
               FH.HumanRematch();
               PBuddy.AIRematch();
               Testnet.NNAI.AIRematch();
               tFlag=true;
               playing=false;
               std::cout<<std::endl<<"Yay the AI has done it!"<<std::endl;
               break;
          }
          if(score>=100){//If the score is too high end playing session
               FH.HumanRematch();
               PBuddy.AIRematch();
               Testnet.NNAI.AIRematch();
               playing=false;
               std::cout<<std::endl<<"Failed!"<<std::endl;
               break;
          }
     }
return score;
}
//////////////////////////////////////////
/////////////////////////////////////////
////////////////////////////////////////


//Starting prompt function
bool StartPrompt(){
    using namespace std;
    std::cout<<"This is the beginning of the program.\nYou will compete against AI in a game of Tic-Tac-Toe.\nBelow is the layout of the board.\n"<<std::endl;
    std::cout<<"1|2|3\n- - -\n4|5|6\n- - -\n7|8|9"<<std::endl<<std::endl;
    std::cout<<"To place your X you shall input the number that corresponds to the position you see on the board.\n"<<std::endl;
return true;
}
//Menu function
int Menu(){
    using namespace std;
    int OChoice, choice; // Options choice// Choice made for where the user wishes to go
    bool inMenu=true; // Bool that determines if user is in menu
    std::cout<<"\nMENU\n"<<std::endl<<"Play: 1"<<std::endl<<"Randomize Weights: 2"<<std::endl<<"NN Testing: 3"<<std::endl<<"Exit: 4"<<std::endl; // Displays options to user
    while(inMenu){
        cin>>choice; std::cout<<std::endl;
            switch(choice){
                case 1: inMenu=false; break; // Begins game
                case 2: std::cout<<"\nRandomized Weights\n"<<std::endl; return 2; break;
                case 3: std::cout<<"\nTesting will begin.\nPreparing data..."<<std::endl; return 3; break;
                case 4: std::cout<<"\nBye. Have a good time."<<std::endl; return 0; break;
                default: std::cout<<"That is not a offered option. Please re-input your choice."<<std::endl; break;
            };
    }
return 1;
}

//Main
int main(){
    using namespace std;
    srand (time(NULL));
    bool playing=true; // bool for if the game is being played by the human
    //Test stuff
    bool testing=false,cw=false, ultimate=false, randWB = false, TDataSet = false;
    bool inMenu = true, started=false;
    //NN Testnet;
    //
    int test=0,moveP=0;
    StartPrompt();
    class TTTAI RAI;    //Running AI is made
    class TTTHuman P1; //P1 == Player1
    while ( inMenu ) {
        switch ( Menu() ) { // Switch for the menu options // 0 is exit game // 1 is play game // 3-5 are board sizes
            case 0: playing = false; inMenu=false; std::cout<<"Exiting...\n"; return 0; break; // breaks while loop before it starts
            case 1: std::cout << "Game will now commence." << std::endl; playing=true; break;
            case 2: playing = false; randWB = true; break;
            case 3: {
                    testing = true;

                    if ( !TDataSet ) {
                        PWCC ( false );
                        TDataSet=true;
                    }
                    //
                    std::cout << "TDATA size: " << TData.size() << std::endl;
                    for ( int i = 0; i < TData.size(); i++ ) {
                        //std::cout<<"Training set "<<i<<":"<<std::endl;
                        //TData.at(i).print();
                    }
                } break; //Testing for now
            case 4: ultimate = true;
                std::cout << "\nMENU\n" << std::endl << "Play Ultimate TTT: 1" << std::endl << "Ultimate NN Testing: 2" << std::endl << "Exit: 3" << std::endl; cin >> test;
                switch ( test ) {
                    case 1: break;
                    case 2: testing = true; break;
                    case 3: return 0;
                    default: break;
                };
                break; //Ultimate tic tac toe
            case 5: break; //
            default: std::cout << "ERROR: Main, switch Menu" << std::endl; break;
        };
        //Ultimate
        if ( ultimate ) {
            SBoard UB; int rDec = 0; int mCount = 0;
            if ( !started ) {
                Testnet.startNN ( true );
                started = true;
            }
            Testnet.Ultimate();
            while ( testing ) {
                std::cout << std::endl << "Train: 1" << std::endl << "Exit: 2" << std::endl << std::endl;
                cin >> test;
                if ( test == 1 ) {
                    //Testnet.Randomize();
                    //Testnet.Save();
                    Testnet.Load();
                    Testnet.OPT ( false );

                } else { return 0;}
            }
            while ( ultimate ) {
                while ( cw != true ) {
                    UB.DisplayU();
                    if ( UB.getWon() != 0 ) {cw = true;} if ( cw == true ) {break;}
                    cin >> moveP; std::cout << std::endl;
                    while ( UB.placeMove ( moveP, true ) != true ) { mCount++; if ( mCount > 10 ) {break;} }
                    UB.DisplayU();
                    cin >> moveP; std::cout << std::endl;
                    while ( UB.placeMove ( moveP, false ) != true ) { mCount++; if ( mCount > 10 ) {break;} }
                    if ( UB.getWon() != 0 ) {cw = true;} if ( cw == true ) {break;}
                    //Testnet.Place(P1);
                    //Testnet.NNAI.Play(P1.HPositions, Testnet.NNAI.AIPositions);
                }
                cw = false;
                std::cout << "Wish to play again?\nYes: 1\nNo: 2" << std::endl;
                cin >> rDec;
                switch ( rDec ) {
                    case 1: UB.clearB(); break;
                    case 2: ultimate = false; return 0; break;
                    default: std::cout << "DEFAULT: rDec Ultimate TTT" << std::endl; break;
                }
            }
        }
        if ( !started ) {
                Testnet.startNN ( false );
                started = true;
        }
        if ( randWB ) {
            Testnet.Randomize();
            //TODO: add a function that allows you to determine size of nn
            Testnet.Save();
            //Testnet.BI(P1.HPositions, Testnet.NNAI.AIPositions,3);
            Testnet.Load();
            randWB=false;
            std::cout<<"\nRandomized."<<std::endl;
            // return 0;
        }
        while ( testing ) {
            //P1.HPositions[1]=1;
            //Testnet.Randomize();
            // Testnet.BI(P1.HPositions, Testnet.NNAI.AIPositions,3);
            // Testnet.OPT(false);

            //Testnet.Randomize();
            //PT();
            std::cout << "Enter Next Action: " << std::endl << "PT: 1" << std::endl << "Train: 2" << std::endl;
            cin >> test;
            if ( test == 1 ) {
                Testnet.OPT ( true );
            } else if ( test == 2 ) {
                do {
                    Testnet.OPT ( false );
                    Testnet.OPT ( true );
                } while ( tFlag != true );
            } else {
                return 0;
            }
        }

        while ( playing ) {
            Testnet.Load();
            while ( cw != true ) {
                DisplayB ( P1.HPositions, Testnet.NNAI.AIPositions );
                cw = checkWin ( P1, Testnet.NNAI );
                if ( cw == true ) {break;}
                P1.HPlay ( P1.HPositions, Testnet.NNAI.AIPositions );
                DisplayB ( P1.HPositions, Testnet.NNAI.AIPositions );
                cw = checkWin ( P1, Testnet.NNAI );
                if ( cw == true ) {break;}
                Testnet.Place ( P1 );
                //Testnet.NNAI.Play(P1.HPositions, Testnet.NNAI.AIPositions);
            }
            cw = false;
            //DisplayB(P1.HPositions, Testnet.NNAI.AIPositions); // Look at end board
            switch ( EndGame ( P1, Testnet.NNAI, true ) ) {
                case 3: P1.HumanRematch(); Testnet.NNAI.AIRematch(); break;
                case 4: playing = false; break;
                default: break;
            }
        }
    }
    std::cout << "Game End" << std::endl; // Exit text
    std::cout << std::endl;
    return 0;
}



