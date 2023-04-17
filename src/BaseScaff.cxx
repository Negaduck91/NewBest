#include "lib/FacadeScaff.hxx"
//#include "Logger.hxx"

using namespace Scaff;

FacadeScaff::FacadeScaff(double length_,double height_){
    this->calced_dims_.Floors = this->calced_dims_.AllFieldsL = 
                    this->calced_dims_.LongFieldsL=this->calced_dims_.ShortFieldsL=0;
    calced_dims_.Restheight_ = calced_dims_.Restlength_ = 0.0L;
    //calced_dims_.FI = {first,second,third,fourth,fifth,sixth};
    calced_dims_.FL[5] =  zero;
    calced_dims_.FL[4] =  zero;
    calced_dims_.FL[3] =  zero;
    calced_dims_.FL[0] =  zero;
    calced_dims_.FL[2] =  zero;
    calced_dims_.FL[1] =  two_h;
    calced_dims_.FC = normal;
    calced_dims_.FW=W06;
    this->calced_dims_.height_ = height_;
    this->calced_dims_.Restheight_ = calced_dims_.Restlength_ = 0.0;
    this->calced_dims_.Wishedlength_ = length_;
    this->load_class_ = 3;
    this->calced_dims_.FieldRep[0][0]=seventy;
    this->calced_dims_.FieldRep[0][1]=one;
    this->calced_dims_.FieldRep[0][2]=one_h;
    this->calced_dims_.FieldRep[0][3]=two;
    this->calced_dims_.FieldRep[0][4]=two_h;
    this->calced_dims_.FieldRep[0][5]=three;
    this->calced_dims_.FieldRep[0][6]=zero;
    this->calced_dims_.FieldRep[1][0]=0;
    this->calced_dims_.FieldRep[1][1]=0;
    this->calced_dims_.FieldRep[1][2]=0;
    this->calced_dims_.FieldRep[1][3]=0;
    this->calced_dims_.FieldRep[1][4]=0;
    this->calced_dims_.FieldRep[1][5]=0;
    this->calced_dims_.FieldRep[1][6]=0;

	CalcFieldsL(length_);
	CalcFloors(height_);
	Calcnum_of_comps_();
}


FacadeScaff::FacadeScaff(const UserInput &Input){
	this->calced_dims_.Floors =   this->calced_dims_.AllFieldsL =
                                this->calced_dims_.LongFieldsL=
                                this->calced_dims_.ShortFieldsL=0;
    //calced_dims_.FI = {first,second,third,fourth,fifth,sixth};
    this->calced_dims_.Wishedlength_ = Input.length_;
    calced_dims_.FL[0] =  zero;
    calced_dims_.FL[2] =  zero;
    calced_dims_.FL[1] =  two_h;
    calced_dims_.FL[5] =  zero;
    calced_dims_.FL[4] =  zero;
    calced_dims_.FL[3] =  zero;
	calced_dims_.FC = normal;
    calced_dims_.FW=W06;
    if(Input.frame_type_ == 2){
        calced_dims_.FW=W09;
        this->load_class_ = 4;
    }
    this->num_of_comps_=  { {{0,0,0,0,0},      //alu-planks
                        {0,0,0,0,0},      //steel-planks
                        {0,0,0,0,0}},     //plates-planks
                        {0,0,0,0,0},      //guards
                        {0,0,0,0,0},      //toe board
                        {0,0},        //vertical-braces
                        {0,0,0,0},        //normal-frames
                        {0,0},            //end-guard
                        {0}};
    this->calced_dims_.height_ = Input.height_;
    this->plank_choice_ = Input.plank_choice_;
    this->calced_dims_.Restheight_ = calced_dims_.Restlength_ = 0.0;
    this->calced_dims_.FieldRep[0][0]=seventy;
    this->calced_dims_.FieldRep[0][1]=one;
    this->calced_dims_.FieldRep[0][2]=one_h;
    this->calced_dims_.FieldRep[0][3]=two;
    this->calced_dims_.FieldRep[0][4]=two_h;
    this->calced_dims_.FieldRep[0][5]=three;
    this->calced_dims_.FieldRep[0][6]=zero;
    this->calced_dims_.FieldRep[1][0]=0;
    this->calced_dims_.FieldRep[1][1]=0;
    this->calced_dims_.FieldRep[1][2]=0;
    this->calced_dims_.FieldRep[1][3]=0;
    this->calced_dims_.FieldRep[1][4]=0;
    this->calced_dims_.FieldRep[1][5]=0;
    this->calced_dims_.FieldRep[1][6]=0;
	CalcFieldsL(Input.length_);
	CalcFloors(Input.height_);
	Calcnum_of_comps_();
}

