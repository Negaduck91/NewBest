#include "lib/Utilities.hxx"
//#include "Logger.hxx"
#include "lib/FacadeScaff.hxx"
#include <algorithm>


void Scaff::PrintKeyData(std::ostream* WhereToPrint,const Dimensions& DataToPrint){
    *WhereToPrint<<"Eingaben: "<< std::endl;
    *WhereToPrint<<"\nEckdaten:\n";
    *WhereToPrint<<"\nLange Felder: "<< DataToPrint.LongFieldsL <<
    "\t\tGewicht: "<< DataToPrint.Weight <<
    "t\nKurze Felder: "<< DataToPrint.ShortFieldsL <<
    "\t\tStiellast: ca. " << DataToPrint.MaxStalkLoad <<
    "Kg\nGesamlaenge: "<< DataToPrint.Calcedlength_
    <<"\nLagen : "<< DataToPrint.Floors << std::endl << std::endl;
}

int Scaff::EraseFile(const char *FileName) {
   unsigned long size;
   FILE *Remove=fopen(FileName, "w+b");
   if(Remove != NULL)  {
      fseek(Remove, 0L, SEEK_END);
      size=ftell(Remove);
      fwrite((char *)'\1', 1, size, Remove);
      fclose(Remove);
      remove(FileName);
	  return 0;
   }
   return 1;
}

void Scaff::RemoveSpecialCharacter(std::string& InputString){
    int j = 0;
    for (unsigned int i = 0; i < InputString.size(); i++) {
         
        // Store only valid characters
        if ((InputString[i] >= 'A' && InputString[i] <= 'Z') ||
            (InputString[i] >='a' && InputString[i] <= 'z') ||
            //Or numbers 
            (InputString[i] >='0' && InputString[i] <= '9')){
            InputString[j] = InputString[i];
            j++;
        }
    }
    //cout << InputString.substr(0, j);
}

int Scaff::AddDBEntry(const std::string& NameOfFile){
    std::ofstream DataBase("Baustellen\\DB.txt", std::ios_base::app);
    if(DataBase.bad()){
        //Logfile::getInstance().write("Couldnt open DataBase to add new entry...\n");
        return 1;   //1 indicates cant open file 
    }
    DataBase << NameOfFile << std::endl;
    DataBase.close();
    return 0;
}

int Scaff::DeleteDBEntry(const std::string& NameOfFile){
    std::vector<std::string> Allconstruction_site_s= GetAllDBEntrys();
    //Lets go through our vector and lets see if we can find th given string
    for(auto& a : Allconstruction_site_s){
        if(a == NameOfFile){
            std::cout << "Found Construction Site" << std::endl;
            if(Allconstruction_site_s.erase(std::find(Allconstruction_site_s.begin(), 
                Allconstruction_site_s.end(),NameOfFile))==Allconstruction_site_s.end()){
                std::cout << "Could not delete the entry...\n";
                return 1;
            }
            else{
                std::cout <<"Entry successfull deleted\n";
                //Logfile::getInstance().write("Entry successfull deleted\n");
                std::ofstream DataBase("Baustellen\\DB.txt",std::ios_base::trunc);
                if(DataBase.bad()){
                    //Logfile::getInstance().write("Couldnt open DataBase to delete entry...\n");
                    return 2;
                }
                for(auto& a : Allconstruction_site_s)
                    DataBase << a << std::endl;
                break;
            }
        } 
    }
    return 2;
}

std::vector<std::string> Scaff::GetAllDBEntrys(){
    std::ifstream DataBase("Baustellen\\DB.txt");
    if(DataBase.bad()){
        std::cerr << "Couldnt open DataBase...\n" << std::endl;
        //Logfile::getInstance().write("Couldnt open DataBase...\n");
        exit(1);
    }
    std::string content("");
    std::vector<std::string> construction_site_s; 
    while(std::getline(DataBase,content)){
        construction_site_s.push_back(content);
    }
    DataBase.close();
    return construction_site_s;
}

