#ifndef UTILITIES_HXX
#define UTILITIES_HXX

#include <string>
#include <vector>
#include <fstream>

#include "BaseScaff.hxx"
#include "Logger.hxx"

namespace Scaff{

    /*
        The first 4 functions are extensions to the BaseScaff 
        class to provide some basic functionilty
    */
    //Writes the first and second argument to a file with the name of the 
    //third argument. s
    int WriteCalcedMaterial(const BaseComponents<int>& FilledScaff, const Dimensions& CalcedData,std::string ConstructionSite);

    //Like the first function but this time its reads in 
    //the data from the third argument.
    int ReadCalcedMaterial(BaseComponents<int>& FilledScaff,Dimensions& CalcedData,std::string& ConstructionSite);

    
    void PrintListOfMaterial(std::ostream* WhereToPrint, const BaseComponents<int>& FilledScaff,const Dimensions& CalcedData);

    //These two functions always goes together. WisualizeOne calls 
    //VisualizeTwo and just prints a primitive scaffold on the Screen 
    //based on the data in DataToPrint.
    void VisualizeOne(std::ostream* WhereToPrint, const Dimensions& DataToPrint);
    void VisualizeSecond(std::ostream* WhereToPrint, const Dimensions& DataToPrint);

    //Just the output of all relevant data that we calculated
    void PrintKeyData(std::ostream* WhereToPrint,const Scaff::Dimensions& DataToPrint);

    void PrintFieldDivision(const Scaff::Dimensions& DataToPrint);
    //void PrintUpdatedFieldDivision(const Scaff::Dimensions& DataToPrint);

    void ChangeFieldDivision(Scaff::Dimensions DataToPrintScaff, BaseScaff& ScaffToChange,int SubOrAddOrSwap);

    //void UpdateMaterialList(FieldLength FieldToSub,FieldLength FieldToAdd, int Floors);

    //Simple function to delete AND overwrite the complete
    //content of a file
    int EraseFile(const char *FileName);

    //Before we write the data to a file named by a user given input,
    //we remove all special characters and newlines to avoid errors
    void RemoveSpecialCharacter(std::string& InputString);

    //Add a filename to our "Database"(just a txt file) 
    int AddDBEntry(const std::string& NameOfFile);

    //Delete a filename from our Database
    int DeleteDBEntry(const std::string& NameOfFile);

    //Read in all filenames(construction sites) and give them back
    std::vector<std::string> GetAllDBEntrys();

    //Write all current saved construction sites to stdout 
    inline void ShowDB(const std::vector<std::string>& ListOfFiles);

 

}
#endif //UTILITIES_HXX