void Scaff::FacadeScaff::SetExtraField(FieldLength FieldToAdd){
    if(calced_dims_.FL[2] == zero) 
        calced_dims_.FL[2]=FieldToAdd;
    else if(calced_dims_.FL[3] == zero)
        calced_dims_.FL[3]=FieldToAdd;
    else if(calced_dims_.FL[4] == zero)
        calced_dims_.FL[4]=FieldToAdd;
    else if(calced_dims_.FL[5] == zero)
        calced_dims_.FL[5]=FieldToAdd;
}

void FacadeScaff::CalcFloors(double Wishedheight_){
    int height_ = Wishedheight_*100;
    int CalcFloors =0;
    if(height_ % 200) {
        calced_dims_.Restheight_ = height_ % 200;
        CalcFloors = height_ / 200;
        if(calced_dims_.Restheight_ <= 50) {
        } else if(calced_dims_.Restheight_ >= 51 && calced_dims_.Restheight_ <= 100) {
            CalcFloors+=1;
            calced_dims_.FC = mini;
        } else if(calced_dims_.Restheight_ >= 101 && calced_dims_.Restheight_ <= 150) {
            CalcFloors+=1;
            calced_dims_.FC = small;
        } else if(calced_dims_.Restheight_ >= 151) {
            this->calced_dims_.Floors+=1;
            calced_dims_.FC = medium;
        }
    } else
        CalcFloors = height_ / 200;
    FacadeScaff::calced_dims_.Floors = CalcFloors;
}

//Just a function to decide which 
//mainfield is used and which algorithm
//we use to calculate the fields
void FacadeScaff::CalcFieldsL(double Maxlength_){
    if(calced_dims_.FL[1] == three)
        FieldBase300L(Maxlength_);
    else if (calced_dims_.FL[1] == two_h)
        FieldBase250L(Maxlength_);
    else
        FieldBase250L(Maxlength_);
}

//This is a small algorithm that starts with 3,07m
//fields and tries to use as less small fields as possible
void FacadeScaff::FieldBase300L(double Maxlength_){

    int Convertedlength_ = Maxlength_ * 100;
    this->calced_dims_.AllFieldsL = Convertedlength_ / 307;
    calced_dims_.Restlength_ = Convertedlength_ % 307;
    calced_dims_.Restlength_ = Convertedlength_ % 307;

    /* Here happens the magic.
       We basically just check how much length_
       we got left and change then the ammount 
       of the different fields length_ to get the
       needed centimeters 
    */
    if(calced_dims_.Restlength_ != 0  ) {
       if(calced_dims_.Restlength_ <= 39){
        std::cout << "1111111111111111111111111111" << std::endl;
        this->calced_dims_.LongFieldsL = this->calced_dims_.AllFieldsL;
        calced_dims_.FieldRep[1][three]=this->calced_dims_.AllFieldsL;
        this->calced_dims_.Calcedlength_ = ((double)(this->calced_dims_.LongFieldsL * 307)/100);
        } else if(calced_dims_.Restlength_ >= 40 && calced_dims_.Restlength_ <= 75) {
            std::cout << "2222222222222222222222222222" << std::endl;
            this->calced_dims_.AllFieldsL += 1;
            if(this->calced_dims_.AllFieldsL > 5) {
                this->calced_dims_.ShortFieldsL = 5;
                this->calced_dims_.LongFieldsL -= 4;
            } else {
                //std::cout << "3333333333333333333333333333" << std::endl;
                this->calced_dims_.ShortFieldsL = 1;
                this->calced_dims_.LongFieldsL -= 1;
                calced_dims_.FL[1] = one;
            }
        } else if(calced_dims_.Restlength_ >= 76 && calced_dims_.Restlength_ <= 125) {
            std::cout << "3333333333333333333333333333" << std::endl;
            this->calced_dims_.AllFieldsL += 1;
            if(this->calced_dims_.AllFieldsL > 4) {
                this->calced_dims_.ShortFieldsL = 4;
                this->calced_dims_.LongFieldsL -= 4;
            } else {
                this->calced_dims_.ShortFieldsL = 1;
                this->calced_dims_.LongFieldsL -= 1;
                calced_dims_.FL[1] = one;
            }
        } else if(calced_dims_.Restlength_ >= 126 && calced_dims_.Restlength_ <= 175) {
            std::cout << "4444444444444444444444444444" << std::endl;
            this->calced_dims_.AllFieldsL += 1;
            if(this->calced_dims_.AllFieldsL > 3) {
                this->calced_dims_.ShortFieldsL = 3;
                this->calced_dims_.LongFieldsL -= 3;
            } else {
                this->calced_dims_.ShortFieldsL = 1;
                this->calced_dims_.LongFieldsL -= 1;
                calced_dims_.FL[1] = one_h;
            }
        } else if(calced_dims_.Restlength_ >= 176 && calced_dims_.Restlength_ <= 225) {
            std::cout << "5555555555555555555555555555" << std::endl;
            this->calced_dims_.AllFieldsL +=1;
            if(this->calced_dims_.AllFieldsL > 2) {
                this->calced_dims_.ShortFieldsL = 2;
                this->calced_dims_.LongFieldsL -= 2;
            } else {
                this->calced_dims_.ShortFieldsL = 1;
                this->calced_dims_.LongFieldsL -= 1;
                calced_dims_.FL[1] = two;
            }
        } else if(calced_dims_.Restlength_ >= 226 && calced_dims_.Restlength_ <= 275) {
            std::cout << "6666666666666666666666666666" << std::endl;
            this->calced_dims_.AllFieldsL +=1;
            this->calced_dims_.ShortFieldsL = 1;
            this->calced_dims_.LongFieldsL -= 1;
        } else if( calced_dims_.Restlength_ >= 276) {
            std::cout << "7777777777777777777777777777" << std::endl;
            this->calced_dims_.AllFieldsL +=1 ;
        } 
    } else{

    } 
}

