#include "config.h"
#include "gl_const.h"
#include "tinyxml2.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <math.h>

Config::Config()
{
    LogParams = nullptr;
    SearchParams = nullptr;
}

Config::~Config()
{
    if (SearchParams) delete[] SearchParams;
    if (LogParams) delete[] LogParams;
}

bool Config::getConfig(const char *FileName)
{
    std::string value;
    std::stringstream stream;
    tinyxml2::XMLElement *root = nullptr, *algorithm = nullptr, *element = nullptr, *options = nullptr;

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(FileName) != tinyxml2::XMLError::XML_SUCCESS) {
        std::cout << "Error opening XML file!" << std::endl;
        return false;
    }

    root = doc.FirstChildElement(CNS_TAG_ROOT);
    if (!root) {
        std::cout << "Error! No '" << CNS_TAG_ROOT << "' element found in XML file!" << std::endl;
        return false;
    }

    algorithm = root->FirstChildElement(CNS_TAG_ALG);
    if (!algorithm) {
        std::cout << "Error! No '" << CNS_TAG_ALG << "' tag found in XML file!" << std::endl;
        return false;
    }


    SearchParams = new double[9];
    SearchParams[CN_SP_ST] = CN_SP_ST_MAPF;
    element = algorithm->FirstChildElement(CNS_TAG_HW);
    if (!element) {
        std::cout << "Warning! No '" << CNS_TAG_HW << "' tag found in algorithm section." << std::endl;
        std::cout << "Value of '" << CNS_TAG_HW << "' was defined to 1." << std::endl;
        SearchParams[CN_SP_HW] = 1;
    } else {
        stream << element->GetText();
        stream >> SearchParams[CN_SP_HW];
        stream.str("");
        stream.clear();

        if (SearchParams[CN_SP_HW] < 1) {
            std::cout << "Warning! Value of '" << CNS_TAG_HW << "' tag is not correctly specified. Should be >= 1."
                      << std::endl;
            std::cout << "Value of '" << CNS_TAG_HW << "' was defined to 1." << std::endl;
            SearchParams[CN_SP_HW] = 1;
        }
    }

    element = algorithm->FirstChildElement(CNS_TAG_MT);
    if (!element) {
        std::cout << "Warning! No '" << CNS_TAG_MT << "' tag found in XML file." << std::endl;
        std::cout << "Value of '" << CNS_TAG_MT << "' was defined to 'euclidean'." << std::endl;
        SearchParams[CN_SP_MT] = CN_SP_MT_EUCL;
    } else {
        if (element->GetText())
            value = element->GetText();
        std::transform(value.begin(), value.end(), value.begin(), ::tolower);
        if (value == CNS_SP_MT_MANH) SearchParams[CN_SP_MT] = CN_SP_MT_MANH;
        else if (value == CNS_SP_MT_EUCL) SearchParams[CN_SP_MT] = CN_SP_MT_EUCL;
        else if (value == CNS_SP_MT_DIAG) SearchParams[CN_SP_MT] = CN_SP_MT_DIAG;
        else if (value == CNS_SP_MT_CHEB) SearchParams[CN_SP_MT] = CN_SP_MT_CHEB;
        else if (value == CNS_SP_MT_ZERO) SearchParams[CN_SP_MT] = CN_SP_MT_ZERO;
        else {
            std::cout << "Warning! Value of'" << CNS_TAG_MT << "' is not correctly specified." << std::endl;
            std::cout << "Value of '" << CNS_TAG_MT << "' was defined to 'euclidean'" << std::endl;
            SearchParams[CN_SP_MT] = CN_SP_MT_EUCL;
        }
    }


    element = algorithm->FirstChildElement(CNS_TAG_BT);
    if (!element) {
        std::cout << "Warning! No '" << CNS_TAG_BT << "' tag found in XML file" << std::endl;
        std::cout << "Value of '" << CNS_TAG_BT << "' was defined to 'g-max'" << std::endl;
        SearchParams[CN_SP_BT] = CN_SP_BT_GMAX;
    } else {
        value = element->GetText();
        std::transform(value.begin(), value.end(), value.begin(), ::tolower);
        if (value == CNS_SP_BT_GMIN) SearchParams[CN_SP_BT] = CN_SP_BT_GMIN;
        else if (value == CNS_SP_BT_GMAX) SearchParams[CN_SP_BT] = CN_SP_BT_GMAX;
        else {
            std::cout << "Warning! Value of '" << CNS_TAG_BT << "' is not correctly specified." << std::endl;
            std::cout << "Value of '" << CNS_TAG_BT << "' was defined to 'g-max'" << std::endl;
            SearchParams[CN_SP_BT] = CN_SP_BT_GMAX;
        }
    }


    element = algorithm->FirstChildElement(CNS_TAG_AP);
    if (!element) {
        std::cout << "Warning! No '" << CNS_TAG_AP << "' tag found in XML file" << std::endl;
        std::cout << "Value of '" << CNS_TAG_AP << "' was defined to '" << CNS_SP_AP_FIFO << "'" << std::endl;
        SearchParams[CN_SP_AP] = CN_SP_AP_FIFO;
    } else {
        value = element->GetText();
        std::transform(value.begin(), value.end(), value.begin(), ::tolower);
        if (value == CNS_SP_AP_HRMIN) SearchParams[CN_SP_AP] = CN_SP_AP_HRMIN;
        else if (value == CNS_SP_AP_HRMAX) SearchParams[CN_SP_AP] = CN_SP_AP_HRMAX;
        else if (value == CNS_SP_AP_RAND) SearchParams[CN_SP_AP] = CN_SP_AP_RAND;
        else if (value == CNS_SP_AP_FIFO) SearchParams[CN_SP_AP] = CN_SP_AP_FIFO;
        else {
            std::cout << "Warning! Value of '" << CNS_TAG_AP << "' is not correctly specified." << std::endl;
            std::cout << "Value of '" << CNS_TAG_AP << "' was defined to '" << CNS_SP_AP_FIFO << "'" << std::endl;
            SearchParams[CN_SP_AP] = CN_SP_AP_FIFO;
        }
    }

    element = algorithm->FirstChildElement(CNS_TAG_BS);
    if (!element) {
        std::cout << "Warning! No '" << CNS_TAG_BS << "' tag found in XML file" << std::endl;
        std::cout << "Value of '" << CNS_TAG_BS << "' was defined to '" << 0 << "'" << std::endl;
        SearchParams[CN_SP_BS] = 0;
    } else {
        int valueInt;
        if (element->QueryIntText(&valueInt) == tinyxml2::XML_SUCCESS && valueInt >= 0) {
            SearchParams[CN_SP_BS] = valueInt;
        } else {
            std::cout << "Warning! Value of '" << CNS_TAG_BS << "' is not correctly specified." << std::endl;
            std::cout << "Value of '" << CNS_TAG_BS << "' was defined to '" << 0 << "'" << std::endl;
            SearchParams[CN_SP_BS] = 0;
        }
    }


    /*
    element = algorithm->FirstChildElement(CNS_TAG_AD);
    if (!element) {
        std::cout << "Warning! No '" << CNS_TAG_AD << "' element found in XML file." << std::endl;
        std::cout << "Value of '" << CNS_TAG_AD << "' was defined to default - true" << std::endl;
        SearchParams[CN_SP_AD] = 1;
    } else {
        std::string check;
        stream << element->GetText();
        stream >> check;
        stream.clear();
        stream.str("");

        if (check != "1" && check != "true" && check != "0" && check != "false") {
            std::cout << "Warning! Value of '" << CNS_TAG_AD << "' is not correctly specified." << std::endl;
            std::cout << "Value of '" << CNS_TAG_AD << "' was defined to default - true " << std::endl;
            SearchParams[CN_SP_AD] = 1;
        } else if (check == "1" || check == "true")
            SearchParams[CN_SP_AD] = 1;
        else
            SearchParams[CN_SP_AD] = 0;
    }

    if (SearchParams[CN_SP_AD] == 0) {
        SearchParams[CN_SP_CC] = 0;
        SearchParams[CN_SP_AS] = 0;
    } else {
        element = algorithm->FirstChildElement(CNS_TAG_CC);
        if (!element) {
            std::cout << "Warning! No '" << CNS_TAG_CC << "' element found in XML file." << std::endl;
            std::cout << "Value of '" << CNS_TAG_CC << "' was defined to default - false" << std::endl;
            SearchParams[CN_SP_CC] = 0;
        } else {
            std::string check;
            stream << element->GetText();
            stream >> check;
            stream.clear();
            stream.str("");
            if (check != "1" && check != "true" && check != "0" && check != "false") {
                std::cout << "Warning! Value of '" << CNS_TAG_CC << "' is not correctly specified." << std::endl;
                std::cout << "Value of '" << CNS_TAG_CC << "' was defined to default - false" << std::endl;
                SearchParams[CN_SP_CC] = 0;
            } else {
                if (check == "1" || check == "true")
                    SearchParams[CN_SP_CC] = 1;
                else
                    SearchParams[CN_SP_CC] = 0;
            }
        }
        if (SearchParams[CN_SP_CC] == 0) {
            SearchParams[CN_SP_AS] = 0;
        } else {
            element = algorithm->FirstChildElement(CNS_TAG_AS);
            if (!element) {
                std::cout << "Warning! No '" << CNS_TAG_AS << "' element found in XML file." << std::endl;
                std::cout << "Value of '" << CNS_TAG_AS << "' was defined to default - false." << std::endl;
                SearchParams[CN_SP_AS] = 0;
            } else {
                std::string check;
                stream << element->GetText();
                stream >> check;
                stream.clear();
                stream.str("");
                if (check != "1" && check != "true" && check != "0" && check != "false") {
                    std::cout << "Warning! Value of '" << CNS_TAG_AS << "' is not correctly specified." << std::endl;
                    std::cout << "Value of '" << CNS_TAG_AS << "' was defined to default - false." << std::endl;
                    SearchParams[CN_SP_AS] = 0;
                } else {
                    if (check == "1" || check == "true")
                        SearchParams[CN_SP_AS] = 1;
                    else
                        SearchParams[CN_SP_AS] = 0;
                }
            }
        }
    }
    */

    options = root->FirstChildElement(CNS_TAG_OPT);
    LogParams = new std::string[3];
    LogParams[CN_LP_PATH] = "";
    LogParams[CN_LP_NAME] = "";

    if (!options) {
        std::cout << "Warning! No '" << CNS_TAG_OPT << "' tag found in XML file." << std::endl;
        std::cout << "Value of '" << CNS_TAG_LOGLVL << "' tag was defined to 'short log' (1)." << std::endl;
        LogParams[CN_LP_LEVEL] = CN_LP_LEVEL_SHORT_WORD;
    } else {
        element = options->FirstChildElement(CNS_TAG_LOGLVL);
        if (!element) {
            std::cout << "Warning! No '" << CNS_TAG_LOGLVL << "' tag found in XML file." << std::endl;
            std::cout << "Value of '" << CNS_TAG_LOGLVL << "' tag was defined to 'short log' (1)." << std::endl;
            LogParams[CN_LP_LEVEL] = CN_LP_LEVEL_SHORT_WORD;
        } else {
            stream << element->GetText();
            stream >> value;
            stream.str("");
            stream.clear();
            //std::transform(value.begin(), value.end(), value.begin(), ::tolower);
            if (value == CN_LP_LEVEL_NOPE_WORD || value == CN_LP_LEVEL_NOPE_VALUE)
                LogParams[CN_LP_LEVEL] = CN_LP_LEVEL_NOPE_WORD;
            else if (value == CN_LP_LEVEL_TINY_WORD || value == CN_LP_LEVEL_TINY_VALUE)
                LogParams[CN_LP_LEVEL] = CN_LP_LEVEL_TINY_WORD;
            else if (value == CN_LP_LEVEL_SHORT_WORD || value == CN_LP_LEVEL_SHORT_VALUE)
                LogParams[CN_LP_LEVEL] = CN_LP_LEVEL_SHORT_WORD;
            else if (value == CN_LP_LEVEL_MEDIUM_WORD || value == CN_LP_LEVEL_MEDIUM_VALUE)
                LogParams[CN_LP_LEVEL] = CN_LP_LEVEL_MEDIUM_WORD;
            else if (value == CN_LP_LEVEL_FULL_WORD || value == CN_LP_LEVEL_FULL_VALUE)
                LogParams[CN_LP_LEVEL] = CN_LP_LEVEL_FULL_WORD;
            else {
                std::cout << "'" << CNS_TAG_LOGLVL << "' is not correctly specified" << std::endl;
                std::cout << "Value of '" << CNS_TAG_LOGLVL << "' tag was defined to 'short log' (1)." << std::endl;
                LogParams[CN_LP_LEVEL] = CN_LP_LEVEL_SHORT_WORD;
            }
            std::cout << LogParams[CN_LP_LEVEL] << std::endl;
        }


        element = options->FirstChildElement(CNS_TAG_LOGPATH);
        if (!element) {
            std::cout << "Warning! No '" << CNS_TAG_LOGPATH << "' tag found in XML file." << std::endl;
            std::cout << "Value of '" << CNS_TAG_LOGPATH << "' tag was defined to 'current directory'." << std::endl;
        } else if (!element->GetText()) {
            std::cout << "Warning! Value of '" << CNS_TAG_LOGPATH << "' tag is missing!" << std::endl;
            std::cout << "Value of '" << CNS_TAG_LOGPATH << "' tag was defined to 'current directory'." << std::endl;
        } else {
            LogParams[CN_LP_PATH] = element->GetText();
        }


        element = options->FirstChildElement(CNS_TAG_LOGFN);
        if (!element) {
            std::cout << "Warning! No '" << CNS_TAG_LOGFN << "' tag found in XML file!" << std::endl;
            std::cout << "Value of '" << CNS_TAG_LOGFN
                      << "' tag was defined to default (original filename +'_log' + original file extension."
                      << std::endl;
        } else if (!element->GetText()) {
            std::cout << "Warning! Value of '" << CNS_TAG_LOGFN << "' tag is missing." << std::endl;
            std::cout << "Value of '" << CNS_TAG_LOGFN
                      << "' tag was defined to default (original filename +'_log' + original file extension."
                      << std::endl;
        } else
            LogParams[CN_LP_NAME] = element->GetText();
    }
    return true;
}
