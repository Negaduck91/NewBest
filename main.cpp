#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <time.h>


#include "Menus.hxx"            //Recoding 99% done
#include "BaseScaff.hxx"        //Recoding 99% done
//#include "Logger.hxx"           //Singleton pattern Logger Class to Log 
                                //every problem that occurs while executing 
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
    if(argc == 3 ){
        double L,H=0;
        std::vector<std::string> args;
        args.push_back(argv[1]);        //Store the program arguments in a way that we can
        args.push_back(argv[2]);        //conviently convert it in a type we accepted it to be 
        L = std::stod(args[0]);         //The actual conversion
        H = std::stod(args[1]);         //...
        Scaff::BaseScaff NewScaff(L,H);
        Scaff::PrintKeyData(&std::cout,NewScaff.GetDimensions());
        Scaff::VisualizeOne(&std::cout,NewScaff.GetDimensions());
        Scaff::PrintFieldDivision(NewScaff.GetDimensions());
        std::string CS("");
        int choice, choice2=0;
        do{
            choice =SubMenu();
            switch(choice){
			    case 1: 
                
                    choice2 = SubMenuChangeFieldDivision();
                    switch(choice2){
                        case 1:
                            Scaff::ChangeFieldDivision(NewScaff.GetDimensions(),NewScaff,1);
					        Scaff::PrintKeyData(&std::cout,NewScaff.GetDimensions());
					        Scaff::VisualizeOne(&std::cout,NewScaff.GetDimensions());
					        Scaff::PrintFieldDivision(NewScaff.GetDimensions());
					        break;
                        case 2:
                            Scaff::ChangeFieldDivision(NewScaff.GetDimensions(),NewScaff,2);
					        Scaff::PrintKeyData(&std::cout,NewScaff.GetDimensions());
					        Scaff::VisualizeOne(&std::cout,NewScaff.GetDimensions());
					        Scaff::PrintFieldDivision(NewScaff.GetDimensions());
					        break;
                        case 3:
                            Scaff::ChangeFieldDivision(NewScaff.GetDimensions(),NewScaff,3);
					        Scaff::PrintKeyData(&std::cout,NewScaff.GetDimensions());
					        Scaff::VisualizeOne(&std::cout,NewScaff.GetDimensions());
					        Scaff::PrintFieldDivision(NewScaff.GetDimensions());
					        break;
                        case 4:
                            exit(0);
                    }
                
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
    //switch(MainMenu()) {
		//Normal scaffold without extensions
    //case 1:
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
                std::cerr << "Datei konnte nicht gel"<<oe<<"scht werden" << std::endl;
            }
            break;
        }
        //break;

        //Here the User wants an Allround Scaffold
    //case 2: //new_modul();
            //break;

    //}
    return 0;
}