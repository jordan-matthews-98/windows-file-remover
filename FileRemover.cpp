#include <windows.h>
#include <stdio.h>
#include <iterator>
#include <vector>
#include <iostream>

using namespace std;

string getDirectory();
string getSubstring();
void findFiles(string, string, vector<string>&);
bool getAccept(vector<string>);
void deleteFiles(vector<string>);

int main()
{
    vector<string> files;
    string directory = getDirectory();
    string substring = getSubstring();
    findFiles(directory, substring, files);
    if(getAccept(files)){
        deleteFiles(files);
    }
    return 0;
}

string getDirectory(){
    string s;
    cout << "What directory do you want to delete files from? (will delete from all subdirectories too)" << endl;
    cin >> s;
    return s;
}

string getSubstring(){
    string s;
    cout << "Enter the string contained by the titles of files you want to delete." << endl;
    cin >> s;
    return s;
}

void findFiles(string directory, string substring, vector<string>& files){
    //cout << directory << endl;
    string pattern(directory);
    pattern.append("\\*");
    WIN32_FIND_DATA data;
    HANDLE hFind;
    if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
        do {
            if(GetFileAttributesA(data.cFileName) == 16 && (string)data.cFileName != ".." || GetFileAttributesA(data.cFileName) == 4294967295LL){
                if(((string)data.cFileName).find(substring) != -1){
                    files.push_back(directory + '/' + data.cFileName);
                }
                char full_path[MAX_PATH];
                GetFullPathName (data.cFileName, MAX_PATH, full_path, NULL);
                findFiles(directory + '/' + data.cFileName, substring, files);
            }
        } while (FindNextFile(hFind, &data) != 0);
        FindClose(hFind);
    }
}

bool getAccept(vector<string> files){
    copy(files.begin(), files.end(), ostream_iterator<std::string>(std::cout, "\n"));
    string choice;
    while(true){
        cout << "Delete files listed above? (CONFIRM or DENY)";
        cin >> choice;
        if(choice == "CONFIRM"){
            return true;
        }
        if(choice == "DENY"){
            return false;
        }
    }
}

void deleteFiles(vector<string> files){
    for(int i = 0; i < files.size(); ++i){
        remove(files[i].c_str());
    }
}
