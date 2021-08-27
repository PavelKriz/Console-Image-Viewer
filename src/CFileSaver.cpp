#include "CFileSaver.hpp"

CFileSaver::CFileSaver(string relativeFilePath)
    :relativeFilePath_(relativeFilePath)
{
}

void CFileSaver::safeFile(const vector<vector<char>> & toSafe){
    ofstream outStream(relativeFilePath_);

    if(!outStream.good()){
        throw ofstream::failure("Problem occcured during opening file with filepath: " + relativeFilePath_);
    }

    for(size_t i = 0; i < toSafe.size(); ++i){
        for(size_t j = 0; j < toSafe[i].size(); ++j){
            outStream.put(toSafe[i][j]);
            if(!outStream.good()){
                throw ofstream::failure("Problem occcured during saving file with filepath: " + relativeFilePath_);
            }
        }
        outStream.put('\n');
    }


    outStream.close();
} 