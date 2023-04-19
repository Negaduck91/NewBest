#include <iostream>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>


#include "Menus.hxx"
#include "umlaute.h"
#include "BaseScaff.hxx"
#include "Utilities.hxx"



/*
	We try to seperate the whole user interaction
	in this menu part ore in some simple Input functions 
	for further processing
*/

using namespace Scaff;

int WantToSafe(){
	std::cout << "Wollen Sie die Daten speichern?" << std::endl;
	std::cout << "[J]a" << std::endl;
	std::cout << "[N]ein" << std::endl;
	std::string temp;
	std::getline(std::cin, temp);
	temp.erase(remove(temp.begin(),temp.end(),' '),temp.end());
	char answer=lexical_cast<char,std::string>(temp);
	if(answer == 'J' || answer =='j') return 0;
	return 1;
		
}

/*
	Workaround for the exception that get thrown
	is that we first check if the string contains a 
	newline and if yes. we delete it.
*/
template<typename Target,typename Source>
Target lexical_cast(Source arg){
    std::stringstream interpreter;
    Target result;
    if(   !(interpreter << arg)
       || !(interpreter >> result)
       || !(interpreter >> std::ws).eof())
            return 0;
        return result;
}

void Banner();

int MainMenu(){
	int Choice=0;
	std::string Temp("");
	Banner();
	do{
		std::cout << "\t\n\n\t+---------------------+\n";
		std::cout << "\t|  [1] Fassadenger"<< ue << "st |\n";
		std::cout << "\t|  [2] Modulger"<< ue << "st    |\n";
		std::cout << "\t|  [3] Beenden        |\n";
		std::cout << "\t+---------------------+\n";
		std::cout << "\n\n\tIhre Auswahl: ";
		std::getline(std::cin,Temp);
		if(Temp==""){
			std::cerr<<"Leere eingaben schliesen das Programm sofort\n";
			Logfile::getInstance().write("Leere eingaben schliesen das Programm sofort\n");	
			exit(0);
		}
		Temp.erase(remove(Temp.begin(),Temp.end(),' '),Temp.end());
		Choice = lexical_cast<int,std::string>(Temp);
	}while(Choice != 1 && Choice != 2);
	return Choice;
}

/*int FacadeScaffMenu(){
	int Choice=0;
	std::string Temp("");
	Banner();
	std::cout << "\n\n\t[1] Aufbau Variante 1 (30er Konsole nur Oberster Lage)\n";
	std::cout << "\t[2] Aufbau Variante 2 (30er Konsole in jeder Lage)\n\n";
	std::cout << "\t[2] Aufbau Variante 3 (70er Konsole nur Oberster Lage)\n\n";
	std::cout << "Ihre Auswahl: ";
	std::getline(std::cin,Temp);
	Temp.erase(remove(Temp.begin(),Temp.end(),' '),Temp.end());
	Choice = lexical_cast<int,std::string>(Temp);
	return Choice;
}*/

int BaseScaffMenu(){
	int Choice=0;
	std::string Temp("");
	Banner();
	do{
		std::cout << "\n\n\t+---------------+\n";
		std::cout << "\t| [1] Aufbau	|\n";
		std::cout << "\t| [2] Abbau 	|\n";
		std::cout << "\t+---------------+\n\n";
		std::cout << "\tIhre Auswahl: ";
		std::getline(std::cin,Temp);
		Temp.erase(remove(Temp.begin(),Temp.end(),' '),Temp.end());
		Choice = lexical_cast<int,std::string>(Temp);
	}while(Choice != 1 && Choice != 2);
	return Choice;
}

