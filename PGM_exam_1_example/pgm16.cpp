#include "pgm16.h"

uint16_t swap_bytes(char buffer[2]){
    const char buf = buffer[0];
    buffer[0] = buffer[1];
    buffer[1] = buf;
    return *(reinterpret_cast<uint16_t*>(buffer));
}

bool load(const std::string& filename, mat<uint16_t>& img, uint16_t& maxvalue){

    // open file with filename
    std::ifstream inputFile(filename);

    if(!inputFile.is_open()){
        std::cerr << "Cannot open file: " << filename << std::endl;
        return false;
    }

    std::clog << filename << " opened correctly. Checking file formato..." << std::endl;
    
    const int specialSymbolLength = 2;
    char specialSymbol[specialSymbolLength];

    inputFile.read(specialSymbol, specialSymbolLength);

    if(specialSymbol[0] != 'P' || specialSymbol[1] != '5'){
        std::cerr << "Given file " << filename << " is not in PGM format. Missing the opening sequence \"P5\"" << std::endl;
        return false;
    }

    if(inputFile.get() != '\n'){
        std::cerr << "Given file " << filename << " is not in PGM format. Missing the line feed after \"P5\"" << std::endl;
        return false;
    }

    std::clog << filename << " is in PGM format. Moving along with the loading part..." << std::endl;

    if(inputFile.peek() == '#'){
        std::cout << "Comment found:" << fendl;
        int read = inputFile.get();
        while (read != '\n' && read != EOF) {
            read = inputFile.get();
            std::cout << static_cast<char>(read);
            read = inputFile.get();
        }
        std::cout << std::endl << fendl;
    }

    int read = inputFile.get();
    std::string buffer;
    while (read != ' ' && read != EOF){
        buffer.push_back(static_cast<char>(read));
        read = inputFile.get();
    }

    if(read == EOF){
        ferr << "The input file " << filename << " ended before the reading of the sizes was completed. Missing height!" << fendl;
        return false;
    }

    fout << "Reading image size..." << fendl;
    
    const int width = std::stoi(buffer);
    buffer.clear();
    read = inputFile.get();
    while (read != '\n' && read != EOF)
    {
        buffer.push_back(static_cast<char>(read));
        read = inputFile.get();
    }

    const int height = std::stoi(buffer);

    buffer.clear();

    for(size_t i = 0; i < 5; i++){
        read = inputFile.get();
        if(read == '\n' || read == EOF){
            break;
        } else {
            buffer.push_back(static_cast<char>(read));
        }
    }
    maxvalue = std::stoi(buffer);
    buffer.clear();

    if(read != '\n')   {
        read == inputFile.get();
        if(read != '\n' || maxvalue > 65535){
            ferr << "The maxvalue is higher than 65535. This is a problem." << fendl;
            return false;
        }
    }

    if(read == EOF){
        ferr << "File finished right after image size read. Maybe the file is corrupted therefore data is missing?" << fendl;
        return false;
    }

    fout << fendl << "Image settings read succesfully." << fendl << fendl;
    fout << "   Width: " << width << fendl;
    fout << "   Height: " << height << fendl;
    fout << "   Max Value: " << maxvalue << fendl << fendl << fendl;

    fout << "Reading data..." << fendl;
    img.resize(height, width);
    
    size_t stepSize;
    if(maxvalue < 256)
        stepSize = 1;
    else
        stepSize = 2;
    
    char elementBuffer[stepSize];

    for(size_t i = 0; i < height; i++){
        for(size_t j = 0; j < width; j++){
            if(inputFile.peek() == EOF){
                ferr << "File finishes too early." << fendl;
                return false;
            }
            inputFile.read(elementBuffer, stepSize);
            if(maxvalue < 256){
                img(i,j) = static_cast<uint16_t>(elementBuffer[0]);
            } else {
                img(i,j) = swap_bytes(elementBuffer);
            }
            // fout << img(i,j) << " ";
        }
        // fout << fendl;
    }
    inputFile.close();
    return true;
}