#include <filesystem>
#include <fstream>
#include <vector>
#include <iostream>
#include "inliner.hpp"

Inliner::Inliner(std::string source_file_path, std::string result_file_path, unsigned int tab_spaces)
: file_path(source_file_path), result_file(result_file_path), tab_spaces(tab_spaces) {}

bool is_file_exists(const char* file) {
    return std::filesystem::exists(file);
}

bool Inliner::verify_source() const {
    return is_file_exists(this->file_path.c_str());
}

void Inliner::inline_file() {
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

void Inliner::store_result() const {
    std::fstream file;
    unsigned int i;

    file.open(this->result_file, std::ios_base::out);

    for(i = 0; i < this->file_inlined.size(); i++) {
        file.write(&this->file_inlined[i], 1);
    }

    file.close();

    std::cout << "The inlined file is stored on '" << this->result_file << "'\n" << std::endl;
}