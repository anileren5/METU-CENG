#include "Graph.h"
#include "GraphExceptions.h"

#include <iostream>
#include <iomanip>
#include <queue>
#include <fstream>
#include <sstream>
#include <cstdlib>

// Literally do nothing here
// default constructors of the std::vector is enough
Graph::Graph()
{}

Graph::Graph(const std::string& filePath)
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    // Tokens
    std::string tokens[3];

    std::ifstream mapFile(filePath.c_str());
    // Read line by line
    std::string line;
    while (std::getline(mapFile, line))
    {
        // Empty Line Skip
        if(line.empty()) continue;
        // Comment Skip
        if(line[0] == '#') continue;

        // Tokenize the line
        int i = 0;
        std::istringstream stream(line);
        while(stream >> tokens[i]) i++;

        // Single token (Meaning it is a vertex)
        if(i == 1)
        {
            InsertVertex(tokens[0]);
        }
        // Exactly three tokens (Meaning it is an edge)
        else if(i == 3)
        {
            int weight = std::atoi(tokens[0].c_str());
            if(!ConnectVertices(tokens[1], tokens[2], weight))
            {
                std::cerr << "Duplicate edge on "
                          << tokens[0] << "-"
                          << tokens[1] << std::endl;
            }
        }
        else std::cerr << "Token Size Mismatch" << std::endl;
    }
}

void Graph::InsertVertex(const std::string& vertexName)
{
    // TODO
    
    int i;
    bool isExist = false; // Check if a vertex with vertexName exists.
    for (i=0;i < (int)vertexList.size();i++){
        if (vertexList[i].name == vertexName){
            isExist = true;
            break;
        }
    }
    
    if (isExist == true) throw DuplicateVertexNameException();
    else {
        struct GraphVertex new_vertex;
        new_vertex.name = vertexName;
        new_vertex.edgeCount = 0;
        vertexList.push_back(new_vertex);
    }
}

bool Graph::ConnectVertices(const std::string& fromVertexName,
                            const std::string& toVertexName,
                            int weight)
{
    // TODO
    
    int i;
    bool isFromExist = false, isToExist = false;
    for (i=0;i<(int)vertexList.size();i++){
        if (vertexList[i].name == fromVertexName) isFromExist = true;
        if (vertexList[i].name == toVertexName) isToExist = true;
    }
    
    if ((isFromExist == false) || (isToExist == false)) throw VertexNotFoundException();
    
    int n_from_edge,n_to_edge;
    for (i=0;i<(int)vertexList.size();i++){
        if (vertexList[i].name == fromVertexName) n_from_edge = vertexList[i].edgeCount;
        if (vertexList[i].name == toVertexName) n_to_edge = vertexList[i].edgeCount;
    }
    
    if (((n_from_edge+1) > MAX_EDGE_PER_VERTEX) || ((n_to_edge+1) > MAX_EDGE_PER_VERTEX)) throw TooManyEdgeOnVertexExecption();
    
    bool alreadyEdge = false;
    for (i=0;i<(int)edgeList.size();i++){
        struct GraphEdge current_edge = edgeList[i];
        struct GraphVertex vertex0 = vertexList[current_edge.vertexId0];
        struct GraphVertex vertex1 = vertexList[current_edge.vertexId1];
        if ((vertex0.name == fromVertexName && vertex1.name == toVertexName) ||  (vertex0.name == toVertexName && vertex1.name == fromVertexName)){
            alreadyEdge = true;
        }
    }
    
    if (alreadyEdge == true) return false;
    else {
        int i0,i1;
        for (i=0;i<(int)vertexList.size();i++){
            if (vertexList[i].name == fromVertexName) i0 = i;
            if (vertexList[i].name == toVertexName) i1 = i;
        }
        if (i0 == i1) return false;
        struct GraphEdge new_edge;
        new_edge.weight = weight;
        new_edge.masked = false;
        new_edge.vertexId0 = i0;
        new_edge.vertexId1 = i1;
        
        edgeList.push_back(new_edge);
        
        int id_of_new_edge = edgeList.size()-1;
        
        
        vertexList[i0].edgeCount += 1;
        vertexList[i1].edgeCount += 1;
        
        vertexList[i0].edgeIds[vertexList[i0].edgeCount-1] = id_of_new_edge;
        vertexList[i1].edgeIds[vertexList[i1].edgeCount-1] = id_of_new_edge;
        
        return true;
    }
}

