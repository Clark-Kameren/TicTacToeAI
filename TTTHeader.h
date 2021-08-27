#ifndef TTTHEADER_H_INCLUDED
#define TTTHEADER_H_INCLUDED
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <strstream>
#include <math.h>
#include <time.h>
#include <sstream>
#include <stdlib.h>
#include <array>
#include <algorithm>
#include <vector>
#include <armadillo>
#include <dlib/global_optimization.h>
#include <dlib/svm.h>

#endif // TTTHEADER_H_INCLUDED
using namespace arma;

//
//Template to create a class object
template <class T>
T createObject(){
     T Object;
     return Object;
}
// X=Positive O=Negative
class iBoard{
     public:
     iBoard(){
          Board.set_size(3,3);
          Board.fill(fill::zeros);
          won=false; controller=0;
     }
     bool Place(int p, bool X){
          bool success=true;
          int pCc= (p+2 % 3); //Converted to matrix format col and below converted row
          int pCr= (p-1 / 3);
          if(p>9 || p<1){
               success=false;
          }else if( Board(pCr,pCc) != 0 ){
               success=false;
          }else{
               if(X){
                    Board(pCr,pCc) = 1;
               }else{
                    Board(pCr,pCc) = -1;
               }
          }
     return success;
     }
     void clearIB(){
          Board.fill(fill::zeros);
          won=false; controller=0;
     }
     Mat<int> Board;
     bool won;//if the board has been won
     int controller; //States who controls the board / X = 1 / O = -1 / Neither = 0
};
///Super tic tac toe board
class SBoard{
     public:
          SBoard(){
               for(int i=0; i<9; i++){
                    TBoard.push_back( createObject<iBoard>() );
               }
               lastBoardP=0;
               for(int i=0; i<9; i++){
                    pBoards.push_back(0);
               }
               won=0; firstmove=true;
          }
          void clearB(){
               std::for_each(TBoard.begin(),TBoard.end(), [this](iBoard& ib){ ib.clearIB(); } );
               lastBoardP=0;
               pBoards.clear();
               for(int i=0; i<9; i++){
                    pBoards.push_back(0);
               }
               won=0; firstmove=true;
          }
          bool placeMove(int mP, bool X); // When updating for new moves // needs the current board out of total boards
          bool bCheck(iBoard& CB);
          void DisplayU(); //Display function
          int getWon(){bCheck(TBoard.at(lastBoardP)); return won;}
          std::vector<iBoard> getTBoard(){ return TBoard;}
          std::vector<int> getpBoards(){ return pBoards;}
     private:
          std::vector<iBoard> TBoard; // The total ultimate board that includes all 9 individual tic tac toe boards
          std::vector<int> pBoards;  //Playable inner boards // 0 means playable // -1 not playable and won by O // 1 not payable and won by X // 2 means not playable due to last move
          int lastBoardP;
          bool firstmove;
          int won;
};
bool SBoard::placeMove(int mP, bool X){
     if(won!=0){
          return false;
     }
     int success=true;
     int P =1; if(X!=true) P=-1;
     int mPCc; //Converted to matrix format col and below converted rowmP
     int mPCr;
     if(mP>81 || mP<1){ //If the input is out of bounds or already taken
          success=false; return success;
     }
     //
     if( mP>=1 && mP<=9 ){
          mPCr= ( ( mP - ( 0 * 9 ) ) - 1 ) / 3;
          mPCc= ( ( mP - ( 0 * 9 ) ) + 2 ) % 3;
     }
     if( mP>=10 && mP<=18 ){
          mPCr= ( ( mP - ( 1 * 9 ) ) - 1 ) / 3;
          mPCc= ( ( mP - ( 1 * 9 ) ) + 2 ) % 3;
     }
     if( mP>=19 && mP<=27 ){
          mPCr= ( ( mP - ( 2 * 9 ) ) - 1 ) / 3;
          mPCc= ( ( mP - ( 2 * 9 ) ) + 2 ) % 3;
     }
     if( mP>=28 && mP<=36 ){
          mPCr= ( ( mP - ( 3 * 9 ) ) - 1 ) / 3;
          mPCc= ( ( mP - ( 3 * 9 ) ) + 2 ) % 3;
     }
     if( mP>=37 && mP<=45 ){
          mPCr= ( ( mP - ( 4 * 9 ) ) - 1 ) / 3;
          mPCc= ( ( mP - ( 4 * 9 ) ) + 2 ) % 3;
     }
     if( mP>=46 && mP<=54 ){
          mPCr= ( ( mP - ( 5 * 9 ) ) - 1 ) / 3;
          mPCc= ( ( mP - ( 5 * 9 ) ) + 2 ) % 3;
     }
     if( mP>=55 && mP<=63 ){
          mPCr= ( ( mP - ( 6 * 9 ) ) - 1 ) / 3;
          mPCc= ( ( mP - ( 6 * 9 ) ) + 2 ) % 3;
     }
     if( mP>=64 && mP<=72 ){
          mPCr= ( ( mP - ( 7 * 9 ) ) - 1 ) / 3;
          mPCc= ( ( mP - ( 7 * 9 ) ) + 2 ) % 3;
     }
     if( mP>=73 && mP<=81 ){
          mPCr= ( ( mP - ( 8 * 9 ) ) - 1 ) / 3;
          mPCc= ( ( mP - ( 8 * 9 ) ) + 2 ) % 3;
     }
     //
     for(int i=0; i<9; i++){
          if(pBoards.at(i)!=0 ||  (mP>=(1+(i*9)) && mP<=(9+(i*9)) )==false ){//checks if the move is in playable inner board
                    //std::cout<<"\nNot in current board"<<std::endl;
                    success=false; //an attempted play on a won inner board
          }else{
               if( firstmove!=true){ //Not first move
                    if( TBoard.at(i).Board(mPCr,mPCc) != 0 ){
                         std::cout<<"\nSpot taken"<<std::endl;
                         success=false; break;
                    }else{
                         TBoard.at(i).Board( mPCr, mPCc )=P;
                         lastBoardP=i;
                         bCheck(TBoard.at( i ));
                         if(pBoards.at( mP - (i*9) -1 ) ==-1 || pBoards.at( mP - (i*9) -1 ) ==1){
                              std::for_each(pBoards.begin(),pBoards.end(), [this](int& val){ if(val==-1 || val==1){}else{val=0;} } );
                         }else{
                              std::for_each(pBoards.begin(),pBoards.end(), [this](int& val){ if(val==-1 || val==1){}else{val=2;} } );
                              pBoards.at( mP - (i*9) -1 )=0;
                         }
                         success=true; break;
                    }
               }else{ //if first move
                    TBoard.at(i).Board( mPCr, mPCc )=P;
                    lastBoardP=i;
                    bCheck(TBoard.at( i )); firstmove=false;
                    if(pBoards.at( mP - (i*9) -1 ) ==-1 || pBoards.at( mP - (i*9) -1 ) ==1){
                         std::for_each(pBoards.begin(),pBoards.end(), [this](int& val){ if(val==-1 || val==1){}else{val=0;} } );
                    }else{
                         std::for_each(pBoards.begin(),pBoards.end(), [this](int& val){ if(val==-1 || val==1){}else{val=2;} } );
                         pBoards.at( mP - (i*9) -1 )=0;
                    }
                    success=true; break;
               }

          }
     }
     std::string oB;
     /*
     std::std::cout<<"\n Current Playable boards\n"<<std::endl;
     for(int i=0; i<9; i++){
          switch(pBoards.at(i)){
               case -1: oB="O"; break;
               case 1: oB="X"; break;
               case 2: oB="-"; break;
               case 0: oB=std::to_string(i); break;
          };
          std::std::cout<<oB;
          if(i==2 || i==5 || i==8){
               std::std::cout<<std::endl;
          }else{
               std::std::cout<<"|";
          }
     }
     std::cout<<std::endl;
     */
return success;
}
///
bool SBoard::bCheck(iBoard& CB){ // boards win check
     if(won!=0){
          return true;
     }
     //
     int bp[10]; //Board Place
     //
     bp[1] = CB.Board(0,0); bp[2] = CB.Board(0,1); bp[3] = CB.Board(0,2);
     bp[4] = CB.Board(1,1); bp[5] = CB.Board(1,1); bp[6] = CB.Board(1,2);
     bp[7] = CB.Board(2,1); bp[8] = CB.Board(2,1); bp[9] = CB.Board(2,2);
     //If statements checking for winning orientations of the Human side
     if( (bp[1]+bp[2]+bp[3]) == 3 ){ CB.won=true; CB.controller=1;  } // [1,2,3]
     if( (bp[4]+bp[5]+bp[6]) == 3 ){ CB.won=true; CB.controller=1;  } // [4,5,6]
     if( (bp[7]+bp[8]+bp[9]) == 3 ){ CB.won=true; CB.controller=1;  } // [7,8,9]
     if( (bp[1]+bp[4]+bp[7]) == 3 ){ CB.won=true; CB.controller=1;  } // [1,4,7]
     if( (bp[2]+bp[5]+bp[8]) == 3 ){ CB.won=true; CB.controller=1;  } // [2,5,8]
     if( (bp[3]+bp[6]+bp[9]) == 3 ){ CB.won=true; CB.controller=1;  } // [3,6,9]
     if( (bp[1]+bp[5]+bp[9]) == 3 ){ CB.won=true; CB.controller=1;  } // [1,5,9]
     if( (bp[3]+bp[5]+bp[7]) == 3 ){ CB.won=true; CB.controller=1;  } // [3,5,7]
     //
     if( (bp[1]+bp[2]+bp[3]) == -3 ){ CB.won=true; CB.controller=-1;  } // [1,2,3]
     if( (bp[4]+bp[5]+bp[6]) == -3 ){ CB.won=true; CB.controller=-1;  } // [4,5,6]
     if( (bp[7]+bp[8]+bp[9]) == -3 ){ CB.won=true; CB.controller=-1;  } // [7,8,9]
     if( (bp[1]+bp[4]+bp[7]) == -3 ){ CB.won=true; CB.controller=-1;  } // [1,4,7]
     if( (bp[2]+bp[5]+bp[8]) == -3 ){ CB.won=true; CB.controller=-1;  } // [2,5,8]
     if( (bp[3]+bp[6]+bp[9]) == -3 ){ CB.won=true; CB.controller=-1;  } // [3,6,9]
     if( (bp[1]+bp[5]+bp[9]) == -3 ){ CB.won=true; CB.controller=-1;  } // [1,5,9]
     if( (bp[3]+bp[5]+bp[7]) == -3 ){ CB.won=true; CB.controller=-1;  } // [3,5,7]
     //
     for(int i=0; i<9; i++){
          pBoards.at(i)=TBoard[i].controller;
     }
     //
     //If statements for the total board
     //
     int Tbp[10];
     Tbp[1] = TBoard[0].controller; Tbp[2] = TBoard[1].controller; Tbp[3] = TBoard[2].controller;
     Tbp[4] = TBoard[3].controller; Tbp[5] = TBoard[4].controller; Tbp[6] = TBoard[5].controller;
     Tbp[7] = TBoard[6].controller; Tbp[8] = TBoard[7].controller; Tbp[9] = TBoard[8].controller;
     //
     if( (Tbp[1]+Tbp[2]+Tbp[3]) == 3 ){  won=1; return true; } // [1,2,3]
     if( (Tbp[4]+Tbp[5]+Tbp[6]) == 3 ){  won=1; return true; } // [4,5,6]
     if( (Tbp[7]+Tbp[8]+Tbp[9]) == 3 ){  won=1; return true; } // [7,8,9]
     if( (Tbp[1]+Tbp[4]+Tbp[7]) == 3 ){  won=1; return true; } // [1,4,7]
     if( (Tbp[2]+Tbp[5]+Tbp[8]) == 3 ){  won=1; return true; } // [2,5,8]
     if( (Tbp[3]+Tbp[6]+Tbp[9]) == 3 ){  won=1; return true; } // [3,6,9]
     if( (Tbp[1]+Tbp[5]+Tbp[9]) == 3 ){  won=1; return true; } // [1,5,9]
     if( (Tbp[3]+Tbp[5]+Tbp[7]) == 3 ){  won=1; return true; } // [3,5,7]
     //
     if( (Tbp[1]+Tbp[2]+Tbp[3]) == -3 ){ won=-1; return true; } // [1,2,3]
     if( (Tbp[4]+Tbp[5]+Tbp[6]) == -3 ){ won=-1; return true; } // [4,5,6]
     if( (Tbp[7]+Tbp[8]+Tbp[9]) == -3 ){ won=-1; return true; } // [7,8,9]
     if( (Tbp[1]+Tbp[4]+Tbp[7]) == -3 ){ won=-1; return true; } // [1,4,7]
     if( (Tbp[2]+Tbp[5]+Tbp[8]) == -3 ){ won=-1; return true; } // [2,5,8]
     if( (Tbp[3]+Tbp[6]+Tbp[9]) == -3 ){ won=-1; return true; } // [3,6,9]
     if( (Tbp[1]+Tbp[5]+Tbp[9]) == -3 ){ won=-1; return true; } // [1,5,9]
     if( (Tbp[3]+Tbp[5]+Tbp[7]) == -3 ){ won=-1; return true; } // [3,5,7]
     //
return false;
}
void SBoard::DisplayU(){
     using namespace std;
     string p1,p2,p3;
     for(int i=0; i<3; i++){ //For each layer of boards
               //
               if(pBoards.at(i*3) ==0 || pBoards.at( (i*3) ) ==2){
                    if( TBoard[ i*3 ].Board(0,0) == 1){ p1=" X"; }else if( TBoard[ (i*3) ].Board(0,0) == -1 ){ p1=" O"; }else{ p1=to_string( (i*27) + 1 ); if(i==0){p1="0"+p1;} }
                    if( TBoard[ i*3 ].Board(0,1) == 1){ p2=" X"; }else if( TBoard[ (i*3) ].Board(0,1) == -1 ){ p2=" O"; }else{ p2=to_string( (i*27) + 2 ); if(i==0){p2="0"+p2;} }
                    if( TBoard[ i*3 ].Board(0,2) == 1){ p3=" X"; }else if( TBoard[ (i*3) ].Board(0,2) == -1 ){ p3=" O"; }else{ p3=to_string( (i*27) + 3 ); if(i==0){p3="0"+p3;} }
                    if(i>0){std::cout<<"\n________________________________________"<<std::endl;}
                    std::cout<<p1<<"|"<<p2<<"|"<<p3;
                    std::cout<<" || ";
               }else{
                    if(i>0){std::cout<<"\n________________________________________"<<std::endl;}
                    std::cout<<" -"<<"|"<<" -"<<"|"<<" -";
                    std::cout<<" || ";
               }
                    //
               if(pBoards.at( (i*3) +1 ) ==0 || pBoards.at( (i*3) +1 ) ==2){
                    if( TBoard[ (i*3) + 1 ].Board(0,0) == 1){ p1=" X"; }else if( TBoard[ (i*3) + 1 ].Board(0,0) == -1 ){ p1=" O"; }else{ p1=to_string( (i*27) + 10 ); }
                    if( TBoard[ (i*3) + 1 ].Board(0,1) == 1){ p2=" X"; }else if( TBoard[ (i*3) + 1 ].Board(0,1) == -1 ){ p2=" O"; }else{ p2=to_string( (i*27) + 11 ); }
                    if( TBoard[ (i*3) + 1 ].Board(0,2) == 1){ p3=" X"; }else if( TBoard[ (i*3) + 1 ].Board(0,2) == -1 ){ p3=" O"; }else{ p3=to_string( (i*27) + 12 ); }
                    std::cout<<p1<<"|"<<p2<<"|"<<p3;
                    std::cout<<" || ";
               }else{
                    std::cout<<" -"<<"|"<<" -"<<"|"<<" -";
                    std::cout<<" || ";
               }
                    //
               if(pBoards.at( (i*3) +2 ) ==0 || pBoards.at( (i*3) +2 ) ==2){
                    if( TBoard[ (i*3) + 2 ].Board(0,0) == 1){ p1=" X"; }else if( TBoard[ (i*3) + 2 ].Board(0,0) == -1 ){ p1=" O"; }else{ p1=to_string( (i*27) + 19 ); }
                    if( TBoard[ (i*3) + 2 ].Board(0,1) == 1){ p2=" X"; }else if( TBoard[ (i*3) + 2 ].Board(0,1) == -1 ){ p2=" O"; }else{ p2=to_string( (i*27) + 20 ); }
                    if( TBoard[ (i*3) + 2 ].Board(0,2) == 1){ p3=" X"; }else if( TBoard[ (i*3) + 2 ].Board(0,2) == -1 ){ p3=" O"; }else{ p3=to_string( (i*27) + 21 ); }
                    std::cout<<p1<<"|"<<p2<<"|"<<p3;
                    std::cout<<"\n________________________________________"<<std::endl;
               }else{
                    std::cout<<" -"<<"|"<<" -"<<"|"<<" -";
                    std::cout<<"\n________________________________________"<<std::endl;
               }
               //
               if(pBoards.at( (i*3) ) ==0 || pBoards.at( (i*3) ) ==2){
                    if( TBoard[ (i*3) ].Board(1,0) == 1){ p1=" X"; }else if( TBoard[ (i*3) ].Board(1,0) == -1 ){ p1=" O"; }else{ p1=to_string( (i*27) + 4 ); if(i==0){p1="0"+p1;} }
                    if( TBoard[ (i*3) ].Board(1,1) == 1){ p2=" X"; }else if( TBoard[ (i*3) ].Board(1,1) == -1 ){ p2=" O"; }else{ p2=to_string( (i*27) + 5 ); if(i==0){p2="0"+p2;} }
                    if( TBoard[ (i*3) ].Board(1,2) == 1){ p3=" X"; }else if( TBoard[ (i*3) ].Board(1,2) == -1 ){ p3=" O"; }else{ p3=to_string( (i*27) + 6 ); if(i==0){p3="0"+p3;} }

                    std::cout<<p1<<"|"<<p2<<"|"<<p3;
                    std::cout<<" || ";
               }else{

                    std::cout<<" -"<<"|"<<" -"<<"|"<<" -";
                    std::cout<<" || ";
               }
               //
               if(pBoards.at( (i*3) +1 ) ==0 || pBoards.at( (i*3) +1 ) ==2){
                    if( TBoard[ (i*3) + 1 ].Board(1,0) == 1){ p1=" X"; }else if( TBoard[ (i*3) + 1 ].Board(1,0) == -1 ){ p1=" O"; }else{ p1=to_string( (i*27) + 13 ); }
                    if( TBoard[ (i*3) + 1 ].Board(1,1) == 1){ p2=" X"; }else if( TBoard[ (i*3) + 1 ].Board(1,1) == -1 ){ p2=" O"; }else{ p2=to_string( (i*27) + 14 ); }
                    if( TBoard[ (i*3) + 1 ].Board(1,2) == 1){ p3=" X"; }else if( TBoard[ (i*3) + 1 ].Board(1,2) == -1 ){ p3=" O"; }else{ p3=to_string( (i*27) + 15 ); }
                    std::cout<<p1<<"|"<<p2<<"|"<<p3;
                    std::cout<<" || ";
               }else{
                    std::cout<<" -"<<"|"<<" -"<<"|"<<" -";
                    std::cout<<" || ";
               }
               //
               if(pBoards.at( (i*3) +2 ) ==0 || pBoards.at( (i*3) +2 ) ==2){
                    if( TBoard[ (i*3) + 2 ].Board(1,0) == 1){ p1=" X"; }else if( TBoard[ (i*3) + 2 ].Board(1,0) == -1 ){ p1=" O"; }else{ p1=to_string( (i*27) + 22 ); }
                    if( TBoard[ (i*3) + 2 ].Board(1,1) == 1){ p2=" X"; }else if( TBoard[ (i*3) + 2 ].Board(1,1) == -1 ){ p2=" O"; }else{ p2=to_string( (i*27) + 23 ); }
                    if( TBoard[ (i*3) + 2 ].Board(1,2) == 1){ p3=" X"; }else if( TBoard[ (i*3) + 2 ].Board(1,2) == -1 ){ p3=" O"; }else{ p3=to_string( (i*27) + 24 ); }
                    std::cout<<p1<<"|"<<p2<<"|"<<p3;
                    std::cout<<"\n________________________________________"<<std::endl;
               }else{
                    std::cout<<" -"<<"|"<<" -"<<"|"<<" -";
                    std::cout<<"\n________________________________________"<<std::endl;
               }
               //
               if(pBoards.at( (i*3) ) ==0 || pBoards.at( (i*3) ) ==2){
                    if( TBoard[ (i*3) ].Board(2,0) == 1){ p1=" X"; }else if( TBoard[ (i*3) ].Board(2,0) == -1 ){ p1=" O"; }else{ p1=to_string( (i*27) + 7 ); if(i==0){p1="0"+p1;} }
                    if( TBoard[ (i*3) ].Board(2,1) == 1){ p2=" X"; }else if( TBoard[ (i*3) ].Board(2,1) == -1 ){ p2=" O"; }else{ p2=to_string( (i*27) + 8 ); if(i==0){p2="0"+p2;} }
                    if( TBoard[ (i*3) ].Board(2,2) == 1){ p3=" X"; }else if( TBoard[ (i*3) ].Board(2,2) == -1 ){ p3=" O"; }else{ p3=to_string( (i*27) + 9 ); if(i==0){p3="0"+p3;} }

                    std::cout<<p1<<"|"<<p2<<"|"<<p3;
                    std::cout<<" || ";
               }else{

                    std::cout<<" -"<<"|"<<" -"<<"|"<<" -";
                    std::cout<<" || ";
               }
               //
               if(pBoards.at( (i*3) +1 ) ==0 || pBoards.at( (i*3) +1 ) ==2){
                    if( TBoard[ (i*3) + 1 ].Board(2,0) == 1){ p1=" X"; }else if( TBoard[ (i*3) + 1 ].Board(2,0) == -1 ){ p1=" O"; }else{ p1=to_string( (i*27) + 16 ); }
                    if( TBoard[ (i*3) + 1 ].Board(2,1) == 1){ p2=" X"; }else if( TBoard[ (i*3) + 1 ].Board(2,1) == -1 ){ p2=" O"; }else{ p2=to_string( (i*27) + 17 ); }
                    if( TBoard[ (i*3) + 1 ].Board(2,2) == 1){ p3=" X"; }else if( TBoard[ (i*3) + 1 ].Board(2,2) == -1 ){ p3=" O"; }else{ p3=to_string( (i*27) + 18 ); }
                    std::cout<<p1<<"|"<<p2<<"|"<<p3;
                    std::cout<<" || ";
               }else{
                    std::cout<<" -"<<"|"<<" -"<<"|"<<" -";
                    std::cout<<" || ";
               }
               //
               if(pBoards.at( (i*3) +2 ) ==0 || pBoards.at( (i*3) +2 ) ==2){
                    if( TBoard[ (i*3) + 2 ].Board(2,0) == 1){ p1=" X"; }else if( TBoard[ (i*3) + 2 ].Board(2,0) == -1 ){ p1=" O"; }else{ p1=to_string( (i*27) + 25 ); }
                    if( TBoard[ (i*3) + 2 ].Board(2,1) == 1){ p2=" X"; }else if( TBoard[ (i*3) + 2 ].Board(2,1) == -1 ){ p2=" O"; }else{ p2=to_string( (i*27) + 26 ); }
                    if( TBoard[ (i*3) + 2 ].Board(2,2) == 1){ p3=" X"; }else if( TBoard[ (i*3) + 2 ].Board(2,2) == -1 ){ p3=" O"; }else{ p3=to_string( (i*27) + 27 ); }
                    std::cout<<p1<<"|"<<p2<<"|"<<p3;
                    std::cout<<"\n________________________________________"<<std::endl;
               }else{
                    std::cout<<" -"<<"|"<<" -"<<"|"<<" -";
                    std::cout<<"\n________________________________________"<<std::endl;
               }

     }
     std::cout<<std::endl;
}
