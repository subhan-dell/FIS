#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <windows.h>

using namespace std;

#define MAX_FILES 100

class File {
public:
    string fileName;
    string fileType;
    string location;
    time_t creationDate;

    File(string name = "", string type = "", string loc = "") {
        fileName = name;
        fileType = type;
        location = loc;
        creationDate = time(0);
    }

    void displayFileInfo() {
        cout << "File Name: " << fileName << endl;
        cout << "File Type: " << fileType << endl;
        cout << "Location: " << location << endl;
        cout << "Creation Date: " << ctime(&creationDate);
    }
};

class FileSystem {
private:
    File* files[MAX_FILES];
    int fileCount;

public:
    FileSystem() {
        fileCount = 0;
        for (int i = 0; i < MAX_FILES; i++) {
            files[i] = NULL;
        }
    }

    void createFile(string name, string type, string location) {
        string fullPath = location + "\\" + name + "." + type; 
	cout<<"\n------------------------CREATE FILES----------------------"<<endl;
        ofstream outFile(fullPath.c_str());
        if (outFile) {
            outFile.close();
            files[fileCount] = new File(name, type, fullPath);
            fileCount++;
            cout << "File '" << fullPath << "' created successfully." << endl;
        } else {
            cout << "Error: Unable to create file at specified location!" << endl;
        }
    }

    void deleteFile(string name, string type, string location) {
    	cout<<"\n------------------------DELETE FILES----------------------"<<endl;
        string fullPath = location + "\\" + name + "." + type; 
        cout << "Attempting to delete file: " << fullPath << endl; 

        for (int i = 0; i < fileCount; i++) {
            if (files[i] != NULL && files[i]->fileName == name && files[i]->location == fullPath) {
                if (remove(fullPath.c_str()) == 0) {
                    cout << "File '" << fullPath << "' deleted successfully." << endl;
                } else {
                    perror("Error deleting file"); 
                }
                delete files[i];
                files[i] = NULL;
                for (int j = i; j < fileCount - 1; j++) {
                    files[j] = files[j + 1];
                }
                files[fileCount - 1] = NULL;
                fileCount--;
                return;
            }
        }
        cout << "Deletion not allowed! Only files created through this program can be deleted.\n";
    }

    void listFiles(string directory) {
    	cout<<"\n------------------------LIST FILES----------------------"<<endl;
        cout << "Listing files and folders in: " << directory << endl;
        WIN32_FIND_DATA findFileData;
        HANDLE hFind = FindFirstFile((directory + "\\*").c_str(), &findFileData);
        if (hFind == INVALID_HANDLE_VALUE) {
            cout << "Error accessing directory." << endl;
            return;
        }
        do {
            string name = findFileData.cFileName;
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                cout << "\t[Folder] " << name << endl;
            } else {
                cout << "\t[File] " << name << endl;
            }
        } while (FindNextFile(hFind, &findFileData));
        FindClose(hFind);
    }

    void updateFile(string oldName, string oldType, string location, string newName, string newType) {
        string oldPath = location + "\\" + oldName + "." + oldType;
        string newPath = location + "\\" + newName + "." + newType;
        cout<<"\n------------------------UPDATE FILES----------------------"<<endl;

        if (rename(oldPath.c_str(), newPath.c_str()) == 0) {
            cout << "File successfully renamed to: " << newPath << endl;
        } else {
            perror("Error renaming file");
        }
    }

    void searchFile(string name, string type, string location) {
        string fullPath = location + "\\" + name + "." + type;
        cout<<"\n------------------------SEARCH FILES----------------------"<<endl;
        ifstream file(fullPath.c_str());
        if (file) {
            cout << "File found at: " << fullPath << endl;
            file.close();
        } else {
            cout << "File not found!" << endl;
        }
    }
};

int main() {
    FileSystem fs;
    int choice;
    while (true) {
        cout << "\nFile System Operations\n";
        cout << "1. Create File\n";
        cout << "2. Delete File\n";
        cout << "3. List Files\n";
        cout << "4. Update File\n";
        cout << "5. Search File\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            string name, type, location;
            cout << "Enter file name: ";
            cin >> name;
            cout << "Enter file type (extension): ";
            cin >> type;
            cout << "Enter file location (directory path, e.g., C:\\Users\\YourName\\Desktop): ";
            cin >> location;
            fs.createFile(name, type, location);
        } else if (choice == 2) {
            string name, type, location;
            cout << "Enter file name to delete: ";
            cin >> name;
            cout << "Enter file type (extension): ";
            cin >> type;
            cout << "Enter file location (directory path, e.g., C:\\Users\\YourName\\Desktop): ";
            cin >> location;
            fs.deleteFile(name, type, location);
        } else if (choice == 3) {
            string directory;
            cout << "Enter directory path to list files and folders: ";
            cin >> directory;
            fs.listFiles(directory);
        } else if (choice == 4) {
            string oldName, oldType, location, newName, newType;
            cout << "Enter current file name: ";
            cin >> oldName;
            cout << "Enter current file type (extension): ";
            cin >> oldType;
            cout << "Enter file location: ";
            cin >> location;
            cout << "Enter new file name: ";
            cin >> newName;
            cout << "Enter new file type (extension): ";
            cin >> newType;
            fs.updateFile(oldName, oldType, location, newName, newType);
        } else if (choice == 5) {
            string name, type, location;
            cout << "Enter file name to search: ";
            cin >> name;
            cout << "Enter file type (extension): ";
            cin >> type;
            cout << "Enter file location: ";
            cin >> location;
            fs.searchFile(name, type, location);
        } else if (choice == 6) {
            cout << "Exiting file system.\n";
            break;
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }
    return 0;
}

