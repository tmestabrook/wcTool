#include "wcObj.h"

int main(int argc, char* argv[]) {
    wcObj wcObjMain(argc, argv);
    wcObjMain.countStats();
    wcObjMain.executeCommands();
    wcObjMain.printRuntimeStats();
    return 0;
}
