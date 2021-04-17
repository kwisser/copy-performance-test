#include <iostream>
#include <chrono>
#include <pthread.h>

// Copy File, ask User for Source and Target Folder, then Copy
// example:
// Enter the Name of Source File: /home/klemon/CLionProjects/open_pearl_projektarbeit/testfiles/50mb
// Enter the Name of Target File: /home/klemon/CLionProjects/open_pearl_projektarbeit/testfiles/50mbcopied

void* copyFile( void* help){ char ch, sourceFile[80], targetFile[80]; 
    FILE *fs, *ft;
    std::chrono::high_resolution_clock::time_point start, end;
    std::chrono::high_resolution_clock::duration difference;
    int read, write, open, close, writeraw, readraw;
    
    start = std::chrono::high_resolution_clock::now();
    fs = fopen("/media/marcel/B282A48982A4539F/open_pearl_projektarbeit-main/testfiles/5mb", "r");
    end = std::chrono::high_resolution_clock::now();
    if(fs == NULL) {
        std::cout<<"\nError Occurred!"; return 0;
    }
    difference = end - start;
    open = std::chrono::duration_cast<std::chrono::microseconds>(difference).count();
    
    
    start = std::chrono::high_resolution_clock::now();
    fclose(fs);
    end = std::chrono::high_resolution_clock::now();
    difference = end - start;
    close = std::chrono::duration_cast<std::chrono::microseconds>(difference).count();
    
    
     /*std::cout << "Enter the Name of the File you want 
    to Copy: "; std::cin >> sourceFile;*/ 
    start = std::chrono::high_resolution_clock::now(); 
    fs = fopen("/media/marcel/B282A48982A4539F/open_pearl_projektarbeit-main/testfiles/5mb", "r"); 
    if(fs == NULL) {
        std::cout<<"\nError Occurred!";
        return 0;
    }
    
    /*std::cout << "\nEnter the Name of Target File: "; std::cin >> 
    targetFile;*/
    // Start Time
   
    ch = fgetc(fs); 
    	while(ch != EOF) { 
    	ch = fgetc(fs);
    }
    fclose(fs);
    
    end = std::chrono::high_resolution_clock::now();
    difference = end - start;
    read = std::chrono::duration_cast<std::chrono::milliseconds>(difference).count();
    
    
    
    
    start = std::chrono::high_resolution_clock::now();
    
    fs = fopen("/media/marcel/B282A48982A4539F/open_pearl_projektarbeit-main/testfiles/5mb", "r"); 
    if(fs == NULL) {
        std::cout<<"\nError Occurred!";
        return 0;
    }
    
    ft = fopen("/media/marcel/B282A48982A4539F/open_pearl_projektarbeit-main/testfiles/output", "w");
    if(ft == NULL) {
        std::cout<<"\nError Occurred!";
        return 0;
    }
    
    ch = fgetc(fs); 
    	while(ch != EOF) { 
    	fputc(ch, ft);
    	ch = fgetc(fs);
    }
    
    fclose(fs); 
    fclose(ft);
    end = std::chrono::high_resolution_clock::now();
    difference = end - start;
    write = std::chrono::duration_cast<std::chrono::milliseconds>(difference).count();
    readraw = (float)read - (float)open/1000 - (float)close/1000;
    writeraw = write - read;
    
    std::cout << "Open Took: " << open << "µs." << std::endl;
    std::cout << "Close Took: " << close << "µs.\n" << std::endl;
    
    std::cout << "Read Took: " << read << "ms." << std::endl;
    std::cout << "Write Took: " << write << "ms.\n" << std::endl;
    
    std::cout << "Read_raw Took: " << readraw << "ms" << std::endl;
    std::cout << "Write_raw Took: " << writeraw << "ms" << std::endl;
    
    std::cout << "\nFile copied successfully." << std::endl;

    return nullptr;
}

int main() { 
	pthread_t test; int rc;

	copyFile(nullptr);
	std::cout << "\n\n\nThreads\n" << std::endl;
	rc = pthread_create(&test, NULL, copyFile, NULL); 
	if (rc) { 
        	printf("Error:unable to create thread, %d\n", rc);
        	exit(-1);
      	}
	pthread_join(test, NULL); 
	return 0;
}
