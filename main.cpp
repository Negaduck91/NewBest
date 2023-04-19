#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <time.h>


#include "Menus.hxx"            //Recoding 99% done
#include "BaseScaff.hxx"        //Recoding 99% done 
#include "Utilities.hxx"

//
//TODO
//
//Implement the different calc functions
//for the classes, test if every class
//calculates the wright numbers and look
//when everything is putting together
//if it works like the old program plus the
//added flexibility
//

int main(int argc, char* argv[]){
    //If the user inputs two doubles we assume 
    //the first is the length of the Scaffold
    //and the second value is the height 
    if(argc == 4 ){
        double L,H=0;
        signed int WishedBaseField=0;
        std::vector<std::string> args;
        args.push_back(argv[1]);        //Store the program arguments in a way that we can
        args.push_back(argv[2]);        //conviently convert it in a type we accepted it to be 
        args.push_back(argv[3]);
        L = std::stod(args[0]);         //The actual conversion
        H = std::stod(args[1]);         //...
        WishedBaseField = std::stoi(args[2]);
        Scaff::FieldIndicator Wish = static_cast<Scaff::FieldIndicator>(WishedBaseField);
        Scaff::BaseScaff NewScaff(L,H,Wish);
        Scaff::PrintKeyData(&std::cout,NewScaff.GetDimensions());
        Scaff::VisualizeOne(&std::cout,NewScaff.GetDimensions());
        Scaff::PrintFieldDivision(NewScaff.GetDimensions());
        std::string CS("");
        int choice=0;
        do{
            choice =SubMenu();
            switch(choice){
			    case 1: 
                    switch(SubMenuChangeFieldDivision()){
                        case 1:
                            Scaff::ChangeFieldDivision(NewScaff.GetDimensions(),NewScaff,1);
					        break;
                        case 2:
                            Scaff::ChangeFieldDivision(NewScaff.GetDimensions(),NewScaff,2);
					        break;
                        case 3:
                            Scaff::ChangeFieldDivision(NewScaff.GetDimensions(),NewScaff,3);
					        break;
                        case 4:
                            exit(0);
                    }
                    Scaff::PrintKeyData(&std::cout,NewScaff.GetDimensions());
			        Scaff::VisualizeOne(&std::cout,NewScaff.GetDimensions());
				    Scaff::PrintFieldDivision(NewScaff.GetDimensions());
                    break;
                
			    case 2: 
                    Scaff::PrintListOfMaterial(&std::cout,NewScaff.GetComponents(),
                                            NewScaff.GetDimensions());
                    break;
			    case 3: 
                    CS = InputConstructionSiteName();
					WriteCalcedMaterial(NewScaff.GetComponents(), NewScaff.GetDimensions(),CS);
					Scaff::AddDBEntry(CS);	
                    break;
			    case 4: exit(0);
		    }
        }while(choice !=4);
    exit(0);
    }


	Scaff::UserInput Input;
        switch(BaseScaffMenu()) {
            //CONSTRUCTION
			//Then get some User Data required to calculate the scaff
            case 1:
                GetMontageInput(Input);
			    NewBaseScaff(Input);
			    break;

			//DISMANTLING
            //Do we want to delete the File from our 
            //Data base or do we just want to look it up 
            //for some reason??
            case 2:
                if(GetDismantlingInput()==1){
                    std::cerr << "Datei konnte nicht geloescht werden" << std::endl;

                }
            break;
        }
    return 0;
}