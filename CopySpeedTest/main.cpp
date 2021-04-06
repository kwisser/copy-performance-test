#include <iostream>
#include <chrono>

int copyFile(){
    char ch, sourceFile[80], targetFile[80];
    FILE *fs, *ft;
    std::cout << "Enter the Name of the File you want to Copy: ";
    std::cin >> sourceFile;
    fs = fopen(sourceFile, "r");
    if(fs == NULL)
    {
        std::cout<<"\nError Occurred!";
        return 0;
    }
    std::cout << "\nEnter the Name of Target File: ";
    std::cin >> targetFile;
    ft = fopen(targetFile, "w");
    if(ft == NULL)
    {
        std::cout<<"\nError Occurred!";
        return 0;
    }
    ch = fgetc(fs);
    while(ch != EOF)
    {
        fputc(ch, ft);
        ch = fgetc(fs);
    }
    std::cout << "\nFile copied successfully.";
    fclose(fs);
    fclose(ft);
    std::cout << std::endl;
    return 0;
}

int main() {
    // Start Time
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    // Copy File, ask User for Source and Target Folder, then Copy
    // example:
    // Enter the Name of Source File: /home/klemon/CLionProjects/open_pearl_projektarbeit/testfiles/50mb
    // Enter the Name of Target File: /home/klemon/CLionProjects/open_pearl_projektarbeit/testfiles/50mbcopied
    copyFile();
    // Stop Time
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    // end - start = time needed for copying File from Source to Target
    std::chrono::high_resolution_clock::duration difference = end - start;
    // Cast into milliseconds
    int ms = std::chrono::duration_cast<std::chrono::milliseconds>(difference).count();
    std::cout << "Test Took: " << ms << "ms." << std::endl;
    return 0;
}