void Scaff::VisualizeOne(std::ostream* WhereToPrint,const Dimensions& DataToPrint){
    for(int i=1; i<=DataToPrint.AllFieldsL; i++) {
        *WhereToPrint<< " ___";
        if(i == DataToPrint.AllFieldsL)
            *WhereToPrint<< "\n";
    }
    VisualizeSecond(WhereToPrint, DataToPrint);
}

void Scaff::VisualizeSecond(std::ostream* WhereToPrint,const Dimensions& DataToPrint){
    for(int k=1; k<=DataToPrint.Floors; k++)
        for(int j=1; j<=DataToPrint.AllFieldsL; j++) {
            *WhereToPrint<< "|___";
            if(j==DataToPrint.AllFieldsL)
                *WhereToPrint<< "|\n";
        }
    *WhereToPrint<< "\n";
}

int Scaff::ReadCalcednum_of_comps_(const BaseComponents<int>& FilledScaff, const Dimensions& calced_dims_,std::string& construction_site_){
	std::string PathToFile(".\\Baustellen\\");
	PathToFile += construction_site_;
    PathToFile += ".bin";
	std::ifstream FileToRead;
    FileToRead.open(PathToFile.c_str());
	if(FileToRead.bad()){
        //Logfile::getInstance().write("Couldt not open file to read Data\n");
        return 1;
	}
    FileToRead.seekg(0);
    FileToRead.read((char*)&FilledScaff,sizeof(BaseComponents<int>));
    FileToRead.read((char*)&calced_dims_,sizeof(Dimensions));
	FileToRead.close();
    return 0;
}

int Scaff::WriteCalcednum_of_comps_(const BaseComponents<int>& FilledScaff, const Dimensions& calced_dims_,std::string construction_site_){
	std::string PathToFile(".\\Baustellen\\");
	construction_site_ += ".bin";
    PathToFile += construction_site_;
    std::ofstream    FileToWrite;
    FileToWrite.open(PathToFile.c_str(), std::ios::binary);
    if(FileToWrite.good()){
        FileToWrite.write((char*)&FilledScaff,sizeof(BaseComponents<int>));
        FileToWrite.write((char*)&calced_dims_,sizeof(Dimensions));
        FileToWrite.close();
        //Logfile::getInstance().write("File successfully saved\n");        
    }else{
        //Logfile::getInstance().write("Could not open the file to store Data\n");
        FileToWrite.close();
        exit(1);
    }
    return 0;
}

