#include "BaseScaff.hxx"
#include "Logger.hxx"

using namespace Scaff;

BaseScaff::BaseScaff(double Length,double Height, FieldIndicator BaseField){
    this->CalcedData.Floors = this->CalcedData.AllFieldsL = 
                    this->CalcedData.LongFieldsL=this->CalcedData.ShortFieldsL=0;
    CalcedData.RestHeight = CalcedData.RestLength = 0.0L;
    //CalcedData.FI = {first,second,third,fourth,fifth,sixth};
    CalcedData.FL[5] =  three_h;
    CalcedData.FL[4] =  three_h;
    CalcedData.FL[3] =  three_h;
    CalcedData.FL[0] =  three_h;
    CalcedData.FL[2] =  three_h;
    CalcedData.FL[1] =  three_h;
    CalcedData.FC = normal;
    CalcedData.FW=W06;
    this->CalcedData.Height = Height;
    this->CalcedData.RestHeight = CalcedData.RestLength = 0.0;
    this->CalcedData.WishedLength = Length;
    this->LoadClass = 3;
    this->CalcedData.FieldRep[0][0]=seventy;
    this->CalcedData.FieldRep[0][1]=one;
    this->CalcedData.FieldRep[0][2]=one_h;
    this->CalcedData.FieldRep[0][3]=two;
    this->CalcedData.FieldRep[0][4]=two_h;
    this->CalcedData.FieldRep[0][5]=three;
    this->CalcedData.FieldRep[0][6]=three_h;
    this->CalcedData.FieldRep[1][0]=0;
    this->CalcedData.FieldRep[1][1]=0;
    this->CalcedData.FieldRep[1][2]=0;
    this->CalcedData.FieldRep[1][3]=0;
    this->CalcedData.FieldRep[1][4]=0;
    this->CalcedData.FieldRep[1][5]=0;
    this->CalcedData.FieldRep[1][6]=0;

	CalcFieldsL(Length,BaseField);
	CalcFloors(Height);
	CalcMaterial();
}

BaseScaff::BaseScaff(const UserInput &Input){
	this->CalcedData.Floors =   this->CalcedData.AllFieldsL =
                                this->CalcedData.LongFieldsL=
                                this->CalcedData.ShortFieldsL=0;
    //CalcedData.FI = {first,second,third,fourth,fifth,sixth};
    this->CalcedData.WishedLength = Input.Length;
    CalcedData.FL[0] =  three_h;
    CalcedData.FL[2] =  three_h;
    CalcedData.FL[1] =  three_h;
    CalcedData.FL[5] =  three_h;
    CalcedData.FL[4] =  three_h;
    CalcedData.FL[3] =  three_h;
	CalcedData.FC = normal;
    CalcedData.FW=W06;
    if(Input.FrameType == 2){
        CalcedData.FW=W09;
        this->LoadClass = 4;
    }
    this->Material=  { {{0,0,0,0,0},      //alu-planks
                        {0,0,0,0,0},      //steel-planks
                        {0,0,0,0,0}},     //wodden-planks
                        {0,0,0,0,0},      //guards
                        {0,0,0,0,0},      //toe board
                        {0,0},        //vertical-braces
                        {0,0,0,0},        //normal-frames
                        {0,0},            //end-guard
                        0};
    this->CalcedData.Height = Input.Height;
    this->PlankChoice = Input.PlankChoice;
    this->CalcedData.RestHeight = CalcedData.RestLength = 0.0;
    this->CalcedData.FieldRep[0][0]=seventy;
    this->CalcedData.FieldRep[0][1]=one;
    this->CalcedData.FieldRep[0][2]=one_h;
    this->CalcedData.FieldRep[0][3]=two;
    this->CalcedData.FieldRep[0][4]=two_h;
    this->CalcedData.FieldRep[0][5]=three;
    this->CalcedData.FieldRep[0][6]=three_h;
    this->CalcedData.FieldRep[1][0]=0;
    this->CalcedData.FieldRep[1][1]=0;
    this->CalcedData.FieldRep[1][2]=0;
    this->CalcedData.FieldRep[1][3]=0;
    this->CalcedData.FieldRep[1][4]=0;
    this->CalcedData.FieldRep[1][5]=0;
    this->CalcedData.FieldRep[1][6]=0;
	CalcFieldsL(Input.Length);
	CalcFloors(Input.Height);
	CalcMaterial();
}

