#ifndef FacadeScaff_HXX
#define FacadeScaff_HXX

#include <iostream>
#include <fstream>
//#include <bits/stdc++.h>

#include "umlaute.h"
  
namespace Scaff{

//Used Multiplier to calculate
//the planks dependent on the 
//Fieldwidth 
enum FieldWidth :
int{W06=2,W09};

//Used as an internal flag to determine
//which field length_ are calculated and 
//simultaneously used as the indize for 
//corresponding array FieldRep. zero 
//indicates that this particular field
//is not used/set
enum FieldLength :
int {seventy,one,one_h,two,two_h,three,zero};

enum FieldIndicator :
int{first=73,second=109,third=157,fourth=207,fifth=257,sixth=307};


//Determines the height_ of the first floor
//Used as indicator for the frames array
enum FloorCompensation:
int {mini,small,medium,normal,none};


struct Dimensions{
    int FieldRep[2][7];                 //Decided for a new represantation of our calculated fields. 
                                        //the 0 row represents the fields length_ and second row indicates how many of each
                                        //this is useful when changing field afterwards  
    FieldLength FL[6];                  //After the calculation is done we set our enum FieldLength to the corresponding
                                        // fields and so later we can refer to them in the two dimensional array to look up
                                        //which fields are chosen
    FloorCompensation FC;
    FieldWidth FW;
    FieldIndicator FI[6] = {first,second,third,fourth,fifth,sixth};
    double  Calcedlength_,
            Wishedlength_,
            height_,
            Weight,
            Squaremetre,
            MaxStalkLoad,
            Restlength_,
            Restheight_;
    int     Floors,
            AllFieldsL,
            LongFieldsL,
            ShortFieldsL;
    Dimensions()=default;
};

struct UserInput {
    std::string construction_site_;
    int plank_choice_,frame_type_;
    double height_, length_;
    UserInput()=default;
};


template<typename T>
struct Planks {
    T alu[6];
    T steel[6];
    T plates[6];
    Planks()=default;
};

//Holds all common components that are similar
//in a facade-scaffold and a modul-scaffold
template<class T>
struct BaseComponents{
    Planks<T> UsedPlanks;
    T ToeBoard[6];
    T Ladder[4];                //TODO need to hardcode the weight in the initialasation
    T Console[4];               //TODO add in the initialazation
    T BaseJack;
    BaseComponents()=default;
};


//We take all the base components and  
//add our facade specific ones on top
template<class T>
struct FacadeComponents : BaseComponents<T>{
    T side_guard[6];
    T dia[6];
    T frames[4];
    T end_guard[2];
    T single_pole;
    T small_pole;
    T big_pole;
    T anchor;  
    FacadeComponents()=default; 
};



//This is our Base/main class which holds
//all functions and components that are
//equal in every Scaffold
//This class only supports calculating
//some keydata for the Scaffold
//The derived classes have to implement
//upon that for their special reqiurements
//for num_of_comps_ calculation...
class FacadeScaff{
private:
    FacadeComponents<double> num_of_comps_;               //gets filled after calculation 
    FacadeComponents<double> weight_of_comps_;            //hardcoded weight gets multiplied with num_of_comps_
    
    /*
      =  { {    {4.0,5.8,7.6,9.5,11.3},      //alu-planks
                {8.0,11.2,15.0,18.0,21.0},   //steel-planks
                {7.7,10.8,13.9,17.0,20.1}},  //plates-planks
                {2.2,3.0,3.8,4.6,6.6},       //guards
                {2.5,3.8,5.0,6.3,7.3},       //toe board
                {7.6,7.8,8.8,9.8},           //vertical-braces
                {8.4,12.0,15.0,19.0},        //normal-frames
                {3.0,4.0},                   //end-guard
                3.2};                       //base jack;
    */

    Dimensions calced_dims_;                            //gets filled after calc with key data for the scaffold
    int plank_choice_,load_class_;                      //both of them gets used like a flag to determine what to calc

public:
    FacadeScaff()=default;
    //This one is used for a quick field calculation
    //e.g direct as start argument
	FacadeScaff(double wished_length_, double wished_height_);
    //This one is used if we get full input from the user
	FacadeScaff(const UserInput& Input);
    ~FacadeScaff()=default;

    
    
    /*  Not needed yet
    inline void SetLongFieldsL(int LongFields){this->calced_dims_.LongFieldsL=LongFields;}
    inline void SetShortFieldsL(int ShortFields){this->calced_dims_.ShortFieldsL=ShortFields;}
    inline void SetFloors(int CalcedFloors){ this->calced_dims_.Floors    = CalcedFloors;}

    inline unsigned int GetAllFieldsL(){return this->calced_dims_.AllFieldsL;}
    inline unsigned int GetLongFieldsL(){return this->calced_dims_.LongFieldsL;}
    inline unsigned int GetShortFieldsL(){return this->calced_dims_.ShortFieldsL;}
    inline unsigned int GetFloors(){return this->calced_dims_.Floors;}
    */

    void SetDimensions(const Dimensions& DataToSet);
    void SetUpdatetComponents(FieldLength FieldToSub,FieldLength FieldToAdd, int Floors);
    void SetExtraField(FieldLength FieldToAdd);
    inline void SetCalcedlength_(double input){ this->calced_dims_.Calcedlength_ = input;}

    inline BaseComponents<double> GetComponents(void){return this->num_of_comps_;}
    inline Dimensions GetDimensions(void){return this->calced_dims_;}


   

private:

    void CalcFloors(double Wishedheight_);
    void CalcFieldsL(double Maxlength_);
    void FieldBase300L(double Maxlength_);
    void FieldBase250L(double Maxlength_);
    void Calcnum_of_comps_();
    void CalcTechnicalData();
};

}
#endif // SCAFF_H