void Scaff::PrintListOfnum_of_comps_(std::ostream* WhereToPrint, const BaseComponents<int>& FilledScaff, const Dimensions& calced_dims_){

    *WhereToPrint << "\t+-------------------------------+\n";
    *WhereToPrint << "\t|         num_of_comps_liste         |\n";
    *WhereToPrint << "\t+-------------------------------+\n";
    *WhereToPrint << "\t|   Fuesse        |\t"<< FilledScaff.BaseJack <<"\t|\n";
    *WhereToPrint << "\t+-------------------------------+\n";
    *WhereToPrint << "\t|   Unterlagen    |\t"<< FilledScaff.BaseJack <<"\t|\n";
    *WhereToPrint << "\t+-------------------------------+\n";
    *WhereToPrint << "\t|   Durchstiege   |\t"<< calced_dims_.Floors <<"\t|\n";
    *WhereToPrint << "\t+-------------------------------+\n";
    *WhereToPrint << "\t|   diagonalen    |\t"<< FilledScaff.dia[0] <<"\t|\n";
    *WhereToPrint << "\t+-------------------------------+---------------+\n";
    *WhereToPrint << "\t|                 |\t73er\t|\t1.09m\t|\n";   
    *WhereToPrint << "\t+-----------------------------------------------+\n";
    if(calced_dims_.FW == W09){
    *WhereToPrint << "\t| 66er  Rahmen    |\t0\t|\t"<<FilledScaff.frames[0]<<"\t|"<<std::endl;
    *WhereToPrint << "\t+-----------------+-------------+---------------+\n";
    *WhereToPrint << "\t| 1.00m Rahmen    |\t0\t|\t"<<FilledScaff.frames[1]<<"\t|"<<std::endl;
    *WhereToPrint << "\t++----------------+-------------+---------------+\n";
    *WhereToPrint << "\t| 1.50m Rahmen    |\t0\t|\t"<<FilledScaff.frames[2]<<"\t|"<<std::endl;
    *WhereToPrint << "\t+-----------------+-------------+---------------+\n";
    *WhereToPrint << "\t| 2.00m Rahmen    |\t0\t|\t"<<FilledScaff.frames[3]<<"\t|"<<std::endl;
    }else{
        *WhereToPrint << "\t| 66er  Rahmen    |\t"<<FilledScaff.frames[0]<< '\t'<<"| \t0\t|"<<std::endl;
        *WhereToPrint << "\t+-----------------+-------------+---------------+\n";
        *WhereToPrint << "\t| 1.00m Rahmen    |\t"<<FilledScaff.frames[1]<< '\t'<<"| \t0\t|"<<std::endl;
        *WhereToPrint << "\t++----------------+-------------+---------------+\n";
        *WhereToPrint << "\t| 1.50m Rahmen    |\t"<<FilledScaff.frames[2]<< '\t'<<"| \t0\t|"<<std::endl;
        *WhereToPrint << "\t+-----------------+-------------+---------------+\n";
        *WhereToPrint << "\t| 2.00m Rahmen    |\t"<<FilledScaff.frames[3]<< '\t'<<"| \t0\t|"<<std::endl;
    }
    *WhereToPrint << "\t+---------------+---------------+---------------+---------------+---------------+\n";
    *WhereToPrint<<"\t|\t\t|\talu\t|\tStahl\t|\tLehnen\t|   B.Bretter\t|\n"
                << "\t+---------------+---------------+---------------+---------------+---------------+\n"
                <<"\t|\t3,07m\t|\t"<<FilledScaff.UsedPlanks.alu[5]<<"\t|\t"<<FilledScaff.UsedPlanks.steel[5]<<"\t|\t"<< FilledScaff.side_guard[5] <<"\t|\t"<< FilledScaff.ToeBoard[5] <<"\t|\n"
                << "\t+---------------+---------------+---------------+---------------+---------------+\n"
                <<"\t|\t2,57m\t|\t"<<FilledScaff.UsedPlanks.alu[4]<<"\t|\t"<<FilledScaff.UsedPlanks.steel[4]<<"\t|\t"<< FilledScaff.side_guard[4] <<"\t|\t"<< FilledScaff.ToeBoard[4] <<"\t|\n"
                << "\t+---------------+---------------+---------------+---------------+---------------+\n"
                <<"\t|\t2,07m\t|\t"<<FilledScaff.UsedPlanks.alu[3]<<"\t|\t"<<FilledScaff.UsedPlanks.steel[3]<<"\t|\t"<< FilledScaff.side_guard[3] <<"\t|\t"<< FilledScaff.ToeBoard[3] <<"\t|\n"
                << "\t+---------------+---------------+---------------+---------------+---------------+\n"
                <<"\t|\t1,57m\t|\t"<<FilledScaff.UsedPlanks.alu[2]<<"\t|\t"<<FilledScaff.UsedPlanks.steel[2]<<"\t|\t"<<FilledScaff.side_guard[2]<<"\t|\t"<< FilledScaff.ToeBoard[2] <<"\t|\t\n"
                << "\t+---------------+---------------+---------------+---------------+---------------+\n"
                <<"\t|\t1,09m\t|\t"<<FilledScaff.UsedPlanks.alu[1]<<"\t|\t"<<FilledScaff.UsedPlanks.steel[1]<<"\t|\t"<< FilledScaff.side_guard[1]<<"\t|\t"<< FilledScaff.ToeBoard[1] <<"\t|\t\n"
                << "\t+---------------+---------------+---------------+---------------+---------------+\n"
                <<"\t|\t0,73m\t|\t"<<FilledScaff.UsedPlanks.alu[0]<<"\t|\t"<<FilledScaff.UsedPlanks.steel[0]<<"\t|\t"<< FilledScaff.side_guard[0]<<"\t|\t"<< FilledScaff.ToeBoard[0] <<"\t|\t\n"
                << "\t+---------------+---------------+---------------+---------------+---------------+\n";
    //Scaff::VisualizeOne(WhereToPrint,calced_dims_);
    
}