void Scaff::BaseScaff::SetExtraField(FieldLength FieldToAdd){
    if(CalcedData.FL[2] == three_h) 
        CalcedData.FL[2]=FieldToAdd;
    else if(CalcedData.FL[3] == three_h)
        CalcedData.FL[3]=FieldToAdd;
    else if(CalcedData.FL[4] == three_h)
        CalcedData.FL[4]=FieldToAdd;
    else if(CalcedData.FL[5] == three_h)
        CalcedData.FL[5]=FieldToAdd;
}

void BaseScaff::CalcFloors(double WishedHeight){
    int Height = WishedHeight*100;
    int CalcFloors =0;
    if(Height % 200) {
        CalcedData.RestHeight = Height % 200;
        CalcFloors = Height / 200;
        if(CalcedData.RestHeight <= 50) {
        } else if(CalcedData.RestHeight >= 51 && CalcedData.RestHeight <= 100) {
            CalcFloors+=1;
            CalcedData.FC = mini;
        } else if(CalcedData.RestHeight >= 101 && CalcedData.RestHeight <= 150) {
            CalcFloors+=1;
            CalcedData.FC = small;
        } else if(CalcedData.RestHeight >= 151) {
            this->CalcedData.Floors+=1;
            CalcedData.FC = medium;
        }
    } else
        CalcFloors = Height / 200;
    BaseScaff::CalcedData.Floors = CalcFloors;
}

//Just a function to decide which 
//mainfield is used and which algorithm
//we use to calculate the fields
void BaseScaff::CalcFieldsL(double MaxLength, FieldIndicator BaseField){
    if(BaseField == sixth)
        FieldBase300L(MaxLength);
    else if (BaseField == fifth)
        FieldBase250L(MaxLength);
    else
        FieldBase250L(MaxLength);
}

