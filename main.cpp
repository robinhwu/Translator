#include <iostream>
#include <fstream>
#include <bitset>
#include <map>
#include <algorithm>


using namespace std;

string translate(string input);
string delSpaces(string &str);

int currentAddress = 16;

map<string, string> mapOfComp = {
        {"0", "0101010"},
        {"1", "0111111"},
        {"-1", "0111010"},
        {"D", "0001100"},
        {"A", "0110000"},
        {"M", "1110000"},
        {"!D", "0001101"},
        {"!A", "0110001"},
        {"!M", "1110001"},
        {"-D", "0001111"},
        {"-A", "0110011"},
        {"-M", "0110011"},
        {"D+1", "0011111"},
        {"A+1", "0110111"},
        {"M+1", "1110111"},
        {"D-1", "0001110"},
        {"A-1", "0110010"},
        {"M-1", "1110010"},
        {"D+A", "0000010"},
        {"D+M", "1000010"},
        {"D-A", "0010011"},
        {"D-M", "1010011"},
        {"A-D", "0000111"},
        {"M-D", "1000111"},
        {"D&A", "0000000"},
        {"D&M", "1000000"},
        {"D|A", "0010101"},
        {"D|M", "1010101"},
};

map<string, string> mapOfDest = {
        {"","000"},
        {"M", "001"},
        {"D", "010"},
        {"MD", "011"},
        {"A", "100"},
        {"AM", "101"},
        {"AD", "110"},
        {"ADM", "111"},
};

map<string, string> mapOfJmp = {
        {"","000"},
        {"JGT", "001"},
        {"JEQ", "010"},
        {"JGE", "011"},
        {"JLT", "100"},
        {"JNE", "101"},
        {"JLE", "110"},
        {"JMP", "111"},
};

map<string, string> mapOfSymbol = {
        {"R0", "0000000000000000"},
        {"R1", "0000000000000001"},
        {"R2", "0000000000000010"},
        {"R3", "0000000000000011"},
        {"R4", "0000000000000100"},
        {"R5", "0000000000000101"},
        {"R6", "0000000000000110"},
        {"R7", "0000000000000111"},
        {"R8", "0000000000001000"},
        {"R9", "0000000000001001"},
        {"R10", "0000000000001010"},
        {"R11", "0000000000001011"},
        {"R12", "0000000000001100"},
        {"R13", "0000000000001101"},
        {"R14", "0000000000001110"},
        {"R15", "0000000000001111"},
        {"SCREEN", "0100000000000000"},
        {"KEYBOARD", "0110000000000000"},
        {"SP", "0000000000000000"},
        {"LCL", "0000000000000001"},
        {"ARG", "0000000000000010"},
        {"THIS", "0000000000000011"},
        {"THAT", "0000000000000100"}
};

int main() {

    string line;

    // Creation of ifstream class object to read the file
    ifstream fin;

    // by default open mode = ios::in mode
//    fin.open("Add.asm");
//    fin.open("MaxL.asm");
//    fin.open("PongL.asm");
//    fin.open("RectL.asm");
//    fin.open("Max.asm");
    fin.open("Pong.asm");
//    fin.open("Rect.asm");

    // Creation of ofstream class object
    ofstream fout;

    // by default ios::out mode, automatically deletes
    // the content of file. To append the content, open in ios:app
    // fout.open("sample.txt", ios::app)
//    fout.open("Add.hack");
//    fout.open("MaxL.hack");
//    fout.open("PongL.hack");
//    fout.open("RectL.hack");
//    fout.open("Max.hack");
    fout.open("Pong.hack");
//    fout.open("Rect.hack");

    int pc = 0;

    while(fin) {
        // Read a Line from File
        getline(fin, line);

        if (line[0] == '(') {
            string label = line.substr(1, line.size()-2);
//            cout << label << "***" << bitset<16>(pc).to_string()<< "***" << pc << endl;
            mapOfSymbol[label] = bitset<16>(pc).to_string();
        } else if (line[0] != '/' && !line.empty()){
            pc++;
        }
    }

    fin.close();

//    fin.open("Add.asm");
//    fin.open("MaxL.asm");
//    fin.open("PongL.asm");
//    fin.open("RectL.asm");
//    fin.open("Max.asm");
    fin.open("Pong.asm");
//    fin.open("Rect.asm");

    // Execute a loop until EOF (End of File)
    while (fin) {

        // Read a Line from File
        getline(fin, line);

        line = delSpaces(line);

        if (!line.empty() && line[0] != '/') {

            if (line[0] != '(') {
                string res = translate(line);
                // Write line in file
                fout << res << endl;
            }
        }

    }

    // Close the file
    fin.close();

    fout.close();

    return 0;
}

string translate(string input) {

    string res;

    int equalIndex = -1, semicolonIndex = -1, slashIndex = -1;
    int size = input.size();

    for (int i = 0; i < size; i++) {
        if (input[i] == '/') {
            slashIndex = i;
        }
    }

    if (slashIndex != -1) {
        input = input.substr(0, slashIndex-1);
    }

    size = input.size();

    for (int i = 0; i < size; i++) {
        if (input[i] == '=') {
            equalIndex = i;
        } else if (input[i] == ';') {
            semicolonIndex = i;
        } else if (input[i] == '/') {
            slashIndex = i;
        }
    }

    string dest = "", destStr = "", comp = "", compStr = "", jmp = "", jmpStr = "";

    if (input[0] == '@') {
        int address = 0;
        input = input.substr(1);
        if (isdigit(input[0])) {
            address = stoi(input);
            res = bitset<16>(address).to_string();
        } else if (isalpha(input[0])) {
            if (mapOfSymbol.count(input)) {
                res = mapOfSymbol[input];
            } else {
                mapOfSymbol[input] = bitset<16>(currentAddress).to_string();
                address = currentAddress;
                currentAddress++;
                res = bitset<16>(address).to_string();
            }
        }

    } else {
        res += "111";
        if (equalIndex != -1 && semicolonIndex != -1) {
            dest = input.substr(0, equalIndex), comp = input.substr(equalIndex+1, semicolonIndex-equalIndex), jmp = input.substr(semicolonIndex+1);

        } else if (equalIndex != -1 && semicolonIndex == -1){
            dest = input.substr(0, equalIndex), comp = input.substr(equalIndex+1);

        } else {
            comp = input.substr(0, semicolonIndex), jmp = input.substr(semicolonIndex+1);

        }
        res += mapOfComp[comp];
        res += mapOfDest[dest];
        res += mapOfJmp[jmp];
    }
    cout << res << endl;
    return res;
}

string delSpaces(string &str)
{
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    return str;
}
