#include "CApplication.hpp"

using namespace std;

int main(int argc, const char *argv[]){
    try{
    	CApplication app(argc, argv);
        int ret = app.run();
    	return ret;
    } catch (int retCode){
    	return retCode;
    }
    return 0;
}