/*
    TODO
    instead of writing the indicator for which 
    combination of fields we calculated to stdout 
    we should make an extra  logfile maybe

*/
//This is a small algorithm that starts with 2,57m
//fields and tries to use as less small fields as possible
void FacadeScaff::FieldBase250L(double Maxlength_){
    

    //These are the import values we need to calculate the 
    //amount of Fields
    int Convertedlength_ = Maxlength_ * 100 + 1;
    this->calced_dims_.AllFieldsL = Convertedlength_ / 257;
    calced_dims_.Restlength_ = Convertedlength_ % 257;

    //Here happens the magic
    //We basically just check how much length_ 
    //we got left and change then the ammount 
    //of the different fields length_ to get the
    //needed centimeters

    /*          TODO
        Need to fine tune this algo. 
        for example the first if clause 
        is not optimal..
    */
    if(calced_dims_.Restlength_ != 0) {
        if(calced_dims_.Restlength_ <= 39){
        std::cout << "ZZZZZZZZZZZZZZZZZZZZZZZZZZZZ" << std::endl;
        this->calced_dims_.LongFieldsL = this->calced_dims_.AllFieldsL;
        calced_dims_.FieldRep[1][two_h]=this->calced_dims_.AllFieldsL;
        this->calced_dims_.Calcedlength_ = ((double)(this->calced_dims_.LongFieldsL * 257)/100);
        }
        else if(calced_dims_.Restlength_ >= 40 && calced_dims_.Restlength_ <= 60 ) {
            std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAA" << std::endl;
            calced_dims_.FL[0] = three;                       
            calced_dims_.FL[1] = two_h;
            //this->calced_dims_.AllFieldsL += 1;
            this->calced_dims_.ShortFieldsL = this->calced_dims_.AllFieldsL - 1;
            this->calced_dims_.LongFieldsL = 1;
            calced_dims_.FieldRep[1][three]=1;
            calced_dims_.FieldRep[1][two_h]=this->calced_dims_.ShortFieldsL;
            this->calced_dims_.Calcedlength_ = ((double)(this->calced_dims_.LongFieldsL * 307)/100) +
                                            ((double)(this->calced_dims_.ShortFieldsL * 257)/100);
        
        } else if(calced_dims_.Restlength_ >= 61 && calced_dims_.Restlength_ <= 75) {
            std::cout << "BBBBBBBBBBBBBBBBBBBBBBBBBBBB" << std::endl;
            calced_dims_.FL[0] = one_h;
            this->calced_dims_.AllFieldsL += 1;
            this->calced_dims_.ShortFieldsL =2;
            this->calced_dims_.LongFieldsL = this->calced_dims_.AllFieldsL - this->calced_dims_.ShortFieldsL;
            calced_dims_.FieldRep[1][two_h]=this->calced_dims_.LongFieldsL;
            calced_dims_.FieldRep[1][one_h]=this->calced_dims_.ShortFieldsL;
            this->calced_dims_.Calcedlength_ = ((double)(this->calced_dims_.LongFieldsL  * 257)/100) + 
                                            ((double)(this->calced_dims_.ShortFieldsL * 157)/100);
        } else if(calced_dims_.Restlength_ >= 76 && calced_dims_.Restlength_ <= 125) {
            //TODO
            /*
                MAybe something is wrong in this case.
                set MaxStalkload and weight sometime wrong.
                Need to look out for this
            */
            std::cout << "CCCCCCCCCCCCCCCCCCCCCCCCCCCC" << std::endl;
            calced_dims_.FL[1] = two_h;
            calced_dims_.FL[0] = three;
            //this->calced_dims_.AllFieldsL += 1;
            this->calced_dims_.ShortFieldsL = 2;
            this->calced_dims_.LongFieldsL = this->calced_dims_.AllFieldsL - 2;
            calced_dims_.FieldRep[1][two_h]=this->calced_dims_.LongFieldsL;
            calced_dims_.FieldRep[1][three]=this->calced_dims_.ShortFieldsL;
            this->calced_dims_.Calcedlength_ = ((double)(this->calced_dims_.LongFieldsL * 257)/100)+
                                            ((double)(this->calced_dims_.ShortFieldsL * 307)/100);
              
        } else if(calced_dims_.Restlength_ >= 126 && calced_dims_.Restlength_ <= 175) {
            std::cout << "DDDDDDDDDDDDDDDDDDDDDDDDDDDD" << std::endl;
            calced_dims_.FL[0] = two;
            this->calced_dims_.AllFieldsL+=1;
            this->calced_dims_.ShortFieldsL =2;
            this->calced_dims_.LongFieldsL = this->calced_dims_.AllFieldsL - this->calced_dims_.ShortFieldsL;
            calced_dims_.FieldRep[1][two_h]=this->calced_dims_.LongFieldsL;
            calced_dims_.FieldRep[1][two]=this->calced_dims_.ShortFieldsL;
            this->calced_dims_.Calcedlength_ = ((double)(this->calced_dims_.LongFieldsL * 257)/100) +
                                            ((double)(this->calced_dims_.ShortFieldsL * 207)/100);
        } else if(calced_dims_.Restlength_ >= 176 && calced_dims_.Restlength_ <= 225) {
            std::cout << "FFFFFFFFFFFFFFFFFFFFFFFFFFFF" << std::endl;
            calced_dims_.FL[0] = two;
            this->calced_dims_.AllFieldsL+=1;
            this->calced_dims_.ShortFieldsL =1;
            this->calced_dims_.LongFieldsL = this->calced_dims_.AllFieldsL - this->calced_dims_.ShortFieldsL;
            calced_dims_.FieldRep[1][two_h]=this->calced_dims_.LongFieldsL;
            calced_dims_.FieldRep[1][two]=this->calced_dims_.ShortFieldsL;
            this->calced_dims_.Calcedlength_ = ((double)(this->calced_dims_.LongFieldsL * 257)/100)+
                                            ((double)(this->calced_dims_.ShortFieldsL * 207)/100);
        } else if(calced_dims_.Restlength_ >= 226) {
            std::cout << "GGGGGGGGGGGGGGGGGGGGGGGGGGGG" << std::endl;
            this->calced_dims_.AllFieldsL+=1;
            this->calced_dims_.LongFieldsL = this->calced_dims_.AllFieldsL;
            calced_dims_.FieldRep[1][two_h]=this->calced_dims_.AllFieldsL;
            this->calced_dims_.Calcedlength_ = ((double)(this->calced_dims_.LongFieldsL * 257)/100);
        }
    }else{ 
        std::cout << "HHHHHHHHHHHHHHHHHHHHHHHHHHHH" << std::endl;
        this->calced_dims_.LongFieldsL = this->calced_dims_.AllFieldsL;
        calced_dims_.FieldRep[1][two_h]=this->calced_dims_.AllFieldsL;
        this->calced_dims_.Calcedlength_ = ((double)(this->calced_dims_.LongFieldsL * 257)/100);
    }
    CalcTechnicalData();
    
    //for(int i =0; i<6;i++){
    //   test = calced_dims_.FieldRep[1][i] * calced_dims_.FI[i];
        //std::cout << test << std::endl;
    //}
}

