#include "CApplication.hpp"

using namespace std;

int main(int argc, const char *argv[]){
    try{
    	CApplication app(argc, argv);
    	app.run();
    } catch (int retCode){
    	return retCode;
    }
    return 0;
}
