// include stuff for stding and stfdout
#include <iostream>
#include <fstream>
// include stuff for linked lists
#include <vector>

// Let's create the packbits compression function, that taken a buffer outputs a compressed buffer
// the function will return the new buffer
std::vector<char> packbits(char* buffer, size_t size){
    // since we do not know the final zie of the new buffer, we will use a dynamic data
    // structure that can grow in size easily, like for example a linked list
    // we will use a linked list of bytes

    // create a linked list
    std::vector<char> newBuffer;

    for(size_t i = 0; i < size; i++){
        unsigned char count = 0;
        while(i + 1 < size && buffer[i] == buffer[i+1]){
            count++;
            i++;
        }
        if(count > 0){
            // we need to write the count of letter, using max size of char - count + 1
            newBuffer.push_back(256 - count + 1);
            // now we need to write the byte to the new buffer
            newBuffer.push_back(buffer[i]);
        } else {
            // we could simply save the current list place where to update the counter, but i'm lazy so i'll do a double cycle
            for(unsigned char j; j < 128 && i + j < size && buffer[i+j] != buffer[i+j+1]; j++){
                count++;
            }
            // now we need to write the count to the new buffer
            newBuffer.push_back(count);
            // now we need to write the byte to the new buffer
            for(unsigned char j = 0; j < count; j++){
                newBuffer.push_back(buffer[i+j]);
            }
        }
    }
    return newBuffer;
}

// let's uncompress instead
std::vector<char> unpackbits(char* buffer, size_t size){
    std::vector<char> newBuffer;
    for(size_t i = 0; i < size; i++) {
        //let's see if it has enough long shittyful shit
        if(buffer[i] < 128){
            size_t j;
            // this means i need to read buffer[i] characters from the array
            for(j = i + 1; (j - i < buffer[i]) && (j < size); j++){
                newBuffer.push_back(buffer[j]);
            }
            i = j;
        } else {
            size_t length = 256 - buffer[i++] + 1;
            while(length-- > 0){
                newBuffer.push_back(buffer[i]);
            }
        }
    }
    return newBuffer;
}

int main(int argc, char** argv){
    bool compressMode = false;

    if(argc < 4){
        std::cerr << "Usage: " << argv[0] << " <mode> <input file> <output file>" << std::endl;
        return 1;
    }
    // I need to confront if the argv[1] is equale to the character 'c' or 'd'
    // if it is not, I need to return an error
    if(argv[1][0] == 'c'){
        compressMode = true;
    } else if(argv[1][0] == 'd'){
        compressMode = false;
    } else {
        std::cerr << "Error: Invalid mode. Mode must be 'c' or 'd'." << std::endl;
        std::cerr << "Usage: " << argv[0] << " <mode> <input file> <output file>" << std::endl;
        return 1;
    }
    
    // now open input file in c++ style
    std::ifstream inputFile(argv[2], std::ios::binary);
    if(!inputFile){
        std::cerr << "Error: Could not open input file " << argv[2] << std::endl;
        return 1;
    }

    std::ofstream outputFile(argv[3], std::ios::binary);
    if(!outputFile){
        std::cerr << "Error: Could not open output file " << argv[3] << "!" << std::endl;
    }
    /*
    Since we are supposedly working with small enough files (a version that works with chunks could be easily made too),
    we can just read the whole file into memory so that we can take advantage of DMA for it and put less strain on the CPU.
    */
    // find file size
    inputFile.seekg(0, std::ios::end);
    int fileSize = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);

    // allocate a buffer to hold the file
    char* buffer = new char[fileSize];
    inputFile.read(buffer, fileSize);

    // now we have the file in memory, we can close the input file
    inputFile.close();

    std::vector<char> newBuffer;

    if(compressMode){
        // now we can start the compression/decompression
        newBuffer = packbits(buffer, fileSize);
    } else {
        newBuffer = unpackbits(buffer, fileSize);
    }

    //print the newBuffer on std::out
    for(size_t i = 0; i < newBuffer.size(); i++){
        outputFile << newBuffer[i];
    }
    outputFile.close();
    return 0;
}