bool Graph::ShortestPath(std::vector<int>& orderedVertexIdList,
                         const std::string& from,
                         const std::string& to) const
{
    // TODO
    bool isFromExist = false;
    bool isToExist = false;
    int i;
    for (i=0;i<(int)vertexList.size();i++){
        if (vertexList[i].name == from) isFromExist = true;
        if (vertexList[i].name == to) isToExist = true;
    }
    
    if ((isFromExist == false) || (isToExist == false)) throw VertexNotFoundException();
    

    int fromId,toId;
    for (i=0;i<vertexList.size();i++){
        if (vertexList[i].name == from){
            fromId = i;
        }
        if (vertexList[i].name == to){
            toId = i;
        }
    }
    
    std::vector<bool> vis;
    for (i=0;i<vertexList.size();i++) vis.push_back(false);
    
    std::vector<int> prev;
    for (i=0;i<vertexList.size();i++) prev.push_back(-1);
    
    std::vector<int> dist;
    for (i=0;i<vertexList.size();i++) dist.push_back(LARGE_NUMBER);
    dist[fromId] = 0;
    
    std::priority_queue<DistanceVertexIdPair> pq;
    pq.push(DistanceVertexIdPair(fromId,0));
    
    while(!pq.empty())
    {
        int index = pq.top().vId;
        int minValue = pq.top().currentWeight;
        pq.pop();
        if (dist[index] < minValue) continue;
        for (i=0;i<vertexList[index].edgeCount;i++){
            struct GraphEdge edge = edgeList[vertexList[index].edgeIds[i]];
            if (edge.masked == true) continue;
            int toId = (index == edge.vertexId0) ? edge.vertexId1 : edge.vertexId0;
            if (vis[toId]) continue;
            int new_dist = dist[index] + edge.weight;
            if (new_dist < dist[toId]){
                prev[toId] = index;
                dist[toId] = new_dist;
                pq.push(DistanceVertexIdPair(toId,new_dist));
            }
        }
    }
    
    if (dist[toId] == LARGE_NUMBER) return false;
    else{
        std::vector<int> reverse_path;
        int at = toId;
        while (at != -1){
            reverse_path.push_back(at);
            at = prev[at];
        }
        for (i=reverse_path.size()-1;i>=0;i--) orderedVertexIdList.push_back(reverse_path[i]);
        return true;
    }
}

int Graph::MultipleShortPaths(std::vector<std::vector<int> >& orderedVertexIdList,
                              const std::string& from,
                              const std::string& to,
                              int numberOfShortestPaths)
{
    // TODO
    int i,j;
    UnMaskAllEdges();
    
    std::vector<int> first_path;
    bool isFirstPathExist = ShortestPath(first_path,from,to);

    if (isFirstPathExist == false) return 0;
    
    orderedVertexIdList.push_back(first_path);
    
    for (i=1;i<numberOfShortestPaths;i++){
        std::vector<int> last_path = orderedVertexIdList[orderedVertexIdList.size()-1];
        std::vector<std::vector<int> > edges_on_the_last_path;
        for (j=0;j<last_path.size()-1;j++){
            std::vector<int> edge;
            edge.push_back(last_path[j]);
            edge.push_back(last_path[j+1]);
            edges_on_the_last_path.push_back(edge);
        }
        
        int highest_weight = 0;
        for (j=0;j<edges_on_the_last_path.size();j++){
            int v0 = edges_on_the_last_path[j][0];
            int v1 = edges_on_the_last_path[j][1];
            int k;
            for (k=0;k<edgeList.size();k++){
                if ((edgeList[k].vertexId0 == v0 && edgeList[k].vertexId1 == v1) || edgeList[k].vertexId0 == v1 && edgeList[k].vertexId1 == v0){
                    if (edgeList[k].weight > highest_weight){
                        highest_weight = edgeList[k].weight;
                    }
                }
            }
            
            for (k=0;k<edgeList.size();k++){
                if (edgeList[k].weight == highest_weight){
                    edgeList[k].masked = true;
                }
            }
        }

        std::vector<int> new_path;
            
        bool isNewPathExist = ShortestPath(new_path,from,to);
        if (isNewPathExist == false){
            UnMaskAllEdges();
            return orderedVertexIdList.size();
        }
        else {
            orderedVertexIdList.push_back(new_path);
        }
    }
    
    UnMaskAllEdges();
    return orderedVertexIdList.size();
    

}

