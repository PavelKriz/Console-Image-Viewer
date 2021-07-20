#include "CApplication.hpp"

using namespace std;

int main(int argc, const char *argv[]){
    try{
    	CApplication app(argc, argv);
    	app.run();
    } catch (const exception& e){
    	return CApplication::handleErrors(e);
    }
    return 0;
}