//This is a small algorithm that starts with 3,07m
//fields and tries to use as less small fields as possible
void BaseScaff::FieldBase300L(double MaxLength){

    int ConvertedLength = MaxLength * 100+1;
    this->CalcedData.AllFieldsL = ConvertedLength / 307;
    this->CalcedData.LongFieldsL = this->CalcedData.AllFieldsL;
    CalcedData.RestLength = ConvertedLength % 307;
    CalcedData.FieldRep[1][three]=this->CalcedData.AllFieldsL;
	CalcedData.FL[0] = three; 
    /* Here happens the magic.
       We basically just check how much length
       we got left and change then the ammount 
       of the different fields length to get the
       needed centimeters 
    */
    if(CalcedData.RestLength != 0  ) {
       if(CalcedData.RestLength <= 39){
        std::cout << "999999999999999999999999999" << std::endl;
        this->CalcedData.LongFieldsL = this->CalcedData.AllFieldsL;
        CalcedData.FieldRep[1][three]=this->CalcedData.AllFieldsL;
        this->CalcedData.CalcedLength = ((double)(this->CalcedData.LongFieldsL * 307)/100);
        } 
        else if(CalcedData.RestLength >= 40 && CalcedData.RestLength <= 60) {
            std::cout << "0000000000000000000000000" << std::endl;
            this->CalcedData.AllFieldsL += 1;
            this->CalcedData.ShortFieldsL = 2;
            this->CalcedData.LongFieldsL = this->CalcedData.AllFieldsL - 2;

            CalcedData.FieldRep[1][two]= 1;
            CalcedData.FieldRep[1][one_h]= 1;
            CalcedData.FieldRep[1][three] = this->CalcedData.LongFieldsL;
            
			CalcedData.FL[0] = three;                       
            CalcedData.FL[1] = two;
			CalcedData.FL[2] = one_h;

            this->CalcedData.CalcedLength = ((double)(this->CalcedData.LongFieldsL * 307)/100) +
                                            ((double)(207/100))+
                                            ((double)(157/100));
            /*if(this->CalcedData.AllFieldsL > 5) {
                this->CalcedData.ShortFieldsL = 5;
                this->CalcedData.LongFieldsL -= 4;
            } else {
                //std::cout << "3333333333333333333333333333" << std::endl;
                this->CalcedData.ShortFieldsL = 1;
                this->CalcedData.LongFieldsL -= 1;
                CalcedData.FL[1] = one;
            }*/
        } else if(CalcedData.RestLength >= 60 && CalcedData.RestLength <= 125) {
            std::cout << "1111111111111111111111111111" << std::endl;
            this->CalcedData.AllFieldsL += 1;
            this->CalcedData.ShortFieldsL = 3;
            this->CalcedData.LongFieldsL = this->CalcedData.AllFieldsL - 3;
			
            CalcedData.FieldRep[1][two_h]= 2;
            CalcedData.FieldRep[1][two]= 1;
             CalcedData.FieldRep[1][three] = this->CalcedData.LongFieldsL;

            CalcedData.FL[0] = three;                       
            CalcedData.FL[1] = two_h;
			CalcedData.FL[2] = two;

            this->CalcedData.CalcedLength = ((double)(this->CalcedData.LongFieldsL * 307)/100) +
                                            ((double)((2*257)/100))+
                                            ((double)(207/100));

            
            /*} else {
                this->CalcedData.ShortFieldsL = 1;
                this->CalcedData.LongFieldsL -= 1;
                CalcedData.FL[1] = one;
            }*/
        } else if(CalcedData.RestLength >= 126 && CalcedData.RestLength <= 175) {
            std::cout << "2222222222222222222222222222" << std::endl;
            
            this->CalcedData.AllFieldsL += 1;
            this->CalcedData.ShortFieldsL = 3;
            this->CalcedData.LongFieldsL = this->CalcedData.AllFieldsL - 3;
			
            CalcedData.FieldRep[1][two_h]= 3;
            CalcedData.FieldRep[1][three] = this->CalcedData.LongFieldsL;

            CalcedData.FL[0] = three;                       
            CalcedData.FL[1] = two_h;

            this->CalcedData.CalcedLength = ((double)(this->CalcedData.LongFieldsL * 307)/100) +
                                            ((double)((3*257)/100));


			
            /*if(this->CalcedData.AllFieldsL > 3) {
                this->CalcedData.ShortFieldsL = 3;
                this->CalcedData.LongFieldsL -= 3;
            } else {
                this->CalcedData.ShortFieldsL = 1;
                this->CalcedData.LongFieldsL -= 1;
                CalcedData.FL[1] = one_h;
            }*/
        } else if(CalcedData.RestLength >= 176 && CalcedData.RestLength <= 225) {
            std::cout << "3333333333333333333333333333" << std::endl;
            this->CalcedData.AllFieldsL += 1;
            this->CalcedData.ShortFieldsL = 1;
            this->CalcedData.LongFieldsL = this->CalcedData.AllFieldsL - 1;
			
            CalcedData.FieldRep[1][two]= 1;
            CalcedData.FieldRep[1][three] = this->CalcedData.LongFieldsL;

            CalcedData.FL[0] = three;                       
            CalcedData.FL[1] = two;

            this->CalcedData.CalcedLength = ((double)(this->CalcedData.LongFieldsL * 307)/100) +
                                            ((double)((207)/100));
            /*if(this->CalcedData.AllFieldsL > 2) {
                this->CalcedData.ShortFieldsL = 2;
                this->CalcedData.LongFieldsL -= 2;
            } else {
                this->CalcedData.ShortFieldsL = 1;
                this->CalcedData.LongFieldsL -= 1;
                CalcedData.FL[1] = two;
            }*/
        } else if(CalcedData.RestLength >= 226 && CalcedData.RestLength <= 275) {
            std::cout << "4444444444444444444444444444" << std::endl;
            this->CalcedData.AllFieldsL += 1;
            this->CalcedData.ShortFieldsL = 1;
            this->CalcedData.LongFieldsL = this->CalcedData.AllFieldsL - 1;
			
            CalcedData.FieldRep[1][two_h]= 1;
            CalcedData.FieldRep[1][three] = this->CalcedData.LongFieldsL;

            CalcedData.FL[0] = three;                       
            CalcedData.FL[1] = two_h;

            this->CalcedData.CalcedLength = ((double)(this->CalcedData.LongFieldsL * 307)/100) +
                                            ((double)((257)/100));
        } else if( CalcedData.RestLength >= 276) {
            std::cout << "5555555555555555555555555555" << std::endl;
            this->CalcedData.AllFieldsL += 1;
            this->CalcedData.ShortFieldsL = 0;
            this->CalcedData.LongFieldsL = this->CalcedData.AllFieldsL;
            CalcedData.FieldRep[1][three] = this->CalcedData.LongFieldsL;
            CalcedData.FL[0] = three;                       
            this->CalcedData.CalcedLength = ((double)(this->CalcedData.LongFieldsL * 307)/100);
        } 
    } else{
        std::cout << "0000000000000000000000000000" << std::endl;
        this->CalcedData.ShortFieldsL = 0;
            this->CalcedData.LongFieldsL = this->CalcedData.AllFieldsL;
            CalcedData.FieldRep[1][three] = this->CalcedData.LongFieldsL;
            CalcedData.FL[0] = three;                       
            this->CalcedData.CalcedLength = ((double)(this->CalcedData.LongFieldsL * 307)/100);
    } 
    CalcTechnicalData();
}