//TODO
//Need to get the User interaction out 
//of that function so that it parameters probably gets
//changed but then its independent  
//There needs to be an function in the menus section that 
//asks the questins and thern calls the member function that
//actually changes the fields
void Scaff::ChangeFieldDivision(Dimensions DataToPrintScaff, Scaff::FacadeScaff& ScaffToChange){
    int count =0;
    //Lets count how manys field are set 
    //It seems like there is a a bug when when 
    //we have calculated just one field
    //There is no Field output 
    for(int i=0;i<6;i++){
        if((ScaffToChange.GetDimensions()).FL[i] != zero)
            count++;
    }
    std::cout << "Welches Feld soll ersetzt werden: "<< std::endl;
    enum Scaff::FieldLength GezeigteFelder[count],e_AltesFeld;
    int AltesFeld=0;
    if(count ==1){
        std::cout << "[0] ";
        switch((ScaffToChange.GetDimensions()).FL[1]){
            case three: std::cout << "3.07m Feld\n";GezeigteFelder[0]=three;break;
            case two_h: std::cout << "2.57m Feld\n";GezeigteFelder[0]=two_h;break;
            case two: std::cout << "2.07m Feld\n";GezeigteFelder[0]=two;break;
            case one_h: std::cout << "1.57m Feld\n";GezeigteFelder[0]=one_h;break;
            case one: std::cout << "1.09m Feld\n";GezeigteFelder[0]=one;break;
            case seventy: std::cout << "0.73m Feld\n";GezeigteFelder[0]=one;break;
            case zero: std::cout << "We only have one FIeld\n"<< std::endl;break;
        }
    }else{
        for(int i=0;i<count;i++){
            std::cout << "["<<i<<"] ";
            switch((ScaffToChange.GetDimensions()).FL[i]){
                case three: std::cout << "3.07m Feld\n";GezeigteFelder[i]=three;break;
                case two_h: std::cout << "2.57m Feld\n";GezeigteFelder[i]=two_h;break;
                case two: std::cout << "2.07m Feld\n";GezeigteFelder[i]=two;break;
                case one_h: std::cout << "1.57m Feld\n";GezeigteFelder[i]=one_h;break;
                case one: std::cout << "1.09m Feld\n";GezeigteFelder[i]=one;break;
                case seventy: std::cout << "0.73m Feld\n";GezeigteFelder[i]=one;break;
                case zero: std::cout << "We only have one FIeld\n"<< std::endl; break;
            }     
        }
    }
    std::cout << "Ihre Auswahl: ";
        std::cin >> AltesFeld;     
        switch(AltesFeld){
            case 0:
                e_AltesFeld = GezeigteFelder[0];
                break;
            case 1:
                e_AltesFeld = GezeigteFelder[1];
                break;
            case 2:
                e_AltesFeld = GezeigteFelder[2];
                break;
            case 3:
                e_AltesFeld = GezeigteFelder[3];
                break;
            case 4:
                e_AltesFeld = GezeigteFelder[4];
                break;
            case 5:
                e_AltesFeld = GezeigteFelder[5];
                break;

        }
        /*
        std::cout <<"Feld das getauscht werden soll "<< e_AltesFeld<< std::endl;
        std::cout << "Welches Feld soll ersetzt werden: "<< std::endl;
        std::cout << "[1] ";*/
    
    /*
        We have an array of possible fields(ChosenFields[2]).
        We use this switch command to determine which fields 
        the algorithm has calculated, store them in our array 
        so that we can interprete the input of the user into 
        a specific field length_ which happens int he if else 
        statement later(r. 265-266);
        */
    /*switch((ScaffToChange.GetDimensions()).FL[0]){
        case three: std::cout << "3.07m Feld\n";ChosenFields[0]=three;break;
        case two_h: std::cout << "2.57m Feld\n";ChosenFields[0]=two_h;break;
        case two: std::cout << "2.07m Feld\n";ChosenFields[0]=two;break;
        case one_h: std::cout << "1.57m Feld\n";ChosenFields[0]=one_h;break;
        case one: std::cout << "1.09m Feld\n";ChosenFields[0]=one;break;
    }
    std::cout << "[2] ";
     switch((ScaffToChange.GetDimensions()).FL[1]){
        case three: std::cout << "3.07m Feld\n";ChosenFields[1]=three;break;
        case two_h: std::cout << "2.57m Feld\n";ChosenFields[1]=two_h;break;
        //case two: std::cout << "2.07m Feld\n";break;
        //case one_h: std::cout << "1.57m Feld\n";break;
        //case one: std::cout << "1.09m Feld\n";break;
     }
    std::cout << "Ihre Auswahl: ";                                              
    int OldField,NewField=0;                                                    
    std::cin >> OldField;                                                       
    if(OldField==1)e_OldField=ChosenFields[0];
    else e_OldField=ChosenFields[1];
    std::cout <<"Feld das getauscht werden soll "<< e_OldField<< std::endl;
    */


    int NewField=0;
    std::cout << "Welches Feld soll dazu kommen: "<< std::endl;
    std::cout <<"[0]  0,73m Feld\n" <<
                "[1]  1,09m Feld\n" <<
                "[2]  1,57m Feld\n" <<
                "[3]  2,07m Feld\n" <<
                "[4]  2,57m Feld\n" <<
                "[5]  3,07m Feld\n" << std::endl;
    std::cin >> NewField;
    enum Scaff::FieldLength e_NewField=static_cast<FieldLength>(NewField);
    if(e_NewField != e_AltesFeld) ScaffToChange.SetExtraField(e_NewField);
    DataToPrintScaff.FieldRep[1][e_AltesFeld]-=1;
    DataToPrintScaff.FieldRep[1][e_NewField]+=1;
    
    ScaffToChange.SetDimensions(DataToPrintScaff);
    ScaffToChange.SetUpdatetComponents(e_AltesFeld,e_NewField,DataToPrintScaff.Floors);
}