void Graph::MaskEdges(const std::vector<StringPair>& vertexNames)
{
    // TODO
    int i;
    for (i=0;i<(int)vertexNames.size();i++){
        int j;
        std::string name0 = vertexNames[i].s0;
        std::string name1 = vertexNames[i].s1;
        bool is0Exist = false;
        bool is1Exist = false;
        for (j=0;j<(int)vertexList.size();j++){
            if (vertexList[j].name == name0) is0Exist = true;
            if (vertexList[j].name == name1) is1Exist = true;
        }
        
        if ((is0Exist == false) || (is1Exist == false)) throw VertexNotFoundException();
        
        for (j=0;j<(int)edgeList.size();j++){
            int i0 = edgeList[j].vertexId0; 
            int i1 = edgeList[j].vertexId1;
            
            if ((vertexList[i0].name == name0 && vertexList[i1].name == name1) || (vertexList[i0].name == name1 && vertexList[i1].name == name0)){
                edgeList[j].masked = true;
                break;
            }
        }
    }
}

void Graph::UnMaskEdges(const std::vector<StringPair>& vertexNames)
{
    // TODO
    int i;
    for (i=0;i<(int)vertexNames.size();i++){
        int j;
        std::string name0 = vertexNames[i].s0;
        std::string name1 = vertexNames[i].s1;
        bool is0Exist = false;
        bool is1Exist = false;
        for (j=0;j<(int)vertexList.size();j++){
            if (vertexList[j].name == name0) is0Exist = true;
            if (vertexList[j].name == name1) is1Exist = true;
        }
        
        if ((is0Exist == false) || (is1Exist == false)) throw VertexNotFoundException();
        
        for (j=0;j<(int)edgeList.size();j++){
            int i0 = edgeList[j].vertexId0;
            int i1 = edgeList[j].vertexId1;
            
            if ((vertexList[i0].name == name0 && vertexList[i1].name == name1) || (vertexList[i0].name == name1 && vertexList[i1].name == name0)){
                edgeList[j].masked = false;
                break;
            }
        }
    }
}

void Graph::UnMaskAllEdges()
{
    // TODO
    int i;
    for (i=0;i<(int)edgeList.size();i++) edgeList[i].masked = false;
}

void Graph::MaskVertexEdges(const std::string& name)
{
    // TODO
    int i;
    bool isVertexExist = false;
    for (i=0;i<(int)vertexList.size();i++){
        if (vertexList[i].name == name){
            isVertexExist = true;
            break;
        }
    }
    
    if (isVertexExist == false) throw VertexNotFoundException();
    
    for (i=0;i<(int)edgeList.size();i++){
        std::string name0 = vertexList[edgeList[i].vertexId0].name;
        std::string name1 = vertexList[edgeList[i].vertexId1].name;
        if (name == name0 || name == name1) edgeList[i].masked = true;
    }
}

void Graph::UnMaskVertexEdges(const std::string& name)
{
    // TODO
    int i;
    bool isVertexExist = false;
    for (i=0;i<(int)vertexList.size();i++){
        if (vertexList[i].name == name){
            isVertexExist = true;
            break;
        }
    }
    
    if (isVertexExist == false) throw VertexNotFoundException();
    
    for (i=0;i<(int)edgeList.size();i++){
        std::string name0 = vertexList[edgeList[i].vertexId0].name;
        std::string name1 = vertexList[edgeList[i].vertexId1].name;
        if (name == name0 || name == name1) edgeList[i].masked = false;
    }
}

