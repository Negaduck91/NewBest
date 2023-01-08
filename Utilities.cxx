#include "Utilities.hxx"
//#include "Logger.hxx"
#include "BaseScaff.hxx"
#include <algorithm>


void Scaff::PrintKeyData(std::ostream* WhereToPrint,const Dimensions& DataToPrint){
    *WhereToPrint<<"Eingaben: "<< std::endl;
    *WhereToPrint<<"\nEckdaten:\n";
    *WhereToPrint<<"\nLange Felder: "<< DataToPrint.LongFieldsL <<
    "\t\tGewicht: "<< DataToPrint.Weight <<
    "t\nKurze Felder: "<< DataToPrint.ShortFieldsL <<
    "\t\tStiellast: ca. " << DataToPrint.MaxStalkLoad <<
    "Kg\nGesamlaenge: "<< DataToPrint.CalcedLength
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
        Logfile::getInstance().write("Couldnt open DataBase to add new entry...\n");
        return 1;   //1 indicates cant open file 
    }
    DataBase << NameOfFile << std::endl;
    DataBase.close();
    return 0;
}

int Scaff::DeleteDBEntry(const std::string& NameOfFile){
    std::vector<std::string> AllConstructionSites= GetAllDBEntrys();
    //Lets go through our vector and lets see if we can find th given string
    for(auto& a : AllConstructionSites){
        if(a == NameOfFile){
            std::cout << "Found Construction Site" << std::endl;
            if(AllConstructionSites.erase(std::find(AllConstructionSites.begin(), 
                AllConstructionSites.end(),NameOfFile))==AllConstructionSites.end()){
                std::cout << "Could not delete the entry...\n";
                return 1;
            }
            else{
                std::cout <<"Entry successfull deleted\n";
                Logfile::getInstance().write("Entry successfull deleted\n");
                std::ofstream DataBase("Baustellen\\DB.txt",std::ios_base::trunc);
                if(DataBase.bad()){
                    Logfile::getInstance().write("Couldnt open DataBase to delete entry...\n");
                    return 2;
                }
                for(auto& a : AllConstructionSites)
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
        Logfile::getInstance().write("Couldnt open DataBase...\n");
        exit(1);
    }
    std::string content("");
    std::vector<std::string> ConstructionSites; 
    while(std::getline(DataBase,content)){
        ConstructionSites.push_back(content);
    }
    DataBase.close();
    return ConstructionSites;
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

int Scaff::ReadCalcedMaterial(BaseComponents<int>& FilledScaff,Dimensions& CalcedData,std::string& ConstructionSite){
	std::string PathToFile(".\\Baustellen\\");
	PathToFile += ConstructionSite;
    PathToFile += ".bin";
	std::ifstream FileToRead;
    FileToRead.open(PathToFile.c_str(),std::ios::binary);
	if(FileToRead.bad()){
        Logfile::getInstance().write("Couldt not open file to read Data\n");
        return 1;
	}
    FileToRead.seekg(0);
    FileToRead.read((char*)&FilledScaff,sizeof(BaseComponents<int>));
    FileToRead.read((char*)&CalcedData,sizeof(Dimensions));
	FileToRead.close();
    return 0;
}

int Scaff::WriteCalcedMaterial(const BaseComponents<int>& FilledScaff, const Dimensions& CalcedData,std::string ConstructionSite){
	std::string PathToFile(".\\Baustellen\\");
	ConstructionSite += ".bin";
    PathToFile += ConstructionSite;
    std::ofstream    FileToWrite;
    FileToWrite.open(PathToFile.c_str(), std::ios::binary);
    if(FileToWrite.good()){
        FileToWrite.write((char*)&FilledScaff,sizeof(BaseComponents<int>));
        FileToWrite.write((char*)&CalcedData,sizeof(Dimensions));
        FileToWrite.close();
        Logfile::getInstance().write("File successfully saved\n");        
    }else{
        Logfile::getInstance().write("Could not open the file to store Data\n");
        FileToWrite.close();
        exit(1);
    }
    return 0;
}

void Scaff::PrintListOfMaterial(std::ostream* WhereToPrint, const BaseComponents<int>& FilledScaff, const Dimensions& CalcedData){

    *WhereToPrint << "\t+-------------------------------+\n";
    *WhereToPrint << "\t|         Materialliste         |\n";
    *WhereToPrint << "\t+-------------------------------+\n";
    *WhereToPrint << "\t|   Fuesse        |\t"<< FilledScaff.BaseJack <<"\t|\n";
    *WhereToPrint << "\t+-------------------------------+\n";
    *WhereToPrint << "\t|   Unterlagen    |\t"<< FilledScaff.BaseJack <<"\t|\n";
    *WhereToPrint << "\t+-------------------------------+\n";
    *WhereToPrint << "\t|   Durchstiege   |\t"<< CalcedData.Floors <<"\t|\n";
    *WhereToPrint << "\t+-------------------------------+\n";
    *WhereToPrint << "\t|   Diagonalen    |\t"<< FilledScaff.Dia[0] <<"\t|\n";
    *WhereToPrint << "\t+-------------------------------+---------------+\n";
    *WhereToPrint << "\t|                 |\t73er\t|\t1.09m\t|\n";   
    *WhereToPrint << "\t+-----------------------------------------------+\n";
    if(CalcedData.FW == W09){
    *WhereToPrint << "\t| 66er  Rahmen    |\t0\t|\t"<<FilledScaff.Frames[0]<<"\t|"<<std::endl;
    *WhereToPrint << "\t+-----------------+-------------+---------------+\n";
    *WhereToPrint << "\t| 1.00m Rahmen    |\t0\t|\t"<<FilledScaff.Frames[1]<<"\t|"<<std::endl;
    *WhereToPrint << "\t++----------------+-------------+---------------+\n";
    *WhereToPrint << "\t| 1.50m Rahmen    |\t0\t|\t"<<FilledScaff.Frames[2]<<"\t|"<<std::endl;
    *WhereToPrint << "\t+-----------------+-------------+---------------+\n";
    *WhereToPrint << "\t| 2.00m Rahmen    |\t0\t|\t"<<FilledScaff.Frames[3]<<"\t|"<<std::endl;
    }else{
        *WhereToPrint << "\t| 66er  Rahmen    |\t"<<FilledScaff.Frames[0]<< '\t'<<"| \t0\t|"<<std::endl;
        *WhereToPrint << "\t+-----------------+-------------+---------------+\n";
        *WhereToPrint << "\t| 1.00m Rahmen    |\t"<<FilledScaff.Frames[1]<< '\t'<<"| \t0\t|"<<std::endl;
        *WhereToPrint << "\t++----------------+-------------+---------------+\n";
        *WhereToPrint << "\t| 1.50m Rahmen    |\t"<<FilledScaff.Frames[2]<< '\t'<<"| \t0\t|"<<std::endl;
        *WhereToPrint << "\t+-----------------+-------------+---------------+\n";
        *WhereToPrint << "\t| 2.00m Rahmen    |\t"<<FilledScaff.Frames[3]<< '\t'<<"| \t0\t|"<<std::endl;
    }
    *WhereToPrint << "\t+---------------+---------------+---------------+---------------+---------------+\n";
    *WhereToPrint<<"\t|\t\t|\tAlu\t|\tStahl\t|\tLehnen\t|   B.Bretter\t|\n"
                << "\t+---------------+---------------+---------------+---------------+---------------+\n"
                <<"\t|\t3,07m\t|\t"<<FilledScaff.UsedPlanks.alu[5]<<"\t|\t"<<FilledScaff.UsedPlanks.steel[5]<<"\t|\t"<< FilledScaff.SideGuard[5] <<"\t|\t"<< FilledScaff.ToeBoard[5] <<"\t|\n"
                << "\t+---------------+---------------+---------------+---------------+---------------+\n"
                <<"\t|\t2,57m\t|\t"<<FilledScaff.UsedPlanks.alu[4]<<"\t|\t"<<FilledScaff.UsedPlanks.steel[4]<<"\t|\t"<< FilledScaff.SideGuard[4] <<"\t|\t"<< FilledScaff.ToeBoard[4] <<"\t|\n"
                << "\t+---------------+---------------+---------------+---------------+---------------+\n"
                <<"\t|\t2,07m\t|\t"<<FilledScaff.UsedPlanks.alu[3]<<"\t|\t"<<FilledScaff.UsedPlanks.steel[3]<<"\t|\t"<< FilledScaff.SideGuard[3] <<"\t|\t"<< FilledScaff.ToeBoard[3] <<"\t|\n"
                << "\t+---------------+---------------+---------------+---------------+---------------+\n"
                <<"\t|\t1,57m\t|\t"<<FilledScaff.UsedPlanks.alu[2]<<"\t|\t"<<FilledScaff.UsedPlanks.steel[2]<<"\t|\t"<<FilledScaff.SideGuard[2]<<"\t|\t"<< FilledScaff.ToeBoard[2] <<"\t|\t\n"
                << "\t+---------------+---------------+---------------+---------------+---------------+\n"
                <<"\t|\t1,09m\t|\t"<<FilledScaff.UsedPlanks.alu[1]<<"\t|\t"<<FilledScaff.UsedPlanks.steel[1]<<"\t|\t"<< FilledScaff.SideGuard[1]<<"\t|\t"<< FilledScaff.ToeBoard[1] <<"\t|\t\n"
                << "\t+---------------+---------------+---------------+---------------+---------------+\n"
                <<"\t|\t0,73m\t|\t"<<FilledScaff.UsedPlanks.alu[0]<<"\t|\t"<<FilledScaff.UsedPlanks.steel[0]<<"\t|\t"<< FilledScaff.SideGuard[0]<<"\t|\t"<< FilledScaff.ToeBoard[0] <<"\t|\t\n"
                << "\t+---------------+---------------+---------------+---------------+---------------+\n";
    //Scaff::VisualizeOne(WhereToPrint,CalcedData);
    
}

void Scaff::ChangeFieldDivision(Dimensions DataToPrintScaff, Scaff::BaseScaff& ScaffToChange,int SubOrAddOrSwap){
    int count =0;
    //Lets count how manys field are set 
    //It seems like there is a a bug when when 
    //we have calculated just one field
    //There is no Field output 
    for(int i=0;i<6;i++){
        if((ScaffToChange.GetDimensions()).FL[i] != three_h)
            count++;
    }
    if(SubOrAddOrSwap ==1){         //Want to add a field
        std::cout << "Welches Feld wollen Sie hinzufuegen "<< std::endl;
        std::cout <<
                "[0]  0,73m Feld\n" <<
                "[1]  1,09m Feld\n" <<
                "[2]  1,57m Feld\n" <<
                "[3]  2,07m Feld\n" <<
                "[4]  2,57m Feld\n" <<
                "[5]  3,07m Feld\n" << std::endl;
        int NewField1=0;
        std::cin >> NewField1;
        //Convert our Input to enum to work with. So we can index our array accordingly
        enum Scaff::FieldLength e_NewField1=static_cast<FieldLength>(NewField1);
        DataToPrintScaff.FieldRep[1][e_NewField1]+=1;
        ScaffToChange.SetExtraField(e_NewField1);
        ScaffToChange.SetDimensions(DataToPrintScaff);
        ScaffToChange.AddField(e_NewField1, DataToPrintScaff.Floors);
        ScaffToChange.SetAllFieldsL(ScaffToChange.GetAllFieldsL()+1);
        return;

    }else if(SubOrAddOrSwap ==2){   //Want to sub a field
        std::cout << "Welches Feld wollen Sie wegnehmen "<< std::endl;
    
        enum Scaff::FieldLength GezeigteFelder[7],e_AltesFeld;
        int AltesFeld=0;
        /*
        First we check if we have more than one field calculated and give accordingly
        to that output.
        */
        if(count ==1){
            std::cout << "[0] ";
            switch((ScaffToChange.GetDimensions()).FL[1]){
                case three: std::cout << "3.07m Feld\n";GezeigteFelder[0]=three;break;
                case two_h: std::cout << "2.57m Feld\n";GezeigteFelder[0]=two_h;break;
                case two: std::cout << "2.07m Feld\n";GezeigteFelder[0]=two;break;
                case one_h: std::cout << "1.57m Feld\n";GezeigteFelder[0]=one_h;break;
                case one: std::cout << "1.09m Feld\n";GezeigteFelder[0]=one;break;
                case seventy: std::cout << "0.73m Feld\n";GezeigteFelder[0]=one;break;
                case three_h: std::cout << "We only have one Field\n"<< std::endl;break;
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
                    case three_h: std::cout << "We only have one Field\n"<< std::endl; break;
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
        DataToPrintScaff.FieldRep[1][e_AltesFeld]-=1;
        ScaffToChange.SetDimensions(DataToPrintScaff);
        ScaffToChange.SubField(e_AltesFeld, DataToPrintScaff.Floors);
        ScaffToChange.SetAllFieldsL(ScaffToChange.GetAllFieldsL()-1);
        return;
    }else if(SubOrAddOrSwap ==3){   //Want to swap fields

        std::cout << "Welches Feld wollen Sie tauschen "<< std::endl;
        enum Scaff::FieldLength GezeigteFelder[7],e_AltesFeld;
        int AltesFeld=0;
        /*
        First we check if we have more than one field calculated and give accordingly
        to that output.
        */
        if(count ==1){
            std::cout << "[0] ";
            switch((ScaffToChange.GetDimensions()).FL[1]){
                case three: std::cout << "3.07m Feld\n";GezeigteFelder[0]=three;break;
                case two_h: std::cout << "2.57m Feld\n";GezeigteFelder[0]=two_h;break;
                case two: std::cout << "2.07m Feld\n";GezeigteFelder[0]=two;break;
                case one_h: std::cout << "1.57m Feld\n";GezeigteFelder[0]=one_h;break;
                case one: std::cout << "1.09m Feld\n";GezeigteFelder[0]=one;break;
                case seventy: std::cout << "0.73m Feld\n";GezeigteFelder[0]=one;break;
                case three_h: std::cout << "We only have one FIeld\n"<< std::endl;break;
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
                    case three_h: std::cout << "We only have one FIeld\n"<< std::endl; break;
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
    


    std::cout << "Welches Feld wollen Sie hinzufuegen "<< std::endl;
    std::cout <<
                "[0]  0,73m Feld\n" <<
                "[1]  1,09m Feld\n" <<
                "[2]  1,57m Feld\n" <<
                "[3]  2,07m Feld\n" <<
                "[4]  2,57m Feld\n" <<
                "[5]  3,07m Feld\n" << std::endl;
    int NewField1=0;
    std::cin >> NewField1;
    //Convert our Input to enum to work with. So we can index our array accordingly
    enum Scaff::FieldLength e_NewField1=static_cast<FieldLength>(NewField1);
    if(e_NewField1 != e_AltesFeld) ScaffToChange.SetExtraField(e_NewField1);
    DataToPrintScaff.FieldRep[1][e_NewField1]+=1;
    DataToPrintScaff.FieldRep[1][e_AltesFeld]-=1;
    ScaffToChange.SetDimensions(DataToPrintScaff);
    ScaffToChange.SwapFields(e_AltesFeld,e_NewField1, DataToPrintScaff.Floors);

    }
    return;
}



void Scaff::PrintFieldDivision(const Scaff::Dimensions& DataToPrint){
    double TestLength=0;
    for(int i=0;i<6;i++){
        switch(DataToPrint.FL[i]){
        case three:for(int i =0; i<DataToPrint.FieldRep[1][three];i++){std::cout << "|307";TestLength+=3.07;}break;
        case two_h:for(int i =0; i<DataToPrint.FieldRep[1][two_h];i++){std::cout << "|257";TestLength+=2.57;}break;
        case two:for(int i =0; i<DataToPrint.FieldRep[1][two];i++){std::cout << "|207";TestLength+=2.07;}break;
        case one_h:for(int i =0; i<DataToPrint.FieldRep[1][one_h];i++){std::cout << "|157";TestLength+=1.57;}break;
        case one:for(int i =0; i<DataToPrint.FieldRep[1][one];i++){std::cout << "|109";TestLength+=1.09;}break;
        case seventy:for(int i =0; i<DataToPrint.FieldRep[1][seventy];i++){std::cout << "|073";TestLength+=0.73;}break;
        case three_h: break;
        }  
    }
    std::cout << "|\n"<< std::endl;
    std::cout << "Gesamtlaenge: " << TestLength << "m\n"<< std::endl;
    std::cout << "Gewuenschte Laenge: " << DataToPrint.WishedLength<<"m\n"<<std::endl;
}
 