/*
    TODO
    instead of writing the indicator for which 
    combination of fields we calculated to stdout 
    we should make an extra  logfile maybe

*/
//This is a small algorithm that starts with 2,57m
//fields and tries to use as less small fields as possible
void BaseScaff::FieldBase250L(double MaxLength){
    

    //These are the import values we need to calculate the 
    //amount of Fields with
    int ConvertedLength = MaxLength * 100 + 1;
    this->CalcedData.AllFieldsL = ConvertedLength / 257;
    CalcedData.RestLength = ConvertedLength % 257;

    //Here happens the magic
    //We basically just check how much length 
    //we got left and change then the ammount 
    //of the different fields length to get the
    //needed centimeters

    /*          TODO
        Need to fine tune this algo. 
        for example the first if clause 
        is not optimal..
    */
    if(CalcedData.RestLength != 0) {
        if(CalcedData.RestLength <= 39){
        std::cout << "ZZZZZZZZZZZZZZZZZZZZZZZZZZZZ" << std::endl;
        CalcedData.FL[0] = two_h;
        this->CalcedData.LongFieldsL = this->CalcedData.AllFieldsL;
        CalcedData.FieldRep[1][two_h]=this->CalcedData.AllFieldsL;
        this->CalcedData.CalcedLength = ((double)(this->CalcedData.LongFieldsL * 257)/100);
        }
        else if(CalcedData.RestLength >= 40 && CalcedData.RestLength <= 60 ) {
            std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAA" << std::endl;
            CalcedData.FL[0] = three;                       
            CalcedData.FL[1] = two_h;
            //this->CalcedData.AllFieldsL += 1;
            this->CalcedData.ShortFieldsL = this->CalcedData.AllFieldsL - 1;
            this->CalcedData.LongFieldsL = 1;
            CalcedData.FieldRep[1][three]=1;
            CalcedData.FieldRep[1][two_h]=this->CalcedData.ShortFieldsL;
            this->CalcedData.CalcedLength = ((double)(this->CalcedData.LongFieldsL * 307)/100) +
                                            ((double)(this->CalcedData.ShortFieldsL * 257)/100);
        
        } else if(CalcedData.RestLength >= 61 && CalcedData.RestLength <= 75) {
            std::cout << "BBBBBBBBBBBBBBBBBBBBBBBBBBBB" << std::endl;
            CalcedData.FL[0] = two_h;
            CalcedData.FL[1] = one_h;
            this->CalcedData.AllFieldsL += 1;
            this->CalcedData.ShortFieldsL =2;
            this->CalcedData.LongFieldsL = this->CalcedData.AllFieldsL - this->CalcedData.ShortFieldsL;
            CalcedData.FieldRep[1][two_h]=this->CalcedData.LongFieldsL;
            CalcedData.FieldRep[1][one_h]=this->CalcedData.ShortFieldsL;
            this->CalcedData.CalcedLength = ((double)(this->CalcedData.LongFieldsL  * 257)/100) + 
                                            ((double)(this->CalcedData.ShortFieldsL * 157)/100);
        } else if(CalcedData.RestLength >= 76 && CalcedData.RestLength <= 125) {
            //TODO
            /*
                MAybe something is wrong in this case.
                set MaxStalkload and weight sometime wrong.
                Need to look out for this
            */
            std::cout << "CCCCCCCCCCCCCCCCCCCCCCCCCCCC" << std::endl;
            CalcedData.FL[1] = two_h;
            CalcedData.FL[0] = three;
            //this->CalcedData.AllFieldsL += 1;
            this->CalcedData.ShortFieldsL = 2;
            this->CalcedData.LongFieldsL = this->CalcedData.AllFieldsL - 2;
            CalcedData.FieldRep[1][two_h]=this->CalcedData.LongFieldsL;
            CalcedData.FieldRep[1][three]=this->CalcedData.ShortFieldsL;
            this->CalcedData.CalcedLength = ((double)(this->CalcedData.LongFieldsL * 257)/100)+
                                            ((double)(this->CalcedData.ShortFieldsL * 307)/100);
              
        } else if(CalcedData.RestLength >= 126 && CalcedData.RestLength <= 175) {
            std::cout << "DDDDDDDDDDDDDDDDDDDDDDDDDDDD" << std::endl;
            CalcedData.FL[1] = two;
            CalcedData.FL[0] = two_h;
            this->CalcedData.AllFieldsL+=1;
            this->CalcedData.ShortFieldsL =2;
            this->CalcedData.LongFieldsL = this->CalcedData.AllFieldsL - this->CalcedData.ShortFieldsL;
            CalcedData.FieldRep[1][two_h]=this->CalcedData.LongFieldsL;
            CalcedData.FieldRep[1][two]=this->CalcedData.ShortFieldsL;
            this->CalcedData.CalcedLength = ((double)(this->CalcedData.LongFieldsL * 257)/100) +
                                            ((double)(this->CalcedData.ShortFieldsL * 207)/100);
        } else if(CalcedData.RestLength >= 176 && CalcedData.RestLength <= 225) {
            std::cout << "EEEEEEEEEEEEEEEEEEEEEEEEEEEE" << std::endl;
            CalcedData.FL[1] = two;
            CalcedData.FL[0] = two_h;
            this->CalcedData.AllFieldsL+=1;
            this->CalcedData.ShortFieldsL =1;
            this->CalcedData.LongFieldsL = this->CalcedData.AllFieldsL - this->CalcedData.ShortFieldsL;
            CalcedData.FieldRep[1][two_h]=this->CalcedData.LongFieldsL;
            CalcedData.FieldRep[1][two]=this->CalcedData.ShortFieldsL;
            this->CalcedData.CalcedLength = ((double)(this->CalcedData.LongFieldsL * 257)/100)+
                                            ((double)(this->CalcedData.ShortFieldsL * 207)/100);
        } else if(CalcedData.RestLength >= 226) {
            std::cout << "FFFFFFFFFFFFFFFFFFFFFFFFFFFF" << std::endl;
            CalcedData.FL[0] = two_h;
            this->CalcedData.AllFieldsL+=1;
            this->CalcedData.LongFieldsL = this->CalcedData.AllFieldsL;
            CalcedData.FieldRep[1][two_h]=this->CalcedData.AllFieldsL;
            this->CalcedData.CalcedLength = ((double)(this->CalcedData.LongFieldsL * 257)/100);

        }
    }else{ 
        std::cout << "HHHHHHHHHHHHHHHHHHHHHHHHHHHH" << std::endl;
        CalcedData.FL[0] = two_h;
        this->CalcedData.LongFieldsL = this->CalcedData.AllFieldsL;
        CalcedData.FieldRep[1][two_h]=this->CalcedData.AllFieldsL;
        this->CalcedData.CalcedLength = ((double)(this->CalcedData.LongFieldsL * 257)/100);
    }
    CalcTechnicalData();
    
}

