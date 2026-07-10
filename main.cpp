#include <iostream>
#include <vector>
#include <string>
#include "inliner.hpp"

int main(int args_counter, char** args) {
    int i, tab_spaces = 4;
    std::string source_file;
    std::string result_file = "result.txt";
    std::string result_folder = "inlined_files";
    std::string error;
    std::vector<std::string> arguments;
    bool demand_help = false;
    bool result_file_exists = false;
    bool source_file_exists = false;
    bool inline_list = false;

    for(i = 0; i < args_counter; i++) {
        arguments.push_back(args[i]);
    }

    for(i = 1; i < args_counter; i++) {
        if(arguments[i] == "-src" || arguments[i] == "--src" || arguments[i] == "-s" || arguments[i] == "--s") {
            i++;
            source_file = arguments[i];
            source_file_exists = true;
        } else if(arguments[i] == "-out" || arguments[i] == "--out" || arguments[i] == "-o" || arguments[i] == "--o") {
            i++;
            result_file = arguments[i];
            result_folder = arguments[i];
            result_file_exists = true;
        } else if(arguments[i] == "-tab-spaces" || arguments[i] == "--tab-spaces" || arguments[i] == "-t" || arguments[i] == "--t") {
            i++;
            char number = arguments[i][0];
            if(std::isdigit(number) != 0) {
                tab_spaces = std::atoi(&number);
            } else {
                std::cerr << "[Error] : The specified tab spaces number : '" << number << "' must be a digit";
                return EXIT_FAILURE;
            }
        } else if(arguments[i] == "-help" || arguments[i] == "--help" || arguments[i] == "-h" || arguments[i] == "--h") {
            demand_help = true;
        } else if(arguments[i] == "-list" || arguments[i] == "--list" || arguments[i] == "-l" || arguments[i] == "--l") {
            inline_list = true;
        } else {
            std::cerr << "[Error] : The specified argument : '" << arguments[i] << "' is unknown";
            return EXIT_FAILURE;
        }
    }
    
    if(!demand_help) {
        if(!source_file_exists){
            std::cerr << "[Error] : No source files was given";
            return EXIT_FAILURE;
        }
        if(!inline_list) {
            FileInliner inliner(source_file, result_file, tab_spaces);

            if(!inliner.verify_source(&error)) {
                std::cerr << error;
                return EXIT_FAILURE;
            }

            if(!result_file_exists)
                std::cout << "[Warning] : No result file was specified, it will be stored in 'inlined_files' foler" << std::endl;

            inliner.inline_file();
            inliner.store_result();
        } else {
            FileListInliner list_inliner(source_file, result_folder, tab_spaces);

            if(!list_inliner.verify_sources(&error)) {
                std::cerr << error;
                return EXIT_FAILURE;
            }

            if(!result_file_exists)
                std::cout << "[Warning] : No result file was specified, the files will be stored in 'inlined_files' folder" << std::endl;
            
            list_inliner.inline_files();
            list_inliner.store_results();
            list_inliner.clear();
        }
    } else {
        std::cout << std::endl;

        std::cout << "Inliner program's help" << std::endl;
        std::cout << std::endl;

        std::cout << "Command pattern : " << std::endl;
        std::cout << "\t inline <option> <value>" << std::endl;
        std::cout << std::endl;

        std::cout << "The <option> can be :" << std::endl;
        std::cout << "\t -help, --help, -h, --h             : print this help section" << std::endl;
        std::cout << "\t -src, --src, -s, --s               : specifies the source file" << std::endl;
        std::cout << "\t -out, --out, -o, --o               : specifies the result file" << std::endl;
        std::cout << "\t -tab-spaces, --tab-spaces, -t, --t : specifies the spaces number of the tabulation" << std::endl;
        std::cout << "\t -list, --list, -l, --l             : allow to the inliner to inline multiple files" << std::endl;
        std::cout << std::endl;

        std::cout << "The <value> can be : a file/folder path, a number or nothing (depending on the specified <option>)" << std::endl;
        std::cout << "Can input multiple <option> and <value> like this (for example) :" << std::endl;
        std::cout << "\t inline -src file.txt -out result.txt -t 2" << std::endl;
        std::cout << std::endl;
        std::cout << "If you input one of the help option, this section will be printed and no more action will be executed" << std::endl;
        std::cout << std::endl;
        std::cout << "If you input one of the list option, the source path must be a file that contains all files to inline" << std::endl;
        std::cout << "And the result path must be a folder where we want to store the results" << std::endl;
        std::cout << "For example :" << std::endl;
        std::cout << "\t inline -list -src files_to_inline_list.txt -out list" << std::endl;
        std::cout << std::endl;
    }

    return 0;
}