void Graph::ModifyEdge(const std::string& vName0,
                       const std::string& vName1,
                       float newWeight)
{
    // TODO
    int i;
    bool is0Exist = false, is1Exist = false;
    for (i=0;i<(int)vertexList.size();i++){
        if (vertexList[i].name == vName0) is0Exist = true;
        if (vertexList[i].name == vName1) is1Exist = true;
    }
    
    if ((is0Exist == false) || (is1Exist == false)) throw VertexNotFoundException();
    
    bool isEdgeExist = false;
    for (i=0;i<(int)edgeList.size();i++){
        struct GraphEdge current_edge = edgeList[i];
        std::string name0 = vertexList[current_edge.vertexId0].name;
        std::string name1 = vertexList[current_edge.vertexId1].name;
        if (((name0 == vName0) && (name1 == vName1))  || ((name0 == vName1) && (name1 == vName0))){
            isEdgeExist = true;
            break;
        }
    }
    
    if (isEdgeExist == false) return;
    else {
        int indexOfEdge;
        for (i=0;i<(int)edgeList.size();i++){
            struct GraphEdge current_edge = edgeList[i];
            std::string name0 = vertexList[current_edge.vertexId0].name;
            std::string name1 = vertexList[current_edge.vertexId1].name;
            if (((name0 == vName0) && (name1 == vName1)) || ((name0 == vName1) && (name1 == vName0))){
                edgeList[i].weight = newWeight;
            }
        }
    }
}

void Graph::ModifyEdge(int vId0, int vId1,
                       float newWeight)
{
    // TODO
    if ((vId0 < 0) || (vId0 >= vertexList.size()) || (vId1 < 0) || (vId1 >= vertexList.size())) return;
    
    int i;
    bool isAnEdgeExist = false;
    
    for (i=0;i<(int)edgeList.size();i++){
        std::string name0 = vertexList[edgeList[i].vertexId0].name;
        std::string name1 = vertexList[edgeList[i].vertexId1].name;
        if (((name0 == vertexList[vId0].name) && (name1 == vertexList[vId1].name)) || ((name0 == vertexList[vId1].name) && (name1 == vertexList[vId0].name))){
            isAnEdgeExist = true;
        }
    }
    
    if (isAnEdgeExist == false) return ;
    else {
        for (i=0;i<(int)edgeList.size();i++){
            std::string name0 = vertexList[edgeList[i].vertexId0].name;
            std::string name1 = vertexList[edgeList[i].vertexId1].name;
            if (((name0 == vertexList[vId0].name) && (name1 == vertexList[vId1].name)) || ((name0 == vertexList[vId1].name) && (name1 == vertexList[vId0].name))){
                edgeList[i].weight = newWeight;
            }
        }
    }
    
}

void Graph::PrintAll() const
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    for(size_t i = 0; i < vertexList.size(); i++)
    {
        const GraphVertex& v = vertexList[i];
        std::cout << v.name << "\n";
        for(int j = 0; j < v.edgeCount; j++)
        {
            int edgeId = v.edgeIds[j];
            const GraphEdge& edge = edgeList[edgeId];
            // Skip printing this edge if it is masked
            if(edge.masked)
                continue;

            // List the all vertex names and weight
            std::cout << "-" << std::setfill('-')
                             << std::setw(2) << edge.weight
                             << "-> ";
            int neigVertexId = (static_cast<int>(i) == edge.vertexId0)
                                 ? edge.vertexId1
                                 : edge.vertexId0;
            std::cout << vertexList[neigVertexId].name << "\n";
        }
    }
    // Reset fill value because it "sticks" to the std out
    std::cout << std::setfill(' ');
    std::cout.flush();
}