void BaseScaff::CalcMaterial(){
    int i_Planks = CalcedData.Floors*2;
    if(CalcedData.FW==W09)i_Planks = CalcedData.Floors*3;
    int i_ToeBoard = CalcedData.Floors;
    int i_SideGuard =(CalcedData.Floors*2)+1;
    for(int i =0;i<6;i++){
        Material.SideGuard[i] = CalcedData.FieldRep[1][i] * i_SideGuard;
        Material.ToeBoard[i] =CalcedData.FieldRep[1][i] * i_ToeBoard;
        Material.UsedPlanks.alu[i] = CalcedData.FieldRep[1][i] * i_Planks;
    }
    Material.Frames[3] = (BaseScaff::CalcedData.AllFieldsL+1)*(BaseScaff::CalcedData.Floors);
    if(CalcedData.FC != normal){
        Material.Frames[CalcedData.FC] = BaseScaff::CalcedData.AllFieldsL + 1;
        Material.Frames[3] -= Material.Frames[CalcedData.FC];
    }
	//And here we check, as always, which main field we are using
	//and then calculate the other base components
    Material.Dia[0] = ( (int)( this->CalcedData.AllFieldsL / 5)+1)* this->CalcedData.Floors;
    if(this->CalcedData.AllFieldsL <= 5)Material.Dia[0] = this->CalcedData.Floors;
        Material.BaseJack = (this->CalcedData.AllFieldsL+1)*2;
    CalcTechnicalData();
}