void FacadeScaff::Calcnum_of_comps_(){

    //FIrst of all we go to calculate the frames
    int i_ToeBoard = calced_dims_.Floors;
    int i_Planks = calced_dims_.Floors*2;
    int i_side_guard =(calced_dims_.Floors*2)+1;
    for(int i =0;i<6;i++){
        num_of_comps_.side_guard[i] = calced_dims_.FieldRep[1][i] * i_side_guard;
        //num_of_comps_.side_guard[i] = i_side_guard;
        num_of_comps_.ToeBoard[i] =calced_dims_.FieldRep[1][i] * i_ToeBoard;
        //num_of_comps_.ToeBoard[i] += i_ToeBoard;
        num_of_comps_.UsedPlanks.alu[i] = calced_dims_.FieldRep[1][i] * i_Planks;
        //num_of_comps_.UsedPlanks.alu[i] += i_Planks;
    }
    num_of_comps_.frames[3] = (FacadeScaff::calced_dims_.AllFieldsL+1)*(FacadeScaff::calced_dims_.Floors);
    if(calced_dims_.FC != normal){
        num_of_comps_.frames[calced_dims_.FC] = FacadeScaff::calced_dims_.AllFieldsL + 1;
        num_of_comps_.frames[3] -= num_of_comps_.frames[calced_dims_.FC];
    }
	//And here we check, as always, which main field we are using
	//and then calculate the other base components
    num_of_comps_.dia[0] = ( (int)( this->calced_dims_.AllFieldsL / 5)+1)* this->calced_dims_.Floors;
    if(this->calced_dims_.AllFieldsL <= 5)num_of_comps_.dia[0] = this->calced_dims_.Floors;
        
    
        
        //num_of_comps_.side_guard[calced_dims_.FL[1]]     = ((this->calced_dims_.AllFieldsL*2*this->calced_dims_.Floors)+this->calced_dims_.AllFieldsL)-(((this->calced_dims_.Floors*2)+1) * this->calced_dims_.ShortFieldsL);
        //num_of_comps_.side_guard[calced_dims_.FL[0]]    = (2*this->calced_dims_.Floors+1)* this->calced_dims_.ShortFieldsL;
        //num_of_comps_.ToeBoard[calced_dims_.FL[1]]  = (this->calced_dims_.Floors * this->calced_dims_.AllFieldsL) - ( this->calced_dims_.Floors * this->calced_dims_.ShortFieldsL);
        //num_of_comps_.ToeBoard[calced_dims_.FL[0]] = this->calced_dims_.Floors * this->calced_dims_.ShortFieldsL;
        num_of_comps_.BaseJack 		 = (this->calced_dims_.AllFieldsL+1)*2;
        //num_of_comps_.Ladder[calced_dims_.FL[1]]      		 = this->calced_dims_.Floors;
       
        //switch(this->plank_choice_) {
        //case 1:
            //num_of_comps_.UsedPlanks.alu[calced_dims_.FL[0]]   = (( this->calced_dims_.Floors * this->calced_dims_.FW ) * this->calced_dims_.ShortFieldsL);
            //num_of_comps_.UsedPlanks.alu[calced_dims_.FL[1]]   = (( this->calced_dims_.AllFieldsL * this->calced_dims_.Floors)*calced_dims_.FW) - (( this->calced_dims_.Floors * calced_dims_.FW ) * this->calced_dims_.ShortFieldsL);
            /*break;
        case 2:
            num_of_comps_.UsedPlanks.steel[calced_dims_.FL[0]]   = (( this->calced_dims_.Floors * calced_dims_.FW ) * this->calced_dims_.ShortFieldsL);
            num_of_comps_.UsedPlanks.steel[calced_dims_.FL[1]]   = (( this->calced_dims_.AllFieldsL * this->calced_dims_.Floors)*calced_dims_.FW) - (( this->calced_dims_.Floors * calced_dims_.FW ) * this->calced_dims_.ShortFieldsL);
            break;
        case 3:
            num_of_comps_.UsedPlanks.plates[calced_dims_.FL[0]]   = (( this->calced_dims_.Floors * calced_dims_.FW ) * this->calced_dims_.ShortFieldsL);
            num_of_comps_.UsedPlanks.plates[calced_dims_.FL[1]]   = (( this->calced_dims_.AllFieldsL * this->calced_dims_.Floors)*calced_dims_.FW) - (( this->calced_dims_.Floors * calced_dims_.FW ) * this->calced_dims_.ShortFieldsL);
            break;
        }*/
        CalcTechnicalData();
}

