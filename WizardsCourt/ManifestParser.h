//
//  ManifestParser.h
//  wizards-court
//
//  Created by Daniel Fuller on 9/23/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#ifndef __wizards_court__ManifestParser__
#define __wizards_court__ManifestParser__

#include <stdio.h>
#include <string>
#include <map>
#include <iostream>

class ManifestParser{
    
private:
    std::map<std::string, std::string> Models;
    std::map<std::string, std::string> Textures;
    
public:
    ManifestParser(std::string path);
    ManifestParser();
    std::map<std::string, std::string> GetModels();
    std::map<std::string, std::string> GetTextures();
    void parseFile(std::string path);
    
private:
    void parseLine(std::string line);
    
};

#endif /* defined(__wizards_court__ManifestParser__) */