void Banner(){
	std::cout << "\t\t************  *****   *****   ***********    ******   *****" << std::endl;
    std::cout << "\t\t*   ********  *   *   *   *   *  .....  *    *    *  *   *" << std::endl;
	std::cout << "\t\t*   *         *   *   *   *   *  :...:  *    *    * *   *" <<std::endl;
    std::cout << "\t\t*   *         *   *****   *   *         *    *    **   *" <<std::endl;
	std::cout << "\t\t*   ****      *           *   *   *****      *    *   *" << std::endl;
	std::cout << "\t\t*   ****      *   *****   *   *  .....  *    *       *" << std::endl;
	std::cout << "\t\t*   *         *   *   *   *   *  :...:  *    *         *" <<std::endl;
	std::cout << "\t\t*   *         *   *   *   *   *         *    *     **   *"  <<std::endl;
    std::cout << "\t\t*****         *****   *****   ***********    ******  *****" <<std::endl;
}

void GetMontageInput(UserInput& ToFill){
	int FT=0;

	do{
		std::cout <<  "\n\tRahmenwahl \n\t<1> 70er Geruest\n"
					 "\t<2> 100er Geruest \n";
		std::cout << "\tIhre Wahl: ";
		std::string temp("");
		std::getline(std::cin,temp);
		if(temp=="")return;						//Lets see if we can catch an empty input with this
		FT = lexical_cast<int,std::string>(temp);
		if(FT == 1)ToFill.FrameType =1;
		else ToFill.FrameType = 2;
	}while(FT != 1 && FT != 2);
	/*std::cout << "\n\tBevorzugte Bohlen\n\t<1> Alubohlen\n"
					 "\t<2> Stahlbohlen\n";
	std::cout << "\tIhre Wahl: ";
	std::getline(std::cin,temp);
	ToFill.PlankChoice = lexical_cast<int,std::string>(temp);*/
	
	std::cout << "\tHoehe: ";
	std::string temp("");
	std::getline(std::cin,temp);
	if(temp==""){	//Lets see if we can catch an empty input with this
		std::cerr<<"Leere eingaben schliesen das Programm sofort\n";
		Logfile::getInstance().write("Programm aufgrung leerer eingabe geschlossen\n");	
		exit(1);
	}										
	if(temp.size()==1){
		std::cerr <<"Es muss eine Dezimalzahl eingegeben werden\n";
		exit(1);
	}
	temp.erase(remove(temp.begin(),temp.end(),' '),temp.end());
	temp.erase(remove(temp.begin(),temp.end(),'c'),temp.end());
	temp.erase(remove(temp.begin(),temp.end(),'m'),temp.end());
	temp.erase(remove(temp.begin(),temp.end(),'.'),temp.end());
	temp.erase(remove(temp.begin(),temp.end(),','),temp.end());


	ToFill.Height = lexical_cast<double,std::string>(temp)/100;
	
	std::cout << "\tLaenge: ";
	std::getline(std::cin,temp);
	if(temp==""){	//Lets see if we can catch an empty input with this
		std::cerr<<"Leere eingaben schliesen das Programm sofort\n";
		Logfile::getInstance().write("Programm aufgrung leerer eingabe geschlossen\n");	
		exit(1);
	}						
	if(temp.size()==1){
		std::cerr <<"Es muss eine Dezimalzahl eingegeben werden\n";
		exit(1);
	}
	temp.erase(remove(temp.begin(),temp.end(),' '),temp.end());
	temp.erase(remove(temp.begin(),temp.end(),'c'),temp.end());
	temp.erase(remove(temp.begin(),temp.end(),'m'),temp.end());
	temp.erase(remove(temp.begin(),temp.end(),'.'),temp.end());
	temp.erase(remove(temp.begin(),temp.end(),','),temp.end());
	temp.erase(remove(temp.begin(),temp.end(),' '),temp.end());
	ToFill.Length = lexical_cast<double,std::string>(temp)/100;
}


