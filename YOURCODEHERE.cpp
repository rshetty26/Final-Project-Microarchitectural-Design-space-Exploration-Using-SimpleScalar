#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <algorithm>
#include <fstream>
#include <map>
#include <math.h>
#include <fcntl.h>

#include "431project.h"
#include <list>


unsigned int getdl1size(std::string configuration);
unsigned int getil1size(std::string configuration);
unsigned int getl2size(std::string configuration);
double cycleTime(std::string configuration);
double EPCI(std::string configuration);
double PipelineLeakage(std::string configuration);
double getcacheleak(unsigned int size);


/*
 * Returns 1 if valid, else 0
 */
int validateConfiguration(std::string configuration){
  int configurationDimsAsInts[18];
  int returnValue=1; // assume true, set to zero otherwise
  if(isan18dimconfiguration(configuration)){ // necessary, but insufficient
    extractConfiguration(configuration, configurationDimsAsInts); // Configuration parameters now available in array
    
    unsigned int dl1size = getdl1size(configuration);
    unsigned int il1size = getil1size(configuration);
    unsigned int ul2size = getl2size(configuration);
    
    if (ul2size < (2 * dl1size)){ returnValue=0; }
    else if (ul2size < (2 * il1size)) { returnValue=0; }
    else if (ul2size < (il1size + dl1size)) { returnValue=0; }
    
    if (dl1size / 1024 == 8) {
      if (configurationDimsAsInts[9] == 0 && configurationDimsAsInts[14] != 0) { returnValue = 0; }
      else if (configurationDimsAsInts[9] == 1 && configurationDimsAsInts[14] != 1) { returnValue = 0; }
      else if (configurationDimsAsInts[9] == 2 && configurationDimsAsInts[14] != 2) { returnValue = 0; }
    } else if (dl1size / 1024 == 16) {
      if (configurationDimsAsInts[9] == 0 && configurationDimsAsInts[14] != 1) { returnValue = 0; }
      else if (configurationDimsAsInts[9] == 1 && configurationDimsAsInts[14] != 2) { returnValue = 0; }
      else if (configurationDimsAsInts[9] == 2 && configurationDimsAsInts[14] != 3) { returnValue = 0; }
    } else if (dl1size / 1024 == 32) {
      if (configurationDimsAsInts[9] == 0 && configurationDimsAsInts[14] != 2) { returnValue = 0; } 
      else if (configurationDimsAsInts[9] == 1 && configurationDimsAsInts[14] != 3) { returnValue = 0; }
      else if (configurationDimsAsInts[9] == 2 && configurationDimsAsInts[14] != 4) { returnValue = 0; }
    } else if (dl1size / 1024 == 64) {
      if (configurationDimsAsInts[9] == 0 && configurationDimsAsInts[14] != 3) { returnValue = 0; }
      else if (configurationDimsAsInts[9] == 1 && configurationDimsAsInts[14] != 4) { returnValue = 0; }
      else if (configurationDimsAsInts[9] == 2 && configurationDimsAsInts[14] != 5) { returnValue = 0; }
    } else { returnValue = 0; }

    if (il1size / 1024 == 8) {
      if (configurationDimsAsInts[9] == 0 && configurationDimsAsInts[15] != 0) { returnValue = 0; }
      else if (configurationDimsAsInts[9] == 1 && configurationDimsAsInts[15] != 1) { returnValue = 0; }
      else if (configurationDimsAsInts[9] == 2 && configurationDimsAsInts[15] != 2) { returnValue = 0; }
    } else if (il1size / 1024 == 16) {
      if (configurationDimsAsInts[9] == 0 && configurationDimsAsInts[15] != 1) { returnValue = 0; }
      else if (configurationDimsAsInts[9] == 1 && configurationDimsAsInts[15] != 2) { returnValue = 0; }
      else if (configurationDimsAsInts[9] == 2 && configurationDimsAsInts[15] != 3) { returnValue = 0; }
    } else if (il1size / 1024 == 32) {
      if (configurationDimsAsInts[9] == 0 && configurationDimsAsInts[15] != 2) { returnValue = 0; } 
      else if (configurationDimsAsInts[9] == 1 && configurationDimsAsInts[15] != 3) { returnValue = 0; }
      else if (configurationDimsAsInts[9] == 2 && configurationDimsAsInts[15] != 4) { returnValue = 0; }
    } else if (il1size / 1024 == 64) {
      if (configurationDimsAsInts[9] == 0 && configurationDimsAsInts[15] != 3) { returnValue = 0; }
      else if (configurationDimsAsInts[9] == 1 && configurationDimsAsInts[15] != 4) { returnValue = 0; }
      else if (configurationDimsAsInts[9] == 2 && configurationDimsAsInts[15] != 5) { returnValue = 0; }
    } else { returnValue = 0; }

    if (ul2size / 1024 == 128) {
      if (configurationDimsAsInts[12] == 0 && configurationDimsAsInts[16] != 0) { returnValue = 0; }
      else if (configurationDimsAsInts[12] == 1 && configurationDimsAsInts[16] != 1) { returnValue = 0; } 
      else if (configurationDimsAsInts[12] == 2 && configurationDimsAsInts[16] != 2) { returnValue = 0; }
      else if (configurationDimsAsInts[12] == 3 && configurationDimsAsInts[16] != 3) { returnValue = 0; }
      else if (configurationDimsAsInts[12] == 4 && configurationDimsAsInts[16] != 4) { returnValue = 0; } 
    } else if (ul2size / 1024 == 256) {
      if (configurationDimsAsInts[12] == 0 && configurationDimsAsInts[16] != 1) { returnValue = 0; }
      else if (configurationDimsAsInts[12] == 1 && configurationDimsAsInts[16] != 2) { returnValue = 0; }
      else if (configurationDimsAsInts[12] == 2 && configurationDimsAsInts[16] != 3) { returnValue = 0; }
      else if (configurationDimsAsInts[12] == 3 && configurationDimsAsInts[16] != 4) { returnValue = 0; }
      else if (configurationDimsAsInts[12] == 4 && configurationDimsAsInts[16] != 5) { returnValue = 0; }
    } else if (ul2size / 1024 == 512) {
      if (configurationDimsAsInts[12] == 0 && configurationDimsAsInts[16] != 2) { returnValue = 0; }
      else if (configurationDimsAsInts[12] == 1 && configurationDimsAsInts[16] != 3) { returnValue = 0; }
      else if (configurationDimsAsInts[12] == 2 && configurationDimsAsInts[16] != 4) { returnValue = 0; }
      else if (configurationDimsAsInts[12] == 3 && configurationDimsAsInts[16] != 5) { returnValue = 0; }
      else if (configurationDimsAsInts[12] == 4 && configurationDimsAsInts[16] != 6) { returnValue = 0; }
    } else if (ul2size / 1024 == 1024) {
      if (configurationDimsAsInts[12] == 0 && configurationDimsAsInts[16] != 3) { returnValue = 0; }
      else if (configurationDimsAsInts[12] == 1 && configurationDimsAsInts[16] != 4) { returnValue = 0; }
      else if (configurationDimsAsInts[12] == 2 && configurationDimsAsInts[16] != 5) { returnValue = 0; }
      else if (configurationDimsAsInts[12] == 3 && configurationDimsAsInts[16] != 6) { returnValue = 0; }
      else if (configurationDimsAsInts[12] == 4 && configurationDimsAsInts[16] != 7) { returnValue = 0; }
    } else if (ul2size / 1024 == 2048) {
      if (configurationDimsAsInts[12] == 0 && configurationDimsAsInts[16] != 4) { returnValue = 0; }
      else if (configurationDimsAsInts[12] == 1 && configurationDimsAsInts[16] != 5) { returnValue = 0; }
      else if (configurationDimsAsInts[12] == 2 && configurationDimsAsInts[16] != 6) { returnValue = 0; }
      else if (configurationDimsAsInts[12] == 3 && configurationDimsAsInts[16] != 7) { returnValue = 0; }
      else if (configurationDimsAsInts[12] == 4 && configurationDimsAsInts[16] != 8) { returnValue = 0; }
    } else { returnValue = 0; }

    cycleTime(configuration);
    EPCI(configuration);
    PipelineLeakage(configuration);
    if (getcacheleak(dl1size) == 40e-3 || getcacheleak(il1size) == 40e-3 || getcacheleak(ul2size) == 40e-3) { returnValue = 0; }

  } else {
    returnValue=0;
  }
  return returnValue;
}


std::string YourProposalFunction(std::string currentconfiguration, std::string bestEXECconfiguration, std::string bestEDPconfiguration, int optimizeforEXEC, int optimizeforEDP){
  std::string nextconfiguration = GLOB_baseline;
  std::stringstream ss;

  if (optimizeforEDP == 1) { // check for efficiency
    int x = 0;
    for(int dim = 0; dim < 9; ++dim) {
      x = rand()%GLOB_dimensioncardinality[dim];
      ss << x << " ";
    }
    ss << "0 2 2 2 ";
    ss << rand()%GLOB_dimensioncardinality[13];
    ss << " 0 0 3 5";
    //ss << rand()%GLOB_dimensioncardinality[17]

  } else { // check for performance
    ss << "0 0 1 ";
    ss << rand()%GLOB_dimensioncardinality[3];
    ss << " 0 0 ";
    int x = 0;
    for(int dim = 6; dim<17; ++dim) {
      x = rand()%GLOB_dimensioncardinality[dim];
      ss << x << " "; 
    }
    ss << "0"; 
  }

  nextconfiguration=ss.str();
  ss.str("");    
  return nextconfiguration;
  
}
