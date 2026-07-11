#include <fstream>
#include <filesystem>
#include <iostream>
#include "inliner.hpp"

FileListInliner::FileListInliner(std::string file_list, std::string folder, unsigned int tab_spaces)
: file_list_path(file_list), result_folder_path(folder), tab_spaces(tab_spaces) {
    this->inlined_files.clear();
}

bool FileListInliner::verify_sources(std::string* error) {
    std::fstream file;
    std::string file_directory;
    std::string reversed_file_directory;
    char letter;
    std::string line;
    std::vector<std::string> all_files_name;

    if(std::filesystem::exists(this->file_list_path)){
        
        file.open(this->file_list_path, std::ios_base::in);

        if(file.is_open()) {
            do {
                file.read(&letter, sizeof(char));
                if(letter != '\n' && !file.eof()) {
                    line.push_back(letter);
                } else if(line.size() > 0) {
                    all_files_name.push_back(line);
                    line = "";
                }
            } while(!file.eof());

            for(std::string file_name : all_files_name) {
                FileInliner* inliner = new FileInliner(file_name, this->result_folder_path + "/" + file_name, this->tab_spaces);
                if(!inliner->verify_source(error))
                    return false;
                this->inlined_files.push_back(inliner);
            }
            return true;
        } else {
            error->clear();
            error->append("[Error] => The specified file '" + this->file_list_path + "' can not be opened");
            return false;
        }
    } else {
        error->clear();
        error->append("[Error] => The specified file '" + this->file_list_path + "' doesn't exist");
        return false;
    }
}

void FileListInliner::inline_files() const {
    auto init_time = std::chrono::high_resolution_clock::now();

    std::cout << "\nInlining files..." << std::endl;

    for(FileInliner* inliner : this->inlined_files) {
        inliner->inline_file();
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    float duration = std::chrono::duration<float>(end_time - init_time).count();
    std::cout << "Files inlined in " << duration <<  "seconds\n" << std::endl;
}

bool FileListInliner::store_results(std::string* error) const {
    for(FileInliner* inliner : this->inlined_files) {
        if(!inliner->store_result(error))
            return false;
    }
    return true;
}

void FileListInliner::clear() {
    for(FileInliner* inliner : this->inlined_files) {
        delete inliner;
        inliner = nullptr;
    }
    this->inlined_files.clear();
}