void NewBaseScaff(const UserInput& Input){
		//Create the object and calculate everything with the given data
		BaseScaff NewScaff(Input);
		std::string NCS("");
		//After the creation we first want to print the key that
		//the key that gets calculated, then a small visualization 
		//of the scaffold  
		Scaff::PrintKeyData(&std::cout,NewScaff.GetDimensions());
		Scaff::VisualizeOne(&std::cout,NewScaff.GetDimensions());
		Scaff::PrintFieldDivision(NewScaff.GetDimensions());
		do{
			switch(SubMenu()){

				case 1: 
					switch(SubMenuChangeFieldDivision()){
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
                        case 4 :
                            exit(0);
                    }
					break;
				case 2: 
					Scaff::PrintListOfMaterial(&std::cout,NewScaff.GetComponents(),NewScaff.GetDimensions());
					break;
				case 3: 
					NCS = InputConstructionSiteName();
					WriteCalcedMaterial(NewScaff.GetComponents(), NewScaff.GetDimensions(),NCS);
					AddDBEntry(NCS);	
					break;
				case 4: 
					exit(0);
			}
			
		}while(true);
}

std::string InputConstructionSiteName(void){
	std::cout << "\nGeben Sie den Baustellennamen ein:  " << std::endl;
	std::string temp("");
	
	std::getline(std::cin >> std::ws,temp);
	if(temp==""){	//Lets see if we can catch an empty input with this
		std::cerr<<"Leere eingaben schliesen das Programm sofort\n";
		Logfile::getInstance().write("Programm aufgrung leerer eingabe geschlossen\n");	
		exit(1);
	}					
	temp.erase(remove(temp.begin(),temp.end(),' '),temp.end());
	return temp;
}

int GetDismantlingInput(){
		BaseScaff Data;
		BaseComponents<int> ReadedComponents;
		Dimensions ReadedDimensions;
        std::string CS("");
		for(auto& v : GetAllDBEntrys())std::cout << v << std::endl;
		CS=InputConstructionSiteName();
        ReadCalcedMaterial(ReadedComponents,ReadedDimensions,CS);
		Data.SetDimensions(ReadedDimensions);
		Data.SetComponents(ReadedComponents);
		Scaff::PrintKeyData(&std::cout, Data.GetDimensions());
        Scaff::VisualizeOne(&std::cout,Data.GetDimensions());
		Scaff::PrintFieldDivision(Data.GetDimensions());
		Scaff::PrintListOfMaterial(&std::cout,Data.GetComponents(), Data.GetDimensions());
	    
		if(WantToSafe()==1){
            std::string del(".\\Baustellen\\");
			DeleteDBEntry(CS);
            del += CS;
            del += ".bin";
            if(EraseFile(del.c_str())){
				Logfile::getInstance().write("Couldnt delete File\n");
				return 1;
			}
			Logfile::getInstance().write("Deleting File complete.\n");

		}
		return 0;
}

int SubMenu(){
	int Choice=0;
	std::string Temp("");
	do{
		std::cout << "\t\n\n\t+----------------------------------------------------+\n";
		std::cout << "\t|  [1] Feldeinteilung aendern                        |\n";
		std::cout << "\t|  [2] Materialliste anzeigen                        |\n";
		std::cout << "\t|  [3] Materialliste und Feldeinteilung so speichern |\n";
		std::cout << "\t|  [4] Beenden                                       |\n";
		std::cout << "\t+----------------------------------------------------+\n";
		std::cout << "\n\n\tIhre Auswahl: ";
		
		//std::getline(std::cin,Temp);
		std::cin >> Choice;
		//std::cin.ignore('\n');
		//std::cin >> ws;
		//if(Temp=="")exit(0);
		//Temp.erase(remove(Temp.begin(),Temp.end(),' '),Temp.end());
		//Choice = lexical_cast<int,std::string>(Temp);
	}while(Choice < 1 && Choice > 4);
	return Choice;
}

int SubMenuChangeFieldDivision(){
	int Choice=0;
	std::cout << "\t\n\n\t+----------------------------------------------------+\n";
	std::cout << "\t|  [1] Feld hinzufuegen                              |\n";
	std::cout << "\t|  [2] Feld abziehen                                 |\n";
	std::cout << "\t|  [3] Feld tauschen                                 |\n";
	std::cout << "\t|  [4] Beenden                                       |\n";
	std::cout << "\t+----------------------------------------------------+\n";
	std::cout << "\n\n\tIhre Auswahl: ";
	std::cin >> Choice;
	return Choice;
}