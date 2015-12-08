//
//  ManifestParser.cpp
//  wizards-court
//
//  Created by Daniel Fuller on 9/23/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#include "ManifestParser.h"
//
//  ObjReader.cpp
//  wizards-court
//
//  Created by Daniel Fuller on 9/16/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include "ManifestParser.h"
using namespace std;

ManifestParser::ManifestParser() {
}

ManifestParser::ManifestParser(string path) {
    parseFile(path);
}

map<string, string>
ManifestParser::GetModels() {
    return Models;
}

map<string, string>
ManifestParser::GetTextures() {
    return Textures;
}


void ManifestParser::parseFile(string path) {
    ifstream objFile;
    string buffer;
    
    objFile.open (path);
    
    if (objFile.is_open())
    {
        while ( getline (objFile, buffer) )
        {
            if(buffer[0] == '#' || buffer.length() == 0) // ignore comments and blank lines
                continue;
            
            parseLine(buffer);
        }
        objFile.close();
    }
    
    else cout << "Unable to open .manifest file [" << path << "]\n";
    
    objFile.close();
}

void ManifestParser::parseLine(string line) {
    string varname, model, texture;
    size_t start, end;
    
    // Find Variable Name (first token)
    start = 0;
    end = line.find(" ");
    varname = line.substr(start, end-start);
    
    // Find Model file path (second token)
    start = end;
    line[start++] = '`';
    end = line.find(" ");
    model = line.substr(start, end-start);
    
    // Find Texture file path (third token)
    start = ++end;
    end = line.length();
    texture = line.substr(start, end-start);
    
    // Insert assets into Manifest variables
    Models.insert(pair<string, string>(varname, model));
    Textures.insert(pair<string, string>(varname, texture));
}