void FacadeScaff::CalcTechnicalData(){
    this->calced_dims_.Squaremetre = (double)this->calced_dims_.Calcedlength_ * (this->calced_dims_.height_+1.0);
    this->calced_dims_.Weight =  (calced_dims_.Squaremetre*10)/1000;
    this->calced_dims_.MaxStalkLoad = (calced_dims_.height_ +1) * 58;
    
}

void FacadeScaff::SetUpdatetComponents(FieldLength FieldToSub,FieldLength FieldToAdd, int Floors){
    int i_ToeBoard = Floors;
    int i_Planks = Floors*2;
    int i_side_guard =(Floors*2)+1;
    num_of_comps_.side_guard[FieldToSub] -= i_side_guard;
    num_of_comps_.side_guard[FieldToAdd] += i_side_guard;
    num_of_comps_.ToeBoard[FieldToSub] -= i_ToeBoard;
    num_of_comps_.ToeBoard[FieldToAdd] += i_ToeBoard;
    num_of_comps_.UsedPlanks.alu[FieldToSub] -= i_Planks;
    num_of_comps_.UsedPlanks.alu[FieldToAdd] += i_Planks;

}

void FacadeScaff::SetDimensions(const Dimensions& DataToSet){
    this->calced_dims_.AllFieldsL = DataToSet.AllFieldsL;
    this->calced_dims_.Calcedlength_ = DataToSet.Calcedlength_;
    this->calced_dims_.FC = DataToSet.FC;
    this->calced_dims_.Floors = DataToSet.Floors;
    this->calced_dims_.FW = DataToSet.FW;
    this->calced_dims_.height_ = DataToSet.height_;
    this->calced_dims_.LongFieldsL = DataToSet.LongFieldsL;
    this->calced_dims_.MaxStalkLoad = DataToSet.MaxStalkLoad;
    this->calced_dims_.Restheight_ = DataToSet.Restheight_;
    this->calced_dims_.Restlength_ = DataToSet.Restlength_;
    this->calced_dims_.ShortFieldsL = DataToSet.ShortFieldsL;
    this->calced_dims_.Squaremetre = DataToSet.Squaremetre;
    this->calced_dims_.Weight = DataToSet.Weight;
    this->calced_dims_.Wishedlength_ = DataToSet.Wishedlength_;
    this->calced_dims_.FL[0] = DataToSet.FL[0];
    this->calced_dims_.FL[1] = DataToSet.FL[1];
    this->calced_dims_.FieldRep[0][0] = DataToSet.FieldRep[0][0];
    this->calced_dims_.FieldRep[0][1] = DataToSet.FieldRep[0][1];
    this->calced_dims_.FieldRep[0][2] = DataToSet.FieldRep[0][2];
    this->calced_dims_.FieldRep[0][3] = DataToSet.FieldRep[0][3];
    this->calced_dims_.FieldRep[0][4] = DataToSet.FieldRep[0][4];
    this->calced_dims_.FieldRep[0][5] = DataToSet.FieldRep[0][5];
    this->calced_dims_.FieldRep[1][0] = DataToSet.FieldRep[1][0];
    this->calced_dims_.FieldRep[1][1] = DataToSet.FieldRep[1][1];
    this->calced_dims_.FieldRep[1][2] = DataToSet.FieldRep[1][2];
    this->calced_dims_.FieldRep[1][3] = DataToSet.FieldRep[1][3];
    this->calced_dims_.FieldRep[1][4] = DataToSet.FieldRep[1][4];
    this->calced_dims_.FieldRep[1][5] = DataToSet.FieldRep[1][5];
}