void BaseScaff::CalcTechnicalData(){
    this->CalcedData.Squaremetre = (double)this->CalcedData.CalcedLength * (this->CalcedData.Height+1.0);
    this->CalcedData.Weight =  (CalcedData.Squaremetre*10)/1000;
    this->CalcedData.MaxStalkLoad = (CalcedData.Height +1) * 58;
}

void BaseScaff::SwapFields(FieldLength FieldToSub,FieldLength FieldToAdd, int Floors){
    SubField(FieldToSub,Floors);
    AddField(FieldToAdd,Floors);
}

void BaseScaff::SubField(FieldLength FieldToSub, int Floors){
    int i_ToeBoard = Floors;
    int i_Planks = Floors*2;
    int i_SideGuard =(Floors*2)+1;
        
    Material.UsedPlanks.alu[FieldToSub] -= i_Planks;
    Material.SideGuard[FieldToSub] -= i_SideGuard;
    Material.ToeBoard[FieldToSub] -= i_ToeBoard;
    Material.Frames[3] -= Floors;
    if(Material.Frames[0]!=0)Material.Frames[0]-=1;
    else if(Material.Frames[1]!=0)Material.Frames[1]-=1;
    else if(Material.Frames[2]!=0)Material.Frames[2]-=1;

    
}
    
void BaseScaff::AddField(FieldLength FieldToAdd, int Floors){
    int i_ToeBoard = Floors;
    int i_Planks = Floors*2;
    int i_SideGuard =(Floors*2)+1;

    Material.UsedPlanks.alu[FieldToAdd] += i_Planks;
    Material.SideGuard[FieldToAdd] += i_SideGuard;
    Material.ToeBoard[FieldToAdd] += i_ToeBoard;

    Material.Frames[3] += Floors;
    if(Material.Frames[0]!=0)Material.Frames[0]+=1;
    else if(Material.Frames[1]!=0)Material.Frames[1]+=1;
    else if(Material.Frames[2]!=0)Material.Frames[2]+=1;

}

void BaseScaff::SetDimensions(const Dimensions& DataToSet){
    this->CalcedData.AllFieldsL = DataToSet.AllFieldsL;
    this->CalcedData.CalcedLength = DataToSet.CalcedLength;
    this->CalcedData.FC = DataToSet.FC;
    this->CalcedData.Floors = DataToSet.Floors;
    this->CalcedData.FW = DataToSet.FW;
    this->CalcedData.Height = DataToSet.Height;
    this->CalcedData.LongFieldsL = DataToSet.LongFieldsL;
    this->CalcedData.MaxStalkLoad = DataToSet.MaxStalkLoad;
    this->CalcedData.RestHeight = DataToSet.RestHeight;
    this->CalcedData.RestLength = DataToSet.RestLength;
    this->CalcedData.ShortFieldsL = DataToSet.ShortFieldsL;
    this->CalcedData.Squaremetre = DataToSet.Squaremetre;
    this->CalcedData.Weight = DataToSet.Weight;
    this->CalcedData.WishedLength = DataToSet.WishedLength;
    this->CalcedData.FL[0] = DataToSet.FL[0];
    this->CalcedData.FL[1] = DataToSet.FL[1];
    this->CalcedData.FieldRep[0][0] = DataToSet.FieldRep[0][0];
    this->CalcedData.FieldRep[0][1] = DataToSet.FieldRep[0][1];
    this->CalcedData.FieldRep[0][2] = DataToSet.FieldRep[0][2];
    this->CalcedData.FieldRep[0][3] = DataToSet.FieldRep[0][3];
    this->CalcedData.FieldRep[0][4] = DataToSet.FieldRep[0][4];
    this->CalcedData.FieldRep[0][5] = DataToSet.FieldRep[0][5];
    this->CalcedData.FieldRep[1][0] = DataToSet.FieldRep[1][0];
    this->CalcedData.FieldRep[1][1] = DataToSet.FieldRep[1][1];
    this->CalcedData.FieldRep[1][2] = DataToSet.FieldRep[1][2];
    this->CalcedData.FieldRep[1][3] = DataToSet.FieldRep[1][3];
    this->CalcedData.FieldRep[1][4] = DataToSet.FieldRep[1][4];
    this->CalcedData.FieldRep[1][5] = DataToSet.FieldRep[1][5];
}

void BaseScaff::SetComponents(const BaseComponents<int> ComponentsToSet){
    Material = ComponentsToSet;
}

