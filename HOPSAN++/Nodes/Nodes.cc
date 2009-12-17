#include "Nodes.h"
#include <fstream>
#include <cassert>
#include <iostream>

//Node constructor
Node::Node()
{
    //mName = name;
    mNodeType = "Node";
    mDataVector.clear();
    mDataStorage.clear();
    mTimeStorage.clear();
    mLogSpaceAllocated = false;
    mLogCtr = 0;
}

//string &Node::getName()
//{
//    return mName;
//}

string &Node::getNodeType()
{
    return mNodeType;
}

void Node::setData(const size_t data_type, double data)
{
    mDataVector[data_type] = data;
}

double Node::getData(const size_t data_type)
{
    return mDataVector[data_type];
}

double &Node::getDataRef(const size_t data_type)
{
    return mDataVector[data_type];
}

void Node::preAllocateLogSpace(const size_t nSlots)
{
    size_t data_size = mDataVector.size();
    mTimeStorage.resize(nSlots);
    mDataStorage.resize(nSlots, vector<double>(data_size));
    //mDataStorage.reserve(nSlots);
//    vector<vector<double> >::iterator it;
//    for (it=mDataStorage.begin(); it!=mDataStorage.end(); ++it)
//    {
//        it->reserve(data_size);
//    }
    cout << "requestedSize: " << nSlots << " " << data_size << " Capacities: " << mTimeStorage.capacity() << " " << mDataStorage.capacity() << " " << mDataStorage[1].capacity() << " maxSize: " << mTimeStorage.max_size() << " " << mDataStorage.max_size() << " " << mDataStorage[1].max_size() << endl;
    mLogSpaceAllocated = true;
}

void Node::logData(const double time)
{
    //Check if vectors are large enough, else alocate
    //if

    if (mLogSpaceAllocated)
    {
        //cout << "mLogCtr: " << mLogCtr << endl;
        mTimeStorage[mLogCtr] = time;
        mDataStorage[mLogCtr] = mDataVector;
        //mDataStorage.push_back(mDataVector);
        ++mLogCtr;
    }
    else
    {
        ///TODO: for now always append
        mTimeStorage.push_back(time);
        mDataStorage.push_back(mDataVector);
    }

}

void Node::saveLogData(string filename)
{
    ofstream out_file;
    out_file.open(filename.c_str());

    if (out_file.good())
    {
        assert(mTimeStorage.size() == mDataStorage.size());
        //Write log data to file
        for (size_t row=0; row<mTimeStorage.size(); ++row)
        {
            out_file << mTimeStorage[row];
            for (size_t datacol=0; datacol<mDataVector.size(); ++datacol)
            {
                out_file << " " << mDataStorage[row][datacol];
            }
            out_file << endl;
        }
        out_file.close();
        cout << "Done! Saving node data to file" << endl;
    }
    else
    {
        cout << "Warning! Could not open out file for writing" << endl;
    }
}