void Graph::PrintPath(const std::vector<int>& orderedVertexIdList,
                      bool sameLine) const
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this file !       //
    // ============================= //
    for(size_t i = 0; i < orderedVertexIdList.size(); i++)
    {
        int vertexId = orderedVertexIdList[i];
        if(vertexId >= static_cast<int>(vertexList.size()))
            throw VertexNotFoundException();

        const GraphVertex& vertex = vertexList[vertexId];
        std::cout << vertex.name;
        if(!sameLine) std::cout << "\n";
        // Only find and print the weight if next is available
        if(i == orderedVertexIdList.size() - 1) break;
        int nextVertexId = orderedVertexIdList[i + 1];
        if(nextVertexId >= static_cast<int>(vertexList.size()))
            throw VertexNotFoundException();

        // Find the edge between these two vertices
        int edgeId = INVALID_INDEX;
        if(vertexId     < static_cast<int>(vertexList.size()) &&
           nextVertexId < static_cast<int>(vertexList.size()))
        {
            // Check all of the edges of vertex
            // and try to find
            const GraphVertex& fromVert = vertexList[vertexId];
            for(int i = 0; i < fromVert.edgeCount; i++)
            {
                int eId = fromVert.edgeIds[i];
                // Since the graph is not directional
                // check the both ends
                if((edgeList[eId].vertexId0 == vertexId &&
                    edgeList[eId].vertexId1 == nextVertexId)
                ||
                   (edgeList[eId].vertexId0 == nextVertexId &&
                    edgeList[eId].vertexId1 == vertexId))
                {
                    edgeId = eId;
                    break;
                }
            }
        }
        if(edgeId != INVALID_INDEX)
        {
            const GraphEdge& edge = edgeList[edgeId];
            std::cout << "-" << std::setfill('-')
                      << std::setw(2)
                      << edge.weight << "->";
        }
        else
        {
            std::cout << "-##-> ";
        }
    }
    // Print endline on the last vertex if same line is set
    if(sameLine) std::cout << "\n";
    // Reset fill value because it "sticks" to the std out
    std::cout << std::setfill(' ');
    std::cout.flush();
}

int Graph::TotalVertexCount() const
{
    // TODO
    return vertexList.size();
}

int Graph::TotalEdgeCount() const
{
    // TODO
    return edgeList.size();
}

std::string Graph::VertexName(int vertexId) const
{
    // TODO
    if ((vertexId < 0) || (vertexId >= vertexList.size())) return "";
    else return vertexList[vertexId].name;
}

int Graph::TotalWeightInBetween(std::vector<int>& orderedVertexIdList)
{
    // TODO
    int i,j;
    bool notAvailableVertexExist = false;
    for (i=0;i<orderedVertexIdList.size();i++){
        if ((orderedVertexIdList[i] < 0) || (orderedVertexIdList[i] >= vertexList.size())){
            notAvailableVertexExist = true;
            break;
        }
    }
    
    if (notAvailableVertexExist == true) throw VertexNotFoundException();
    
    int l=0;
    for (i=0;i<orderedVertexIdList.size()-1;i++){
        int i0 = orderedVertexIdList[i];
        int i1 = orderedVertexIdList[i+1];
        
        bool isAnEdgeExist = false;
        for (j=0;j<edgeList.size();j++){
            std::string name0 = vertexList[edgeList[j].vertexId0].name;
            std::string name1 = vertexList[edgeList[j].vertexId1].name;
            if ((vertexList[i0].name == name0 && vertexList[i1].name == name1) || (vertexList[i0].name == name1 && vertexList[i1].name == name0)){
                isAnEdgeExist = true;
                break;
            }
        }
        
        if (isAnEdgeExist == false) return -1;
        else {
            for (j=0;j<edgeList.size();j++){
                std::string name0 = vertexList[edgeList[j].vertexId0].name;
                std::string name1 = vertexList[edgeList[j].vertexId1].name;
                if ((vertexList[i0].name == name0 && vertexList[i1].name == name1) || (vertexList[i0].name == name1 && vertexList[i1].name == name0)){
                    l += (edgeList[j].weight);
                }
            }
        }
    }
    
    return l;
}






































