#ifndef GTEST_CONFIGURE_H
#define GTEST_CONFIGURE_H
#include <string>
#include <fstream>


class GtestConfigure {

public:
    std::string original2D {"../data/images/2v5i7_640x480_BIN_1-0.pbm"};
    std::string original3D {"../data/images/IBS0105-50x50x50.dbm"};
    bool criarImagensResultantes {true};

   GtestConfigure() =  default;
   ~GtestConfigure() = default;

    inline bool FileNotExists(std::string filename) {
	    std::ifstream file (filename.c_str(), std::ifstream::in);
	    bool exists = file.good();
	    file.close();
	    return ! exists;
    }

};
#endif