void Scaff::PrintFieldDivision(const Scaff::Dimensions& DataToPrint){
    double Testlength_=0;
    for(int i=0;i<6;i++){
        switch(DataToPrint.FL[i]){
        case three:for(int i =0; i<DataToPrint.FieldRep[1][three];i++){std::cout << "|307";Testlength_+=3.07;}break;
        case two_h:for(int i =0; i<DataToPrint.FieldRep[1][two_h];i++){std::cout << "|257";Testlength_+=2.57;}break;
        case two:for(int i =0; i<DataToPrint.FieldRep[1][two];i++){std::cout << "|207";Testlength_+=2.07;}break;
        case one_h:for(int i =0; i<DataToPrint.FieldRep[1][one_h];i++){std::cout << "|157";Testlength_+=1.57;}break;
        case one:for(int i =0; i<DataToPrint.FieldRep[1][one];i++){std::cout << "|109";Testlength_+=1.09;}break;
        case seventy:for(int i =0; i<DataToPrint.FieldRep[1][seventy];i++){std::cout << "|073";Testlength_+=0.73;}break;
        case zero: break;
        }  
    }
    std::cout << "|\n"<< std::endl;
    std::cout << "Gesamtlaenge: " << Testlength_ << "m\n"<< std::endl;
    std::cout << "Gewuenschte Laenge: " << DataToPrint.Wishedlength_<<"m\n"<<std::endl;
}
 