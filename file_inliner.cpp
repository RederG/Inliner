#include <filesystem>
#include <fstream>
#include <vector>
#include <iostream>
#include "inliner.hpp"

FileInliner::FileInliner(std::string source_file_path, std::string result_file_path, unsigned int tab_spaces)
: file_path(source_file_path), result_file(result_file_path), tab_spaces(tab_spaces) {
    this->result_folder.clear();
}

bool is_file_exists(const char* file) {
    return std::filesystem::exists(file);
}

bool FileInliner::verify_source(std::string* error) {
    std::fstream file;
    std::string folders_path;
    int i;
    unsigned int j;
    bool is_folder = false;

    file.open(this->file_path, std::ios_base::in);

    if(!is_file_exists(this->file_path.c_str())) {
        error->clear();
        error->append("[Error] => The specified file : '" + this->file_path + "' doesn't exist");
        return false;
    } else if(!file.is_open()) {
        error->clear();
        error->append("[Error] => The specified file : '" + this->file_path + "' can not be opened");
        return false;
    } else {
        for(i = this->result_file.size() - 1; i >= 0; i--) {
            if(this->result_file[i] == '/')
                is_folder = true;
            if(is_folder)
                folders_path.push_back(this->result_file[i]);
        }
        if(folders_path.size() > 0) {
            this->result_folder.resize(folders_path.size());
            for(j = 0; j < folders_path.size(); j++)
                this->result_folder[j] = folders_path[folders_path.size() - 1 - j];
        }
        file.close();
    }
    return true;
}

void FileInliner::inline_file() {
    std::fstream file;
    std::string content;
    unsigned int i, j, space_number, space_removed;
    char letter;
    unsigned int spaces_positions[this->tab_spaces];
    std::vector<unsigned int> positions_to_remove;
    auto init_time = std::chrono::high_resolution_clock::now();

    std::cout << "\nInlining '" << this->file_path << "' in progress..." << std::endl;

    file.open(this->file_path, std::ios_base::in);

    // Getting the content of the file and ignoring all '\n' and '\t' characters
    do {
        file.read(&letter, sizeof(char));
        if(file.eof())
            break;
        if(letter != '\n' && letter != '\t')
            content.push_back(letter);
    } while(!file.eof());

    // Shearching for tabulations (made with spaces)
    space_number = 0;
    positions_to_remove.clear();
    for(i = 0; i < content.size(); i++) {
        if(content[i] == ' ') {
            spaces_positions[space_number] = i;
            space_number++;
            if(space_number == this->tab_spaces) {
                for(j = 0; j < this->tab_spaces; j++)
                    positions_to_remove.push_back(spaces_positions[j]);
                space_number = 0;
            }
        } else {
            space_number = 0;
        }
    }

    // Removing all tabulations (made with spaces)
    space_removed = 0;
    for(unsigned int position : positions_to_remove) {
        content.erase(content.begin() + position - space_removed);
        space_removed++;
    }
    
    this->file_inlined = content;

    file.close();

    auto end_time = std::chrono::high_resolution_clock::now();
    float duration = std::chrono::duration<float>(end_time - init_time).count();
    std::cout << "Inlining terminated in " << duration << "seconds" << std::endl;
}

void FileInliner::create_folders() const {
    if(this->result_folder.size() > 0 && !std::filesystem::exists(this->result_folder)) {
        std::filesystem::create_directories(this->result_folder);
    }
}

bool FileInliner::store_result(std::string* error) const {
    std::fstream file;
    unsigned int i;

    this->create_folders();
    file.open(this->result_file, std::ios_base::out);
    
    if(file.is_open()) {
        for(i = 0; i < this->file_inlined.size(); i++) {
            file.write(&this->file_inlined[i], 1);
        }

        file.close();
    } else {
        error->clear();
        error->append("[Error] => Cannot create/open the result file : '" + this->result_file + "'\n");
        error->append("           Maybe you should create a folder or something ?");
        return false;
    }

    std::cout << "The inlined file is stored on '" << this->result_file << "'\n" << std::endl;
    return true;
}