#include <iostream>
#include <chrono>

// Copy File, ask User for Source and Target Folder, then Copy
// example:
// Enter the Name of Source File: /home/klemon/CLionProjects/open_pearl_projektarbeit/testfiles/50mb
// Enter the Name of Target File: /home/klemon/CLionProjects/open_pearl_projektarbeit/testfiles/50mbcopied

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
    // Start Read Time Counter
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    ft = fopen(targetFile, "w");
    if(ft == NULL)
    {
        std::cout<<"\nError Occurred!";
        return 0;
    }
    ch = fgetc(fs);

    // Stop Read Time Time Counter
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    // end - start = time needed to Read File
    std::chrono::high_resolution_clock::duration difference = end - start;
    // Cast into milliseconds
    int read_time_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(difference).count();

    // Start Write Time Counter
    start = std::chrono::high_resolution_clock::now();

    while(ch != EOF)
    {
        fputc(ch, ft);
        ch = fgetc(fs);
    }

    // Stop Write Time Time Counter
    end = std::chrono::high_resolution_clock::now();
    // end - start = time needed to Read File
    difference = end - start;
    // Cast into milliseconds
    int write_time_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(difference).count();

    std::cout << "\nFile copied successfully." << std::endl;
    std::cout << "Read Time: " << read_time_in_ms << "ms" << std::endl;
    std::cout << "Write Time: " << write_time_in_ms << "ms" << std::endl;
    std::cout << "Total Time: " << read_time_in_ms+write_time_in_ms << "ms" << std::endl;

    fclose(fs);
    fclose(ft);
    std::cout << std::endl;
    return 0;
}

int main() {
    copyFile();
    return 0;
}

