/**
 * @file map.cpp
 * @author Morticzek (https://github.com/Morticzek)
 * @brief The C++ Map project - source file
 * */
#include <iostream>
#include <fstream>
#include <limits>
#include <iomanip>
#include <getopt.h>
#include <memory.h>
#include "map.h"
using namespace std;
void Map::nodeAccumulate(cityDataBase cityNode)
{
    struct finalResult* n = new finalResult;
    (*n).city1 = cityNode.city1;
    (*n).city2 = cityNode.city2;
    (*n).distance = cityNode.distance;
    if(headFinal == NULL)
    {
        headFinal = n;
        headFinal->next = nullptr;
    }
    else
    {
        (*n).next = headFinal;
        headFinal = n;
    }
}
void Map::databaseAccumulate(cityDataBase cityNode)
{
    cityDataBase* m = new cityDataBase;
    m->city1 = cityNode.city1;
    m->city2 = cityNode.city2;
    m->distance = cityNode.distance;
    if(headBase == NULL)
    {
        headBase = m;
        headBase->next = NULL;
    }
    else
    {
        m->next = headBase;
        headBase = m;
    }
}
void Map::dataReceiverAccumulate(cityDataReceived cityNode)
{
    cityDataReceived* o = new cityDataReceived;
    o->cityBegin = cityNode.cityBegin;
    o->cityEnd = cityNode.cityEnd;
    if(headReceived == NULL)
    {
        headReceived = o;
        headReceived->next = NULL;
    }
    else
    {
        currReceived = headReceived;
        while(currReceived->next)
        {
            currReceived = currReceived->next;
        }
        currReceived->next = o;
        currReceived->next->next = NULL;
    }
}
void Map::dataChartAccumulate(temporaryList cityNode)
{
    struct cityDataChart *c = new cityDataChart;
    c->city = cityNode.cityName;
    c->visited = 0;
    c->sumOfDistances = INFINITY;
    if (headChart == nullptr) {
        headChart = c;
        headChart->next = nullptr;
    } else {
        c->next = headChart;
        headChart = c;
    }
}
void Map::uniqueLinkedList(temporaryList* node)
{
    temporaryList *temp1, *temp2, *temp3;
    temp1 = node;
    while(temp1 != nullptr && temp1->next != nullptr)
    {
        temp2 = temp1;
        while(temp2->next != nullptr)
        {
            if(temp1->cityName == temp2->next->cityName)
            {
                temp3 = temp2->next;
                temp2->next = temp2->next->next;
                delete(temp3);
            }
            else temp2 = temp2->next;
        }
        temp1 = temp1->next;
    }
}
void Map::clearLinkedList()
{
    currFinal = headFinal;
    while(currFinal)
    {
        tempFinal = (*currFinal).next;
        free(currFinal);
        currFinal = tempFinal;
    }
    headFinal = nullptr;
}
void Map::outputAllocator(string endCity, string beginCity, cityDataChart* headC, cityDataBase* headB, string outputFileName, cityDataChart* currC, Map linkedList)
{
    int finalDistance = 0;
    int tmp = INFINITY;
    currChart = headC;
    while(currChart)
    {
        if(currChart->city == endCity)
        {
            finalDistance = currChart->sumOfDistances;
            break;
        }
        currChart = currChart->next;
    }
    if(finalDistance == tmp)
    {
        ofstream Output (outputFileName, std::ios_base::app);
        Output << "route: " << beginCity << " --> " << endCity << ":" << endl << "   NO ROUTE EXISTS!" << endl;
        Output.close();
        clearLinkedList();
    }
    else
    {
        ofstream Output(outputFileName, std::ios_base::app);
        Output << "route: " << beginCity << " --> " << endCity << " (" << finalDistance << "km):" << endl;
        currFinal = headFinal;
        while (currFinal)
        {
            Output << currFinal->city1 << " --> " << currFinal->city2 << setw(8) << currFinal->distance << endl;
            currFinal = currFinal->next;
        }
        Output.close();
        clearLinkedList();
    }
    currChart = headC;
    while(currChart)
    {
        currChart->prevCity.clear();
        currChart->sumOfDistances = INFINITY;
        currChart->visited = false;
        currChart = currChart->next;
    }
    currChart = headC;
}
void Map::recursiveBacktracker(string endCity, string beginCity, string currentCity, cityDataChart* headC, cityDataBase* headB, cityDataChart* currC, string outputFileName, Map linkedList)
{
    string string2 = currentCity;
    string string1 = beginCity;
    if(beginCity == string2)
    {
        outputAllocator(endCity, beginCity, headC, headB, outputFileName, currC, linkedList);
    }
    else
        {
        currChart = headC;
        while(currChart)
        {
            if(currChart->city == string2 && currChart->visited == 1)
            {
                currBase = headB;
                while(currBase)
                {
                    if(currBase->city2 == string2 && currBase->city1 == currChart->prevCity)
                    {
                        cityDataBase cdb;
                        cdb.city2 = currBase->city2;
                        cdb.city1 = currBase->city1;
                        cdb.distance = currBase->distance;
                        string2 = currBase->city1;
                        nodeAccumulate(cdb);
                        recursiveBacktracker(endCity, beginCity, string2, headC, headB, currC, outputFileName, linkedList);
                        break;
                    }
                    currBase = currBase->next;
                }
                break;
            }
            currChart = currChart->next;
        }
    }
}
void Map::createInstanceAndDebug(string beginCity, string endCity, cityDataChart* headC, cityDataBase* headB, cityDataChart* currC, string outputFileName)
{
    Map finals;
/*    currChart = headC;
    currC = currChart;
    while(currChart)
    {
        cout << currChart->city << " " << currChart->prevCity << " " << currChart->sumOfDistances << " " << currChart->visited << endl;
        currChart = currChart->next;
    }
    currChart = headChart;*/
    recursiveBacktracker(endCity, beginCity, endCity, headC, headB, currC, outputFileName, finals);
}
void Map::visitingCalculator(string temporaryCity, string endCity, string currentCity, string beginCity, cityDataChart* headC, cityDataBase* headB, cityDataChart* currC, string outputFileName)
{
    int startDist = 0;
    currChart = headC;
    while(currChart)
    {
        if(currChart->city == temporaryCity)
        {
            startDist = currChart->sumOfDistances;
        }
        currChart = currChart->next;
    }
    currBase = headB;
    while(currBase)
    {
        if(currBase->city1 == temporaryCity)
        {
            currChart = headC;
            while(currChart)
            {
                if(currChart->city == currBase->city2 && currChart->sumOfDistances > startDist + currBase->distance)
                {
                    currChart->sumOfDistances = startDist + currBase->distance;
                    currChart->prevCity = temporaryCity;
                }
                currChart = currChart->next;
            }
        }
        currBase = currBase->next;
    }
    currChart = headC;
    while(currChart != nullptr)
    {
        if(currChart->city == temporaryCity)
        {
            currChart->visited = 1;
        }
        currChart = currChart->next;
    }
    visitingMarker(temporaryCity, currentCity, beginCity, headC, headB, currC, outputFileName);
}

