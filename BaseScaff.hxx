#ifndef BASESCAFF_HXX
#define BASESCAFF_HXX

#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

#include "umlaute.h"

namespace Scaff{

//Used Multiplier to calculate
//the planks dependent on the 
//Fieldwidth
enum FieldWidth :
int{W06=2,W09};



//Used as an internal flag to determine
//which field length are calculated and 
//simultaneously used as the indize for 
//corresponding array
enum FieldLength :
int {seventy,one,one_h,two,two_h,three,three_h};

enum FieldIndicator :
int{first=73,second=109,third=157,fourth=207,fifth=257,sixth=307};


//Determines the height of the first floor
//Used as indicator for the Frames array
enum FloorCompensation:
int {mini,small,medium,normal};

struct Dimensions{
    int FieldRep[2][7];                 //Decided for a new represantation of our calculated fields. 
                                        //the 0 row represents the fields length and second row indicates how many of each
                                        //this is useful when changing field afterwards  
    FieldLength FL[6];                  //After the calculation is done we set our enum Fieldlength to the corresponding
                                        // fields after later we can refer to them in the two dimensional array to look up
                                        //which fields are chosen
    FloorCompensation FC;
    FieldWidth FW;
    FieldIndicator FI[6] = {first,second,third,fourth,fifth,sixth};
    double  CalcedLength,
            WishedLength,
            Height,
            Weight,
            Squaremetre,
            MaxStalkLoad,
            RestLength,
            RestHeight;
    int     Floors,
            AllFieldsL,
            LongFieldsL,
            ShortFieldsL;
            //PlankChoice;    	    //Determines if to take plates or planks
};

struct UserInput {
    std::string ConstructionSite;
    int PlankChoice,FrameType;
    double Height, Length;
    UserInput()=default;
};


//Here are stored all common length
//of the three types of planks that
//are used in all types of Scaffolds
template<typename T>
struct Planks {
    T alu[6];
    T steel[6];
    T wodden[6];
    Planks()=default;
};

//All unique comps in one struct
//from which all the other classes
//will inherit from
template<class T>
struct BaseComponents{
    Planks<T> UsedPlanks;
    T SideGuard[6];
    T ToeBoard[6];
    T Dia[2];
    T Frames[4];
    T EndGuard[2];
    T Ladder[2];
    T BaseJack;
    BaseComponents()=default;
};



//This is our Base/main class which holds
//all functions and components that are
//equal in every Scaffold
//This class only supports calculating
//some keydata for the Scaffold
//The derived classes have to implement
//upon that for their special reqiurements
//for Material calculation...
class BaseScaff{
private:
    BaseComponents<int> Material=  { {  {0,0,0,0,0},      //alu-planks
                                        {0,0,0,0,0},      //steel-planks
                                        {0,0,0,0,0}},     // 
                                        {0,0,0,0,0},      //guards
                                        {0,0,0,0,0},      //toe board
                                        {0,0},        //vertical-braces
                                        {0,0,0,0},        //normal-frames
                                        {0,0},            //end-guard
                                        0};               //base jack;
   /* BaseComponents<double> MatWeight=  { {  {4.0,5.8,7.6,9.5,11.3},      //alu-planks
                                            {8.0,11.2,15.0,18.0,21.0},   //steel-planks
                                            {7.7,10.8,13.9,17.0,20.1}},  //wodden-planks
                                            {2.2,3.0,3.8,4.6,6.6},       //guards
                                            {2.5,3.8,5.0,6.3,7.3},       //toe board
                                            {7.6,7.8,8.8,9.8},           //vertical-braces
                                            {8.4,12.0,15.0,19.0},        //normal-frames
                                            {3.0,4.0},                   //end-guard
                                             3.2};                       //base jack;*/
    Dimensions CalcedData;
    int PlankChoice=0,LoadClass;

public:
    BaseScaff()=default;
    //This one is used for a quick field calculation
	BaseScaff(double WishedLength, double WishedHeight, FieldIndicator BaseField = fifth);
    //This one is used if we get full input from the user
	BaseScaff(const UserInput &Input);
    ~BaseScaff()=default;

    
    inline void SetCalcedLength(double input){ this->CalcedData.CalcedLength = input;}
    /*  Not needed yet
    inline void SetLongFieldsL(int LongFields){this->CalcedData.LongFieldsL=LongFields;}
    inline void SetShortFieldsL(int ShortFields){this->CalcedData.ShortFieldsL=ShortFields;}
    inline void SetFloors(int CalcedFloors){ this->CalcedData.Floors    = CalcedFloors;}

    inline unsigned int GetLongFieldsL(){return this->CalcedData.LongFieldsL;}
    inline unsigned int GetShortFieldsL(){return this->CalcedData.ShortFieldsL;}
    inline unsigned int GetFloors(){return this->CalcedData.Floors;}
    */

    void SetAllFieldsL(int FieldsToAdd){this->CalcedData.AllFieldsL=FieldsToAdd;}
    void SetDimensions(const Dimensions& DataToSet);
    void SetComponents(const BaseComponents<int> ComponentsToSet);
    void SetExtraField(FieldLength FieldToAdd);

    inline BaseComponents<int> GetComponents(void){return this->Material;}
    inline Dimensions GetDimensions(void){return this->CalcedData;}
    inline unsigned int GetAllFieldsL(){return this->CalcedData.AllFieldsL;}

    void SwapFields(FieldLength FieldToSub,FieldLength FieldToAdd, int Floors);
    void SubField(FieldLength FieldToSub, int Floors);
    void AddField(FieldLength FieldToAdd, int Floors);


   

private:

    void CalcFloors(double WishedHeight);
    void CalcFieldsL(double MaxLength, FieldIndicator BaseField = fifth);
    void FieldBase300L(double MaxLength);
    void FieldBase250L(double MaxLength);
    void CalcMaterial();
    void CalcTechnicalData();
};

}
#endif // SCAFF_H
