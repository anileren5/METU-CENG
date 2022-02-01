#include "METUMaps.h"
#include "GraphExceptions.h"

#include <iostream>

void METUMaps::PrintNotInJourney() const
{
    std::cout << "Device is not in a journey!" << std::endl;
}

void METUMaps::PrintUnableToChangeDestination() const
{
    std::cout << "Cannot change Destination during journey!" << std::endl;
}

void METUMaps::PrintUnableToChangeStartingLoc() const
{
    std::cout << "Cannot change Starting Location during journey!" << std::endl;
}

void METUMaps::PrintAlreadyInJourney() const
{
    std::cout << "Device is already in a journey!" << std::endl;
}

void METUMaps::PrintJourneyIsAlreadFinished() const
{
    std::cout << "Journey is already finished!" << std::endl;
}

void METUMaps::PrintLocationNotFound() const
{
    std::cout << "One (or both) of the locations are not found in the maps!" << std::endl;
}

void METUMaps::PrintJourneyCompleted() const
{
    std::cout << "Journey Completed!" << std::endl;
}

void METUMaps::PrintCachedLocationFound(const std::string& location0,
                                        const std::string& location1) const
{
    std::cout << "Route between \""
              << location0 << "\" and \""
              << location1 << "\" is in cache, using that..."
              << std::endl;
}

void METUMaps::PrintCalculatingRoutes(const std::string& location0,
                                      const std::string& location1) const
{
    std::cout << "Calculating Route(s) between \""
              << location0 << "\" and \""
              << location1 << "\"..."
              << std::endl;
}

std::string METUMaps::GenerateKey(const std::string& location0,
                                  const std::string& location1)
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    return location0 + "/" + location1;
}

METUMaps::METUMaps(int potentialPathCount,
                   const std::string& mapFilePath)
{
    // TODO
    map = Graph(mapFilePath);
    this->potentialPathCount = potentialPathCount;
    inJourney = false;
    cachedPaths = KeyedHashTable(potentialPathCount * map.TotalVertexCount());
}

void METUMaps::SetDestination(const std::string& name)
{
    // TODO
    
    if (inJourney == true) PrintUnableToChangeDestination();
    else destination = name;
}

void METUMaps::SetStartingLocation(const std::string& name)
{
    // TODO
    
    if (inJourney == true) PrintUnableToChangeStartingLoc();
    else startingLoc = name;
}

void METUMaps::StartJourney()
{
    // TODO
    PrintCalculatingRoutes(startingLoc,destination);
    int i,j,k;
    
    
    // If this function is called during journey.
    if (inJourney == true){
        PrintAlreadyInJourney();
        return ;
    }
    
    
    // If either startingLoc and destination is not on the map.
    bool isStartingLocExist = false;
    bool isDestinationExist = false;
    for (i=0;i<map.TotalVertexCount();i++){
        if (map.VertexName(i) == startingLoc) isStartingLocExist = true;
        if (map.VertexName(i) == destination) isDestinationExist = true;
    }
    if ((isStartingLocExist == false) || (isDestinationExist == false)){
        PrintLocationNotFound();
        return;
    }
    
    // ##### Start The Journey #####
    
    inJourney = true;
    
    std::vector<std::vector<int> > paths;
    map.MultipleShortPaths(paths,startingLoc,destination,potentialPathCount);
    
    // ##### Caching #####
    
    for (i=0;i<paths.size();i++){
        std::vector<int> path = paths[i];
        std::vector<int> subpath;
        for (j=0;j<path.size()-1;j++){
            for (k=j;k<path.size();k++) subpath.push_back(path[k]);
            cachedPaths.Insert(GenerateKey(map.VertexName(subpath[0]),map.VertexName(subpath[subpath.size()-1])),subpath);
            subpath.clear();
        }
    }
    
    // ##### Caching Ends #####
    
    
    currentLoc = startingLoc;
    currentRoute = paths[0];
}

void METUMaps::EndJourney()
{
    // TODO
    if (inJourney == false) PrintJourneyIsAlreadFinished();
    else{
        inJourney = false;
        cachedPaths.ClearTable();
        destination.clear();
        startingLoc.clear();
        currentLoc.clear();
    }
    
}

void METUMaps::UpdateLocation(const std::string& name)
{
    // TODO
    int i,j,k;
    
    // If this function is not called during journey.
    if (inJourney == false){
        PrintNotInJourney();
        return;
    }
    
    // If name is not on the map.
    bool isNameExist = false;
    for (i=0;i<map.TotalVertexCount();i++){
        if (map.VertexName(i) == name){
            isNameExist = true;
            break;
        }
    }
    if (isNameExist == false){
        PrintLocationNotFound();
        return;
    }
    
    // If user reached to his/her destination
    currentLoc = name;
    if (currentLoc == destination){
         PrintJourneyCompleted();
         return ;
    }
    
    std::vector<int> cachedPath;
    bool flag = cachedPaths.Find(cachedPath,GenerateKey(name,destination));
    
    if (flag == true){
        PrintCachedLocationFound(name,destination);
        currentRoute = cachedPath;
        
    }
    else{
        PrintCalculatingRoutes(name,destination);  
        std::vector<std::vector<int> > paths;
        map.MultipleShortPaths(paths,name,destination,potentialPathCount);
        for (i=0;i<paths.size();i++){
            std::vector<int> path = paths[i];
            std::vector<int> subpath;
            for (j=0;j<path.size()-1;j++){
                for (k=j;k<path.size();k++) subpath.push_back(path[k]);
                cachedPaths.Insert(GenerateKey(map.VertexName(subpath[0]),map.VertexName(subpath[subpath.size()-1])),subpath);
                subpath.clear();
            }
        }
        currentRoute = paths[0];
    }
    
}

void METUMaps::Display()
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    if(!inJourney)
    {
        // We are not in journey, this function
        // shouldn't be called
        PrintNotInJourney();
        return;
    }

    int timeLeft = map.TotalWeightInBetween(currentRoute);

    // Print last known location
    std::cout << "Journey         : " << startingLoc << "->"
              << destination << "\n";
    std::cout << "Current Location: " << currentLoc << "\n";
    std::cout << "Time Left       : " << timeLeft << " minutes\n";
    std::cout << "Current Route   : ";
    map.PrintPath(currentRoute, true);

    std::cout.flush();
}