void Map::visitingMarker(string endCity, string currentCity, string beginCity, cityDataChart* headC, cityDataBase* headB, cityDataChart* currC, string outputFileName)
{
    int tmp = INFINITY;
    int tmpCompare = INFINITY;
    string tmpCity;
    currChart = headC;
    while(currChart)
    {
        if(tmp > currChart->sumOfDistances && currChart->visited == 0)
        {
            tmp = currChart->sumOfDistances;
            tmpCity = currChart->city;
        }
        currChart = currChart->next;
    }
    currC = currChart;
    if(tmp == tmpCompare)
    {
        createInstanceAndDebug(beginCity, currentCity, headC, headB, currC, outputFileName);
    }
    else visitingCalculator(tmpCity, endCity, currentCity, beginCity, headC, headB, currC, outputFileName);
}
void Map::dataAssigner(string mapFileName, string routeFileName, string outputFileName) {
    ifstream Stream1(mapFileName);
    cityDataBase cdb;
    while (Stream1 >> cdb.city1 >> cdb.city2 >> cdb.distance && Stream1){
        databaseAccumulate(cdb);
    }
    Stream1.close();
    ifstream Stream2(routeFileName);
    cityDataReceived cdr;
    while (Stream2 >> cdr.cityBegin >> cdr.cityEnd && Stream2) {
        dataReceiverAccumulate(cdr);
    }
    currBase = headBase;
    while (currBase)
    {
        struct temporaryList *n = new temporaryList;
        struct temporaryList *m = new temporaryList;
        if(headTemp == nullptr)
        {
            n->cityName = currBase->city1;
            headTemp = n;
            headTemp->next = nullptr;
            m->cityName = currBase->city2;
            m->next = headTemp;
            headTemp = m;
        }
        else
        {
            n->cityName = currBase->city1;
            n->next = headTemp;
            headTemp = n;
            m->cityName = currBase->city2;
            m->next = headTemp;
            headTemp = m;
        }
        currBase = currBase->next;
    }
    uniqueLinkedList(headTemp);
    temporaryList cdc;
    currTemp = headTemp;
    while(currTemp)
    {
        cdc.cityName = currTemp->cityName;
        dataChartAccumulate(cdc);
        currTemp = currTemp->next;
    }
    currReceived = headReceived;
    while(currReceived)
    {
        string finalCity = currReceived->cityEnd;
        string beginningCity = currReceived->cityBegin;
        currChart = headChart;
        while(currChart)
        {
            if(currReceived->cityBegin == currChart->city)
            {
                cityDataChart* currChart2 = headChart;
                while(currChart2)
                {
                    if(currReceived->cityEnd == currChart2->city)
                    {
                        cityDataChart* cdc = currChart;
                        currChart->sumOfDistances = 0;
                        visitingMarker(finalCity, finalCity, beginningCity, headChart, headBase, cdc, outputFileName);
                        break;
                    }
                    currChart2 = currChart2->next;
                }
                break;
            }
            currChart = currChart->next;
        }
        currReceived = currReceived->next;
    }
    currBase = headBase;
    while(currBase)
    {
        tempBase = (*currBase).next;
        free(currBase);
        currBase = tempBase;
    }
    headBase = nullptr;
    free(headBase);
    currChart = headChart;
    while(currChart)
    {
        tempChart = currChart->next;
        free(currChart);
        currChart = tempChart;
    }
    headChart = nullptr;
    free(headChart);
    currTemp = headTemp;
    while(currTemp)
    {
        tempTemp = currTemp->next;
        free(currTemp);
        currTemp = tempTemp;
    }
    headTemp = nullptr;
    free(headTemp);
    currReceived = headReceived;
    while(currReceived)
    {
        tempReceived = currReceived->next;
        free(currReceived);
        currReceived = tempReceived;
    }
    headReceived = nullptr;
    free(headReceived);
    exit(0);
}
int main(int argc, char* argv[])
{
    string dataFile, orderFile, outputFile;
    char ch;
    while ((ch = getopt(argc, argv, "m:r:o:")) != EOF) {
        switch (ch)
        {
            case 'm':
            {
                dataFile = optarg;
                break;
            }
            case 'r':
            {
                orderFile = optarg;
                break;
            }
            case 'o':
            {
                outputFile = optarg;
                break;
            }
        }
    }
    Map startObject;
    dataFile = "mapa.txt";
    orderFile = "orders.txt";
    outputFile = "output.txt";
    if(dataFile.empty() == false && orderFile.empty() == false && outputFile.empty() == false)
        startObject.dataAssigner(dataFile, orderFile, outputFile);
    else
    {
        cerr << "Unsuccessfully launched. Please try again." << endl;
        cerr << "Missing files: " << endl;
        if(dataFile.empty() == true)
        {
            cerr << "Map file (-m) " << endl;
        }
        if(orderFile.empty() == true)
        {
            cerr << "Routes file (-r)" << endl;
        }
        if(outputFile.empty() == true)
        {
            cerr << "Output file (-o)" << endl;
        }
        exit(0);
    }
    return 0;
}